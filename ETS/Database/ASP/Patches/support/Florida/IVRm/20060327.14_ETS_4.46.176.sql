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
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Commodity]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Commodity]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_FutureUnderlying_FutureUnderlyingType]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[FutureUnderlying] DROP CONSTRAINT FK_FutureUnderlying_FutureUnderlyingType
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_FutureRoot_QuotationUnit]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[FutureRoot] DROP CONSTRAINT FK_FutureRoot_QuotationUnit
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FutureUnderlying]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[FutureUnderlying]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FutureUnderlyingType]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[FutureUnderlyingType]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[QuotationUnit]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[QuotationUnit]
GO

CREATE TABLE [dbo].[FutureUnderlying] (
	[iFutureUnderlyingID] [int] NOT NULL ,
	[iFutureUnderlyingTypeID] [int] NOT NULL ,
	[tiGreeksCalculationType] [tinyint] NOT NULL ,
	[tiActive] [tinyint] NOT NULL ,
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[FutureUnderlyingType] (
	[iFutureUnderlyingTypeID] [int] IDENTITY (1, 1) NOT NULL ,
	[vcName] [varchar] (255) COLLATE Latin1_General_CI_AS NOT NULL, 
	[vcDesc] [varchar] (255) COLLATE Latin1_General_CI_AS NULL
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[QuotationUnit] (
	[iQuotationUnitID] [int] IDENTITY (1, 1) NOT NULL ,
	[vcName] [varchar] (255) COLLATE Latin1_General_CI_AS NOT NULL ,
	[vcDesc] [varchar] (255) COLLATE Latin1_General_CI_AS NULL 
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[FutureUnderlying] WITH NOCHECK ADD 
	CONSTRAINT [PK_FutureUnderlying] PRIMARY KEY  CLUSTERED 
	(
		[iFutureUnderlyingID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[FutureUnderlyingType] WITH NOCHECK ADD 
	CONSTRAINT [PK_FutureUnderlyingType] PRIMARY KEY  CLUSTERED 
	(
		[iFutureUnderlyingTypeID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[QuotationUnit] WITH NOCHECK ADD 
	CONSTRAINT [PK_QuotationUnit] PRIMARY KEY  CLUSTERED 
	(
		[iQuotationUnitID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[FutureUnderlying] ADD 
	CONSTRAINT [FK_FutureUnderlying_Contract] FOREIGN KEY 
	(
		[iFutureUnderlyingID]
	) REFERENCES [dbo].[Contract] (
		[contractID]
	),
	CONSTRAINT [FK_FutureUnderlying_FutureUnderlyingType] FOREIGN KEY 
	(
		[iFutureUnderlyingTypeID]
	) REFERENCES [dbo].[FutureUnderlyingType] (
		[iFutureUnderlyingTypeID]
	)
GO



GO
if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='tiQuoteFormat')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [tiQuoteFormat] [tinyint] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='iQuotationUnitAmmount')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [iQuotationUnitAmmount] [int] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='iQuotationUnitID')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [iQuotationUnitID] [int] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='fQuoteDenomination')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [fQuoteDenomination] [float] NULL 
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='fTickSize')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [fTickSize] [float] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='fTickValue')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [fTickValue] [float] NULL
GO

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_FutureRoot_QuotationUnit]'))
ALTER TABLE [dbo].[FutureRoot] ADD 
	CONSTRAINT [FK_FutureRoot_QuotationUnit] FOREIGN KEY 
	(
		[iQuotationUnitID]
	) REFERENCES [dbo].[QuotationUnit] (
		[iQuotationUnitID]
	)
GO



ALTER PROC dbo.usp_BoCommodity_Get
		@iInterestID int = null
	AS
		-- retrieve future
	
		set nocount on
	
		if @iInterestID is not null
			SELECT 	c.tiGreeksCalculationType as GreeksCalcType,
				c.tiActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.iFutureUnderlyingID as iContractID,
				ecv.expCalendarName as expCalendarName,
				c.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID

			FROM         dbo.FutureUnderlying C INNER JOIN
		                      dbo.ContractView CV ON C.iFutureUnderlyingID = CV.contractID
				inner join ExpiryCalendarView ecv on cv.expCalendarID = ecv.expCalendarID
			WHERE 	CV.ContractID =  @iInterestID And CV.ContractTypeID = 6
			Order by CV.Symbol
	
		else
		
			SELECT 	c.tiGreeksCalculationType as GreeksCalcType,
				c.tiActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.iFutureUnderlyingID as iContractID,
				ecv.expCalendarName as expCalendarName,
				c.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID

			FROM         dbo.FutureUnderlying C INNER JOIN
		                      dbo.ContractView CV ON C.iFutureUnderlyingID = CV.contractID
				inner join ExpiryCalendarView ecv on cv.expCalendarID = ecv.expCalendarID
			WHERE 	CV.ContractTypeID = 6
			Order by CV.Symbol
GO

ALTER  PROC dbo.usp_BoFutureRoot_Get
	@iUnderlyingID int = null
AS
	-- retrieve future root(s) by underlying

	set nocount on

	if @iUnderlyingID is not null
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.symbol as vcUnderlyingSymbol,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID,
			fr.actionDate as dtActionDate,
			fr.tiQuoteFormat,
			fr.iQuotationUnitAmmount,
			fr.iQuotationUnitID,
			fr.fQuoteDenomination,
			fr.fTickSize,
			fr.fTickValue,
			qu.vcName as vcQuotationUnitName

		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
			left join QuotationUnit qu on fr.iQuotationUnitID = qu.iQuotationUnitID
		where
			fr.underlyingID = @iUnderlyingID
		order by
			fr.futureRootSymbol
	else
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.symbol as vcUnderlyingSymbol,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID,
			fr.actionDate as dtActionDate,
			fr.tiQuoteFormat,
			fr.iQuotationUnitAmmount,
			fr.iQuotationUnitID,
			fr.fQuoteDenomination,
			fr.fTickSize,
			fr.fTickValue,
			qu.vcName

		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
			left join QuotationUnit qu on fr.iQuotationUnitID = qu.iQuotationUnitID
		order by
			uc.symbol, fr.futureRootSymbol

GO
ALTER  PROC dbo.usp_BoFutureRoot_Save
	@iFutureRootID int out,
	@iUnderlyingID int, -- should be specified for new future root (ignored for update)
	@vcFutureRootSymbol varchar(20) = null,
	@vcFutureRootName varchar(255) = null,
	@iFutureLotSize int = null,
	@iOptionLotSize int = null,
	@vcExportSymbol varchar(20) = null,
	@iMatCalendarID int = null,
	@tiIsRaiseError tinyint = 1,
	@tiQuoteFormat  tinyint = null,
	@iQuotationUnitAmmount int = null,
	@iQuotationUnitID int = null,
	@fQuoteDenomination float = null,
	@fTickSize float = null,
	@fTickValue float = null

AS
	-- create/update future root

	set nocount on
	
	declare @error int		set @error = 0

	if @iFutureRootID is null -- new future root
	begin
		-- check underlying ID
		if @iUnderlyingID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying for new future root is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		-- check future root symbol
		set @vcFutureRootSymbol = ltrim(isnull(@vcFutureRootSymbol, ''))
		if len(@vcFutureRootSymbol) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future root is not specified!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future root
		if exists(select 1 from FutureRoot where futureRootSymbol = @vcFutureRootSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
			set @error = -3
			goto finish
		end

		set @vcExportSymbol = ltrim(isnull(@vcExportSymbol, ''))
		if len(@vcExportSymbol) > 0
		begin
			-- check export symbol
			if exists(select 1 from FutureRoot
					where exportSymbol = @vcExportSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future root with import ID ''%s'' is already exists!', 16, 1, @vcExportSymbol)
				set @error = -104
				goto finish
			end
		end

		-- correct lot sizes
		if isnull(@iFutureLotSize, 0) <= 0
			set @iFutureLotSize = 100

		if isnull(@iOptionLotSize, 0) <= 0
			set @iOptionLotSize = 1

		-- correct maturity calendar ID
		if isnull(@iMatCalendarID, 0) = 0
			set @iMatCalendarID = 1

		-- check maturity calendar ID
		if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
			set @error = -4
			goto finish
		end

		begin tran

		-- insert new future root
		insert into FutureRoot
			(underlyingID, futureRootSymbol, futureRootName,
			futureLotSize, optionLotSize, exportSymbol, matCalendarID, tiQuoteFormat,
			iQuotationUnitAmmount,
			iQuotationUnitID,
			fQuoteDenomination,
			fTickSize,
			fTickValue
)
		values
			(@iUnderlyingID, @vcFutureRootSymbol, @vcFutureRootName,
			@iFutureLotSize, @iOptionLotSize, @vcExportSymbol, @iMatCalendarID, @tiQuoteFormat,
			@iQuotationUnitAmmount,
			@iQuotationUnitID,
			@fQuoteDenomination,
			@fTickSize,
			@fTickValue
)
		if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

		set @iFutureRootID = SCOPE_IDENTITY()

		commit tran
		return @iFutureRootID
	end
	else
	begin

		-- correct future root name (prevent updates to empty string)
		if len(ltrim(@vcFutureRootSymbol)) = 0
			set @vcFutureRootSymbol = null

		-- check future root
		if @vcFutureRootSymbol is not null
		begin
			if exists(select 1 from FutureRoot where futureRootID <> @iFutureRootID and futureRootSymbol = @vcFutureRootSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
				set @error = -6
				goto finish
			end
		end

		-- check future root export symbol
		if @vcExportSymbol is not null
		begin
			set @vcExportSymbol = ltrim(@vcExportSymbol)
			
			if len(@vcExportSymbol) > 0 -- export symbol update
			begin
				-- check export symbol
				if exists(select 1 from FutureRoot
						where futureRootID <> @iFutureRootID and exportSymbol = @vcExportSymbol)
				begin
					if @tiIsRaiseError = 1 raiserror('Future root with import ID ''%s'' is already exists!', 16, 1, @vcExportSymbol)
					set @error = -104
					goto finish
				end
			end
		end
		
		-- correct future lot size (prevent updates to invalid lot size)
		if @iFutureLotSize <= 0
			set @iFutureLotSize = null

		-- correct option lot size (prevent updates to invalid lot size)
		if @iOptionLotSize <= 0
			set @iOptionLotSize = null

		-- correct maturity calendar ID (prevent updates to zero calendar)
		if @iMatCalendarID = 0
			set @iMatCalendarID = null

		-- check maturity calendar
		if @iMatCalendarID is not null
		begin
			if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
				set @error = -7
				goto finish
			end
		end

		begin tran

		-- update future root
		update FutureRoot
		set futureRootSymbol = isnull(@vcFutureRootSymbol, futureRootSymbol),
			futureRootName = isnull(@vcFutureRootName, futureRootName),
			futureLotSize = isnull(@iFutureLotSize, futureLotSize),
			optionLotSize = isnull(@iOptionLotSize, optionLotSize),
			exportSymbol = isnull(@vcExportSymbol, exportSymbol),
			matCalendarID = isnull(@iMatCalendarID, matCalendarID),
			tiQuoteFormat = isnull(@tiQuoteFormat, tiQuoteFormat),
			iQuotationUnitAmmount = isnull(@iQuotationUnitAmmount, iQuotationUnitAmmount),
			iQuotationUnitID = isnull(@iQuotationUnitID, iQuotationUnitID),
			fQuoteDenomination = isnull(@fQuoteDenomination, fQuoteDenomination),
			fTickSize = isnull(@fTickSize, fTickSize),
			fTickValue = isnull(@fTickValue, fTickValue)
		where
			futureRootID = @iFutureRootID
		if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

		commit tran
	end

finish:
	return @error
GO

-- save Interest or FutureUnderlying/fx
	
	ALTER   PROCEDURE dbo.usp_IC_Save
			@iICID int,
			@vcSymbol varchar(20) = null,
			@vcName varchar(255) = null,
			@iICType int,		
			@tiGreeksCalcType tinyint = null,
			@iFutureUnderlyingTypeID int,
			@tiIsActive tinyint = null,
			@tiCheckExistance tinyint = null,
			@iExpCalendarID int = null,
			@iUndPriceProfileID int = null output,
			@iOptPriceProfileID int = null output
	
	   as
		---------------
		set nocount on
		---------------
	--	if ((@iICType <> 6) And (@iICType <> 7 ))
	--		begin
	--			Raiserror ('Wrong Interest/FutureUnderlying type value', 16, 1)
	--			Return (-1)
	--		end
	
		if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID = @iICType and actionID < 3 and (@iICID <> contractID or @iICID is null))
			begin
				if isnull(@tiCheckExistance, 1) <> 0
					Raiserror ('Interest or FutureUnderlying with this ticker already exists!', 16, 1)
				Return (-1)
			end   
	
	   if (@iICID is Null)  
		 begin
			 
			 begin tran
				if @iUndPriceProfileID is null
				begin
					select
						@iUndPriceProfileID = priceProfileID
					from
						PriceProfileView
					where
						defaultID = 3
						and isOptionProfile = 0
				end
	
				if @iOptPriceProfileID is null
				begin
					select
						@iOptPriceProfileID = priceProfileID
					from
						PriceProfileView
					where
						defaultID = 1
						and isOptionProfile = 1
				end

			insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')
			    set @iExpCalendarID = scope_identity()	

	       	     insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
	                  values ( @iICType, @vcSymbol, @vcName, @iExpCalendarID, @iUndPriceProfileID, @iOptPriceProfileID)
	
				 select @iICID = @@identity			 
					 if (@@error <> 0)
						  begin
							RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
							ROLLBACK TRAN
							RETURN (-1)
						  end
				 insert into [FutureUnderlying] (
								iFutureUnderlyingID, 
								tiActive,
								tiGreeksCalculationType,
								iFutureUnderlyingTypeID)
						 values (
								@iICID, 
								@tiIsActive,
								@tiGreeksCalcType,
								@iFutureUnderlyingTypeID)
					 if (@@error <> 0)
						begin
						   RAISERROR ('Error. Can''t insert into [FutureUnderlying]!', 16, 1)
						   ROLLBACK TRAN
						   RETURN (-1)
						end 			
			 commit tran
		 end
	   else
		 begin
			begin tran
			   update Contract
					set symbol = isNull(@vcSymbol,symbol),
						contractName = isNull(@vcName,contractName),
						expCalendarID = isNull(@iExpCalendarID, expCalendarID),
						undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
						optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
					where contractID = @iICID
				if (@@error <> 0)
					 begin
						RAISERROR ('Error. Can''t update Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					 end
			  update [FutureUnderlying]
				   set	tiGreeksCalculationType = isnull(@tiGreeksCalcType, tiGreeksCalculationType),
						tiActive = isnull(@tiIsActive, tiActive),
						iFutureUnderlyingTypeID = isnull(@iFutureUnderlyingTypeID, iFutureUnderlyingTypeID)
				   where iFutureUnderlyingID = @iICID
			   if (@@error <> 0)
				   begin
					  RAISERROR ('Error. Can''t update [FutureUnderlying]!', 16, 1)
					  ROLLBACK TRAN
					  RETURN (-1)
				   end		   
			 commit tran
		 end
	
		--========================================================================
		-- make record with null exchange for this index
		--========================================================================
	--	if not exists(select * from ContractPrice where exchangeID is null and contractID = @iICID)
	--		begin
	--			insert into ContractPrice(contractID,exchangeID)
	--				values(@iICID,null)
	--			if (@@error <> 0) begin rollback tran return(-1) end           		
	--		end		
	
	if (@@error <> 0)
		return (-1)
	else	
		return @iICID
GO

GO
ALTER     PROC dbo.usp_ICFutures_Get
	@iICID int = null
AS
	-- retrieve future

	set nocount on

	if @iICID is not null
	SELECT 
		EIC.expiryDate    	AS MaturityDate, 
		Cm.iFutureUnderlyingID  AS CommodityID, 
		Fr.futureRootName 	AS FutureRootName, 
		Fr.futureLotSize 	AS FutureLotSize, 
		Fr.futureRootSymbol 	AS FutureRootSymbol,
		CF.symbol 		AS vcSymbol, 
		Fr.optionLotSize 	AS OptioLotSize, 
		Fr.exportSymbol 	AS FRExportSymbol, 
		CF.contractName 	AS FutureName, 
		F.futureRootID 		AS FutureRootID
	 from Future F
	inner Join FutureRoot FR on FR.futureRootID = F.futureRootID And Fr.underlyingID =  @iICID
	inner join FutureUnderlying  CM on Cm.iFutureUnderlyingID  =  @iICID	
	inner join Contract   CF  on CF.ContractId    =  F.futureID
	inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID
	ORDER BY FutureName
	else
	SELECT 
		EIC.expiryDate    	AS MaturityDate, 
		Cm.iFutureUnderlyingID  AS CommodityID, 
		Fr.futureRootName 	AS FutureRootName, 
		Fr.futureLotSize 	AS FutureLotSize, 
		Fr.futureRootSymbol 	AS FutureRootSymbol,
		CF.symbol 		AS vcSymbol, 
		Fr.optionLotSize 	AS OptioLotSize, 
		Fr.exportSymbol 	AS FRExportSymbol, 
		CF.contractName 	AS FutureName, 
		F.futureRootID 		AS FutureRootID
	 from Future F
	inner Join FutureRoot FR on FR.futureRootID = F.futureRootID 
	inner join FutureUnderlying  CM on  CM.iFutureUnderlyingID  is not null	
	inner join Contract   CF  on CF.ContractId    =  F.futureID
	inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID
	ORDER BY FutureName
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Get]
GO

CREATE PROC dbo.usp_QuotationUnit_Get 
	@iQuotationUnitID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iQuotationUnitID is null
		select 
			iQuotationUnitID,
			vcName,
			vcDesc
		from QuotationUnit 
	else
		select 
			iQuotationUnitID,
			vcName,
			vcDesc
		from QuotationUnit 
		where iQuotationUnitID = @iQuotationUnitID

	return @@error
GO
ALTER  PROC dbo.usp_BoCommodity_Del
	@iCID int,
	@tiIsRaiseError tinyint = 1 
as
/*
	Description: Commodity Delete procedure
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
	
	if exists (select 1 from Trade where contractID = @iCID)
	   return -1

	if exists (select 1 from [Order] where contractID = @iCID)
	   return -2


	Begin Transaction CommodityDel

		exec @error = usp_TradeLog_Del @iCID
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		exec @error = usp_BoFutureRootByUnderlying_Del @iCID, 0
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		update IndexBeta set actionID=3 where indexID = @iCID
			if @@error != 0 begin set @error = -8 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		update ContractInGroup set actionID=3 where contractID = @iCID 
			if @@error != 0 begin set @error = -10 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end

		delete FutureUnderlying where
		iFutureUnderlyingID = @iCID
		if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran CommodityDel goto finish end



		update TraderContract set actionID=3 where contractID = @iCID  
			if @@error <> 0 begin set @error = -20 if @@TranCount != 0 Rollback Transaction CommodityDel goto finish end

		update Contract set actionID=3 where contractID = @iCID
			if @@error != 0 begin set @error = -21 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end
		
		set @error = @@error
	Commit Transaction CommodityDel
finish:
	return @error
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureUnderlyingType_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FutureUnderlyingType_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureUnderlyingType_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FutureUnderlyingType_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureUnderlyingType_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FutureUnderlyingType_Get]
GO

CREATE  PROCEDURE usp_FutureUnderlyingType_Save
	@iCptID int = null,
	@vcName varchar(255),
	@vcDesc varchar(255) = null,
	@tiRaiseError tinyint = null
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iCptID is null and isnull(@vcName, '') = ''
	begin
		raiserror ('Name for new future underlying type can not be empty!', 16, 1)
		set @error = -1
		goto finish
	end

	if @iCptID is null
	begin
		select
			@iCptID = cpt.iFutureUnderlyingTypeID
		from
			FutureUnderlyingType cpt
		where
			cpt.vcName = @vcName

		if @iCptID is not null and isnull(@tiRaiseError, 0) <> 0 
		begin
			raiserror ('Future underlying type with the same name is already exists!', 16, 1)
			set @error = -2
			goto finish
		end
	end

	Begin Transaction TntCounterParty

	if @iCptID is null
	begin
		insert into FutureUnderlyingType
			(vcName, vcDesc)
		values
			(@vcName, @vcDesc)

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
		set @iCptID = @@identity
	end
	else
	begin
		update FutureUnderlyingType set 
			vcName = isnull(@vcName, vcName),
			vcDesc = isnull(@vcDesc, vcDesc)
		where
			iFutureUnderlyingTypeID = @iCptID

		if @@error <> 0 begin set @error = -4 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
	end

	Commit Transaction TntCounterParty

finish:
	if @error = 0
		return @iCptID
	else
		return @error


GO

GO

CREATE  PROCEDURE dbo.usp_FutureUnderlyingType_Del
	@iCptID int,
	@tiIsRaiseError tinyint = 1 
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if exists(select 1 from FutureUnderlyingType where iFutureUnderlyingTypeID = @iCptID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove future underlying type, because it''s used in root defenitions!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end

	Begin Transaction TntCounterParty

	delete FutureUnderlyingType where iFutureUnderlyingTypeID = @iCptID
	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end

	Commit Transaction TntCounterParty

finish:
	return @error


GO


CREATE  PROCEDURE dbo.usp_FutureUnderlyingType_Get
	@iFutureUnderlyingTypeID int = null
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iFutureUnderlyingTypeID is null
		select
			cpt.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID,
			cpt.vcName as vcName,
			cpt.vcDesc as vcDesc
		from
			FutureUnderlyingType cpt
	else
		select
			cpt.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID,
			cpt.vcName as vcName,
			cpt.vcDesc as vcDesc
		from
			FutureUnderlyingType cpt
		where
			cpt.iFutureUnderlyingTypeID = @iFutureUnderlyingTypeID

return @@error

GO

ALTER proc dbo.usp_BoContractsSymbol_Get
	@iContractTypeID int = null
as
	if isnull(@iContractTypeID, 0) <> 0
		select	
			C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.symbol as vcSymbol ,
			fu.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID
		from ContractView c  
		left join FutureUnderlying fu on C.contractID = fu.iFutureUnderlyingID
		where c.contractTypeID = @iContractTypeID
		order by C.symbol
	else
		select	
			C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.symbol as vcSymbol 
		from ContractView c  
		where c.contractTypeID in (1, 2, 4)
		order by C.symbol

Return (@@Error)

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Get]
GO


CREATE  PROCEDURE usp_QuotationUnit_Save
	@iCptID int = null,
	@vcName varchar(255),
	@vcDesc varchar(255) = null,
	@tiRaiseError tinyint = null
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iCptID is null and isnull(@vcName, '') = ''
	begin
		raiserror ('Name for new future underlying type can not be empty!', 16, 1)
		set @error = -1
		goto finish
	end

	if @iCptID is null
	begin
		select
			@iCptID = cpt.iQuotationUnitID
		from
			QuotationUnit cpt
		where
			cpt.vcName = @vcName

		if @iCptID is not null and isnull(@tiRaiseError, 0) <> 0 
		begin
			raiserror ('Quotation unit with the same name is already exists!', 16, 1)
			set @error = -2
			goto finish
		end
	end

	Begin Transaction TntCounterParty

	if @iCptID is null
	begin
		insert into QuotationUnit
			(vcName, vcDesc)
		values
			(@vcName, @vcDesc)

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
		set @iCptID = @@identity
	end
	else
	begin
		update QuotationUnit set 
			vcName = isnull(@vcName, vcName),
			vcDesc = isnull(@vcDesc, vcDesc)
		where
			iQuotationUnitID = @iCptID

		if @@error <> 0 begin set @error = -4 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
	end

	Commit Transaction TntCounterParty

finish:
	if @error = 0
		return @iCptID
	else
		return @error


GO

GO

CREATE  PROCEDURE dbo.usp_QuotationUnit_Del
	@iCptID int,
	@tiIsRaiseError tinyint = 1 
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if exists(select 1 from QuotationUnit where iQuotationUnitID = @iCptID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove Quotation unit, because it''s used in root defenitions!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end

	Begin Transaction TntCounterParty

	delete QuotationUnit where iQuotationUnitID = @iCptID
	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end

	Commit Transaction TntCounterParty

finish:
	return @error


GO


CREATE  PROCEDURE dbo.usp_QuotationUnit_Get
	@iQuotationUnitID int = null
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iQuotationUnitID is null
		select
			cpt.iQuotationUnitID as iQuotationUnitID,
			cpt.vcName as vcName,
			cpt.vcDesc as vcDesc
		from
			QuotationUnit cpt
	else
		select
			cpt.iQuotationUnitID as iQuotationUnitID,
			cpt.vcName as vcName,
			cpt.vcDesc as vcDesc
		from
			QuotationUnit cpt
		where
			cpt.iQuotationUnitID = @iQuotationUnitID

return @@error

GO

if Not Exists (select 1 from INFORMATION_SCHEMA.TABLE_CONSTRAINTS where constraint_name = 'FK_IndexBeta_Contract')
ALTER TABLE [dbo].[IndexBeta] ADD 
	
	CONSTRAINT [FK_IndexBeta_Contract] FOREIGN KEY 
	(
		[indexID]
	) REFERENCES [dbo].[Contract] (
		[contractID]
	)
GO

if Exists (select 1 from INFORMATION_SCHEMA.TABLE_CONSTRAINTS where constraint_name = 'FK_IndexBeta_Index')
ALTER TABLE [dbo].[IndexBeta] DROP 
	
	CONSTRAINT [FK_IndexBeta_Index]
GO


ALTER      PROC dbo.usp_ICFutures_Get
	@iICID int = null
AS
	-- retrieve future

	set nocount on

	if @iICID is not null
	SELECT 
		EIC.expiryDate    	AS MaturityDate, 
		Cm.iFutureUnderlyingID  AS CommodityID, 
		Fr.futureRootName 	AS FutureRootName, 
		Fr.futureLotSize 	AS FutureLotSize, 
		Fr.futureRootSymbol 	AS FutureRootSymbol,
		CF.symbol 		AS vcSymbol, 
		Fr.optionLotSize 	AS OptioLotSize, 
		Fr.exportSymbol 	AS FRExportSymbol, 
		CF.contractName 	AS FutureName, 
		F.futureRootID 		AS FutureRootID,
		CP.priceClose		AS priceClose
	 from Future F
	inner Join FutureRoot FR on FR.futureRootID = F.futureRootID And Fr.underlyingID =  @iICID
	inner join FutureUnderlying  CM on Cm.iFutureUnderlyingID  =  @iICID	
	inner join Contract   CF  on CF.ContractId    =  F.futureID
	inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID
	left join ContractPrice CP on CP.ContractID = CF.ContractId
	ORDER BY FutureName
	else
	SELECT 
		EIC.expiryDate    	AS MaturityDate, 
		Cm.iFutureUnderlyingID  AS CommodityID, 
		Fr.futureRootName 	AS FutureRootName, 
		Fr.futureLotSize 	AS FutureLotSize, 
		Fr.futureRootSymbol 	AS FutureRootSymbol,
		CF.symbol 		AS vcSymbol, 
		Fr.optionLotSize 	AS OptioLotSize, 
		Fr.exportSymbol 	AS FRExportSymbol, 
		CF.contractName 	AS FutureName, 
		F.futureRootID 		AS FutureRootID,
		CP.priceClose		AS priceClose
	 from Future F
	inner Join FutureRoot FR on FR.futureRootID = F.futureRootID 
	inner join FutureUnderlying  CM on  CM.iFutureUnderlyingID  is not null	
	inner join Contract   CF  on CF.ContractId    =  F.futureID
	inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID
	left join ContractPrice CP on CP.ContractID = CF.ContractId
	ORDER BY FutureName

GO
GO
ALTER  PROC dbo.usp_BoFuture_Get
	@iFutureID int = null
AS
	-- retrieve future

	set nocount on

	if @iFutureID is not null
		select
			f.futureID as iContractID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcSymbol,
			fc.contractName as vcContractName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.exportSymbol as vcFutureRootExportSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			left join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
		where
			f.futureID = @iFutureID
	else
		select
			f.futureID as iContractID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcSymbol,
			fc.contractName as vcContractName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.exportSymbol as vcFutureRootExportSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			left join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
		order by
			fc.symbol

GO

ALTER  PROC dbo.usp_EodFutureOption_Import
	@iFutureID int,
	@fStrike float,
	@iExpiryID int,
	@dtExpiryDate smalldatetime,
	@tiIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@vcBadFutureOptionName varchar(255) output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	declare @iExpCalendarID int
	select @iExpCalendarID = c.expCalendarID
	from future f
	inner join FutureRoot fr on fr.futureRootID = f.futureRootID
	inner join Contract c on c.contractID = fr.underlyingID
	where @iFutureID = f.futureID
	exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID

	if not exists(select top 1 1 from ExpiryView where expiryID = isnull(@iExpiryID, 0))
	begin
		raiserror ('Wrong expiry date.', 16, 1)
		set @error = -100
		goto finish
	end

	declare	@iFutureOptionID int
	declare @iFutureOptionID_bad int

	--===================================================================			
	-- select the @iFutureOptionID
	--===================================================================			
	select @iFutureOptionID = futureOptionID
			from FutureOption (nolock)
			where futureID = @iFutureID and 
				isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID
				
	if not exists (select * from tempdb..sysobjects where id=object_id('tempdb..#FutureContractsForDelete'))
		Create table #FutureContractsForDelete(contractID int)				
	if @@error <> 0 begin set @error = -1 goto finish end
				
	if @iFutureOptionID is not null
	begin
		Begin Transaction
			select @iFutureOptionID_bad = contractID from ContractView where symbol=@vcSymbol and contractID<>@iFutureOptionID and contractTypeID=5
			if @iFutureOptionID_bad is not null
			begin
										
				select @vcBadFutureOptionName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureOptionID_bad

				if @vcBadFutureOptionName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureOptionName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureOptionID_bad

				if @vcBadFutureOptionName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureOptionID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFutureOption_Del

				if @error <> 0
				begin 
					set @error = -5
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			end

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureOptionID and actionID<>3
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureOption set actionDate = getdate() where futureOptionID=@iFutureOptionID
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			select @iFutureOptionID_bad = contractID from ContractView where symbol=@vcSymbol and contractTypeID=5
			if @iFutureOptionID_bad is not null
			begin	
				select @vcBadFutureOptionName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureOptionID_bad
				
				if @vcBadFutureOptionName is not null							
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureOptionName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureOptionID_bad

				if @vcBadFutureOptionName is not null							
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureOptionID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFutureOption_Del

				if @error <> 0
				begin 
					set @error = -5
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			end
				

			insert into Contract (contractTypeID, symbol, contractName)
				 values (5, @vcSymbol,@vcContractName)
			set @iFutureOptionID = @@identity
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into FutureOption (futureOptionID, futureID, isCall, expiryID, strike)
				 values (@iFutureOptionID, @iFutureID, @tiIsCall, @iExpiryID, @fStrike)
			if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureOptionID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iFutureOptionID,null)
				if (@@error <> 0) begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

		Commit Transaction
	end 
	 
   	set @error = @iFutureOptionID
finish:
	Return(@error)

GO




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
	select 'DB VERSION WAS CHANGED TO 4.49.176' '4.49.176 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

