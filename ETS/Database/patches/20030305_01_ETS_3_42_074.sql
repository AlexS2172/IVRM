/*
implementation of MLine00000556  
*/

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.42.073' or (keyValue = '3.42.074')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.42.074 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.42.074' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
--select [name] from sysobjects where type = 'P' and [name] like '%DH%'
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='IndexCorrelation' and COLUMN_NAME='updateDate')
begin
ALTER TABLE IndexCorrelation ADD updateDate smalldatetime not null
   CONSTRAINT updateDate_default DEFAULT (GetDate())
end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='StockCorrelation' and COLUMN_NAME='updateDate')
begin
ALTER TABLE StockCorrelation ADD updateDate smalldatetime not null
   CONSTRAINT updateDate__stock_default DEFAULT (GetDate())
end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='IndexDataHistory' and COLUMN_NAME='updateDate')
begin
ALTER TABLE IndexDataHistory ADD updateDate smalldatetime not null
   CONSTRAINT updateDate_IndexDataHistory_default DEFAULT (GetDate())
end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='StockDataHistory' and COLUMN_NAME='updateDate')
begin
ALTER TABLE StockDataHistory ADD updateDate smalldatetime not null
   CONSTRAINT updateDate_StockDataHistory_default DEFAULT (GetDate())
end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--============================================================================================
-- creation of new SP's:
-- usp_CorrelationDH_Get
-- usp_CorrelationDH_Save
-- usp_DataHistoryDH_Get
-- usp_DataHistoryDH_Save
--============================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CorrelationDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CorrelationDH_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CorrelationDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CorrelationDH_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DataHistoryDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DataHistoryDH_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DataHistoryDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DataHistoryDH_Save]
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE   Procedure dbo.usp_CorrelationDH_Get 
	@iContractID1 int, --indexID
	@iContractID2 int, --stockID
	@iContractTypeID1 int, 
	@iContractTypeID2 int,
	@fCorrelationPrice float = null Output,
	@fCorrelationVola float = null Output,
	@dtUpdateDate smalldatetime = null Output
AS

/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from StockCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iContractID3 int


	if  @iContractTypeID1 = @iContractTypeID2
		begin
			if 	@iContractID1 > @iContractID2
				select @iContractID3=@iContractID1,@iContractID1=@iContractID2,@iContractID2=@iContractID3
		end

	if  @iContractTypeID1 = 1 and @iContractTypeID2 = 1 
		begin
			select 	@fCorrelationPrice = correlationPrice,
					@fCorrelationVola = correlationVola,
					@dtUpdateDate = updateDate
				from IndexCorrelation	
				where indexID1 = @iContractID1 and indexID2 = @iContractID2
		end
	else if @iContractTypeID1 = 1 and @iContractTypeID2 = 2 
		begin
			select 	@fCorrelationPrice = correlationPrice,
					@fCorrelationVola = correlationVola,
					@dtUpdateDate = actionDate
				from IndexDefinition
				where indexID = @iContractID1 and stockID = @iContractID2
		end 
	else if @iContractTypeID1 = 2 and @iContractTypeID2 = 1 
		begin
			select @fCorrelationPrice = correlationPrice,
				   @fCorrelationVola = correlationVola,
				   @dtUpdateDate = actionDate
				from IndexDefinition
				where indexID = @iContractID2 and stockID = @iContractID1
		end 
	else if @iContractTypeID1 = 2 and @iContractTypeID2 = 2 
		begin
			select @fCorrelationPrice = correlationPrice,
                   @fCorrelationVola = correlationVola,
                   @dtUpdateDate = updateDate
				from StockCorrelation	
				where stockID1 = @iContractID1 and stockID2 = @iContractID2
		end


	set @error = @@error
