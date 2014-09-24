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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Commodity]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Commodity]
GO

CREATE TABLE [Commodity] (
	[CommodityID] 		[int] NOT NULL ,
	[GreeksCalculationType] [tinyint] NULL ,
	[IsActive] 		[tinyint] NULL CONSTRAINT [DF_Commodity_IsActive] DEFAULT (1),
	CONSTRAINT [PK_Commodity] PRIMARY KEY  CLUSTERED 
	(
		[CommodityID]
	)  ON [PRIMARY] ,
	CONSTRAINT [FK_Commodity_Contract] FOREIGN KEY 
	(
		[CommodityID]
	) REFERENCES [Contract] 
	(
		[contractID]
	)
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodCommodity_Get]
GO

CREATE PROC dbo.usp_EodCommodity_Get
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
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodCommodityFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodCommodityFutureRoot_Get]
GO

CREATE  PROC dbo.usp_EodCommodityFutureRoot_Get
AS
	-- retrieve all future roots

	set nocount on

	SELECT     fr.underlyingID AS iIndexID, 
		   fr.futureRootID AS iFutureRootID, 
		   fr.futureRootSymbol AS vcFutureRootSymbol, 
	           fr.futureRootName AS vcFutureRootName
	FROM         dbo.FutureRoot fr INNER JOIN
	                      dbo.Contract c ON fr.underlyingID = c.contractID
	WHERE     (c.contractTypeID = 7)


GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureRootWOCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodFutureRootWOCommodity_Get]
GO
CREATE  PROC dbo.usp_EodFutureRootWOCommodity_Get
AS
	-- retrieve all future roots

	set nocount on

	SELECT     fr.underlyingID AS iIndexID, 
		   fr.futureRootID AS iFutureRootID, 
		   fr.futureRootSymbol AS vcFutureRootSymbol, 
	           fr.futureRootName AS vcFutureRootName
	FROM         dbo.FutureRoot fr INNER JOIN
	                      dbo.Contract c ON fr.underlyingID = c.contractID
	WHERE     (c.contractTypeID < 6)


GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodInterest_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodInterest_Get]
GO
create   PROC dbo.usp_EodInterest_Get
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


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodInterestFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodInterestFutureRoot_Get]
GO
CREATE  PROC dbo.usp_EodInterestFutureRoot_Get
AS
	-- retrieve all future roots

	set nocount on

	SELECT     fr.underlyingID AS iIndexID, 
		   fr.futureRootID AS iFutureRootID, 
		   fr.futureRootSymbol AS vcFutureRootSymbol, 
	           fr.futureRootName AS vcFutureRootName
	FROM         dbo.FutureRoot fr INNER JOIN
	                      dbo.Contract c ON fr.underlyingID = c.contractID
	WHERE     (c.contractTypeID = 6)



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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.165')
	begin
		update DataInfo set keyValue = '4.49.165' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.165' '4.49.165 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
