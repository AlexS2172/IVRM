SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/*PREPARATIONS FOR ERROR HANDLING*/
SET NOCOUNT ON
GO
IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#tmpErrors')) DROP TABLE #tmpErrors
GO
CREATE TABLE #tmpErrors (Error int)
GO
SET XACT_ABORT ON
GO
/*SCRIPTS MAIN SECTION*/
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.213'  or (keyValue = '3.46.214') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.214 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.214' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO


ALTER	PROCEDURE dbo.usp_HedgeSymbolBeta_Load
	@iHedgeSymbolID int
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	delete IndexBeta where indexID = @iHedgeSymbolID

	insert into IndexBeta (indexID, contractID, beta) 
	select 	eod_i.contractID,
		eod_u.contractID,
		eod_ib.beta
	from ETS_ASP_EOD.dbo.IndexBeta eod_ib
		inner join EodUnderlyingMap eod_i on eod_i.eodContractID = eod_ib.indexID
		inner join EodUnderlyingMap eod_u on eod_u.eodContractID = eod_ib.contractID
	where eod_ib.actionID <3 and eod_i.contractID = @iHedgeSymbolID
	
	--self correlation is 1.0
	insert into IndexBeta (indexID, contractID, beta) 
	values (@iHedgeSymbolID, @iHedgeSymbolID, 1.0)
	
	set @error = @@error
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/*------------------------------------------------------------------*/
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER               Procedure dbo.usp_IndexUnderlyingBeta_Save 
	@iIndexID int = null, --the index identifier
	@iUnderlyingID int = null, --the underlying identifier
	@fBeta float,
	@vcIndexSymbol varchar(8) = null,
	@vcUnderlyingSymbol varchar(8) = null
as
/*
	Created by Mike Malyshkin
	Description: this procedure save data into IndexBeta table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @m_contractType int 	set @m_contractType = -1
	declare @m_oldState int 	set @m_oldState = -1

	if @iIndexID is null and @iUnderlyingID is null and @vcIndexSymbol is null and @vcUnderlyingSymbol is null 
		begin
			Raiserror ('Impossible define identifiers of the Underlying and Index!', 16, 1)
			Return (-1)
		end   

	if @vcIndexSymbol is not null and @iIndexID is null
		select	@iIndexID = contractID from Contract where symbol = @vcIndexSymbol and contractTypeID in (1,2) and actionID < 3
		if @iIndexID is null 
			begin
				Raiserror ('Impossible define identifier of the Index!', 16, 1)
				Return (-1)
			end   

	if @vcUnderlyingSymbol is not null and @iUnderlyingID is null 
		select	@iUnderlyingID = contractID from Contract where symbol = @vcUnderlyingSymbol and contractTypeID in (1,2) and actionID < 3
		if @iUnderlyingID is null 
			begin
				Raiserror ('Impossible define identifier of the Underlying!', 16, 1)
				Return (-1)
			end   

	Begin Transaction
		
			
		--Set stock or index as HedgeSymbol
		select	@m_contractType = contractTypeID
		from contract 
		where contractID = @iIndexID and actionID < 3

		if @m_contractType <> -1	--if contract found
		begin 
			if @m_contractType = 1 --index type
			begin
					select @m_oldState = isHedgeSymbol
					from [index]
					where (indexID = @iIndexID and actionID < 3)
					
					if @m_oldState = 0
					begin
						update [index] 
							set isHedgeSymbol = 1
							where (indexID = @iIndexID and actionID < 3)
					end
			end

			if @m_contractType = 2 --stock type
			begin
					select @m_oldState = isHedgeSymbol
					from Stock
					where (stockID = @iIndexID and actionID < 3)
					
					if @m_oldState = 0
					begin
						update Stock 
							set isHedgeSymbol = 1
							where (stockID = @iIndexID and actionID < 3)
					end
			end

			if @m_oldState = 0
			begin
				exec @error = usp_HedgeSymbolBeta_Load @iIndexID	
			end
		end
		
		--set new Beta for (@iUnderlyingID, @iIndexID)
		if not exists(select * From IndexBetaView where indexID = @iIndexID and contractID = @iUnderlyingID) 
			begin
				insert into IndexBeta (indexID,contractID,beta) 
				values (@iIndexID,@iUnderlyingID,@fBeta)
					
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexBeta
					set beta = isnull(@fBeta, beta)
					where indexID = @iIndexID and contractID = @iUnderlyingID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end

		if @@error <> 0 begin set @error = 2 if @@TranCount > 1 Rollback Transaction goto finish end

	Commit Transaction
    set @error = 0

finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.214')
	begin
		update DataInfo set keyValue = '3.46.214' where dataInfoID = 1
	end
GO

/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
IF EXISTS (SELECT * FROM #tmpErrors)
BEGIN
	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
END
GO
IF @@TRANCOUNT > 0 
BEGIN
	select 'DB VERSION WAS CHANGED TO 3.46.214' '3.46.214 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
