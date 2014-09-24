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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.44.096' or (keyValue = '3.44.097')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.44.097 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.44.097' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- TntCounterParty table
----------------------------------------------------------------------------------------------------

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TntCounterParty]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[TntCounterParty]
	(
		[cptID] [int] IDENTITY (1, 1) NOT NULL ,
		[cptName] [varchar] (4) NOT NULL ,
		[cptDesc] [varchar] (255) NULL
		CONSTRAINT [PK_TntCounterParty] PRIMARY KEY  CLUSTERED 
			(
				[cptID]
			)
	)
end
GO

if not exists(select * from sysindexes where id=object_id('TntCounterParty') and name='IX_TntCounterParty_CptName')
	CREATE UNIQUE INDEX [IX_TntCounterParty_CptName] ON [dbo].[TntCounterParty]([cptName])
GO

----------------------------------------------------------------------------------------------------
-- usp_TntCounterParty_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TntCounterParty_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TntCounterParty_Get
GO

CREATE PROCEDURE dbo.usp_TntCounterParty_Get
	@iCptID int = null
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iCptID is null
		select
			cpt.cptID as iCptID,
			cpt.cptName as vcName,
			cpt.cptDesc as vcDesc
		from
			TntCounterParty cpt
	else
		select
			cpt.cptID as iCptID,
			cpt.cptName as vcName,
			cpt.cptDesc as vcDesc
		from
			TntCounterParty cpt
		where
			cpt.cptID = @iCptID

return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_TntCounterParty_Save
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TntCounterParty_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TntCounterParty_Save
GO

CREATE PROCEDURE usp_TntCounterParty_Save
	@iCptID int = null,
	@vcName varchar(4),
	@vcDesc varchar(255) = null,
	@tiRaiseError tinyint = null
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iCptID is null and isnull(@vcName, '') = ''
	begin
		raiserror ('Name for new counterparty can not be empty!', 16, 1)
		set @error = -1
		goto finish
	end

	if @iCptID is null
	begin
		select
			@iCptID = cpt.cptID
		from
			TntCounterParty cpt
		where
			cpt.cptName = @vcName

		if @iCptID is not null and isnull(@tiRaiseError, 0) <> 0 
		begin
			raiserror ('Counterparty with the same name is already exists!', 16, 1)
			set @error = -2
			goto finish
		end
	end

	Begin Transaction TntCounterParty

	if @iCptID is null
	begin
		insert into TntCounterParty
			(cptName, cptDesc)
		values
			(@vcName, @vcDesc)

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
		set @iCptID = @@identity
	end
	else
	begin
		update TntCounterParty set 
			cptName = isnull(@vcName, cptName),
			cptDesc = isnull(@vcDesc, cptDesc)
		where
			cptID = @iCptID

		if @@error <> 0 begin set @error = -4 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
	end

	Commit Transaction TntCounterParty

finish:
	if @error = 0
		return @iCptID
	else
		return @error

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

CREATE PROCEDURE dbo.usp_FixOrder_New
	@vcClOrdID varchar(50),
	@iContractID int,
	@iTraderID int,
	@iDestID int,
	@iOrderType int,  
	@tiIsBuy tinyint,
	@iQuantity int,  
	@fPrice float	
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is save information into Order table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	declare @dtOrderDate datetime set @dtOrderDate = getdate()

	--convert to GMT
	declare @iDeltaGMT int

	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	set @dtOrderDate = dateadd(minute, @iDeltaGMT, @dtOrderDate)

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
		select @iNewOrderID as iOrderID,@iSeqNum as iSeqNum, @dtOrderDate as dtOrderDate

	Commit Transaction

    set @error = 0
Return(@error)

finish:
		select -1 as iOrderID, -1 as iSeqNum , null as dtOrderDate where 1=0
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_FixOldOrder_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_FixOldOrder_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_FixOldOrder_Del
GO

CREATE PROCEDURE dbo.usp_FixOldOrder_Del
	@dtOrderDate datetime = null
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is delete old Order & OrderExec
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @dtOrderDate is null set @dtOrderDate = getdate()

	--convert to GMT
	declare @iDeltaGMT int

	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	set @dtOrderDate = dateadd(minute, @iDeltaGMT, @dtOrderDate)

	if not exists ( select 1 from [Order] where datediff(dd, orderDate, @dtOrderDate) > 0 ) 
		goto finish

	Begin Transaction

	delete [OrderExec]
		from [OrderExec] OE join [Order] O on O.orderID=OE.orderID
			where datediff(dd, O.orderDate, @dtOrderDate) > 0
	if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end

	delete [Order]
		where datediff(dd, orderDate, @dtOrderDate) > 0
	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction goto finish end


	Commit Transaction

    set @error = 0