Return(@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE PROCEDURE dbo.usp_CorrelationDH_Save 
	@iContractID1 int, --indexID
	@iContractID2 int, --stockID
	@iContractTypeID1 int,
	@iContractTypeID2 int,
	@fCorrelationPrice float = null,
	@fCorrelationVola float = null
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractID3 int


	if @iContractTypeID1=@iContractTypeID2
		begin
			if 	@iContractID1 > @iContractID2
				select @iContractID3=@iContractID1,@iContractID1=@iContractID2,@iContractID2=@iContractID3
		end

	if @iContractTypeID1= 2 and @iContractTypeID2 = 1
		begin
			select @iContractID3=@iContractID1,@iContractID1=@iContractID2,@iContractID2=@iContractID3
		end

	if  @iContractTypeID1 = 1 and @iContractTypeID2 = 1 
	begin
		if not exists(select indexID1 from IndexCorrelation where indexID1 = @iContractID1 and indexID2 = @iContractID2)
			begin
				insert into IndexCorrelation(indexID1,indexID2,correlationPrice,correlationVola)
					values(@iContractID1,@iContractID2,@fCorrelationPrice,@fCorrelationVola)
			end
		else 
			begin
				update IndexCorrelation 
					set 
						correlationPrice = isnull(@fCorrelationPrice,correlationPrice),
						correlationVola = isnull(@fCorrelationVola,correlationVola),
						updateDate = GetDate()
					where indexID1 = @iContractID1 and indexID2 = @iContractID2
			end
	end
	else if (@iContractTypeID1 != @iContractTypeID2)
	begin
		if not exists(select indexDefinitionID from IndexDefinition where indexID = @iContractID1 and stockID = @iContractID2)
			begin
				insert into IndexDefinition(indexID,stockID,correlationPrice,correlationVola)
					values(@iContractID1,@iContractID2,@fCorrelationPrice,@fCorrelationVola)
			end
		else
			begin
				update IndexDefinition
					set correlationPrice = isnull(@fCorrelationPrice,correlationPrice),
						correlationVola = isnull(@fCorrelationVola,correlationVola)
					where indexID = @iContractID1 and stockID = @iContractID2
			end	
	end
	else if @iContractTypeID1 = 2 and @iContractTypeID2 = 2 
	begin
		if not exists(select stockID1 from StockCorrelation where stockID1 = @iContractID1 and stockID2 = @iContractID2)
			begin
				insert into StockCorrelation(stockID1,stockID2,correlationPrice,correlationVola)
					values(@iContractID1,@iContractID2,@fCorrelationPrice,@fCorrelationVola)
			end
		else
			begin
				update StockCorrelation
					set correlationPrice = isnull(@fCorrelationPrice,correlationPrice),
						correlationVola = isnull(@fCorrelationVola,correlationVola),
						updateDate = GetDate()
				where stockID1 = @iContractID1 and stockID2 = @iContractID2
			end

	end

    set @error = 0
finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_DataHistoryDH_Get
	@iContractID int,	-- indexID or stockID
	@iContractTypeID int,
	@fCurrentIV float = null Output,
	@fCurrentHV float = null Output,
	@fCurrentVolOfVol float = null Output,
	@fCurrentVega float = null Output,
	@dtUpdateDate smalldatetime = null Output	
as
/*
	Created by Zaur Nuraliev
	this procedure returns historical data for index
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iContractTypeID = 1 
		begin
			select 	@fCurrentIV = currentIV, 
					@fCurrentHV = currentHV,
					@fCurrentVolOfVol = currentVolOfVol,
					@fCurrentVega = currentVega,
					@dtUpdateDate = updateDate
				from IndexDataHistory
				where tenorID=8 
					and [date]=(select max([date]) from IndexDataHistory where indexID = @iContractID and tenorID=8)
					and indexID = @iContractID
		end
	else if @iContractTypeID = 2
		begin
			select 	
					@fCurrentIV = IVMean,
					@fCurrentHV = currentHV,
					@fCurrentVolOfVol = currentVolOfVol,
					@dtUpdateDate = updateDate
				from StockDataHistory
				where tenorID=8 
					and [date]=(select max([date]) from StockDataHistory where stockID = @iContractID and tenorID=8)
					and stockID = @iContractID
	 	end

	set @error = @@error

finish:
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_DataHistoryDH_Save
	@iContractID int,		-- indexID or stockID
	@iContractTypeID int,	-- 1 or 2
	@fCurrentIV float = null,
	@fCurrentHV float = null,
	@fCurrentVolOfVol float = null,
	@fCurrentVega float = null
as
/*
	Created by Zaur Nuraliev
	this procedure save historical data for index/stock
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iContractTypeID = 1 
		begin
			if not exists (select indexID from IndexDataHistory where indexID = @iContractID and tenorID=8 and [date]=(select max([date]) from IndexDataHistory where indexID = @iContractID and tenorID=8))
				begin
					insert into IndexDataHistory(
						currentIV,
						currentHV,
						currentVolOfVol,
						currentVega)
					values(
						@fCurrentIV,
						@fCurrentHV,
						@fCurrentVolOfVol,
						@fCurrentVega)
				end
			else
				begin
					update IndexDataHistory
					set 
						currentIV = isnull(@fCurrentIV,currentIV),
						currentHV = isnull(@fCurrentHV,currentHV),
						currentVolOfVol = isnull(@fCurrentVolOfVol,currentVolOfVol),
						currentVega = isnull(@fCurrentIV,currentVega),
						updateDate = GetDate()
					where indexID = @iContractID and tenorID=8 and [date]=(select max([date]) from IndexDataHistory where indexID = @iContractID and tenorID=8)
				end
		end
	else if @iContractTypeID = 2
		begin
			if not exists(select stockID from StockDataHistory where tenorID = 8 and [date]=(select max([date]) from StockDataHistory where stockID = @iContractID and tenorID=8)	and stockID = @iContractID)
				begin
					insert into StockDataHistory (
							stockID,
							tenorID,
							[date],
							IVMean,
							currentHV,
							currentVolOfVol) 
						values(
							@iContractID,
							8,
							GetDate(),
							isnull(@fCurrentIV,0),
							@fCurrentHV,
							@fCurrentVolOfVol)
				end
			else
				begin
					update StockDataHistory
					set 
						IVMean = isnull(@fCurrentIV,IVMean),
						currentHV = isnull(@fCurrentHV,currentHV),
						currentVolOfVol = isnull(@fCurrentVolOfVol,currentVolOfVol),
						updateDate = GetDate()
					where tenorID=8 and [date]=(select max([date]) from StockDataHistory where stockID = @iContractID and tenorID=8) and stockID = @iContractID
				end
	 	end

	set @error = @@error

finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.42.074')
	begin
		update DataInfo set keyValue = '3.42.074' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.42.074' '3.42.074 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO