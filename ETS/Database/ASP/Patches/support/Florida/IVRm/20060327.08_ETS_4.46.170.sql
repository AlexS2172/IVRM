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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

GO

ALTER     Procedure dbo.usp_IndexUnderlyingBeta_Save 
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

	if @iIndexID is null and @iUnderlyingID is null and @vcIndexSymbol is null and @vcUnderlyingSymbol is null 
		begin
			Raiserror ('Impossible define identifiers of the Underlying and Index!', 16, 1)
			Return (-1)
		end   

	if @vcIndexSymbol is not null and @iIndexID is null
		select	@iIndexID = contractID from Contract where symbol = @vcIndexSymbol and contractTypeID = 1 and actionID < 3
		if @iIndexID is null 
			begin
				Raiserror ('Impossible define identifier of the Index!', 16, 1)
				Return (-1)
			end   

	if @vcUnderlyingSymbol is not null and @iUnderlyingID is null 
		select	@iUnderlyingID = contractID from Contract where symbol = @vcUnderlyingSymbol and contractTypeID in (1, 2, 4) and actionID < 3
		if @iUnderlyingID is null 
			begin
				Raiserror ('Impossible define identifier of the Underlying!', 16, 1)
				Return (-1)
			end   

	Begin Transaction
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

	Commit Transaction
    set @error = 0

finish:
Return(@error)
GO


if exists (select * from dbo.sysindexes where name = N'IDX_Contract_ContractType' and id = object_id(N'[dbo].[Contract]'))
begin
drop index [dbo].[Contract].[IDX_Contract_ContractType]
drop statistics [dbo].[contract].[hind_2075870462_2A_1A_3A_7A] 
end
GO

CREATE NONCLUSTERED INDEX [IDX_Contract_ContractType] ON [dbo].[Contract] ([contractTypeID] ASC, [symbol] ASC, [actionID] ASC, [contractID] ASC )
GO

CREATE STATISTICS [hind_2075870462_2A_1A_3A_7A] ON [dbo].[contract] ([contracttypeid], [contractid], [symbol], [actionid])

GO
/* RESULTS ANALYZING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.170')
	begin
		update DataInfo set keyValue = '4.49.170' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.170' '4.49.170 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