Return(@error)

finish:
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_FixOrderAvgPrice_Calc
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_FixOrderAvgPrice_Calc' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_FixOrderAvgPrice_Calc
GO

CREATE PROCEDURE dbo.usp_FixOrderAvgPrice_Calc
	@iOrderID int,
	@fAvgPrc float output
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is calculate Average Price of order
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @iCumQty int

	Select @fAvgPrc = sum(OE.execPrice * OE.execQty) , @iCumQty =  sum(OE.execQty)
		from OrderExec OE 	
	where OE.orderID=@iOrderID and execStatus in (49, 50) -- partially filled, filled	

	if @@error <> 0 begin set @fAvgPrc = null goto finish end

	if isnull(@iCumQty,0) > 0 set @fAvgPrc = @fAvgPrc / @iCumQty
	else set @fAvgPrc = null

finish:
	Return(0)

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

CREATE PROCEDURE dbo.usp_FixOrderExec_New
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

	exec usp_FixOrderAvgPrice_Calc @iOrderID, @fAvgPrice output

	update [Order]
		set 
			status = @iOrderStatus,
			avgPrice = isnull(@fAvgPrice, avgPrice), 
			cumQty = isnull(@iCumQty, cumQty) 
		where orderID = @iOrderID

	if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction goto finish end

		--return output recordset
	select @iNewExecID as iExecID,@iSeqNum as iSeqNum,@fAvgPrice as fAvgPrice

	Commit Transaction

    set @error = 0
Return(@error)

finish:
		select -1 as iExecID, -1 as iSeqNum ,-1 as fAvgPrice where 1=0
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

CREATE PROCEDURE dbo.usp_FixOrderExec_Update
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

	declare @iOrderID int select @iOrderID = orderID from OrderExec where execID=@iExecID 

	exec usp_FixOrderAvgPrice_Calc @iOrderID, @fAvgPrice output

	update [Order]
		set 
			status = @iOrderStatus,
			avgPrice = isnull(@fAvgPrice, avgPrice), 
			cumQty = isnull(@iCumQty, cumQty) 
	from [Order] where orderID=@iOrderID

	if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction goto finish end

		--return output recordset
	select @iSeqNum as iSeqNum, @fAvgPrice as fAvgPrice 

	Commit Transaction

    set @error = 0
Return(@error)

finish:
		select  -1 as iSeqNum, -1 as fAvgPrice  where 1=0
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_TntCounterParty_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TntCounterParty_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TntCounterParty_Del
GO

CREATE PROCEDURE dbo.usp_TntCounterParty_Del
	@iCptID int
AS
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction TntCounterParty

	delete TntCounterParty where cptID = @iCptID
	if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end

	Commit Transaction TntCounterParty

finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- job_Cleanup_DB
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'job_Cleanup_DB' 
	   AND 	  type = 'P')
    DROP PROCEDURE job_Cleanup_DB
GO

