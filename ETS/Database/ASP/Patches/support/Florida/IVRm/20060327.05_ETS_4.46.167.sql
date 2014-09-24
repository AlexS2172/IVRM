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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoCommodity_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoCommodity_Del]
GO

	CREATE  PROC dbo.usp_BoCommodity_Del
		@iCID int,
		@tiIsRaiseError tinyint = 1 
	AS
		-- delete future (with all future options)
	
		set nocount on
		
		declare @error int		set @error = 0
	
		-- check future ID
		if @iCID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Commodity to delete is not specified!', 16, 1)
			set @error = -1
			goto finish
		end
	
		-- create temporary table with contracts to delete
		create table #FutureContractsForDelete(contractID int)
	
		insert into #FutureContractsForDelete
			select @iCID
			union select futureOptionID
			from FutureOption where futureID = @iCID
	
	
		-- check for trades existence
		if exists(select 1 from Trade where contractID in (select contractID from #FutureContractsForDelete)
											or futureID = @iCID)
		begin
			if @tiIsRaiseError = 1 raiserror('Can''t remove future, because it (or its options) has trades!', 16, 1)
			set @error = -2
			goto finish
		end
	
		-- check for orders existence
		if exists(select 1 from [Order] where contractID in (select contractID from #FutureContractsForDelete))
		begin
			if @tiIsRaiseError = 1 raiserror('Can''t remove future, because it (or its options) has orders!', 16, 1)
			set @error = -3
			goto finish
		end
	
		begin tran
		
	
		-- delete future options
		delete FutureOption
		where futureID = @iCID
		if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end
	
		-- delete future
		delete Future where
		futureID = @iCID
		if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end
	
		-- delete future roots
		delete FutureRoot where
		UnderlyingID = @iCID
		if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end
	
		-- delete future roots
		delete Commodity where
		CommodityID = @iCID
		if @@error <> 0 begin set @error = -10 if @@trancount = 1 rollback tran goto finish end
	
		-- delete common future and future options
		delete ContractPrice
		where contractID in (select contractID from #FutureContractsForDelete)
		if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end
	
		update ContractExchangeLink set actionID = 3
		where contractID = @iCID
		if @@error <> 0 begin set @error = -7 if @@trancount = 1 rollback tran goto finish end
	
		update Contract set actionID = 3
		where contractID in (select contractID from #FutureContractsForDelete)
		if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end
	
		commit tran
	
	finish:
		return @error
	
	
	
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoCommodity_Get]
GO

	
	
	CREATE  PROC dbo.usp_BoCommodity_Get
		@iInterestID int = null
	AS
		-- retrieve future
	
		set nocount on
	
		if @iInterestID is not null
			SELECT 	c.GreeksCalculationType as GreeksCalcType,
				c.IsActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.CommodityID as iContractID 
			FROM         dbo.Commodity C INNER JOIN
		                      dbo.ContractView CV ON C.CommodityID = CV.contractID
			WHERE 	CV.ContractID =  @iInterestID And CV.ContractTypeID = 7
			Order by CV.Symbol
	
		else
		
			SELECT 	c.GreeksCalculationType as GreeksCalcType,
				c.IsActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.CommodityID as iContractID 
			FROM         dbo.Commodity C INNER JOIN
		                      dbo.ContractView CV ON C.CommodityID = CV.contractID
			WHERE 	CV.ContractTypeID = 7
			Order by CV.Symbol
	
	
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ICFutures_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ICFutures_Get]
GO
	
	CREATE  PROC dbo.usp_ICFutures_Get
		@iICID int = null
	AS
		-- retrieve future
	
		set nocount on
	
		if @iICID is not null
		SELECT     TOP 100 PERCENT dbo.ExpiryInCalendarView.expiryDate AS MaturityDate, dbo.Commodity.CommodityID AS CommodityID, 
	                      dbo.FutureRoot.futureRootName AS FutureRootName, dbo.FutureRoot.futureLotSize AS FutureLotSize, 
	                      dbo.FutureRoot.futureRootSymbol AS FutureRootSymbol, dbo.Contract.symbol AS vcSymbol, dbo.FutureRoot.optionLotSize AS OptioLotSize, 
	                      dbo.FutureRoot.exportSymbol AS FRExportSymbol, dbo.Contract.contractName AS FutureName, dbo.Future.futureRootID AS FutureRootID
		FROM         dbo.Future INNER JOIN
	                      dbo.ExpiryInCalendarView ON dbo.Future.maturityID = dbo.ExpiryInCalendarView.expiryID INNER JOIN
	                      dbo.Contract ON dbo.Future.futureID = dbo.Contract.contractID RIGHT OUTER JOIN
	                      dbo.Commodity LEFT OUTER JOIN
	                      dbo.FutureRoot ON dbo.Commodity.CommodityID = dbo.FutureRoot.underlyingID ON dbo.Future.futureRootID = dbo.FutureRoot.futureRootID
		WHERE     (dbo.Commodity.CommodityID = @iICID)
		ORDER BY FutureName
	
	GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoInterest_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoInterest_Get]
GO
	
	CREATE  PROC dbo.usp_BoInterest_Get
		@iInterestID int = null
	AS
		-- retrieve future
	
		set nocount on
	
		if @iInterestID is not null
			SELECT 	c.GreeksCalculationType as GreeksCalcType,
				c.IsActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.CommodityID as iContractID 
			FROM         dbo.Commodity C INNER JOIN
		                      dbo.ContractView CV ON C.CommodityID = CV.contractID
			WHERE 	CV.ContractID =  @iInterestID And CV.ContractTypeID = 6
			Order by CV.Symbol
	
		else
		
			SELECT 	c.GreeksCalculationType as GreeksCalcType,
				c.IsActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.CommodityID as iContractID 
			FROM         dbo.Commodity C INNER JOIN
		                      dbo.ContractView CV ON C.CommodityID = CV.contractID
			WHERE 	CV.ContractTypeID = 6
			Order by CV.Symbol
	
	
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureIC_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FutureIC_Get]
GO
	
	
	Create  PROC dbo.usp_FutureIC_Get
		@iICID int = null
	AS
		-- retrieve future
		set nocount on
		if @iICID is not null
		SELECT     dbo.Future.futureID, dbo.Commodity.CommodityID
		FROM         dbo.FutureRoot INNER JOIN
		                      dbo.Future ON dbo.FutureRoot.futureRootID = dbo.Future.futureRootID INNER JOIN
		                      dbo.Contract ON dbo.FutureRoot.underlyingID = dbo.Contract.contractID INNER JOIN
		                      dbo.Commodity ON dbo.Contract.contractID = dbo.Commodity.CommodityID
		WHERE     (dbo.Future.futureID = @iICID)
	
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.167')
	begin
		update DataInfo set keyValue = '4.49.167' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.167' '4.49.167 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
