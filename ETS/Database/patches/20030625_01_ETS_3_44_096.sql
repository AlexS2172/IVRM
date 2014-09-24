/*
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.095' or (keyValue = '3.44.096')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.44.096 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.44.096' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- OrderExecDest table
----------------------------------------------------------------------------------------------------

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OrderExecDest]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[OrderExecDest]
	(
		[destID] [int] IDENTITY (1, 1) NOT NULL ,
		[destCode] [varchar] (50) NOT NULL ,
		[destName] [varchar] (255) NULL,
		[isOption] [tinyint] NOT NULL,
		[isStock] [tinyint] NOT NULL
		CONSTRAINT [PK_OrderExecDest] PRIMARY KEY  CLUSTERED 
			(
				[destID]
			)
	)
end
GO

if not exists(select * from sysindexes where id=object_id('Order') and name='IX_OrderExecDest_DestCode')
	CREATE UNIQUE INDEX [IX_OrderExecDest_DestCode] ON [dbo].[OrderExecDest]([destCode])
GO

----------------------------------------------------------------------------------------------------
-- Order table
----------------------------------------------------------------------------------------------------

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Order]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[Order]
	(
		[orderID] [int] NOT NULL ,
		[seqNum] [int] NOT NULL ,
		[actionDate] [datetime] NOT NULL CONSTRAINT [DF_Order_actionDate] DEFAULT (getdate()),
		[contractID] [int] NOT NULL ,
		[traderID] [int] NOT NULL ,
		[destID] [int] NULL ,
		[status] [int] NOT NULL ,
		[orderType] [int] NOT NULL ,
		[isBuy] [tinyint] NOT NULL ,
		[orderQty] [int] NOT NULL ,
		[orderPrice] [float] NULL ,
		[orderDate] [datetime] NOT NULL ,
		[avgPrice] [float] NULL ,
		[cumQty] [int] NULL ,
		[clOrderID] [varchar] (50) NULL 
		CONSTRAINT [PK_Order] PRIMARY KEY CLUSTERED 
			(
				[orderID]
			),
		CONSTRAINT [FK_Order_Contract] FOREIGN KEY 
			(
				[contractID]
			)
			REFERENCES [dbo].[Contract]
			(
				[contractID]
			),
		CONSTRAINT [FK_Order_Trader] FOREIGN KEY 
			(
				[traderID]
			)
			REFERENCES [dbo].[Trader]
			(
				[traderID]
			),
		CONSTRAINT [FK_Order_DestID] FOREIGN KEY 
			(
				[destID]
			)
			REFERENCES [dbo].[OrderExecDest]
			(
				[destID]
			)
)
end
GO

if not exists(select * from sysindexes where id=object_id('Order') and name='IX_Order_SeqNum')
	CREATE UNIQUE INDEX [IX_Order_SeqNum] ON [dbo].[Order]([seqNum])
GO

if not exists(select * from sysindexes where id=object_id('Order') and name='IX_Order_Trader')
	CREATE INDEX [IX_Order_Trader] ON [dbo].[Order]([traderID])
GO

----------------------------------------------------------------------------------------------------
-- OrderExec table
----------------------------------------------------------------------------------------------------

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OrderExec]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[OrderExec]
	(
		[execID] [int] NOT NULL ,
		[seqNum] [int] NOT NULL ,
		[orderID] [int] NOT NULL ,
		[execStatus] [int] NOT NULL ,
		[execQty] [int] NOT NULL ,
		[execPrice] [float] NOT NULL ,
		[execDate] [datetime] NOT NULL ,
		[brokerExecID] [varchar] (50) NOT NULL 
		CONSTRAINT [PK_OrderExec] PRIMARY KEY  CLUSTERED 
			(
				[execID]
			),
		CONSTRAINT [FK_OrderExec_Order] FOREIGN KEY 
			(
				[orderID]
			)
			REFERENCES [dbo].[Order]
			(
				[orderID]
			)
	)
end
GO

if not exists(select * from sysindexes where id=object_id('OrderExec') and name='IX_OrderExec_OrderID')
	CREATE INDEX [IX_OrderExec_OrderID] ON [dbo].[OrderExec]([orderID])
GO

if not exists(select * from sysindexes where id=object_id('OrderExec') and name='IX_OrderExec_SeqNum')
	CREATE INDEX [IX_OrderExec_SeqNum] ON [dbo].[OrderExec]([seqNum])
GO


----------------------------------------------------------------------------------------------------
-- add default order destinations
----------------------------------------------------------------------------------------------------
begin

	-- stock destinations
	-- ISLD, INST, ARCA, DOT, DOTNX, DOTNY, CAP, CME, CME2
	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('ISLD', 'ISLD', 0, 1)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('INST', 'INST', 0, 1)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('ARCA', 'ARCA', 0, 1)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('DOT', 'DOT', 0, 1)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('DOTNX', 'DOTNX', 0, 1)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('DOTNY', 'DOTNY', 0, 1)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('CAP', 'CAP', 0, 1)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('CME', 'CME', 0, 1)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('CME2', 'CME2', 0, 1)

	-- option destinations
	-- ISE, AMX, PHL, PCX, CBO, BOX
	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('ISE', 'ISE', 1, 0)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('AMX', 'AMX', 1, 0)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('PHL', 'PHL', 1, 0)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('PCX', 'PCX', 1, 0)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('CBO', 'CBO', 1, 0)

	insert into OrderExecDest
		(destCode, destName, isOption, isStock)
	values
		('BOX', 'BOX', 1, 0)

end
GO

----------------------------------------------------------------------------------------------------
-- usp_UnderlyingOptionsForOrder_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_UnderlyingOptionsForOrder_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_UnderlyingOptionsForOrder_Get
GO

CREATE PROC dbo.usp_UnderlyingOptionsForOrder_Get
	@iUnderlyingID int
AS
	set nocount on

	select
		ort.optionRootID as iOptionRootID,
		o.optionID as iOptionID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		o.isCall as tiIsCall,
		ec.expiryDate as dtExpiry
	from OptionRootView ort
		left join OptionView o on ort.optionRootID = o.optionRootID
		left join ContractView c on c.contractID = o.optionID
		left join Contract uc on ort.underlyingID = uc.contractID
		left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
	where
		ort.underlyingID = @iUnderlyingID
		and c.contractTypeID = 3
		and ec.expiryDate >= getdate()
	order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

Return (0)

GO

----------------------------------------------------------------------------------------------------
-- usp_UnderlyingOptionRoot_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_UnderlyingOptionRoot_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_UnderlyingOptionRoot_Get
GO

CREATE PROCEDURE dbo.usp_UnderlyingOptionRoot_Get
	@iUnderlyingID int = null
AS
/*
	Created by Sharky
	Description: this procedure is getting information from OptionRoot table for underlying
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iUnderlyingID is not null
		select
			orv.underlyingID as iUnderlyingID,
			orv.optionRootID as iRootID,
			orv.symbol as vcSymbol,
			orv.IsSynthetic as tiIsSynthetic, 
			orv.lotSize as iLotSize, 
			sor.cashValue as fCash,
			sor.synthSkew as fSyntheticSkew, 
			sor.synthKurt as fSyntheticKurt,
			sor.synthYield as fSyntheticYield, 
			sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView as orv
			left join SyntheticOptionRootView as sor on orv.optionRootID = sor.optionRootID
		where
			orv.underlyingID = @iUnderlyingID
	else
		select
			orv.underlyingID as iUnderlyingID,
			orv.optionRootID as iRootID,
			orv.symbol as vcSymbol,
			orv.IsSynthetic as tiIsSynthetic, 
			orv.lotSize as iLotSize, 
			sor.cashValue as fCash,
			sor.synthSkew as fSyntheticSkew, 
			sor.synthKurt as fSyntheticKurt,
			sor.synthYield as fSyntheticYield, 
			sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView as orv
			left join SyntheticOptionRootView as sor on orv.optionRootID = sor.optionRootID
	
	set @error = @@error

Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_OrderByTrader_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OrderByTrader_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OrderByTrader_Get
GO

CREATE PROCEDURE dbo.usp_OrderByTrader_Get
	@iTraderID int = null
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iTraderID is null
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			c.contractTypeID < 3
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		where
			c.contractTypeID = 3
	end
	else
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			c.contractTypeID < 3
			and ord.traderID = @iTraderID
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		where
			c.contractTypeID = 3
			and ord.traderID = @iTraderID
	end

return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_OrderSeqByTrader_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OrderSeqByTrader_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OrderSeqByTrader_Get
GO

CREATE PROCEDURE dbo.usp_OrderSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int,
	@iMaxSeqNum int
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			ord.seqNum between @iMinSeqNum and @iMaxSeqNum
			and c.contractTypeID < 3
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		where
			ord.seqNum between @iMinSeqNum and @iMaxSeqNum
			and c.contractTypeID = 3
	end
	else
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			ord.seqNum between @iMinSeqNum and @iMaxSeqNum
			and c.contractTypeID < 3
			and ord.traderID = @iTraderID
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		where
			ord.seqNum between @iMinSeqNum and @iMaxSeqNum
			and c.contractTypeID = 3
			and ord.traderID = @iTraderID
	end

return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_OrderExec_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OrderExec_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OrderExec_Get
GO

CREATE PROCEDURE dbo.usp_OrderExec_Get
	@iOrderID int = null,
	@iTraderID int = null
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iTraderID is null
	begin
		if @iOrderID is not null
		begin
			select
				oex.execID as iExecID,
				oex.seqNum as iSeqNum,
				oex.orderID as iOrderID,
				oex.execStatus as iExecStatus,
				oex.execQty as iExecQty,
				oex.execPrice as fExecPrice,
				oex.execDate as dtExecDate,
				oex.brokerExecID as vcBrokerExecID
			from
				OrderExec oex
			where
				oex.orderID = @iOrderID
		end
		else
		begin
			select
				oex.execID as iExecID,
				oex.seqNum as iSeqNum,
				oex.orderID as iOrderID,
				oex.execStatus as iExecStatus,
				oex.execQty as iExecQty,
				oex.execPrice as fExecPrice,
				oex.execDate as dtExecDate,
				oex.brokerExecID as vcBrokerExecID
			from
				OrderExec oex
		end
	end
	else
	begin
		if @iOrderID is not null
		begin
			select
				oex.execID as iExecID,
				oex.seqNum as iSeqNum,
				oex.orderID as iOrderID,
				oex.execStatus as iExecStatus,
				oex.execQty as iExecQty,
				oex.execPrice as fExecPrice,
				oex.execDate as dtExecDate,
				oex.brokerExecID as vcBrokerExecID
			from
				OrderExec oex
				left join [Order] ord on ord.orderID = oex.orderID
			where
				oex.orderID = @iOrderID
				and ord.traderID = @iTraderID
		end
		else
		begin
			select
				oex.execID as iExecID,
				oex.seqNum as iSeqNum,
				oex.orderID as iOrderID,
				oex.execStatus as iExecStatus,
				oex.execQty as iExecQty,
				oex.execPrice as fExecPrice,
				oex.execDate as dtExecDate,
				oex.brokerExecID as vcBrokerExecID
			from
				OrderExec oex
				left join [Order] ord on ord.orderID = oex.orderID
			where
				ord.traderID = @iTraderID
		end
	end	

return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_OrderExecSeq_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OrderExecSeq_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OrderExecSeq_Get
GO

CREATE PROCEDURE dbo.usp_OrderExecSeq_Get
	@iOrderID int = null,
	@iTraderID int = null,
	@iMinSeqNum int,
	@iMaxSeqNum int
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
	begin
		if @iOrderID is not null
		begin
			select
				oex.execID as iExecID,
				oex.seqNum as iSeqNum,
				oex.orderID as iOrderID,
				oex.execStatus as iExecStatus,
				oex.execQty as iExecQty,
				oex.execPrice as fExecPrice,
				oex.execDate as dtExecDate,
				oex.brokerExecID as vcBrokerExecID
			from
				OrderExec oex
			where
				oex.seqNum between @iMinSeqNum and @iMaxSeqNum
				and oex.orderID = @iOrderID
		end
		else
		begin
			select
				oex.execID as iExecID,
				oex.seqNum as iSeqNum,
				oex.orderID as iOrderID,
				oex.execStatus as iExecStatus,
				oex.execQty as iExecQty,
				oex.execPrice as fExecPrice,
				oex.execDate as dtExecDate,
				oex.brokerExecID as vcBrokerExecID
			from
				OrderExec oex
			where
				oex.seqNum between @iMinSeqNum and @iMaxSeqNum
		end
	end
	else
	begin
		if @iOrderID is not null
		begin
			select
				oex.execID as iExecID,
				oex.seqNum as iSeqNum,
				oex.orderID as iOrderID,
				oex.execStatus as iExecStatus,
				oex.execQty as iExecQty,
				oex.execPrice as fExecPrice,
				oex.execDate as dtExecDate,
				oex.brokerExecID as vcBrokerExecID
			from
				OrderExec oex
				left join [Order] ord on ord.orderID = oex.orderID
			where
				oex.seqNum between @iMinSeqNum and @iMaxSeqNum
				and oex.orderID = @iOrderID
				and ord.traderID = @iTraderID
		end
		else
		begin
			select
				oex.execID as iExecID,
				oex.seqNum as iSeqNum,
				oex.orderID as iOrderID,
				oex.execStatus as iExecStatus,
				oex.execQty as iExecQty,
				oex.execPrice as fExecPrice,
				oex.execDate as dtExecDate,
				oex.brokerExecID as vcBrokerExecID
			from
				OrderExec oex
				left join [Order] ord on ord.orderID = oex.orderID
			where
				oex.seqNum between @iMinSeqNum and @iMaxSeqNum
				and ord.traderID = @iTraderID
		end
	end	

return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_OrderExecDest_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OrderExecDest_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OrderExecDest_Get
GO

CREATE PROCEDURE dbo.usp_OrderExecDest_Get
	@iDestID int = null
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iDestID is null
		select
			oed.destID as iDestID,
			oed.destCode as vcCode,
			oed.destName as vcName,
			oed.isOption as tiIsOption,
			oed.isStock as tiIsStock
		from
			OrderExecDest oed
	else
		select
			oed.destID as iDestID,
			oed.destCode as vcCode,
			oed.destName as vcName,
			oed.isOption as tiIsOption,
			oed.isStock as tiIsStock
		from
			OrderExecDest oed
		where
			oed.destID = @iDestID

return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_OrderLastSeqNum_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OrderLastSeqNum_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OrderLastSeqNum_Get
GO

CREATE PROC dbo.usp_OrderLastSeqNum_Get
AS
	select max(seqNum) as iSeqNum
	from [Order] (nolock)
GO

----------------------------------------------------------------------------------------------------
-- usp_OrderExecLastSeqNum_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OrderExecLastSeqNum_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OrderExecLastSeqNum_Get
GO

CREATE PROC dbo.usp_OrderExecLastSeqNum_Get
AS
	select max(seqNum) as iSeqNum
	from OrderExec (nolock)
GO

----------------------------------------------------------------------------------------------------
-- usp_IRCurve_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_IRCurve_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_IRCurve_Get
GO

CREATE PROC dbo.usp_IRCurve_Get
	@iCurveID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iCurveID is null
		select curveID as iCurveID,
			curveName as vcCurveName, 
			curveTypeID as iCurveTypeID, 
			ruleTypeID as iRuleTypeID, 
			positionThreshold as mPositionThreshold,
			actionDate as dtActionDate,
			actionID as tiActionID
		from
			IRCurveView
	else
		select curveID as iCurveID,
			curveName as vcCurveName, 
			curveTypeID as iCurveTypeID, 
			ruleTypeID as iRuleTypeID, 
			positionThreshold as mPositionThreshold,
			actionDate as dtActionDate,
			actionID as tiActionID
		from
			IRCurveView
		where
			curveID = @iCurveID


	set @error = @@error
Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_IRCurve_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_IRPoint_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_IRPoint_Get
GO

CREATE PROC dbo.usp_IRPoint_Get
	@iCurveID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	if @iCurveID is not null
		select
			IRpointID as iIRpointID, 
			curveID as iCurveID, 
			periodTypeID as iPeriodTypeID, 
			num as iNum, 
			shortRate as fShortRate, 
			HTBRate as fHTBRate, 
			longRate as fLongRate,
			actionDate as dtActionDate,
			actionID as tiActionID
		from
			IRPointView
		where
			curveID = @iCurveID
		order by periodTypeID, num 
	else
		select
			IRpointID as iIRpointID, 
			curveID as iCurveID, 
			periodTypeID as iPeriodTypeID, 
			num as iNum, 
			shortRate as fShortRate, 
			HTBRate as fHTBRate, 
			longRate as fLongRate,
			actionDate as dtActionDate,
			actionID as tiActionID
		from
			IRPointView
		order by periodTypeID, num 

	set @error = @@error
Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeByTrader_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TradeByTrader_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TradeByTrader_Get
GO

CREATE proc dbo.usp_TradeByTrader_Get
	@iTraderID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
	begin
			select
				tr.tradeID as iTradeID,
				
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				null as dtExpiry,
				null as fStrike,
				null as bIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				cp.priceClose as fUnderlyingPriceClose,
				null as fPriceTheoClose,
				
				tr.isClose as tiIsClose,
				tr.tradeDate as dtTradeDate,
				tr.quantity as iQuantity,
				tr.price as fPrice,
				tr.isBuy as tiIsBuy,
				tr.bookID as iBookID,     
				tr.strategyID as iStrategyID, 
				tr.traderID as iTraderID,
				tr.brokerID as iBrokerID,
				tr.clearingBrokerID as iClearingBrokerID,
				tr.brokerCommissionID as iBrokerCommissionID,
				tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
				tr.tradedIV as fTradedIV,
				tr.spotReference as fSpotReference,
				tr.isPosition as tiIsPosition,	
				1 as iLotSize,					
				tr.status as tiStatus,
				null as iOptionRootID,
				null as tiIsSyntheticRoot,
				tr.execID as vcExecID
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join Stock s on c.contractID = s.stockID
				left join [Index] i on c.contractID = i.indexID
			where
				c.contractTypeID < 3
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,cp.priceClose,tr.execID
			having min(tr.actionID) = 1
	
	   union
	
			select
				tr.tradeID as iTradeID,
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				ucp.priceClose as fUnderlyingPriceClose,
				cp.priceTheo as fPriceTheoClose,
				
				tr.isClose as tiIsClose,
				tr.tradeDate as dtTradeDate,
				tr.quantity as iQuantity,
				tr.price as fPrice,
				tr.isBuy as tiIsBuy,
				tr.bookID as iBookID,     
				tr.strategyID as iStrategyID, 
				tr.traderID as iTraderID,
				tr.brokerID as iBrokerID,
				tr.clearingBrokerID as iClearingBrokerID,
				tr.brokerCommissionID as iBrokerCommissionID,
				tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
				tr.tradedIV as fTradedIV,
				tr.spotReference as fSpotReference,
				tr.isPosition as tiIsPosition,
				ort.lotSize as iLotSize,
				tr.status as tiStatus,
				ort.optionRootID as iOptionRootID,
				ort.isSynthetic as tiIsSyntheticRoot,
				tr.execID as vcExecID
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				join OptionRoot ort on o.optionRootID = ort.optionRootID
				inner join Contract UC on ort.underlyingID = UC.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
				left join Stock s on uc.contractID = s.stockID
				left join [Index] i on uc.contractID = i.indexID
			where 
				c.contractTypeID = 3
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,
				ort.lotSize,cp.priceClose,ucp.priceClose, ort.optionRootID,ort.isSynthetic,tr.execID
			having min(tr.actionID) = 1
			order by TradeDate
	end
	else
	begin
			select
				tr.tradeID as iTradeID,
				
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				null as dtExpiry,
				null as fStrike,
				null as bIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				cp.priceClose as fUnderlyingPriceClose,
				null as fPriceTheoClose,
				
				tr.isClose as tiIsClose,
				tr.tradeDate as dtTradeDate,
				tr.quantity as iQuantity,
				tr.price as fPrice,
				tr.isBuy as tiIsBuy,
				tr.bookID as iBookID,     
				tr.strategyID as iStrategyID, 
				tr.traderID as iTraderID,
				tr.brokerID as iBrokerID,
				tr.clearingBrokerID as iClearingBrokerID,
				tr.brokerCommissionID as iBrokerCommissionID,
				tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
				tr.tradedIV as fTradedIV,
				tr.spotReference as fSpotReference,
				tr.isPosition as tiIsPosition,	
				1 as iLotSize,					
				tr.status as tiStatus,
				null as iOptionRootID,
				null as tiIsSyntheticRoot,
				tr.execID as vcExecID
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join Stock s on c.contractID = s.stockID
				left join [Index] i on c.contractID = i.indexID
			where
				c.contractTypeID < 3
				and tr.traderID = @iTraderID
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,cp.priceClose,tr.execID
			having min(tr.actionID) = 1
	
	   union
	
			select
				tr.tradeID as iTradeID,
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				ucp.priceClose as fUnderlyingPriceClose,
				cp.priceTheo as fPriceTheoClose,
				
				tr.isClose as tiIsClose,
				tr.tradeDate as dtTradeDate,
				tr.quantity as iQuantity,
				tr.price as fPrice,
				tr.isBuy as tiIsBuy,
				tr.bookID as iBookID,     
				tr.strategyID as iStrategyID, 
				tr.traderID as iTraderID,
				tr.brokerID as iBrokerID,
				tr.clearingBrokerID as iClearingBrokerID,
				tr.brokerCommissionID as iBrokerCommissionID,
				tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
				tr.tradedIV as fTradedIV,
				tr.spotReference as fSpotReference,
				tr.isPosition as tiIsPosition,
				ort.lotSize as iLotSize,
				tr.status as tiStatus,
				ort.optionRootID as iOptionRootID,
				ort.isSynthetic as tiIsSyntheticRoot,
				tr.execID as vcExecID
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				join OptionRoot ort on o.optionRootID = ort.optionRootID
				inner join Contract UC on ort.underlyingID = UC.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
				left join Stock s on uc.contractID = s.stockID
				left join [Index] i on uc.contractID = i.indexID
			where c.contractTypeID = 3
				and tr.traderID = @iTraderID
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,
				ort.lotSize,cp.priceClose,ucp.priceClose, ort.optionRootID,ort.isSynthetic,tr.execID
			having min(tr.actionID) = 1
			order by TradeDate
	end

     Return(@@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_Option_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Option_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Option_Get
GO

CREATE proc dbo.usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
/*
	Created by Zaur Nuraliev
	[Description]: this procedure return inf. from OptionView table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @dtMinExpiry is null set @dtMinExpiry = 0

	if isnull(@tiWithPosition, 0) = 0
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			if @iUnderlyingID is not null
				and @iOptionID is null
				and @tiBaseInfoOnly = 0
				and @tiWithPosition = 0
				
				-- Sharky: speed optimization for quote view

				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName, 
					ORt.underlyingID as iUnderlyingContractID,
					O.strike as fStrike, 
					EC.expiryDate as dtExpiry,
					O.isCall as bIsCall,
					O.actionDate as dtActionDate,
					O.actionID as tiActionID,
					ORt.lotSize as iLotSize,
					ORt.optionRootID as iOptionRootID,
					ORt.symbol as vcOptionRootSymbol,
					O.isManualyEntered as tiIsManualyEntered
				from ContractView C  
					inner join OptionView O on O.optionID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C1 on ORt.underlyingID = C1.contractID
					inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where c.contractTypeID=3
					and EC.expiryDate >= @dtMinExpiry
					and ORt.underlyingID = @iUnderlyingID
				order by ORt.underlyingID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName, 
					ORt.underlyingID as iUnderlyingContractID,
					O.strike as fStrike, 
					EC.expiryDate as dtExpiry,
					O.isCall as bIsCall,
					O.actionDate as dtActionDate,
					O.actionID as tiActionID,
					ORt.lotSize as iLotSize,
					ORt.optionRootID as iOptionRootID,
					ORt.symbol as vcOptionRootSymbol,
					O.isManualyEntered as tiIsManualyEntered
				from ContractView C  
					inner join OptionView O on O.optionID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C1 on ORt.underlyingID = C1.contractID
					inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where c.contractTypeID=3
					and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
					and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
					and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				order by ORt.underlyingID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol,
				O.isManualyEntered as tiIsManualyEntered 
			from ContractView c  
				inner join OptionView O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
	end
	else
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol,
				O.isManualyEntered as tiIsManualyEntered 
			from ContractView C  
				inner join OptionView O on O.optionID = C.contractID
				left join TradeView tr on tr.contractID = c.contractID and (tr.traderID = @iTraderID or @iTraderID is Null)
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID

			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, ORt.underlyingID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,ORt.lotSize, ORt.optionRootID, ORt.symbol,O.isManualyEntered
			order by ORt.underlyingID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol,
				O.isManualyEntered as tiIsManualyEntered 
			from ContractView c  
				inner join OptionView O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null						
				left join TradeView tr on tr.contractID = c.contractID and (tr.traderID = @iTraderID or @iTraderID is Null)
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, ORt.underlyingID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,ORt.lotSize, ORt.optionRootID, ORt.symbol,O.isManualyEntered
	end


	set @error = @@error
Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeSeqByTrader_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TradeSeqByTrader_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TradeSeqByTrader_Get
GO

CREATE PROC dbo.usp_TradeSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int,
	@iMaxSeqNum int
AS
	set nocount on

	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.status as tiStatus,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			--case when exists(select * from TradeLog where contractID = tr.contractID) then 1 else 0 end iStructureMissed,
			case when exists(select * from TradeLog where contractID = tr.contractID and seqNum = tr.seqNum) then 1 else 0 end iStructureMissed,
			null as iOptionRootID,
			null as tiIsSyntheticRoot,
			tr.execID as vcExecID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID < 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		union
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			status as tiStatus,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			case 
			    when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum)
					and exists(select * from TradeLog where contractID = O.optionID and seqNum = tr.seqNum) then 3
				when exists(select * from TradeLog where contractID = o.optionID and seqNum = tr.seqNum) then 2
				when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum) then 1
            else 0 	
			end iStructureMissed,
			ORT.optionRootID as iOptionRootID,
			ORT.isSynthetic as tiIsSyntheticRoot,
			tr.execID as vcExecID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join OptionRootView ORt on ORt.optionRootID = o.optionRootID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID = 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		order by tr.seqNum
	else
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.status as tiStatus,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			--case when exists(select * from TradeLog where contractID = tr.contractID) then 1 else 0 end iStructureMissed,
			case when exists(select * from TradeLog where contractID = tr.contractID and seqNum = tr.seqNum) then 1 else 0 end iStructureMissed,
			null as iOptionRootID,
			null as tiIsSyntheticRoot,
			tr.execID as vcExecID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID < 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		union
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			status as tiStatus,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			case 
			    when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum)
					and exists(select * from TradeLog where contractID = O.optionID and seqNum = tr.seqNum) then 3
				when exists(select * from TradeLog where contractID = o.optionID and seqNum = tr.seqNum) then 2
				when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum) then 1
            else 0 	
			end iStructureMissed,
			ORT.optionRootID as iOptionRootID,
			ORT.isSynthetic as tiIsSyntheticRoot,
			tr.execID as vcExecID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join OptionRootView ORt on ORt.optionRootID = o.optionRootID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID = 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		order by tr.seqNum
GO




----------------------------------------------------------------------------------------------------
-- usp_FixOrder_New
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_FixOrder_New' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_FixOrder_New
GO

CREATE PROC dbo.usp_FixOrder_New
	@vcClOrdID varchar(50),
	@iContractID int,
	@iTraderID int,
	@iDestID int,
	@iOrderType int,  
	@tiIsBuy tinyint,
	@iQuantity int,  
	@fPrice float,
	@dtOrderDate datetime
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is save information into Order table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

	declare @iMinSeqNum int 
		select @iMinSeqNum = max(seqNum)
			from [Order]
			where clOrderID = @vcClOrdID and 
					status > 0 and
					Convert(varchar(8),@dtOrderDate,112) = Convert(varchar(8),orderDate,112)

	if @iMinSeqNum is not null
	begin
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	declare @iNewOrderID int
		select @iNewOrderID = max(orderID) + 1 from [Order]

	declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from [Order]

	if @iNewOrderID is null set @iNewOrderID = 1

	--create new trade	
	insert into [Order] (
		orderID,
		seqNum,
		contractID, 
		traderID,
		destID, 
		status, 
		orderType, 
		isBuy, 
		orderQty, 
		orderPrice, 
		orderDate,
		clOrderID)
	values (
		@iNewOrderID,
		@iSeqNum,
		@iContractID,
		@iTraderID, 
		@iDestID, 
		65, --pending new
		@iOrderType, 
		@tiIsBuy, 
		@iQuantity, 
		@fPrice,
		@dtOrderDate,
		@vcClOrdID)

		if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction goto finish end

		--return output recordset
		select @iNewOrderID as iOrderID,@iSeqNum as iSeqNum

	Commit Transaction

    set @error = 0
Return(@error)

finish:
		select -1 as iOrderID, -1 as iSeqNum where 1=0
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_FixOrder_Update
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_FixOrder_Update' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_FixOrder_Update
GO

CREATE PROC dbo.usp_FixOrder_Update
	@iOrderID int,
	@iStatus int,
	@vcClOrdID varchar(50),
	@iOrderType int,  
	@iQuantity int,
	@fPrice float,
	@dtOrderDate datetime
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is save information into Order table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

	declare @iMinSeqNum int 
		select @iMinSeqNum = max(seqNum)
			from [Order]
			where clOrderID = @vcClOrdID and 
					status > 0 and orderID != @iOrderID and
					Convert(varchar(8),@dtOrderDate,112) = Convert(varchar(8),orderDate,112)

	if @iMinSeqNum is not null
	begin
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from [Order]

	--create new trade	
	update [Order] 
		set
		seqNum = @iSeqNum,
		status = isnull(@iStatus, status),  
		orderType = isnull(@iOrderType, orderType), 
		orderQty = isnull(@iQuantity, orderQty),
		orderPrice = isnull(@fPrice, orderPrice), 
		orderDate = isnull(@dtOrderDate, orderDate),
		clOrderID = isnull(@vcClOrdID, clOrderID)
		where orderID = @iOrderID

	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction goto finish end

	--return output recordset
		select @iSeqNum as iSeqNum,
				ORt.underlyingID as iUndID,
				C.contractID as iOptionID,
				C.contractTypeID as iContractType,
				ORt.optionRootID as iOptionRootID,
				C.symbol as vcSymbol,
				OV.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				OV.isCall as tiIsCall,
				Ord.traderID as iTraderID,
				T.acronym as vcAccount,
				Ord.isBuy as tiIsBuy,
				Ord.orderType as iOrdType,
				Ord.orderQty as iQuantity,
				Ord.orderPrice as fPrice,
				Ord.orderDate as dtSendingTime,
				Ord.actionDate as dtTransactTime,
				Ord.destID as iDestID,
				OD.destCode as vcDestination,
				Ord.avgPrice as fAvgPrice,
				Ord.cumQty as iCumQty,
				ORt.symbol as vcOptionRootSymbol
				from [Order] Ord
					left join OrderExecDest OD on Ord.destID = OD.destID
					inner join OptionView OV on OV.optionID = Ord.contractID
					left join ContractView C on C.contractID = OV.optionID
					left join OptionRootView ORt on OV.optionRootID = ORt.optionRootID
					left join TraderView T on T.traderID = Ord.traderID
					left join ContractView C1 on ORt.underlyingID = C1.contractID
					inner join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where Ord.orderID = @iOrderID
		union
		select	
				@iSeqNum as iSeqNum,
				C.contractID as iUndID,
				null as iOptionID,
				C.contractTypeID as iContractType,
				null as iOptionRootID,
				C.symbol as vcSymbol,
				null as fStrike, 
				null as dtExpiry,
				null as tiIsCall,
				Ord.traderID as iTraderID,
				T.acronym as vcAccount,
				Ord.isBuy as tiIsBuy,
				Ord.orderType as iOrdType,
				Ord.orderQty as iQuantity,
				Ord.orderPrice as fPrice,
				Ord.orderDate as dtSendingTime,
				Ord.actionDate as dtTransactTime,
				Ord.destID as iDestID,
				OD.destCode as vcDestination,
				Ord.avgPrice as fAvgPrice,
				Ord.cumQty as iCumQty,
				null as vcOptionRootSymbol
				from [Order] Ord
					left join OrderExecDest OD on Ord.destID = OD.destID
					inner join ContractView C on C.contractID = Ord.contractID
					left join TraderView T on T.traderID = Ord.traderID
				where Ord.orderID = @iOrderID and C.contractTypeID < 3

	Commit Transaction

    set @error = 0
Return(@error)

finish:
		select -1

	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_FixOrderExec_New
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_FixOrderExec_New' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_FixOrderExec_New
GO

CREATE PROC dbo.usp_FixOrderExec_New
	@vcBrokerExecID varchar(50),
	@iOrderID int,
	@iExecStatus int,
	@iQuantity int,
	@fExecPrice float,
	@dtExecDate datetime,
	@iOrderStatus int,
	@fAvgPrice float = null,
	@iCumQty int = null
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is save information into OrderExec table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

	declare @iMinSeqNum int 
		select @iMinSeqNum = max(seqNum)
			from [OrderExec]
			where brokerExecID = @vcBrokerExecID and 
					execStatus > 0 and
					Convert(varchar(8),@dtExecDate,112) = Convert(varchar(8),execDate,112)

	if @iMinSeqNum is not null
	begin
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	declare @iNewExecID int
		select @iNewExecID = max(execID) + 1 from [OrderExec]

	declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from [OrderExec]

	if @iNewExecID is null set @iNewExecID = 1

	--create new Execution	
	insert into [OrderExec] (
		execID,
		seqNum,
		orderID, 
		execStatus,
		execQty,
		execPrice,
		execDate,
		brokerExecID)
	values (
		@iNewExecID,
		@iSeqNum,
		@iOrderID,
		@iExecStatus, 
		@iQuantity, 
		@fExecPrice, 
		@dtExecDate, 
		@vcBrokerExecID) 

	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction goto finish end

	update [Order]
		set 
			status = @iOrderStatus,
			avgPrice = isnull(@fAvgPrice, avgPrice), 
			cumQty = isnull(@iCumQty, cumQty) 
		where orderID = @iOrderID

	if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction goto finish end

		--return output recordset
	select @iNewExecID as iExecID,@iSeqNum as iSeqNum 

	Commit Transaction

    set @error = 0
Return(@error)

finish:
		select -1 as iExecID, -1 as iSeqNum  where 1=0
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_FixOrderExec_Update
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_FixOrderExec_Update' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_FixOrderExec_Update
GO

CREATE PROC dbo.usp_FixOrderExec_Update
	@iExecID int,
	@vcBrokerExecID varchar(50),
	@iExecStatus int,
	@iQuantity int,
	@fExecPrice float,
	@dtExecDate datetime,
	@iOrderStatus int,
	@fAvgPrice float = null,
	@iCumQty int = null
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is save information into OrderExec table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

	declare @iMinSeqNum int 
		select @iMinSeqNum = max(seqNum)
			from [OrderExec]
			where brokerExecID = @vcBrokerExecID and 
					execStatus > 0 and execID != @iExecID and
					Convert(varchar(8),@dtExecDate,112) = Convert(varchar(8),execDate,112)

	if @iMinSeqNum is not null
	begin
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end


	declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from [OrderExec]

	--create new Execution	
	Update [OrderExec] set
		seqNum = @iSeqNum,
		execStatus = isnull(@iExecStatus, execStatus),
		execQty = isnull(@iQuantity, execQty),
		execPrice = isnull(@fExecPrice, execPrice),
		execDate = isnull(@dtExecDate, execDate),
		brokerExecID = isnull(@vcBrokerExecID, brokerExecID)
	where execID=@iExecID

	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction goto finish end

	update [Order]
		set 
			status = @iOrderStatus,
			avgPrice = isnull(@fAvgPrice, avgPrice), 
			cumQty = isnull(@iCumQty, cumQty) 
	from [Order] O join OrderExec OE on OE.orderID=O.orderID
		where OE.execID=@iExecID

	if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction goto finish end

		--return output recordset
	select @iSeqNum as iSeqNum 

	Commit Transaction

    set @error = 0
Return(@error)

finish:
		select  -1 as iSeqNum  where 1=0
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_FixOrderStatus_Update
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_FixOrderStatus_Update' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_FixOrderStatus_Update
GO

CREATE PROC dbo.usp_FixOrderStatus_Update
	@iOrderID int,
	@iOrderStatus int
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is save information into Order table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

	declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from [Order]

	--create new trade	
	Update [Order] 
		set
		seqNum = @iSeqNum,
		status = @iOrderStatus where orderID = @iOrderID

	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction goto finish end

	--return output recordset
		select @iSeqNum as iSeqNum,
				ORt.underlyingID as iUndID,
				C.contractID as iOptionID,
				C.contractTypeID as iContractType,
				ORt.optionRootID as iOptionRootID,
				C.symbol as vcSymbol,
				OV.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				OV.isCall as tiIsCall,
				Ord.traderID as iTraderID,
				T.acronym as vcAccount,
				Ord.isBuy as tiIsBuy,
				Ord.orderType as iOrdType,
				Ord.orderQty as iQuantity,
				Ord.orderPrice as fPrice,
				Ord.orderDate as dtSendingTime,
				Ord.actionDate as dtTransactTime,
				Ord.destID as iDestID,
				OD.destCode as vcDestination,
				Ord.avgPrice as fAvgPrice,
				Ord.cumQty as iCumQty,
				ORt.symbol as vcOptionRootSymbol
				from [Order] Ord
					left join OrderExecDest OD on Ord.destID = OD.destID
					inner join OptionView OV on OV.optionID = Ord.contractID
					left join ContractView C on C.contractID = OV.optionID
					left join OptionRootView ORt on OV.optionRootID = ORt.optionRootID
					left join TraderView T on T.traderID = Ord.traderID
					left join ContractView C1 on ORt.underlyingID = C1.contractID
					inner join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where Ord.orderID = @iOrderID
		union
		select	
				@iSeqNum as iSeqNum,
				C.contractID as iUndID,
				null as iOptionID,
				C.contractTypeID as iContractType,
				null as iOptionRootID,
				C.symbol as vcSymbol,
				null as fStrike, 
				null as dtExpiry,
				null as tiIsCall,
				Ord.traderID as iTraderID,
				T.acronym as vcAccount,
				Ord.isBuy as tiIsBuy,
				Ord.orderType as iOrdType,
				Ord.orderQty as iQuantity,
				Ord.orderPrice as fPrice,
				Ord.orderDate as dtSendingTime,
				Ord.actionDate as dtTransactTime,
				Ord.destID as iDestID,
				OD.destCode as vcDestination,
				Ord.avgPrice as fAvgPrice,
				Ord.cumQty as iCumQty,
				null as vcOptionRootSymbol
				from [Order] Ord
					left join OrderExecDest OD on Ord.destID = OD.destID
					inner join ContractView C on C.contractID = Ord.contractID
					left join TraderView T on T.traderID = Ord.traderID
				where Ord.orderID = @iOrderID and C.contractTypeID < 3

	Commit Transaction

    set @error = 0
Return(@error)

finish:
		select -1
	Return(@error)

GO

/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.44.096')
	begin
		update DataInfo set keyValue = '3.44.096' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.44.096' '3.44.096 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