CREATE PROCEDURE dbo.job_Cleanup_DB
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is remove information from base tables with actionID = 3
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction DB_Cleanup

		--SyntheticUnderlyingParam by actionID = 3
		exec @error = usp_FixOldOrder_Del
  			if @error <> 0 begin if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--SyntheticUnderlyingParam by actionID = 3
		delete SyntheticUnderlyingParam where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--SyntheticUnderlyingParam by Contract.actionID = 3
		delete SyntheticUnderlyingParam from SyntheticUnderlyingParam s join Contract c on s.underlyingID = c.contractID
			where c.actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IRPoint
		delete IRPoint where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IRCurve
		delete IRPoint where curveID in (select curveID from IRCurve where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete IRCurve where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--ContractInGroup
		delete ContractInGroup where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--ContractGroup
		delete ContractInGroup where groupID in (select groupID from ContractGroup where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete ContractGroup where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IndexDefinition
		delete IndexDefinition where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--ExchangeHoliday
		delete ExchangeHoliday where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Holiday
		delete ExchangeHoliday where holidayID in (select holidayID from Holiday where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete Holiday where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Strategy
		delete Strategy 
			where actionID = 3 and strategyID not in 
				(select distinct strategyID from Trade where strategyID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--TraderContract
		delete TraderContract where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--RMScenario
		delete RMScenario where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--CustomStrikeSkewPoint
		delete from CustomStrikeSkewPoint where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--VolaSurfaceData
		delete VolaSurfaceData where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--VolaSurfaceData
		delete CustomStrikeSkewPoint 
			where volaSurfaceDataID in (select volaSurfaceDataID from VolaSurfaceData where actionID = 3)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--don't forget about TraderRole

		--TraderContract 
		delete from TraderContract 
			where traderID in 
				(select traderID from Trader where actionID = 3 and traderID not in (select distinct traderID from Trade where traderID is not null)
					and traderID not in (select distinct traderID from [Order] where traderID is not null))
			or traderID in 
				(select traderRefID from Trader where actionID = 3 and traderRefID not in (select distinct traderID from Trade where traderID is not null)
					and traderRefID not in (select distinct traderID from [Order] where traderID is not null))
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Trader
		delete Trader 
			where traderRefID in 
				(select traderID from Trader where actionID = 3 and traderID not in (select distinct traderID from Trade where traderID is not null)
					and traderID not in (select distinct traderID from [Order] where traderID is not null))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete Trader where traderID not in (select distinct traderID from Trade) and actionID =3
			and traderID not in (select distinct traderID from [Order] where traderID is not null)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Commission
		delete Commission
			where actionID = 3 and 
				commissionID not in (select distinct brokerCommissionID from Trade where brokerCommissionID is not null) and 
				commissionID not in (select distinct clearingBrokerCommissionID from Trade where clearingBrokerCommissionID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete Commission
			where brokerID in 
					(select brokerID from Broker
						where actionID = 3 and 
							brokerID not in (select distinct clearingBrokerID from Trade where clearingBrokerID is not null) and 
							brokerID not in (select distinct brokerID from Trade where brokerID is not null))
 			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Broker
		delete Broker 
			where actionID = 3 and 
				brokerID not in (select distinct clearingBrokerID from Trade where clearingBrokerID is not null) and 
				brokerID not in (select distinct brokerID from Trade where brokerID is not null)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
	
		--BookRuleItemMask
		delete BookRuleItemMask 
			where bookRuleItemID in (select bookRuleItemID from BookRuleItem where bookID in 
					(select bookID 
						from Book 
							where actionID = 3 and bookID not in (select distinct bookID from Trade where bookID is not null)))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--BookRuleItem
		delete BookRuleItem
			where bookID in 
					(select bookID 
						from Book 
							where actionID = 3 and bookID not in (select distinct bookID from Trade where bookID is not null))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Book
		delete Book
			where actionID = 3 and bookID not in (select distinct bookID from Trade where bookID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--=========================================================================================================
		--INDEX
		if object_id('tempdb..#IDs') is null
			Create table #IDs(contractID int not null)
		else
			Delete from #IDs
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--select indices for deletion (which not releated with Trades)
		insert into #IDs
			select indexID from [Index] 
				where actionID = 3 
						and indexID not in (select distinct contractID from Trade T join [Index] I on T.contractID = I.indexID)
						and indexID not in (select indexID 
												from [Index] I 
														join OptionRoot ort on ort.underlyingID = i.indexID
														join [Option] o on ort.optionRootID = o.optionRootID
														join Trade T on T.contractID = O.optionID)
						and indexID not in (select distinct contractID from [Order] Ord join [Index] I on Ord.contractID = I.indexID)
						and indexID not in (select indexID 
												from [Index] I 
														join OptionRoot ort on ort.underlyingID = i.indexID
														join [Option] o on ort.optionRootID = o.optionRootID
														join [Order] Ord on Ord.contractID = O.optionID)
 			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IndexCorrelation
		delete from IndexCorrelation 
			where indexID1 in (select contractID from #IDs) or indexID2 in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IndexDataHistory
		delete from IndexDataHistory where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		--IndexDefinition
		delete from IndexDefinition where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--delete all related rows in Contract table by indexID
		if object_ID('tempdb..#Contracts_for_delete') is null
			Create table #Contracts_for_delete(contractID int)
		else
			Delete from #Contracts_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		insert into #Contracts_for_delete
			select contractID from #IDs 
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Contract_Del Null,1
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Index
		delete from [Index] where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Contract
		delete from Contract where contractID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		if object_id('tempdb..#IDs') is not null
			Drop table #IDs
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--=====================================================================================================================
		--STOCK
		delete StockDataHistory 
			where stockID in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order]))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete StockCorrelation 
			where stockID1 in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order]))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete StockCorrelation 
			where stockID2 in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order]))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete IndexDefinition 
			where stockID in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order]))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		if object_ID('tempdb..#Contracts_for_delete') is not null
			delete from #Contracts_for_delete
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		insert into #Contracts_for_delete
			select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order])
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Contract_Del Null,1
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete from Stock where stockID in (select contractID from #Contracts_for_delete)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end


		delete from Contract where contractID in (select contractID from #Contracts_for_delete)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		--=====================================================================================================================
		--OPTION

		if object_ID('tempdb..#Options_for_delete') is null
			create table #Options_for_delete(OptionID int)
		else
			delete from #Options_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		insert into #Options_for_delete 
			select optionID 
				from [Option] O
				where optionID not in (select distinct ContractID from Trade) and actionID = 3
					and optionID not in (select distinct ContractID from [Order])
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Option_Del null,1
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		--=====================================================================================================================

		--fix DB cleanup date	
		--update DataInfo set keyValue = GetDate() where dataInfoID = 6

		update DataInfo set keyValue = convert(varchar, GetDate(), 120) where dataInfoID = 6
		if @@ROWCOUNT = 0 
			begin 
				insert into DataInfo 
				    select 6,'lastCleanupDate',convert(varchar, GetDate(), 120)
	  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
			end 
	Commit Transaction DB_Cleanup
    set @error = 0
finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- job_FixMarketStructure
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'job_FixMarketStructure' 
	   AND 	  type = 'P')
    DROP PROCEDURE job_FixMarketStructure
GO

CREATE PROCEDURE dbo.job_FixMarketStructure
	@iNumberOfHours int = null,
	@iContractID int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure mark us deleted unusefull options
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iNumberOfHours is null 
		set @iNumberOfHours = 2

	Create table #Options_for_delete(OptionID int) 

	insert into #Options_for_delete
		select O.optionID--,E.expiryDate 
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract UC on ORt.underlyingID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where E.expiryDate < GetDate() 
					and O.isManualyEntered = 0
					and O.actionID < 3
					and O.optionID not in (select distinct ContractID from Trade)
					and O.optionID not in (select distinct ContractID from [Order])

	exec usp_Option_Del
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

    set @error = 0
finish:
		Return(@error)

GO


----------------------------------------------------------------------------------------------------
-- usp_Contract_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Contract_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Contract_Del
GO

CREATE PROCEDURE dbo.usp_Contract_Del
	@iContractID int,			-- contract identifier
	@bisRemovePermanent bit = 0
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if object_ID('tempdb..#Contracts_for_delete') is null
		Create table #Contracts_for_delete(contractID int)

	if @bisRemovePermanent = 0
		begin
			Raiserror('Couldn''t change actionID of Contracts!',16,1)
			Return(-1)
		end

	if @iContractID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iContractID)
				begin
					Raiserror ('Error. Can''t delete Contract because of the existing Trade', 16, 1)
					Return (-1)
				end   
			if exists (select orderID from [Order] where contractID = @iContractID)
				begin
					Raiserror ('Error. Can''t delete Contract because of the existing Order', 16, 1)
					Return (-2)
				end   
			insert into #Contracts_for_delete
				select @iContractID
		end

	Begin Transaction DelContract
		--SyntheticRootIndexBeta
		delete from SyntheticRootIndexBeta where indexID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--ContractExchangeLink
		delete from ContractExchangeLink
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--ContractPrice
		delete from ContractPrice 
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--ContractInGroup
		delete from ContractInGroup
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--ContractMarketData
		delete from ContractMarketData where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--TraderContract 
		delete from TraderContract where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--NextDaySurface
		delete from NextDaySurface where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--CustomStrikeSkewPoint
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData 
					where contractID in (select contractID from #Contracts_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--IndexDefinition by IndexID
		delete from IndexDefinition where indexID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--IndexDefinition by stockID
		delete from IndexDefinition where indexID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--IndexBeta by indexID
		delete from IndexBeta where indexID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--IndexBeta by contractID
		delete from IndexBeta where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--VolaSurfaceData
		delete from VolaSurfaceData where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
		--VolaBands
		delete from VolaBands where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end

		--remove options of above selected contracts
		if object_ID('tempdb..#Options_for_delete') is null
			create table #Options_for_delete(OptionID int)
		else
			delete from #Options_for_delete

		insert into #Options_for_delete 
			select optionID 
				from [Option] O
					join optionRoot ort on o.optionRootID = ort.optionRootID
					join #Contracts_for_delete C on C.ContractID = Ort.underlyingID

				where O.optionID not in (select distinct contractID from Trade)
					and O.optionID not in (select distinct contractID from [Order])
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end

		exec usp_Option_Del null,1
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end

		delete from #Options_for_delete
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end

		insert into #Options_for_delete 
			select contractID from #Contracts_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end

		exec usp_Option_Del null,1,0
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelContract goto finish end
	Commit Transaction DelContract

finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_ExpiredOptions_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_ExpiredOptions_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_ExpiredOptions_Del
GO

CREATE PROCEDURE dbo.usp_ExpiredOptions_Del
	@iContractID int -- stock or index identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure mark us deleted unusefull options
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Create table #Options_for_delete(OptionID int) 

	Create table #O_with_trades(symbol varchar(20)) 


	--=============================================================
	-- select options for remove
	--=============================================================
	insert into #Options_for_delete
		select O.optionID
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract UC on ORt.underlyingID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where ( E.expiryDate < GetDate() 
					or ( datediff(d, O.actionDate, UC.MSupdateDate) > 0
					and O.isManualyEntered = 0 ))
					and O.actionID < 3
					and O.optionID not in (select distinct ContractID from Trade)
					and O.optionID not in (select distinct ContractID from [Order])
					and UC.contractID = @iContractID

	--=============================================================
	-- select options, which have a trades or orders
	--=============================================================
	insert into #O_with_trades
		select C.symbol
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C on O.optionID = C.contractID
					join Contract UC on ORt.underlyingID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where ( E.expiryDate < GetDate() 
					or ( datediff(d, O.actionDate, UC.MSupdateDate) > 0
					and O.isManualyEntered = 0 ))
					and O.actionID < 3
					and (O.optionID in (select distinct ContractID from Trade) 
					or O.optionID in (select distinct ContractID from [Order]))
					and UC.contractID = @iContractID

	exec usp_Option_Del
		if @@error != 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

	select symbol as vcSymbol from #O_with_trades

set @error = 0

finish:
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_Index_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Index_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Index_Del
GO

CREATE PROCEDURE dbo.usp_Index_Del
		@iIndexID int
  as
/*
	Created by Zaur Nuraliev 
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	 if exists (select tradeID from Trade
					 where Trade.contractID = @iIndexID)
	 begin
	   RAISERROR ('Unable to delete the Index. There is an open position on it', 16, 1)
	   RETURN (-1)
	 end

	 if exists (select 1 from [Order]
					 where [Order].contractID = @iIndexID)
	 begin
	   RAISERROR ('Unable to delete the Index. There is an orders on it', 16, 1)
	   RETURN (-1)
	 end

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iIndexID

	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Unable to delete the Index. There are an open positions on its options', 16, 1)
			Return (-1)
		end 

	if exists (select 1 from [Order] where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Unable to delete the Index. There are an orders on its options', 16, 1)
			Return (-1)
		end 

	Begin Transaction IndexDel

		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = 0
			end

		delete from IndexCorrelation where indexID1 = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexCorrelation where indexID2 = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexDataHistory where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update IndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update IndexDefinition set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update ContractInGroup set actionID=3 where contractID = @iIndexID 
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update [Index] set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update Contract set actionID=3 
			where contractID in 
				(select OptionID 
					from [Option] O 
							join OptionRoot ORt on O.optionRootID=ORt.optionRootID and ORt.underlyingID=@iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticUnderlyingParam set actionID=3 where underlyingID = @iIndexID or syntheticOptRootID in 
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

----------------------------------------------------------------------------------------------------
-- usp_OptionPair_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OptionPair_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OptionPair_Del
GO

CREATE PROCEDURE dbo.usp_OptionPair_Del
	@iOptionIDCall int,
	@iOptionIDPut int
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Option table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Create table #Options_for_delete(OptionID int)
	if  @iOptionIDCall is not null
		begin
			insert into #Options_for_delete values(@iOptionIDCall)
				if @@error > 0 begin set @error = 1 goto finish end
		end
	if  @iOptionIDPut is not null
		begin
			insert into #Options_for_delete values(@iOptionIDPut)
			if @@error > 0 begin set @error = 1 goto finish end
		end
	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Unable to delete the Option. There is an open position on it.', 16, 1)
			Return (-1)
		end 
	if exists (select 1 from [Order] where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Unable to delete the Option. There is an order on it.', 16, 1)
			Return (-2)
		end 
	if object_ID('tempdb..#Options_for_delete') is not null
		begin
			exec usp_Option_Del
			if @@error <> 0 set @error = 1 goto finish
		end
	set @error = @@error
finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_OptionPermanent_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OptionPermanent_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OptionPermanent_Del
GO

CREATE PROCEDURE dbo.usp_OptionPermanent_Del
	@iOptionID int = null 	-- contract identifier
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
			if exists (select 1 from [Order] where contractID = @iOptionID)
				begin
					Raiserror ('Error. Can''t delete [Option]. There is order on it.', 16, 1)
					Return (-2)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete from TradeLog where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from DualQuadraticParams where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaBands where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete [Option] where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete Contract where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	Commit Transaction DelOpt

finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_Option_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Option_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Option_Del
GO

CREATE PROCEDURE dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0,
	@bisRemoveFromContract bit = 1,
	@bisRaiseError bit = 1
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
					if @bisRaiseError = 1 
						begin
							Raiserror ('Unable to delete the Option. There is an open position on it.', 16, 1)
						end
					Return (-2)
				end   
			if exists (select 1 from [Order] where contractID = @iOptionID)
				begin
					if @bisRaiseError = 1 
						begin
							Raiserror ('Unable to delete the Option. There is an order on it.', 16, 1)
						end
					Return (-3)
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

		--IndexDefinition by contractID
		update IndexBeta set actionID = 3 where contractID in (select OptionID from #Options_for_delete)
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


		if @bisRemovePermanent = 1
			begin
				--IndexBeta by contractID
				delete from IndexBeta where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

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

----------------------------------------------------------------------------------------------------
-- usp_Option_Check
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Option_Check' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Option_Check
GO

CREATE PROCEDURE dbo.usp_Option_Check 
	@iOptionID int = null, 
	@vcSymbol varchar(20),
	@iOptionRootID int,
	@dtExpiry smalldatetime,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcUnderlyingSymbol varchar(8),
	@ERR_MSG varchar(8000) = null output,
	@bRemoveOption bit = null   -- call usp_Option_Del for this option
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
		and check if option with same symbol exists
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iOptionID_bad int,
			@vcUnderlying_bad varchar(8),
			@vcOptionRoot_bad varchar(20),
			@dtExpiry_bad smalldatetime,
			@fStrike_bad float,
			@iIsCall_bad tinyint,
			@vcSymbol_bad varchar(20)

	if @iOptionID is null
		select @iOptionID_bad = optionID 
			from OptionView O join Contract C on O.optionID = C.contractID
			where C.symbol = @vcSymbol and
			(optionRootID != @iOptionRootID or expiryID != @iExpiryID or abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)
	else
		select @iOptionID_bad = optionID 
		from OptionView O join Contract C on O.optionID = C.contractID
		where optionID != @iOptionID and C.symbol = @vcSymbol and
		(optionRootID != @iOptionRootID or expiryID != @iExpiryID or abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)


	if @iOptionID_bad is not null
	begin
		if @bRemoveOption is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0
			end
		
		if (@error = -2) or (@error = -3) or (@bRemoveOption is null)
			begin
				select @vcSymbol_bad = symbol from ContractView where contractID = @iOptionID_bad
				select	@vcOptionRoot_bad = ORT.symbol
						from OptionView O join OptionRootView ORT on ORT.optionRootID = O.optionRootID
						where O.optionID = @iOptionID_bad
				select	@vcUnderlying_bad = C.symbol
						from ContractView C 
						join OptionRootView ORT on ORT.underlyingID = C.contractID
						join OptionView O on ORT.optionRootID = O.optionRootID
						where O.optionID = @iOptionID_bad
				select @dtExpiry_bad = expiryPeriod 
						from ExpiryView E join OptionView O on E.expiryID = O.expiryID 
						where O.optionID = @iOptionID_bad
				select 	@fStrike_bad = strike,
						@iIsCall_bad = isCall 
						from OptionView where optionID = @iOptionID_bad
				

			set @ERR_MSG = 'Can''t add option with Symbol=' + '''' +  @vcSymbol + ''''  +  
			CASE WHEN @vcUnderlying_bad != @vcUnderlyingSymbol THEN
				', Underlying=' + '''' + @vcUnderlyingSymbol + ''''
			ELSE '' END + 
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol,len(@vcSymbol)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) + '''' 
			ELSE '' END +  
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END  
			ELSE '' END +  
			CASE WHEN @error = -2 THEN
				' because exists option with trades and Symbol=' + '''' +  @vcSymbol_bad + ''''
				WHEN @error = -3 THEN
				' because exists option with orders and Symbol=' + '''' +  @vcSymbol_bad + ''''
				WHEN @error != -2 and @error != -3 THEN
				' because exists option with Symbol=' + '''' +  @vcSymbol_bad + ''''
			ELSE '' END + 
			CASE WHEN @vcUnderlying_bad != @vcUnderlyingSymbol THEN
				', Underlying=' + '''' + @vcUnderlying_bad + ''''
			ELSE '' END + 
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol_bad,len(@vcSymbol_bad)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike_bad, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) + ''''
			ELSE '' END + 
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall_bad WHEN 1 THEN 'Call' ELSE 'Put' END
			ELSE '' END +
			CASE WHEN @vcUnderlying_bad = @vcUnderlyingSymbol THEN
			', Underlying=' + '''' + @vcUnderlyingSymbol + ''''
			ELSE '' END
		end	
		Return(-1)
	end
	else
		begin
			set @ERR_MSG = null
			set @iOptionID_bad = null
		end

	if @iOptionID is null
		select @iOptionID_bad = optionID, @vcSymbol_bad = C.symbol
			from OptionView O join ContractView C on O.optionID = C.contractID
			where (optionRootID = @iOptionRootID and expiryID = @iExpiryID and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)
	else
		select @iOptionID_bad = optionID, @vcSymbol_bad = C.symbol
			from OptionView O join ContractView C on O.optionID = C.contractID
			where (optionID != @iOptionID) and
			(optionRootID = @iOptionRootID and expiryID = @iExpiryID and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)

	if @iOptionID_bad is not null
	begin
		if @bRemoveOption = 1
			begin
				--===================================================================
				-- update option symbol, if option with same attributes exists
				--===================================================================		
				update contract set symbol = @vcSymbol where contractID = @iOptionID_bad
				set @ERR_MSG = '*'
			end
		else
			begin
				set @ERR_MSG = 'Can''t add option ' + '''' +  @vcSymbol + ''''  +  
				' because already exists option ' + '''' +  @vcSymbol_bad + '''' +
				' with the same parameters'
			end
	end

Return(@@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_Stock_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Stock_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Stock_Del
GO

CREATE PROCEDURE dbo.usp_Stock_Del
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
	
	if exists (select * from Trade(nolock) where contractID = @iStockID)
	begin
		Raiserror ('Unable to delete the Stock. There is an open position on it', 16, 1)
		Return (-1)
	end 

	if exists (select 1 from [Order](nolock) where contractID = @iStockID)
	begin
		Raiserror ('Unable to delete the Stock. There is an order on it', 16, 1)
		Return (-2)
	end 

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iStockID

	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Unable to delete the Stock. There are an open positions on its options', 16, 1)
			Return (-3)
		end 

	if exists (select 1 from [Order] where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Unable to delete the Stock. There are an orders on its options', 16, 1)
			Return (-4)
		end 

	Begin Transaction StockDel
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = @bisRemovePermanent
			end

		exec usp_TradeLog_Del null, @iStockID

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete from NextDaySurface where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete VolaBands where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update IndexBeta set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update ContractExchangeLink set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end


        delete ContractPrice where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update SyntheticUnderlyingParam set actionID=3 where underlyingID = @iStockID or syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update OptionRoot set actionID = 3 where underlyingID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		if (@bisRemovePermanent = 1) 
			begin
        		delete from ContractInGroup where contractID = @iStockID 
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
				delete from ContractExchangeLink where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
	        	delete IndexDefinition where stockID = @iStockID  
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete SyntheticUnderlyingParam where underlyingID = @iStockID or syntheticOptRootID in
					(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete SyntheticRootIndexBeta where syntheticOptRootID in
					(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete SyntheticOptionRoot where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iStockID)
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete IndexBeta where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete OptionRoot where underlyingID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
				delete TraderContract where contractID = @iStockID  
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		       	delete Stock where stockID = @iStockID  
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
	        	delete Contract where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
			end 
  
		set @error = @@error
	Commit Transaction
finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_TraderContract_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TraderContract_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TraderContract_Del
GO

CREATE PROCEDURE dbo.usp_TraderContract_Del 
	@iContractID int = null,
	@iTraderID int = null,
	@tiIsRaiseError tinyint = 1 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from TraderContract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if exists(select 1 from Trade 
		where (traderID = @iTraderID or @iTraderID is null) and (contractID = @iContractID or @iContractID is null))
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove trader contract, because he related with active trades!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end

	if exists(select 1 from [Order] 
		where (traderID = @iTraderID or @iTraderID is null) and (contractID = @iContractID or @iContractID is null))
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove trader contract, because he related with orders!',16,1)
					Return(0)
				end
			else
				begin
					Return(-2)
				end
		end

	if 	@iTraderID is not null and @iContractID is not null
		update TraderContract set actionID=3
			where traderID = @iTraderID  and ContractID=@iContractID
	else if @iTraderID is not null and @iContractID is null
		update TraderContract set actionID=3
			where traderID = @iTraderID
	else if @iTraderID is null and @iContractID is not null
		update TraderContract set actionID=3
			where ContractID=@iContractID
	else if @iTraderID is null and @iContractID is null
		update TraderContract set actionID=3


	set @error = @@error
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_Trader_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trader_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trader_Del
GO

CREATE PROCEDURE dbo.usp_Trader_Del
        @iTraderID int,
		@tiIsRaiseError tinyint = 1 
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if exists(select 1 from Trade where traderID = @iTraderID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove trader, because he have a trades!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end
	
	if exists(select 1 from [Order] where traderID = @iTraderID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove trader, because he have a orders!',16,1)
					Return(0)
				end
			else
				begin
					Return(-2)
				end
		end

	Begin Transaction
        update Trader set actionID=3 where traderRefID = @iTraderID		
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        update Trader set actionID=3 where traderID = @iTraderID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction

finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_OrderExecDest_Del
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OrderExecDest_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OrderExecDest_Del
GO

CREATE PROCEDURE dbo.usp_OrderExecDest_Del
		@iDestID int,
		@tiIsRaiseError tinyint = 1 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if exists (select 1 from [Order] where destID = @iDestID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove order destination, because it has orders!',16,1)
					goto finish
				end
			else
				begin
					set @error = -1
					goto finish
				end
		end

	Begin Transaction

		delete OrderExecDest where destID = @iDestID
			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction
	set @error = 0

finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_OrderExecDest_Save
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OrderExecDest_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OrderExecDest_Save
GO

CREATE PROCEDURE dbo.usp_OrderExecDest_Save
	@iDestID int = null,
	@vcCode varchar(20),
	@vcName varchar(100),
	@tiIsOption tinyint,
	@tiIsStock	tinyint
as
	----------------------------------------------------
	set nocount on
	----------------------------------------------------
	declare @error int		set @error = 0

	if @iDestID is null and isnull(@vcCode, '') = ''
	begin
		raiserror ('Code for new order destination can not be empty!', 16, 1)
		set @error = -1
		goto finish
	end

	if @iDestID is null
	begin
		select
			@iDestID = destID
		from
			OrderExecDest
		where
			destCode = @vcCode

		if @iDestID is not null
		begin
			raiserror ('Order destination with the same code is already exists!', 16, 1)
			set @error = -2
			goto finish
		end
	end

	Begin Transaction

	if @iDestID is Null
		begin
			insert into OrderExecDest
				(destCode,destName,isOption,isStock)
			values
				(@vcCode, @vcName, @tiIsOption, @tiIsStock)
	
			if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

			set @iDestID = @@identity
		end
	else 
		begin
			update OrderExecDest
			set destCode = @vcCode,
				destName = @vcName,
				isOption = @tiIsOption,
				isStock = @tiIsStock
			where 
				destID = @iDestID

			if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

			set @iDestID = @iDestID
		end

	Commit Transaction

finish:
	if @error = 0
		return @iDestID
	else
		return @error

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.44.097')
	begin
		update DataInfo set keyValue = '3.44.097' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.44.097' '3.44.097 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
