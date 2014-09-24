/*
Implementation of beta indices for Synthetic Uderlyings

	DB version in script: 3.39.047
	DB version for update: 3.38.046
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.38.046' or (keyValue = '3.39.047')))
	BEGIN
		PRINT 'You can''t run this script, because you have not run script for previous version!'
		Select cast(keyValue as varchar(25)) as 'DB version', '3.39.047' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
--=====================================================
-- SyntheticRootIndexBeta
--=====================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SyntheticRootIndexBeta]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[SyntheticRootIndexBeta]
GO
CREATE TABLE [dbo].[SyntheticRootIndexBeta] (
	[syntheticIndexBetaID] [int] IDENTITY (1, 1) NOT NULL ,
	[syntheticOptRootID] [int] NOT NULL ,
	[indexID] [int] NOT NULL ,
	[beta] [float] NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
ALTER TABLE [dbo].[SyntheticRootIndexBeta] WITH NOCHECK ADD 
	CONSTRAINT [DF_SyntheticRootIndexBeta_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_SyntheticRootIndexBeta_actionDate] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [PK_SyntheticRootIndexBeta] PRIMARY KEY  NONCLUSTERED 
	(
		[syntheticIndexBetaID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_SyntheticRootIndexBeta] CHECK ([actionID] > 0 and [actionID] < 4)
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
ALTER TABLE [dbo].[SyntheticRootIndexBeta] ADD 
	CONSTRAINT [FK_SyntheticRootIndexBeta_Indexes] FOREIGN KEY 
	(
		[indexID]
	) REFERENCES [dbo].[Index] (
		[indexID]
	),
	CONSTRAINT [FK_SyntheticRootIndexBeta_SyntheticOptionRoot] FOREIGN KEY 
	(
		[syntheticOptRootID]
	) REFERENCES [dbo].[SyntheticOptionRoot] (
		[syntheticOptRootID]
	)
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_SyntheticRootIndexBeta]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_SyntheticRootIndexBeta]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE     Trigger tU_SyntheticRootIndexBeta ON dbo.SyntheticRootIndexBeta
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	-- 1st step - update actionDate if record not already deleted
	update SyntheticRootIndexBeta
		set actionDate = GetDate()
	from SyntheticRootIndexBeta S 
			join inserted on S.syntheticIndexBetaID = inserted.syntheticIndexBetaID
			join deleted on S.syntheticIndexBetaID = deleted.syntheticIndexBetaID
	where deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	update SyntheticRootIndexBeta
		set actionID=2
	from SyntheticRootIndexBeta S
			join inserted on S.syntheticIndexBetaID = inserted.syntheticIndexBetaID
			join deleted on S.syntheticIndexBetaID = deleted.syntheticIndexBetaID
	where deleted.actionID<3 and inserted.actionID = 1

	-- 3d step -disable restore of deleted record
	update SyntheticRootIndexBeta 
		set actionID = 3
	from SyntheticRootIndexBeta S
			join inserted on S.syntheticIndexBetaID = inserted.syntheticIndexBetaID
			join deleted on S.syntheticIndexBetaID = deleted.syntheticIndexBetaID
	where deleted.actionID=3
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--=====================================================
-- SyntheticRootIndexBetaView
--=====================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SyntheticRootIndexBetaView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[SyntheticRootIndexBetaView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    VIEW dbo.SyntheticRootIndexBetaView
AS
	Select
		syntheticIndexBetaID,
		syntheticOptRootID,
		indexID,
		beta,
		actionID,
		actionDate
		from SyntheticRootIndexBeta
		where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--=====================================================
-- changes in SP's
--=====================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroup_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractType_Change]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractType_Change]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionRootCA_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionRootCA_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionRoot_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionRoot_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportMarketStructure_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportMarketStructure_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SyntheticIndexBeta_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SyntheticIndexBeta_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SyntheticIndexBeta_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SyntheticIndexBeta_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SyntheticIndexBeta_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SyntheticIndexBeta_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SyntheticParams_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SyntheticParams_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SyntheticRootIndexBetaHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SyntheticRootIndexBetaHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingByTrader_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptionByExpiry_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingOptionByExpiry_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE     Procedure dbo.usp_ContractGroup_Get
	@iGroupID int = null,
    @bShortFormat bit = 0
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if (@iGroupID is Null and @bShortFormat = 0)
		select groupID as iGroupID, 
			groupName as vcGroupName, 
			[Description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
			from ContractGroupView
			order by groupName
	else if (@iGroupID is Null and @bShortFormat = 1)
		select groupID as iGroupID, 
			groupName as vcGroupName,
			actionDate as dtActionDate,
			actionID as tiActionID 
			from ContractGroupView
			order by groupName
	else 		 
		select groupID as iGroupID, 
			groupName as vcGroupName, 
			[Description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
			from ContractGroupView
			where groupID = @iGroupID

	set @error = @@error
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




CREATE   PROCEDURE dbo.usp_ContractType_Change 
	@iContractID int
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractTypeID int 
		select @iContractTypeID = contractTypeID from Contract where contractID = @iContractID

	if @iContractTypeID > 2 
		begin
			Raiserror('Can''t perform changes with this contract!',16,1)
		end

	Begin Transaction t1
		if @iContractTypeID = 1 
		--change the contract type from index to stock
			begin
				delete from IndexCorrelation where indexID1 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexCorrelation where indexID2 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexDataHistory where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexDefinition where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from SyntheticRootIndexBeta where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from [Index] where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				insert into Stock(stockID) values(@iContractID)
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			end
		else if @iContractTypeID = 2 
		--change the contract type from stock to index
			begin
				delete from StockDataHistory where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from StockCorrelation where stockID1 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from StockCorrelation where stockID2  = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from IndexDefinition where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from Stock where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				insert into [Index](indexID) values(@iContractID)
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			end
			--change the contractType

			if @iContractTypeID = 1 
				set @iContractTypeID = 2 
			else 
				set @iContractTypeID = 1

			update Contract 
				set contractTypeID = @iContractTypeID 
				where contractID = @iContractID
			if @@error != 0 begin if @@TranCount != 0 Rollback Tran t1 goto finish end

	Commit Transaction t1

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




CREATE    Procedure dbo.usp_Index_Del
		@iIndexID int
  as
/*
	Created by Banderlog 
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	 if exists (select tradeID from Trade
					 where Trade.contractID = @iIndexID)
	 begin
	   RAISERROR ('Error. Can''t delete [Index] because of the existing Trade', 16, 1)
	   RETURN (-1)
	 end

	Begin Transaction IndexDel

		delete from IndexCorrelation where indexID1 = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexCorrelation where indexID2 = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexDataHistory where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update IndexDefinition set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update ContractInGroup set actionID=3 where contractID = @iIndexID 
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update [Index] set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update [Option] set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update Contract set actionID=3 
			where contractID in 
				(select OptionID 
					from [Option] O 
							join OptionRoot ORt on O.optionRootID=ORt.optionRootID and ORt.underlyingID=@iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticUnderlyingParam set actionID=3 where syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update OptionRoot set actionID = 3 where underlyingID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update Contract set actionID=3 where contractID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end
		
		set @error = @@error
	Commit Transaction IndexDel
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

CREATE                 PROCEDURE dbo.usp_OptionRootCA_Save 
	@iCorpActionID int, 
	@iOptionRootID int,
	@vcSymbol varchar(20) = null,
	@iUnderlyingID int = null,
	@iLotSize int = null,
	@tiIsSynthetic tinyint = 0,
	@fCashValue float = null
AS
/*
	Created by: Ramhadep Ibn Portvein
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @vcTableName sysname

	--===============================================
	-- variables for work with CorporateActionLog
	--================================================
	declare @iCAActionID int, --1 for insert, 2 - for update, 3 - for delete
			@vcKeyRow varchar(50),  -- name of field with identity atribute
			@iRowID int,			-- tipically is identity value
			@vcFieldName varchar(50),
			@vcFieldValue varchar(250),
			@vcFieldType varchar(50),
			@iOldUnderlyingID int,
			@vcOldSymbol varchar(20),
			@tiOldisSynthetic tinyint,
			@iOldLotSize int,
			@vcLogMessage varchar(250)

	if object_ID('tempdb..#CAStatusLog') is null
		begin	
			create table #CAStatusLog(
						logMessage varchar(250) null)
			if @@error <> 0 begin set @error = 1 goto finish end
		end

	Begin Transaction CA1
		--======================================================
		-- write into CorporateActionLog update of OptionRoot
		--======================================================
		select 	@iOldUnderlyingID = underlyingID,
				@vcOldSymbol = symbol,
				@tiOldisSynthetic = isSynthetic,
				@iOldLotSize = lotSize
			from OptionRootView 
			where optionRootID = @iOptionRootID

		update OptionRoot 
			set symbol = isnull(@vcSymbol, symbol),
				underlyingID = isnull(@iUnderlyingID, underlyingID),
				isSynthetic = isnull(@tiIsSynthetic,isSynthetic),
				lotSize = isnull(@iLotSize, lotSize)
		where optionRootID = @iOptionRootID and actionID < 3
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

		set @vcLogMessage = 'Update ''' + @vcOldSymbol + ''' Option Root : SET (Symbol, Is Synthetic,Lot Size) = (''' + @vcSymbol +  ''',' + 
			convert(varchar(5),@tiIsSynthetic) + ',' + convert(varchar(25),@iLotSize) + ')' 

		insert into #CAStatusLog(logMessage) values(@vcLogMessage)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

		set @iCAActionID = 2
		set @vcKeyRow = 'optionRootID'
		set @iRowID = @iOptionRootID
		set @vcTableName = 'OptionRoot'

		
		select @vcFieldName = 'symbol', @vcFieldValue = @vcOldSymbol, @vcFieldType = 'varchar(20)'
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow 
   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

		select @vcFieldName = 'underlyingID', @vcFieldValue = @iOldUnderlyingID, @vcFieldType = 'int'
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

		select @vcFieldName = 'isSynthetic', @vcFieldValue = @tiOldisSynthetic, @vcFieldType = 'tinyint'
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

		select @vcFieldName = 'lotSize', @vcFieldValue = @iOldLotSize, @vcFieldType = 'int'
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

		if @tiIsSynthetic = 1
			begin
				if not exists(select * From SyntheticOptionRootView where optionRootID = @iOptionRootID)
					begin
						insert into SyntheticOptionRoot(
							optionRootID,
							cashValue)
						values (

							@iOptionRootID,
							@fCashValue)
			   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA1 goto finish end
						set @iRowID = @@identity

						--================================================================
						-- write into CorporateActionLog insert into SyntheticOptionRoot
						--================================================================
						set @iCAActionID = 1
						set @vcTableName = 'SyntheticOptionRoot'
						set @vcKeyRow = 'syntheticOptRootID'

						select @vcFieldName = 'optionRootID', @vcFieldValue = @iOptionRootID, @vcFieldType = 'int'
						exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
					end
				else
					begin
						declare @fOldCashValue float
						
						select 	@iRowID = syntheticOptRootID,
								@fOldCashValue = cashValue
							from SyntheticOptionRootView
							where optionRootID = @iOptionRootID
	

						update SyntheticOptionRoot set
							cashValue = isnull(@fCashValue, cashValue)
						where optionRootID = @iOptionRootID and actionID < 3		
				   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end
						--=================================================================
						-- write into CorporateActionLog update of SyntheticOptionRoot
						--=================================================================
						set @iCAActionID = 2
						set @vcTableName = 'SyntheticOptionRoot'
						set @vcKeyRow = 'syntheticOptRootID'					

						select @vcFieldName = 'cashValue', @vcFieldValue = @fOldCashValue, @vcFieldType = 'float'
						exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

				end

			end
		--we delete unnecessary record
		else
			begin
				--=========================================================================
				-- write into CorporateActionLog update of SyntheticUnderlyingParam table
				--=========================================================================
				set @iCAActionID = 3
				--set @iRowID = @@identity
				set @vcTableName = 'SyntheticUnderlyingParam'
				set	@vcKeyRow = 'syntheticUndParamID'
				
				declare	@iSyntheticUndParamID int,
					@iSyntheticOptRootID int,
					@fSyntheticCoeff float

				DECLARE synthundparam_cursor CURSOR
				READ_ONLY
				FOR select
						S1.syntheticUndParamID,
						S1.syntheticOptRootID,
						S1.underlyingID,
						S1.syntheticCoeff
					from SyntheticUnderlyingParamView S1
							join SyntheticOptionRootView S2	on S1.syntheticOptRootID = S2.syntheticOptRootID
					where S2.optionRootID = @iOptionRootID

				Open synthundparam_cursor
					fetch next from synthundparam_cursor into 
						@iSyntheticUndParamID,
						@iSyntheticOptRootID,
						@iOldUnderlyingID,
						@fSyntheticCoeff
					
				while (@@fetch_status <> -1)
				begin
					if (@@fetch_status <> -2)
					BEGIN

					--============================================
					-- execute update of SyntheticUnderlyingParam
					--============================================
					update SyntheticUnderlyingParam
						set actionID = 3 
						where syntheticUndParamID = @iSyntheticUndParamID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

					set @iRowID = @iSyntheticUndParamID

					-- write into CorporateActionLog
					select @vcFieldName = 'syntheticUndParamID', @vcFieldValue = @iSyntheticUndParamID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'syntheticOptRootID', @vcFieldValue = @iSyntheticOptRootID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'underlyingID', @vcFieldValue = @iOldUnderlyingID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'syntheticCoeff', @vcFieldValue = @fSyntheticCoeff, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					END
					fetch next from synthundparam_cursor into 
						@iSyntheticUndParamID,
						@iSyntheticOptRootID,
						@iOldUnderlyingID,
						@fSyntheticCoeff
				end
				Close synthundparam_cursor
				Deallocate synthundparam_cursor

				--=========================================================================
				-- write into CorporateActionLog update of SyntheticRootIndexBeta table
				--=========================================================================
				set @iCAActionID = 3
				set @vcTableName = 'SyntheticRootIndexBeta'
				set	@vcKeyRow = 'syntheticUndParamID'
				
				declare	@iSyntheticIndexBetaID int,
					@fBeta float

				DECLARE synthindexbeta_cursor CURSOR
				READ_ONLY
				FOR select
						S1.syntheticIndexBetaID,
						S1.syntheticOptRootID,
						S1.indexID,
						S1.beta
					from SyntheticRootIndexBetaView S1
							join SyntheticOptionRootView S2	on S1.syntheticOptRootID = S2.syntheticOptRootID
					where S2.optionRootID = @iOptionRootID

				Open synthindexbeta_cursor
					fetch next from synthindexbeta_cursor into 
						@iSyntheticIndexBetaID,
						@iSyntheticOptRootID,
						@iOldUnderlyingID,
						@fBeta
					
				while (@@fetch_status <> -1)
				begin
					if (@@fetch_status <> -2)
					BEGIN

					--============================================
					-- execute update of SyntheticRootIndexBeta
					--============================================
					update SyntheticRootIndexBeta
						set actionID = 3 
						where syntheticIndexBetaID = @iSyntheticIndexBetaID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

					set @iRowID = @iSyntheticIndexBetaID

					-- write into CorporateActionLog
					select @vcFieldName = 'syntheticIndexBetaID', @vcFieldValue = @iSyntheticIndexBetaID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'syntheticOptRootID', @vcFieldValue = @iSyntheticOptRootID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'indexID', @vcFieldValue = @iOldUnderlyingID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'beta', @vcFieldValue = @fBeta, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					END
					fetch next from synthindexbeta_cursor into 
						@iSyntheticUndParamID,
						@iSyntheticOptRootID,
						@iOldUnderlyingID,
						@fSyntheticCoeff
				end
				Close synthindexbeta_cursor
				Deallocate synthindexbeta_cursor

				--=========================================================================
				-- write into CorporateActionLog update of SyntheticOptionRoot table
				--=========================================================================
				declare	@fSynthSkew float,
					@fSynthKurt float,
					@fSynthYield float,
					@tiSynthIsBasket tinyint

				set @iCAActionID = 3
				set @vcTableName = 'SyntheticOptionRoot'
				set @vcKeyRow = 'syntheticOptRootID'

				if exists ( select * from SyntheticOptionRootView where optionRootID = @iOptionRootID)
				begin
					select 	@iSyntheticOptRootID = syntheticOptRootID, 
						@fSynthSkew = synthSkew,
						@fSynthYield = synthYield,
						@tiSynthIsBasket = synthIsBasket,
						@fOldCashValue = cashValue 
					from SyntheticOptionRootView where optionRootID = @iOptionRootID

					set @iRowID = @iSyntheticOptRootID
					--============================================
					-- execute update of SyntheticOptionRoot
					--============================================
					update SyntheticOptionRoot set actionID = 3 where actionID < 3 and syntheticOptRootID = @iSyntheticOptRootID
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end
					
	
					select @vcFieldName = 'syntheticOptRootID', @vcFieldValue = @iSyntheticOptRootID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'optionRootID', @vcFieldValue = @iOptionRootID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'synthSkew', @vcFieldValue = @fSynthSkew, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'synthYield', @vcFieldValue = @fSynthYield, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'synthIsBasket', @vcFieldValue = @tiSynthIsBasket, @vcFieldType = 'tinyint'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'cashValue', @vcFieldValue = @fOldCashValue, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
		
				end
			end

			if @vcSymbol is not null
			begin
				--=========================================================================
				-- Rename all options of current OptionRoot
				--=========================================================================
				set @iCAActionID = 2
				set @vcTableName = 'Contract'
				set @vcKeyRow = 'contractID'
						
				DECLARE contract_cursor CURSOR
				READ_ONLY
				FOR select
					C.contractID,
					C.symbol
					from ContractView C
						join OptionView O on C.contractID = O.optionID
						where O.optionRootID = @iOptionRootID
	
					Open contract_cursor
					fetch next from contract_cursor into 
						@iRowID,
						@vcOldSymbol
						
					while (@@fetch_status <> -1)
				begin
					if (@@fetch_status <> -2)
					BEGIN

					--============================================
					-- execute update of Contract
					--============================================
					update Contract
						set symbol = @vcSymbol + right(symbol,2) 
						where contractID = @iRowID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end
			
					set @vcLogMessage = 'Update ''' + @vcOldSymbol + ''' Option : SET symbol = ''' + @vcSymbol + right(@vcOldSymbol,2) + ''''

					insert into #CAStatusLog(logMessage) values(@vcLogMessage)
						if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

					-- write into CorporateActionLog
					select @vcFieldName = 'symbol', @vcFieldValue = @vcOldSymbol, @vcFieldType = 'varchar(20)'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					END
					fetch next from contract_cursor into 
						@iRowID,
						@vcOldSymbol
				end
				Close contract_cursor
				Deallocate contract_cursor
			end

Commit Transaction CA1
set @error = 0

finish:

if @error=0
begin
	select logMessage as vcLogMessage from #CAStatusLog 
end

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

CREATE            PROCEDURE dbo.usp_OptionRoot_Save 
	@iRootID int, 
	@vcSymbol varchar(20) = null,
	@iLotSize int = null,
	@tiIsSynthetic tinyint = 0,
	@fCashValue float = null,
	@fSyntheticSkew float = null,
	@fSyntheticKurt float = null,
	@fSyntheticYield float = null,
	@tiSyntheticIsBasket tinyint = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into OptionRoot table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if not exists(select * From OptionRoot where optionRootID = @iRootID) 
		begin
			Raiserror('OptionRoot with this RootID didn''t exists in DB!',16,1)
			Return(-1)
		end 

	Begin Transaction
		update OptionRoot set 
			symbol = isnull(@vcSymbol, symbol),
			lotSize = isnull(@iLotSize, lotSize),
			isSynthetic = isnull(@tiIsSynthetic, isSynthetic)
		where optionRootID = @iRootID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		if @vcSymbol is not null
		begin
			update Contract
				set symbol = @vcSymbol + right(C.symbol,2)
				from Contract C join OptionView O on C.contractID=O.optionID
					where O.optionRootID = @iRootID and C.actionID < 3
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end


		if @tiIsSynthetic = 1
			begin
				if not exists(select * From SyntheticOptionRootView where optionRootID = @iRootID)
					insert into SyntheticOptionRoot (optionRootID,cashValue,synthSkew,synthKurt,synthYield,synthIsBasket)
					values (
						@iRootID,
						@fCashValue,
						@fSyntheticSkew,
						@fSyntheticKurt,
						@fSyntheticYield,
						@tiSyntheticIsBasket)
	
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				else
					update SyntheticOptionRoot set
						cashValue = isnull(@fCashValue, cashValue),
						synthSkew = isnull(@fSyntheticSkew, synthSkew),
						synthKurt = isnull(@fSyntheticKurt, synthKurt),
						synthYield = isnull(@fSyntheticYield, synthYield),
						synthIsBasket = isnull(@tiSyntheticIsBasket, synthIsBasket)
					where optionRootID = @iRootID		

		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update SyntheticUnderlyingParam
					set actionID = 3 
					from SyntheticUnderlyingParam join SyntheticOptionRootView 
					on SyntheticOptionRootView.syntheticOptRootID = SyntheticUnderlyingParam.syntheticOptRootID
					where SyntheticOptionRootView.optionRootID = @iRootID

				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

				update SyntheticRootIndexBeta
					set actionID = 3 
					from SyntheticRootIndexBeta join SyntheticOptionRootView 
					on SyntheticOptionRootView.syntheticOptRootID = SyntheticRootIndexBeta.syntheticOptRootID
					where SyntheticOptionRootView.optionRootID = @iRootID

				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

				update SyntheticOptionRoot set actionID = 3 where optionRootID = @iRootID
		   		
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction
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


CREATE              Proc dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0,
	@bisRemoveFromContract bit = 1
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end

	Begin Transaction DelOpt
		delete from TradeLog where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update ContractExchangeLink set actionID = 3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from DualQuadraticParams where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update ContractInGroup set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaBands where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update SyntheticUnderlyingParam  set actionID=3 where syntheticOptRootID not in 
			(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update SyntheticRootIndexBeta  set actionID=3 where syntheticOptRootID not in 
			(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update OptionRoot set actionID = 3 where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end


		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		if @bisRemovePermanent = 1
			begin
				delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				if @bisRemoveFromContract = 1 
					begin
						delete Contract where contractID in (select OptionID from #Options_for_delete)
							if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
					end

				delete from SyntheticUnderlyingParam where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticRootIndexBeta where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from OptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
			end

	Commit Transaction DelOpt

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


CREATE       Procedure dbo.usp_ReportMarketStructure_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------

	declare @dtToday datetime
	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	declare @error int		set @error = -1

	declare	@iLastExpiryID int
		select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= @dtToday and expCalendarID = 1 order by expiryID desc

	create table #FilteredStocks(stockID int)

	if @iStockID is null 
		begin
			insert into #FilteredStocks exec usp_FilterData_Get @iFilterID
		end

if @iStockID is not null and @iFilterID is null
	select
		ORt.underlyingID as iUnderlyingContractID,
		ORt.optionRootID as iRootID,
		ORt.isSynthetic as tiIsSynthetic,
		SoRt.synthSkew as fSyntheticSkew,
		SoRt.synthKurt as fSyntheticKurt,
		ORt.symbol as vcRootSymbol,
		SoRt.cashValue as fCashAmount,
		SoRt.synthIsBasket as tiSyntheticIsBasket,
		SoRt.synthYield as fSyntheticYield,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join OptionRoot ORt on ORt.optionRootID = O.optionRootID
				left join SyntheticOptionRootView SoRt on SoRt.optionRootID = O.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	O.expiryID > @iLastExpiryID and 
				ORt.underlyingID=@iStockID

else if @iStockID is not null and @iFilterID is not null
	select
		ORt.underlyingID as iUnderlyingContractID,
		ORt.optionRootID as iRootID,
		ORt.isSynthetic as tiIsSynthetic,
		SoRt.synthSkew as fSyntheticSkew,
		SoRt.synthKurt as fSyntheticKurt,
		ORt.symbol as vcRootSymbol,
		SoRt.cashValue as fCashAmount,
		SoRt.synthIsBasket as tiSyntheticIsBasket,
		SoRt.synthYield as fSyntheticYield,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join OptionRoot ORt on ORt.optionRootID = O.optionRootID
				left join SyntheticOptionRootView SoRt on SoRt.optionRootID = O.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	O.expiryID > @iLastExpiryID and
				underlyingID = @iStockID and 
				ORt.underlyingID in (select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is not null
	select
		ORt.underlyingID as iUnderlyingContractID,
		ORt.optionRootID as iRootID,
		ORt.isSynthetic as tiIsSynthetic,
		SoRt.synthSkew as fSyntheticSkew,
		SoRt.synthKurt as fSyntheticKurt,
		ORt.symbol as vcRootSymbol,
		SoRt.cashValue as fCashAmount,
		SoRt.synthIsBasket as tiSyntheticIsBasket,
		SoRt.synthYield as fSyntheticYield,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join OptionRoot ORt on ORt.optionRootID = O.optionRootID
				left join SyntheticOptionRootView SoRt on SoRt.optionRootID = O.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where O.expiryID > @iLastExpiryID and
			ORt.underlyingID in (Select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is null
	select
		ORt.underlyingID as iUnderlyingContractID,
		ORt.optionRootID as iRootID,
		ORt.isSynthetic as tiIsSynthetic,
		SoRt.synthSkew as fSyntheticSkew,
		SoRt.synthKurt as fSyntheticKurt,
		ORt.symbol as vcRootSymbol,
		SoRt.cashValue as fCashAmount,
		SoRt.synthIsBasket as tiSyntheticIsBasket,
		SoRt.synthYield as fSyntheticYield,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join OptionRoot ORt on ORt.optionRootID = O.optionRootID
				left join SyntheticOptionRootView SoRt on SoRt.optionRootID = O.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where O.expiryID > @iLastExpiryID

	if object_id('tempdb..#FilteredStocks') is not null
		drop table #FilteredStocks

	set @error = @@error
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

CREATE        proc dbo.usp_Stock_Del
        @iStockID int,
        @bisRemovePermanent bit = 0
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Stock table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	if exists (select * from TradeView(nolock) where contractID = @iStockID)
	begin
		Raiserror ('Error. Can''t delete Stock because of the existing Trade', 16, 1)
		Return (-1)
	end 


	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iStockID

	if exists (select tradeID from TradeView where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Error. Can''t delete Stock because of the existing Trade by Stock Options', 16, 1)
			Return (-1)
		end 

	Begin Transaction
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = @bisRemovePermanent
			end

		exec usp_TradeLog_Del null, @iStockID

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
		delete VolaBands where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
		update ContractExchangeLink set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

        delete ContractPrice where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

		update SyntheticUnderlyingParam set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

		update OptionRoot set actionID = 3 where underlyingID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

		if (@bisRemovePermanent = 1) 
			begin
        		delete from ContractInGroup where contractID = @iStockID 
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
				delete from ContractExchangeLink where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
	        	delete IndexDefinition where stockID = @iStockID  
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

				delete SyntheticUnderlyingParam where syntheticOptRootID in
					(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

				delete SyntheticRootIndexBeta where syntheticOptRootID in
					(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

				delete SyntheticOptionRoot where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iStockID)
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end

				delete OptionRoot where underlyingID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
				delete TraderContract where contractID = @iStockID  
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
		       	delete Stock where stockID = @iStockID  
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
	        	delete Contract where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction DelOpt goto finish end
			end 
  
		set @error = @@error
	Commit Transaction
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




CREATE       PROCEDURE dbo.usp_SyntheticIndexBeta_Del 
	@iOptRootID int, 
	@iIndexID int = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is delete information from SyntheticRootIndexBeta table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


		update SyntheticRootIndexBeta 
			set actionID = 3 
			from SyntheticRootIndexBeta SIB join SyntheticOptionRootView SOR
			on SOR.syntheticOptRootID = SIB.syntheticOptRootID
			where SOR.optionRootID = @iOptRootID and (SIB.indexID = @iIndexID or @iIndexID is null )

		if @@error <> 0 begin set @error = 2 goto finish end

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



CREATE        PROCEDURE dbo.usp_SyntheticIndexBeta_Get 
	@iOptRootID int
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is getting information from SyntheticRootIndexBeta table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select 	
		SOR.optionRootID as iOptRootID,
		SIB.indexID as iIndexID, 
		SIB.beta as fBeta
	from SyntheticRootIndexBetaView as SIB
	join SyntheticOptionRootView as SOR
	on SOR.syntheticOptRootID = SIB.syntheticOptRootID
	where SOR.optionRootID = @iOptRootID

	set @error = @@error
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

CREATE    PROCEDURE dbo.usp_SyntheticIndexBeta_Save 
	@iOptRootID int, 
	@iIndexID int,
	@fBeta float
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into SyntheticRootIndexBeta table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iSyntheticOptRootID int
		select @iSyntheticOptRootID = syntheticOptRootID 
			from SyntheticOptionRootView where optionRootID = @iOptRootID


	if not exists(select * From SyntheticRootIndexBetaView where syntheticOptRootID = @iSyntheticOptRootID and indexID = @iIndexID) 
		begin
			insert into SyntheticRootIndexBeta (
						syntheticOptRootID, 
						indexID, 
						beta)
				values (@iSyntheticOptRootID,
						@iIndexID,
						@fBeta)
			if @@error <> 0 begin set @error = 1 goto finish end
		end
	else
		begin
			update SyntheticRootIndexBeta set 
				beta = isnull(@fBeta, beta)
			where syntheticOptRootID = @iSyntheticOptRootID and indexID=@iIndexID

   			if @@error <> 0 begin set @error = 2 goto finish end
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


CREATE        PROCEDURE dbo.usp_SyntheticParams_Get 
	@iRootID int,
	@bIsWithPrice bit = 0
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is getting information from SyntheticUnderlyingParam table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @bIsWithPrice = 0
		select 	
			sor.optionRootID as iRootID,
			sup.underlyingID as iUnderlyingID, 
			sup.SyntheticCoeff as fWeight
		from SyntheticUnderlyingParamView as sup
		join SyntheticOptionRootView as sor
		on sor.syntheticOptRootID = sup.syntheticOptRootID
		where sor.optionRootID = @iRootID
	else
		-- use NULL exchange only
		select 
			sor.optionRootID as iRootID,
			sup.underlyingID as iUnderlyingID, 
			sup.SyntheticCoeff as fWeight,
			A.iContractID as iContractID,  
			A.iContractTypeID as iContractTypeID,
			A.vcSymbol as vcSymbol,
			case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
			case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
			case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
			CP.priceClose as fPriceClose,
			A.tiCalcOptionType as tiCalcOptionType,
			A.tiCalcModelType as tiCalcModelType,
			A.tiExpCalendarID as tiExpCalendarID,
			A.tiIsBasket as tiIsBasket
		from SyntheticUnderlyingParamView sup join vALLStockEOD A			
			on A.iContractID = sup.underlyingID
			join SyntheticOptionRootView as sor
			on sor.syntheticOptRootID = sup.syntheticOptRootID
			join ContractPrice CP (nolock)
			on A.iContractID = CP.contractID where CP.exchangeID is null and sor.optionRootID = @iRootID

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

Create procedure dbo.usp_SyntheticRootIndexBetaHH_Get
	@dtActionDate datetime = null,
	@iUnderlyingContractID int
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select
		s1.syntheticIndexBetaID as iSyntheticIndexBetaID, 
		s1.syntheticOptRootID as iSyntheticOptRootID, 
		s1.indexID as iIndexID, 
		s1.beta as fBeta,
		s1.actionDate as dtActionDate,
		s1.actionID as tiActionID
	from SyntheticRootIndexBeta s1 
			join SyntheticOptionRoot s2 on s1.syntheticOptRootID = s2.syntheticOptRootID
			join OptionRoot ort on s2.optionRootID = ort.optionRootID
	where (s1.actionDate > @dtActionDate or @dtActionDate is null) and 
			ort.underlyingID = @iUnderlyingContractID
	order by s1.actionDate

Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE     proc dbo.usp_UnderlyingByTrader_Get
	@iTraderID int = null,
	@tiTraderMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
AS
	set nocount on

	if isnull(@iTraderID, 0) = 0
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			s.calcOptionType as tiCalcOptionType,
			s.isHTB as tiIsHTB,
		
		    null as fYield,
			null as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			null as iTraderID,

			s.skew as fSkew,
			s.kurt as fKurt,
		    c.contractName as vcContractName,
			
			case when exists(select * from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
					then 1 else 0 end tiHasSyntheticRoots

		from ContractView c 
		    inner join StockView s on c.contractID = s.stockID
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 2
		union
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			i.calcOptionType as tiCalcOptionType,
			i.isHTB as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    null as iDivFreq,
		    null as dtDivDate,
		    null as fDivAmt,
		
		    null as tiIsDivCustom,
		    null as iDivFreqCustom,
		    null as dtDivDateCustom,
		    null as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			null as iTraderID,

			i.skew as fSkew,
			i.kurt as fKurt,
		    c.contractName as vcContractName,

			case when exists(select * from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
					then 1 else 0 end tiHasSyntheticRoots

		from ContractView c 
		    inner join IndexView i on c.contractID = i.indexID
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 1
		order by c.symbol	 
	else
	begin
		if isnull(@tiTraderMode, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt,
			    c.contractName as vcContractName,
	
				case when exists(select * from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
						then 1 else 0 end tiHasSyntheticRoots

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt,
			    c.contractName as vcContractName,
		
				case when exists(select * from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
					then 1 else 0 end tiHasSyntheticRoots

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt,
			    c.contractName as vcContractName,

				case when exists(select * from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
						then 1 else 0 end tiHasSyntheticRoots

			from ContractView c 
			    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt,
			    c.contractName as vcContractName,
	
				case when exists(select * from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
						then 1 else 0 end tiHasSyntheticRoots

			from ContractView c 
			    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
			order by c.symbol	 
	end

Return (@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   proc dbo.usp_UnderlyingOptionByExpiry_Get
	@iUnderlyingID int,		 
	@iExpiryID int
AS

	set nocount on

	select
		o.optionID as iContractID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		o.isCall as tiIsCall,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,
		ort.lotSize as iLotSize
	from OptionRootView ort
		inner join OptionView o on ort.optionRootID = o.optionRootID
		inner join ContractView c on o.optionID = c.contractID
	where
		ort.underlyingID = @iUnderlyingID
		and o.expiryID = @iExpiryID

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.39.047')
	begin
		update DataInfo set keyValue = '3.39.047' where dataInfoID = 1
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
	PRINT 'The database update succeeded'
	PRINT 'DB VERSION WAS CHANGED TO 3.39.047'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
