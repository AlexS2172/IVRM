/*
	update for DB - synchronizing support for HH (changes in tables, triggers and views)
*/

set nocount on
GO
--======================================================================================================================
-- changes in table ExpiryCalendar
--======================================================================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryCalendar' and COLUMN_NAME='actionID')
	begin
		Alter Table ExpiryCalendar
			Add actionID tinyint not null constraint DF_ExpiryCalendar_actionID default(1)
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryCalendar' and COLUMN_NAME='actionDate')
	begin
		Alter Table ExpiryCalendar
			Add actionDate datetime not null constraint DF_ExpiryCalendar_actionDate default(GetDate())

	end
GO
--cutting of milliseconds
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryCalendar' and COLUMN_NAME='actionDate')		
	begin
		if object_id('tU_ExpiryCalendar') is not null 
			Alter table ExpiryCalendar Disable trigger tU_ExpiryCalendar
		
		update ExpiryCalendar set actionDate = convert(varchar(19),GetDate(),120)
		
		if object_id('tU_ExpiryCalendar') is not null 
			Alter table ExpiryCalendar Enable trigger tU_ExpiryCalendar
	end
GO
--======================================================================================================================
-- changes in table ExpiryInCalendar
--======================================================================================================================
if exists(select * from INFORMATION_SCHEMA.CONSTRAINT_COLUMN_USAGE where CONSTRAINT_NAME = 'PK_ExpiryInCalendar' and COLUMN_NAME in ('expCalendarID','expiryID'))
		and object_id('PK_ExpiryInCalendar') is not null 
	Alter table ExpiryInCalendar Drop Constraint PK_ExpiryInCalendar 
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryInCalendar' and COLUMN_NAME='expiryInCalendarID')
	begin
		Alter Table ExpiryInCalendar
			Add expiryInCalendarID int identity(1,1) not null
	end
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryInCalendar' and COLUMN_NAME='expiryInCalendarID')
		and object_id('PK_ExpiryInCalendar') is null
	Alter Table ExpiryInCalendar add constraint [PK_ExpiryInCalendar] primary key clustered (expiryInCalendarID)
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryInCalendar' and COLUMN_NAME='actionID')
	begin
		Alter Table ExpiryInCalendar
			Add actionID tinyint not null constraint DF_ExpiryInCalendar_actionID default(1)
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryInCalendar' and COLUMN_NAME='actionDate')
	begin
		Alter Table ExpiryInCalendar
			Add actionDate datetime not null constraint DF_ExpiryInCalendar_actionDate default(GetDate())

	end
GO
--cutting of milliseconds
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryInCalendar' and COLUMN_NAME='actionDate')		
	begin
		if object_id('tU_ExpiryInCalendar') is not null 
			Alter table ExpiryInCalendar Disable trigger tU_ExpiryInCalendar
		
		update ExpiryInCalendar set actionDate = convert(varchar(19),GetDate(),120)
		
		if object_id('tU_ExpiryInCalendar') is not null 
			Alter table ExpiryInCalendar Enable trigger tU_ExpiryInCalendar
	end
GO
--======================================================================================================================
-- changes in table ContractExchangeLink (+ fields actionID and actionDate)
--======================================================================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractExchangeLink' and COLUMN_NAME='actionID')
	begin
		Alter Table ContractExchangeLink
			Add actionID tinyint not null constraint DF_ContractExchangeLink_actionID default(1)
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractExchangeLink' and COLUMN_NAME='actionDate')
	begin
		Alter Table ContractExchangeLink
			Add actionDate datetime not null constraint DF_ContractExchangeLink_actionDate default(GetDate())

	end
GO
--cutting of milliseconds
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractExchangeLink' and COLUMN_NAME='actionDate')		
	begin
		if object_id('tU_ContractExchangeLink') is not null 
			Alter table ContractExchangeLink Disable trigger tU_ContractExchangeLink
		
		update ContractExchangeLink set actionDate = convert(varchar(19),GetDate(),120)
		
		if object_id('tU_ContractExchangeLink') is not null 
			Alter table ContractExchangeLink Enable trigger tU_ContractExchangeLink
	end
GO
--======================================================================================================================
--triggers for ExpiryCalendar, ExpiryInCalendar, ContractExchangeLink
--======================================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_ContractExchangeLink]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_ContractExchangeLink]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_ExpiryCalendar]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_ExpiryCalendar]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_ExpiryInCalendar]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_ExpiryInCalendar]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE   Trigger tU_ContractExchangeLink ON dbo.ContractExchangeLink
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update ContractExchangeLink 
		set actionDate = GetDate(),actionID = inserted.actionID
	from ContractExchangeLink CEL
			join inserted on CEL.ContractExchangeLinkID = inserted.ContractExchangeLinkID
			join deleted on CEL.ContractExchangeLinkID = deleted.ContractExchangeLinkID
	where inserted.actionID > deleted.actionID

	update ContractExchangeLink 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from ContractExchangeLink CEL
			join inserted on CEL.ContractExchangeLinkID = inserted.ContractExchangeLinkID
			join deleted on CEL.ContractExchangeLinkID = deleted.ContractExchangeLinkID
	where inserted.actionID < = deleted.actionID
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE  Trigger tU_ExpiryCalendar ON dbo.ExpiryCalendar
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update ExpiryCalendar 
		set actionDate = GetDate(),actionID = inserted.actionID
	from ExpiryCalendar EC
			join inserted on EC.expCalendarID = inserted.expCalendarID
			join deleted on EC.expCalendarID = deleted.expCalendarID
	where inserted.actionID > deleted.actionID

	update ExpiryCalendar 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from ExpiryCalendar EC
			join inserted on EC.expCalendarID = inserted.expCalendarID
			join deleted on EC.expCalendarID = deleted.expCalendarID
	where inserted.actionID < = deleted.actionID
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE  Trigger tU_ExpiryInCalendar ON dbo.ExpiryInCalendar
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update ExpiryInCalendar 
		set actionDate = GetDate(),actionID = inserted.actionID
	from ExpiryInCalendar EIC
			join inserted on EIC.expiryInCalendarID = inserted.expiryInCalendarID
			join deleted on EIC.expiryInCalendarID = deleted.expiryInCalendarID
	where inserted.actionID > deleted.actionID

	update ExpiryInCalendar 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from ExpiryInCalendar EIC
			join inserted on EIC.expiryInCalendarID = inserted.expiryInCalendarID
			join deleted on EIC.expiryInCalendarID = deleted.expiryInCalendarID
	where inserted.actionID < = deleted.actionID
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--======================================================================================================================
--views for ExpiryCalendar, ExpiryInCalendar, ContractExchangeLink
--======================================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractExchangeLinkView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractExchangeLinkView]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExpiryCalendarView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExpiryCalendarView]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExpiryInCalendarView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExpiryInCalendarView]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
Create View dbo.ExpiryInCalendarView
as
	Select 
		expCalendarID,
		expiryID, 
		expiryDate, 
		vegaWeight, 
		actionID, 
		actionDate
	from ExpiryInCalendar
	where actionID < 3
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
Create View dbo.ExpiryCalendarView
as
	Select 
		expCalendarID, 
		expCalendarName, 
		expCalendarDesc, 
		actionID, 
		actionDate
	from ExpiryCalendar
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
Create View dbo.ContractExchangeLinkView
as
	select 
		contractExchangeLinkID, 
		exchangeID, 
		contractID, 
		isUnderlying, 
		isOption, 
		actionID, 
		actionDate
	from ContractExchangeLink
	where actionID < 3
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--======================================================================================================================
--recreation of all other triggers
--======================================================================================================================
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  trigger tU_TraderContract ON dbo.TraderContract
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update TraderContract 
		set actionDate = GetDate(),actionID = inserted.actionID
	from TraderContract TC
			join inserted on TC.traderContractID = inserted.traderContractID
			join deleted on TC.traderContractID = deleted.traderContractID
	where inserted.actionID > deleted.actionID

	update TraderContract 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from TraderContract TC
			join inserted on TC.traderContractID = inserted.traderContractID
			join deleted on TC.traderContractID = deleted.traderContractID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER        TRIGGER tU_Trader ON dbo.Trader
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on

	update Trader
		set actionDate = GetDate(),actionID = inserted.actionID
	from Trader T
			join inserted on T.traderID = inserted.traderID
			join deleted on T.traderID = deleted.traderID
	where inserted.actionID > deleted.actionID

	update 
		Trader set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Trader T
			join inserted on T.traderID = inserted.traderID
			join deleted on T.traderID = deleted.traderID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER         TRIGGER tU_Broker ON dbo.Broker
    FOR Update
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	--======================================================================
	-- fix row modifications
	--======================================================================
	update Broker
		set actionDate = GetDate(),actionID = inserted.actionID
	from Broker B
			join inserted on B.BrokerID = inserted.BrokerID
			join deleted on B.BrokerID = deleted.BrokerID
	where inserted.actionID > deleted.actionID

	update 
		Broker set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Broker B
			join inserted on B.BrokerID = inserted.BrokerID
			join deleted on B.BrokerID = deleted.BrokerID
	where inserted.actionID < = deleted.actionID	

	--======================================================================
	-- check programm logic
	--======================================================================
	if Update (IsClearing)
	 begin
	  if exists (select t.tradeID 
	                    from inserted i, Trade t
	                 where i.IsClearing = 0
	                   and t.ClearingbrokerID = i.brokerID and t.actionID<3)
	     begin 
	         RAISERROR ('Error Can''t update IsClearing field in Brokers. Some brokers are clearing brokers in Trade table!', 16, 1)
	         ROLLBACK TRANSACTION
	         RETURN
	     end
	  if exists (select t.tradeID 
	                    from inserted i, Trade t
	                 where i.IsClearing <> 0
	                   and t.brokerID = i.brokerID and t.actionID<3)
	     begin 
	         RAISERROR ('Error Can''t update IsClearing field in Brokers. Some brokers are nonclearing brokers in Trade table!', 16, 1)
	         ROLLBACK TRANSACTION
	         RETURN
	     end 
	end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER   Trigger tU_Commission ON dbo.Commission
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update Commission 
		set actionDate = GetDate(),actionID = inserted.actionID
	from Commission C
			join inserted on C.CommissionID = inserted.CommissionID
			join deleted on C.CommissionID = deleted.CommissionID
	where inserted.actionID > deleted.actionID

	update 
		Commission set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Commission C
			join inserted on C.CommissionID = inserted.CommissionID
			join deleted on C.CommissionID = deleted.CommissionID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_Book ON dbo.Book
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update Book 
		set actionDate = GetDate(),actionID = inserted.actionID
	from Book B
			join inserted on B.BookID = inserted.BookID
			join deleted on B.BookID = deleted.BookID
	where inserted.actionID > deleted.actionID

	update 
		Book set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Book B
			join inserted on B.BookID = inserted.BookID
			join deleted on B.BookID = deleted.BookID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_TraderRole ON dbo.TraderRole
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update TraderRole 
		set actionDate = GetDate(),actionID = inserted.actionID
	from TraderRole TR
			join inserted on TR.TraderRoleID = inserted.TraderRoleID
			join deleted on TR.TraderRoleID = deleted.TraderRoleID
	where inserted.actionID > deleted.actionID

	update TraderRole 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from TraderRole TR
			join inserted on TR.TraderRoleID = inserted.TraderRoleID
			join deleted on TR.TraderRoleID = deleted.TraderRoleID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_IRPoint ON dbo.IRPoint
    for Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update IRPoint 
		set actionDate = GetDate(),actionID = inserted.actionID
	from IRPoint IRP
			join inserted on IRP.IRPointID = inserted.IRPointID
			join deleted on IRP.IRPointID = deleted.IRPointID
	where inserted.actionID > deleted.actionID

	update IRPoint 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from IRPoint IRP
			join inserted on IRP.IRPointID = inserted.IRPointID
			join deleted on IRP.IRPointID = deleted.IRPointID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_IRCurve ON dbo.IRCurve
    for Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update IRCurve 
		set actionDate = GetDate(),actionID = inserted.actionID
	from IRCurve IRC
			join inserted on IRC.CurveID = inserted.CurveID
			join deleted on IRC.CurveID = deleted.CurveID
	where inserted.actionID > deleted.actionID

	update IRCurve 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from IRCurve IRC
			join inserted on IRC.CurveID = inserted.CurveID
			join deleted on IRC.CurveID = deleted.CurveID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_RMScenario ON dbo.RMScenario
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update RMScenario 
		set actionDate = GetDate(),actionID = inserted.actionID
	from RMScenario RM
			join inserted on RM.RMScenarioID = inserted.RMScenarioID
			join deleted on RM.RMScenarioID = deleted.RMScenarioID
	where inserted.actionID > deleted.actionID

	update RMScenario 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from RMScenario RM
			join inserted on RM.RMScenarioID = inserted.RMScenarioID
			join deleted on RM.RMScenarioID = deleted.RMScenarioID
	where inserted.actionID < = deleted.actionID
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
ALTER  Trigger tU_Expiry ON dbo.Expiry
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update Expiry 
		set actionDate = GetDate(),actionID = inserted.actionID
	from Expiry E
			join inserted on E.ExpiryID = inserted.ExpiryID
			join deleted on E.ExpiryID = deleted.ExpiryID
	where inserted.actionID > deleted.actionID

	update Expiry 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Expiry E
			join inserted on E.ExpiryID = inserted.ExpiryID
			join deleted on E.ExpiryID = deleted.ExpiryID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_ContractGroup ON dbo.ContractGroup
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update ContractGroup 
		set actionDate = GetDate(),actionID = inserted.actionID
	from ContractGroup CG
			join inserted on CG.groupID = inserted.groupID
			join deleted on CG.groupID = deleted.groupID
	where inserted.actionID > deleted.actionID

	update ContractGroup 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from ContractGroup CG
			join inserted on CG.groupID = inserted.groupID
			join deleted on CG.groupID = deleted.groupID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_Strategy ON dbo.Strategy
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update Strategy 
		set actionDate = GetDate(),actionID = inserted.actionID
	from Strategy S
			join inserted on S.strategyID = inserted.strategyID
			join deleted on S.strategyID = deleted.strategyID
	where inserted.actionID > deleted.actionID

	update Strategy 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Strategy S
			join inserted on S.strategyID = inserted.strategyID
			join deleted on S.strategyID = deleted.strategyID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_ContractInGroup ON dbo.ContractInGroup
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update ContractInGroup 
		set actionDate = GetDate(),actionID = inserted.actionID
	from ContractInGroup O
			join inserted on O.ContractInGroupID = inserted.ContractInGroupID
			join deleted on O.ContractInGroupID = deleted.ContractInGroupID
	where inserted.actionID > deleted.actionID

	update ContractInGroup 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from ContractInGroup O
			join inserted on O.ContractInGroupID = inserted.ContractInGroupID
			join deleted on O.ContractInGroupID = deleted.ContractInGroupID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER    Trigger tU_Option ON dbo.[Option]
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update [Option] 
		set actionDate = GetDate(),actionID = inserted.actionID
	from [Option] O
			join inserted on O.optionID = inserted.optionID
			join deleted on O.optionID = deleted.optionID
	where inserted.actionID > deleted.actionID

	update [Option] 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from [Option] O
			join inserted on O.optionID = inserted.optionID
			join deleted on O.optionID = deleted.optionID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_CustomStrikeSkewPoint ON dbo.CustomStrikeSkewPoint
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update CustomStrikeSkewPoint 
		set actionDate = GetDate(),actionID = inserted.actionID
	from CustomStrikeSkewPoint A
			join inserted on A.CustomStrikeSkewPointID = inserted.CustomStrikeSkewPointID
			join deleted on A.CustomStrikeSkewPointID = deleted.CustomStrikeSkewPointID
	where inserted.actionID > deleted.actionID

	update CustomStrikeSkewPoint 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from CustomStrikeSkewPoint A
			join inserted on A.CustomStrikeSkewPointID = inserted.CustomStrikeSkewPointID
			join deleted on A.CustomStrikeSkewPointID = deleted.CustomStrikeSkewPointID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_Contract ON dbo.Contract
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update Contract 
		set actionDate = GetDate(),actionID = inserted.actionID
	from Contract C
			join inserted on C.ContractID = inserted.ContractID
			join deleted on C.ContractID = deleted.ContractID
	where inserted.actionID > deleted.actionID

	update Contract 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Contract C
			join inserted on C.ContractID = inserted.ContractID
			join deleted on C.ContractID = deleted.ContractID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_Stock ON dbo.Stock
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update Stock 
		set actionDate = GetDate(),actionID = inserted.actionID
	from Stock S
			join inserted on S.StockID = inserted.StockID
			join deleted on S.StockID = deleted.StockID
	where inserted.actionID > deleted.actionID

	update Stock 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Stock S
			join inserted on S.StockID = inserted.StockID
			join deleted on S.StockID = deleted.StockID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER   Trigger tU_IndexDefinition ON dbo.IndexDefinition
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update IndexDefinition 
		set actionDate = GetDate(),actionID = inserted.actionID
	from IndexDefinition IDef
			join inserted on IDef.IndexDefinitionID = inserted.IndexDefinitionID
			join deleted on IDef.IndexDefinitionID = deleted.IndexDefinitionID
	where inserted.actionID > deleted.actionID

	update IndexDefinition 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from IndexDefinition IDef
			join inserted on IDef.IndexDefinitionID = inserted.IndexDefinitionID
			join deleted on IDef.IndexDefinitionID = deleted.IndexDefinitionID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_Index ON dbo.[Index]
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update [Index] 
		set actionDate = GetDate(),actionID = inserted.actionID
	from [Index] I
			join inserted on I.IndexID = inserted.IndexID
			join deleted on I.IndexID = deleted.IndexID
	where inserted.actionID > deleted.actionID

	update [Index] 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from [Index] I
			join inserted on I.IndexID = inserted.IndexID
			join deleted on I.IndexID = deleted.IndexID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_VolaSurfaceData ON dbo.VolaSurfaceData
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update VolaSurfaceData 
		set actionDate = GetDate(),actionID = inserted.actionID
	from VolaSurfaceData VSD
			join inserted on VSD.VolaSurfaceDataID = inserted.VolaSurfaceDataID
			join deleted on VSD.VolaSurfaceDataID = deleted.VolaSurfaceDataID
	where inserted.actionID > deleted.actionID

	update VolaSurfaceData 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from VolaSurfaceData VSD
			join inserted on VSD.VolaSurfaceDataID = inserted.VolaSurfaceDataID
			join deleted on VSD.VolaSurfaceDataID = deleted.VolaSurfaceDataID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER   Trigger tU_Holiday ON dbo.Holiday
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update Holiday 
		set actionDate = GetDate(),actionID = inserted.actionID
	from Holiday H
			join inserted on H.HolidayID = inserted.HolidayID
			join deleted on H.HolidayID = deleted.HolidayID
	where inserted.actionID > deleted.actionID

	update Holiday 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Holiday H
			join inserted on H.HolidayID = inserted.HolidayID
			join deleted on H.HolidayID = deleted.HolidayID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER   Trigger tU_Exchange ON dbo.Exchange
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update Exchange 
		set actionDate = GetDate(),actionID = inserted.actionID
	from Exchange E
			join inserted on E.ExchangeID = inserted.ExchangeID
			join deleted on E.ExchangeID = deleted.ExchangeID
	where inserted.actionID > deleted.actionID

	update Exchange 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from Exchange E
			join inserted on E.ExchangeID = inserted.ExchangeID
			join deleted on E.ExchangeID = deleted.ExchangeID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  Trigger tU_ExchangeHoliday ON dbo.ExchangeHoliday
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	update ExchangeHoliday 
		set actionDate = GetDate(),actionID = inserted.actionID
	from ExchangeHoliday EH
			join inserted on EH.ExchangeHolidayID = inserted.ExchangeHolidayID
			join deleted on EH.ExchangeHolidayID = deleted.ExchangeHolidayID
	where inserted.actionID > deleted.actionID

	update ExchangeHoliday 
		set actionDate = deleted.actionDate,actionID = deleted.actionID
	from ExchangeHoliday EH
			join inserted on EH.ExchangeHolidayID = inserted.ExchangeHolidayID
			join deleted on EH.ExchangeHolidayID = deleted.ExchangeHolidayID
	where inserted.actionID < = deleted.actionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--for eliminate errors of HH synchronization
	update DataInfo set keyValue = convert(varchar, GetDate(), 120) where dataInfoID = 6
GO
--============================================================================================================
--changes in SP's
--============================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[job_Cleanup_DB]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[job_Cleanup_DB]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BrokerHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BrokerHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CommissionHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CommissionHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractExchangeLinkHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractExchangeLinkHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractExchangeLink_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractExchangeLink_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForPriceFeedHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractForPriceFeedHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractForTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroupHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroupHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroupHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroupHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewData_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPointHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPointHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomVolatilityPointIsExist_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomVolatilityPointIsExist_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DataInfoHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DataInfoHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExchangeHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHolidayHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExchangeHolidayHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendarHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendarHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryInCalendarHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryInCalendarHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryInCalendar_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryInCalendar_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_HolidayHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_HolidayHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRCurveHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRCurveHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPointHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPointHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinitionHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinitionHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionVM_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionVM_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionWithPrice_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PositionForReconcile_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Position_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Position_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenarioHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenarioHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportMarketStructure_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportMarketStructure_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StrategyHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StrategyHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeForReconcile_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeqHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeqHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeq_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeq_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractForHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContractForHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContractHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingOptions_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaSurfaceDataHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaSurfaceDataHH_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockOptionEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockOptionEOD_Get]
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE            Procedure dbo.usp_Option_Del
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
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractExchangeLink set actionID = 3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractInGroup set actionID=3 where contractID in (select OptionID from #Options_for_delete)
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
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		if @bisRemovePermanent = 1
			begin
				delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				if @bisRemoveFromContract = 1 
					begin
						delete Contract where contractID in (select OptionID from #Options_for_delete)
							if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
					end
			end

	Commit Transaction DelOpt

finish:
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
CREATE     Procedure dbo.usp_Contract_Del
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
			insert into #Contracts_for_delete
				select @iContractID
		end

	Begin Transaction DelContract

		--ContractExchangeLink
		delete from ContractExchangeLink
		--update ContractExchangeLink set actionID = 3
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--ContractPrice
		delete from ContractPrice 
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--ModelParameter
		delete from ModelParameter
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--ContractInGroup
		delete from ContractInGroup
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--ContractMarketData
		delete from ContractMarketData where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--TraderContract 
		delete from TraderContract where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--HistRangesCache
		delete from HistRangesCache where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--NextDaySurface
		delete from NextDaySurface where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--VolaSurfaceData
		delete from VolaSurfaceData where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		--remove options of above selected contracts
		if object_ID('tempdb..#Options_for_delete') is null
			create table #Options_for_delete(OptionID int)
		else
			delete from #Options_for_delete

		insert into #Options_for_delete 
			select optionID 
				from [Option] O
					join #Contracts_for_delete C on C.ContractID = O.underlyingContractID
				where O.optionID not in (select distinct contractID from Trade)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		exec usp_Option_Del null,1
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		delete from #Options_for_delete
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		insert into #Options_for_delete 
			select contractID from #Contracts_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		exec usp_Option_Del null,1,0
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
	Commit Transaction DelContract
/*		delete StockDataHistory where stockID=@iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		--delete Dividend where stockID=@iContractID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		update Stock set actionID=3 where stockID=@iContractID
		--delete Stock where stockID=@iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		exec @error=usp_Option_Del @iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		update Contract set actionID=0 where contractID = @iContractID
		--delete Contract where contractID = @iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end  */

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

CREATE  Procedure dbo.job_Cleanup_DB

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

		--IRPoint
		delete IRPoint where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--IRCurve
		delete IRPoint where curveID in (select curveID from IRCurve where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end
		delete IRCurve where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--ContractInGroup
		delete ContractInGroup where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--ContractGroup
		delete ContractInGroup where groupID in (select groupID from ContractGroup where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end
		delete ContractGroup where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--IndexDefinition
		delete IndexDefinition where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--ExchangeHoliday
		delete ExchangeHoliday where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--Holiday
		delete ExchangeHoliday where holidayID in (select holidayID from Holiday where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end
		delete Holiday where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--Strategy
		delete Strategy 
			where actionID = 3 and strategyID not in 
				(select distinct strategyID from Trade where strategyID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--TraderContract
		delete TraderContract where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--RMScenario
		delete RMScenario where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--CustomStrikeSkewPoint
		delete from CustomStrikeSkewPoint where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--VolaSurfaceData
		delete CustomStrikeSkewPoint 
			where volaSurfaceDataID in (select volaSurfaceDataID from VolaSurfaceData where actionID = 3)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end
		delete VolaSurfaceData where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

	--don't forget about TraderRole

		--TraderContract 
		delete from TraderContract 
			where traderID in 
				(select traderID from Trader where actionID = 3 and traderID not in (select distinct traderID from Trade where traderID is not null))
			or traderID in 
				(select traderRefID from Trader where actionID = 3 and traderRefID not in (select distinct traderID from Trade where traderID is not null))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--Trader
		delete Trader 
			where traderRefID in 
				(select traderID from Trader where actionID = 3 and traderID not in (select distinct traderID from Trade where traderID is not null))
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		delete Trader where traderID not in (select distinct traderID from Trade) and actionID =3
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--Commission
		delete Commission
			where actionID = 3 and 
				commissionID not in (select distinct brokerCommissionID from Trade where brokerCommissionID is not null) and 
				commissionID not in (select distinct clearingBrokerCommissionID from Trade where clearingBrokerCommissionID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end
		delete Commission
			where brokerID in 
					(select brokerID from Broker
						where actionID = 3 and 
							brokerID not in (select distinct clearingBrokerID from Trade where clearingBrokerID is not null) and 
							brokerID not in (select distinct brokerID from Trade where brokerID is not null))
 			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--Broker
		delete Broker 
			where actionID = 3 and 
				brokerID not in (select distinct clearingBrokerID from Trade where clearingBrokerID is not null) and 
				brokerID not in (select distinct brokerID from Trade where brokerID is not null)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end
	
		--BookRuleItemMask
		delete BookRuleItemMask 
			where bookRuleItemID in (select bookRuleItemID from BookRuleItem where bookID in 
					(select bookID 
						from Book 
							where actionID = 3 and bookID not in (select distinct bookID from Trade where bookID is not null)))
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--BookRuleItem
		delete BookRuleItem
			where bookID in 
					(select bookID 
						from Book 
							where actionID = 3 and bookID not in (select distinct bookID from Trade where bookID is not null))
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--Book
		delete Book
			where actionID = 3 and bookID not in (select distinct bookID from Trade where bookID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--=====================================================================================================================
		--INDEX

		if object_id('tempdb..#IDs') is null
			Create table #IDs(contractID int not null)
		else
			Delete from #IDs
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--select indices for deletion (which not releated with Trades)
		insert into #IDs
			select indexID from [Index] 
				where actionID = 3 
						and indexID not in (select distinct contractID from Trade T join [Index] I on T.contractID = I.indexID)
						and indexID not in (select indexID from [Index] I 
										join [Option] O on I.indexID = O.underlyingContractID
										join Trade T on T.contractID = O.optionID)
 			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--IndexCorrelation
		delete from IndexCorrelation 
			where indexID1 in (select contractID from #IDs) or indexID2 in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--IndexDataHistory
		delete from IndexDataHistory
			where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--delete all related rows in Contract table by indexID
		if object_ID('tempdb..#Contracts_for_delete') is null
			Create table #Contracts_for_delete(contractID int)

		insert into #Contracts_for_delete
			select contractID from #IDs

		exec usp_Contract_Del Null,1
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--Index
		delete from [Index] where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--Contract
		delete from Contract where contractID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		if object_id('tempdb..#IDs') is not null
			Drop table #IDs
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		--=====================================================================================================================
		--STOCK
		delete StockDataHistory 
			where stockID in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade))
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		delete StockCorrelation 
			where stockID1 in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade))
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		delete StockCorrelation 
			where stockID2 in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade))
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		delete IndexDefinition 
			where stockID in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade))
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		if object_ID('tempdb..#Contracts_for_delete') is not null
			delete from #Contracts_for_delete
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		insert into #Contracts_for_delete
			select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Contract_Del Null,1
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		delete from Stock where stockID in (select contractID from #Contracts_for_delete)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		delete from Contract where contractID in (select contractID from #Contracts_for_delete)
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end
		--=====================================================================================================================
		--OPTION

		if object_ID('tempdb..#Options_for_delete') is null
			create table #Options_for_delete(OptionID int)
		else
			delete from #Options_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		insert into #Options_for_delete 
			select optionID 
				from [Option] O
				where optionID not in (select distinct ContractID from Trade) and actionID = 3
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Option_Del null,1
  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end
		--=====================================================================================================================

		--fix DB cleanup date	
		--update DataInfo set keyValue = GetDate() where dataInfoID = 6

		update DataInfo set keyValue = convert(varchar, GetDate(), 120) where dataInfoID = 6
		if @@ROWCOUNT = 0 
			begin 
				insert into DataInfo 
				    select 6,'lastCleanupDate',convert(varchar, GetDate(), 120)
	  			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DB_Cleanup goto finish end
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_BookHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select
		bookID as iBookID,
		bookName as vcBookName,
		[Description] as vcDescription,
		priority as iPriority,
		actionDate as dtActionDate,
		actionID as tiActionID
	from BookView
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_BrokerHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select 	brokerID as iBrokerID,
			brokerName as vcBrokerName,
			[Description] as vcDescription,
			isClearing as tiIsClearing,
			actionDate as dtActionDate,
			actionID as tiActionID
	from Broker
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_CommissionHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	 select commissionID as iCommissionID,
		brokerID as iBrokerID,
		commissionDate as dtCommissionDate,
		commissionValue as fCommissionValue,
		actionDate as dtActionDate,
		actionID as tiActionID 
	from Commission
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROC dbo.usp_ContractExchangeLinkHH_Get
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
		ce.exchangeID as iExchangeID,
		ce.contractID as iContractID,
		ce.isUnderlying as tiIsUnderlying,
		ce.isOption as tiIsOption,
		ce.actionDate as dtActionDate,
		ce.actionID as tiActionID
	from ContractExchangeLink ce
	where ce.contractID = @iUnderlyingContractID
		and (ce.actionDate > @dtActionDate or @dtActionDate is null)
	order by ce.actionDate

return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE  proc dbo.usp_ContractExchangeLink_Get
	@iContractID int = null,
	@dtActionDate datetime = null
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select contractID as iContractID,
			exchangeID as iExchangeID,
			isUnderlying as tiIsUnderlying,
			isOption as tiIsOption,
			ContractExchangeLinkID as iContractExchangeLinkID,
			actionDate as dtActionDate,
			actionID as tiActionID
		from ContractExchangeLink
		where 	(contractID=@iContractID or @iContractID is null) and 
				(actionDate >= @dtActionDate or @dtActionDate is null and actionID < 3)
		
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



CREATE   PROC dbo.usp_ContractForPriceFeedHH_Get
	@iTraderID int
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @dtToday datetime
	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)


	Create table #Exchange(contractID int, exchangeCode varchar(10) null,isUnderlying tinyint, isOption tinyint)

	insert into #Exchange(contractID, exchangeCode,isUnderlying, isOption)
		select 
			tc.contractID as contractID,
			e.ExchangeCode as exchangeCode,
			case when isnull(ce.isUnderlying,0) <> 0 then isnull(e.isUnderlying,0) else 0 end as isUnderlying,
			case when isnull(ce.isOption,0) <> 0 then isnull(e.isOption,0) else 0 end as isOption
		 	from TraderContractView tc 
					join ContractExchangeLinkView ce on tc.contractID = ce.contractID
					join ExchangeView e on ce.exchangeID = e.exchangeID
			where
				tc.traderID = @iTraderID
				  and ((case when isnull(ce.isUnderlying,0) <> 0 then isnull(e.isUnderlying,0) else 0 end) <> 0
				   or (case when isnull(ce.isOption,0) <> 0 then isnull(e.isOption,0) else 0 end) <> 0)
		union
			select 
				tc.contractID as contractID,
				null as exchangeCode,
				1 as isUnderlying,
				1 as isOption
			from TraderContractView tc 
			where  tc.traderID = @iTraderID
--=========================================================================================================
-- base section
--=========================================================================================================

	select distinct 
		c.ContractID as iContractID,
		c.ContractTypeID as iContractTypeID,
		c.Symbol as vcSymbol, 
		null as iUnderlyingContractID,
		null as vcExchangeCode
	from ContractView c
	where c.contractTypeID = 1
union
	select distinct 
		c.ContractID as iContractID,
		c.ContractTypeID as iContractTypeID,
		c.Symbol as vcSymbol, 
		null as iUnderlyingContractID,
		e.ExchangeCode as vcExchangeCode
	from #Exchange e
			join ContractView c on e.contractID = c.contractID
	where e.isUnderlying = 1 
union
	select --distinct 
		c.ContractID as iContractID,
		c.ContractTypeID as iContractTypeID,
		c.Symbol as vcSymbol, 
		o.underlyingContractID as iUnderlyingContractID,
		e.ExchangeCode as vcExchangeCode
	from TraderContractView tc	
		join Contract uc on tc.contractID = uc.contractID
		join OptionView o on uc.contractID = o.underlyingContractID
		join ContractView c on o.optionID = c.contractID
		join ExpiryInCalendarView eic on eic.expCalendarID = uc.expCalendarID and eic.expiryID = o.expiryID
		join #Exchange e on e.contractID = o.underlyingContractID 
	where
		tc.traderID = @iTraderID
		and e.isOption = 1
		and eic.expiryDate > @dtToday

if object_id('tempdb..#Exchange') is not null
	Drop table #Exchange


return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO






CREATE          proc dbo.usp_ContractForTrade_Get
	@iContractID int = null,
	@iUnderlyingID int = null,
	@tiNewOnly tinyint = 0,
	@tiExtendedOrder tinyint = 0
as
	declare @dtToday datetime
	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)
	if isnull(@tiExtendedOrder, 0) = 0
		select
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				null as fPriceTheoClose,
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				null as dtExpiry,
				null as fStrike,
				null as bIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.symbol as vcUnderlyingSymbol,
				cp.priceBid as fUnderlyingPriceBid,
				cp.priceAsk as fUnderlyingPriceAsk,
				cp.priceClose as fUnderlyingPriceClose,
				cp.lotSize as iUnderlyingLotSize
			from ContractView c  
				left join StockView S on S.stockID = C.contractID
				left join IndexView I on I.indexID = C.contractID
				left join ContractPrice CP on C.contractID = CP.contractID
			where	
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and c.contractID = @iUnderlyingID)
				and c.contractTypeID in (1, 2)
				and CP.exchangeID is null
		union
		select	
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				cp.priceTheo as fPriceTheoClose,
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol,
				ucp.priceBid as fUnderlyingPriceBid,
				ucp.priceAsk as fUnderlyingPriceAsk,
				ucp.priceClose as fPriceClose,
				ucp.lotSize as iUnderlyingLotSize
		from ContractView c  
				left join OptionView O on O.optionID = C.contractID
				left join Contract UC on O.underlyingContractID = UC.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
				and (@tiNewOnly = 0 or @tiNewOnly = 1 and EC.expiryDate >= @dtToday)
				and c.contractTypeID = 3
				and CP.exchangeID is null
				and ucp.exchangeID is null
		order by C.symbol
	else
		select * from 
		(select
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				null as fPriceTheoClose,
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				null as dtExpiry,
				null as fStrike,
				null as bIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.symbol as vcUnderlyingSymbol,
				cp.priceBid as fUnderlyingPriceBid,
				cp.priceAsk as fUnderlyingPriceAsk,
				cp.priceClose as fUnderlyingPriceClose,
				cp.lotSize as iUnderlyingLotSize
			from ContractView c  
				left join StockView S on S.stockID = C.contractID
				left join IndexView I on I.indexID = C.contractID
				left join ContractPrice CP on C.contractID = CP.contractID
			where	
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and c.contractID = @iUnderlyingID)
				and c.contractTypeID in (1, 2)
				and CP.exchangeID is null
		union
		select	
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				cp.priceTheo as fPriceTheoClose,
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol,
				ucp.priceBid as fUnderlyingPriceBid,
				ucp.priceAsk as fUnderlyingPriceAsk,
				ucp.priceClose as fPriceClose,
				ucp.lotSize as iUnderlyingLotSize
		from ContractView c  
				left join OptionView O on O.optionID = C.contractID
				left join Contract uc on o.underlyingContractID = uc.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
				and (@tiNewOnly = 0 or @tiNewOnly = 1 and EC.expiryDate >= @dtToday)
				and c.contractTypeID = 3
				and CP.exchangeID is null
				and ucp.exchangeID is null
		) as c order by year(c.dtExpiry), month(c.dtExpiry), c.fStrike, c.bIsCall desc

	Return(0)






GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_ContractGroupHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select groupID as iGroupID, 
		groupName as vcGroupName, 
		[Description] as vcDescription,
		actionDate as dtActionDate,
		actionID as tiActionID
	from ContractGroup
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROC dbo.usp_ContractHH_Get
	@dtActionDate datetime = null,
	@iUnderlyingContractID int = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	if isnull(@iUnderlyingContractID, 0) = 0

		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			c.expCalendarID as iExpCalendarID,
			cp.priceClose as fPriceClose,
			null as fPriceTheoClose,
			cp.lotSize as iLotSize,
			c.actionID as tiActionID,
			c.actionDate as dtActionDate
	
		from Contract c
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where c.contractTypeID in (1, 2)
			and (c.actionDate > @dtActionDate or @dtActionDate is null)

	else

		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			null as iExpCalendarID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			cp.lotSize as iLotSize,
	
			c.actionID as tiActionID,
			c.actionDate as dtActionDate
	
		from Contract c 
			join [Option] o on c.contractID = o.optionID
			join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			o.underlyingContractID = @iUnderlyingContractID
			and (c.actionDate > @dtActionDate or @dtActionDate is null)
		order by c.actionDate

return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO








CREATE     Procedure dbo.usp_ContractInExchange_Del 
	@iExchangeID int,        --the exchangeID
	@iContractID int   	 --the contractID
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure remove exchange link type data from ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	update ContractExchangeLink set actionID = 3
	--	delete from ContractExchangeLink 
		where  contractID = @iContractID and (exchangeID = @iExchangeID or @iExchangeID is null)

    set @error = @@Error
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





CREATE        procedure usp_ContractInExchange_Get
	@iContractID int,
	@bFullInfo tinyint = 1 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractPrice=Exchange table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if @bFullInfo <> 0
		select E.exchangeID as iExchangeID,
				E.ExchangeName as vcExchangeName,
				case when isnull(CP.isUnderlying,0) <> 0  then isnull(e.isUnderlying,0) else 0 end as iType1,
				case when isnull(CP.isOption,0) <> 0 then isnull(e.isOption,0) else 0 end as iType2,
				e.isUnderlying as tiIsUnderlyingExchange,
				e.isOption as tiIsOptionExchange
			from ContractExchangeLinkView CP, ExchangeView E
			where CP.contractID = @iContractID and CP.exchangeID =* E.exchangeID
			order by E.ExchangeName
	else
		select cp.exchangeID as iExchangeID,
				case when isnull(CP.isUnderlying,0) <> 0  then isnull(e.isUnderlying,0) else 0 end as iType1,
				case when isnull(CP.isOption,0) <> 0 then isnull(e.isOption,0) else 0 end as iType2
			from ContractExchangeLinkView CP
					join ExchangeView e on CP.exchangeID=e.exchangeID
			where CP.contractID = @iContractID --and CP.exchangeLinkType <> 0
	
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




CREATE     Procedure dbo.usp_ContractInExchange_Save 
	@iExchangeID int,       --the exchangeID
	@iContractID int,   	--the contractID
	@itype1 tinyint = null, --the link type 1
	@itype2 tinyint = null  --the link type 2
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction
		if not exists(select * from ContractExchangeLinkView where isnull(exchangeID,0)=isnull(@iExchangeID,0) and contractID=@iContractID) 
			begin
				insert into ContractExchangeLink (contractID, exchangeID, isUnderlying, isOption) 
					values (@iContractID, @iExchangeID, @itype1, @itype2)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update ContractExchangeLink set 
					isUnderlying = @itype1,
					isOption = @itype2
				where @iContractID = @iContractID and isnull(exchangeID,0) = isnull(@iExchangeID,0)
	
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_ContractInGroupHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select 	groupID as iGroupID, 
		contractID as iContractID,
		actionDate as dtActionDate,
		actionID as tiActionID
	from ContractInGroup
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE     Procedure dbo.usp_CustomStrikeSkewData_Get
	@iVolaSurfaceDataID int,
	@dtExpDate datetime = null,
	@iSetupMode int,
	@iCustomStrikeSkewID int = null,
	@iCustomStrikeSkewPointID int = null,
	@bType bit = 1
as
/*
	this procedure gets data from CustomStrikeSkewData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	if @bType is null	set @bType = 1

	if @iExpiryID is not null

		begin
			if not exists(select * 
							from CustomStrikeSkewPointView C
									join VolaSurfaceData  on C.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
							where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and C.expiryID = @iExpiryID and type = @bType)

			set @iExpiryID = null		

			select distinct CustomStrikeSkewPoint.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPoint.volaSurfaceDataID iVolaSurfaceDataID, 
				setupMode iSetupMode, 
				strike fStrike, 
				case when expiryID = @iExpiryID then isnull(volatility, 0) else 0 end fVolatility, 
				case when expiryID = @iExpiryID then CustomStrikeSkew.customStrikeSkewID else null end iCustomStrikeSkewID, 
				case when expiryID = @iExpiryID then E.expiryDate else null end dtExpDate,
				type bType
		
				from CustomStrikeSkewPointView
						join VolaSurfaceData on CustomStrikeSkewPoint.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
						join Contract C on  VolaSurfaceData.contractID = C.contractID
						join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID
						left join CustomStrikeSkewData on CustomStrikeSkewPoint.customStrikeSkewPointID = CustomStrikeSkewData.customStrikeSkewPointID
						left join CustomStrikeSkew on CustomStrikeSkewData.customStrikeSkewID = CustomStrikeSkew.customStrikeSkewID
				where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and setupMode = @iSetupMode and type = @bType
					and (@dtExpDate is null or expiry = @dtExpDate)
					and (@iCustomStrikeSkewID is null or CustomStrikeSkew.customStrikeSkewID = @iCustomStrikeSkewID)
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPoint.customStrikeSkewPointID = @iCustomStrikeSkewPointID)

		end
	else
		select  CustomStrikeSkewPointView.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPointView.volaSurfaceDataID as ivolaSurfaceDataID, 
				0 iSetupMode, 
				strike fStrike, 
				volatility as fVolatility, 
				0 as iCustomStrikeSkewID, 
				E.expiryDate as dtExpDate,
				type as bType
			from CustomStrikeSkewPointView
				join VolaSurfaceData on CustomStrikeSkewPointView.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
				join Contract C on  VolaSurfaceData.contractID = C.contractID
				join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID
			where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and type = @bType
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPointView.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
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


CREATE  PROC dbo.usp_CustomStrikeSkewPointHH_Get
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
		cssp.customStrikeSkewPointID as iCustomStrikeSkewPointID,
		cssp.volaSurfaceDataID as iVolaSurfaceDataID,
		cssp.strike as fStrike, 
		cssp.type as iType,
		cssp.volatility as fVolatility,
		cssp.expiryID as iExpiryID,
		cssp.isBasePoint as iIsBasePoint,
		cssp.actionDate as dtActionDate,
		cssp.actionID as tiActionID
	from VolaSurfaceData vsd
		inner join CustomStrikeSkewPoint cssp on vsd.volaSurfaceDataID = cssp.volaSurfaceDataID
	where vsd.contractID = @iUnderlyingContractID
		and (cssp.actionDate > @dtActionDate or @dtActionDate is null)
	order by cssp.actionDate

return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE Procedure dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null,
	@dtActionDate datetime = null 
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	select 
		CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
		CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
		CSSP.strike as fStrike, 
		CSSP.type as iType,
		CSSP.volatility as fVolatility,
		convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
		CSSP.isBasePoint as iIsBasePoint,
		CSSP.actionDate as dtActionDate,
		CSSP.actionID as tiActionID
		from CustomStrikeSkewPoint CSSP
			join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			join Contract C on  VolaSurfaceData.contractID = C.contractID
			join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID and CSSP.expiryID = E.expiryID
		where 
			(@iCustomStrikeSkewPointID is null or customStrikeSkewPointID=@iCustomStrikeSkewPointID)
		and (@iVolaSurfaceDataID is null or CSSP.volaSurfaceDataID=@iVolaSurfaceDataID) 
		and (@iType is null or type=@iType) 
		and (@dtExpDate is null or E.expiryID = @iExpiryID)
		and ((CSSP.actionDate>=@dtActionDate or @dtActionDate is null) and (@dtActionDate is not null or @dtActionDate is null and CSSP.actionID<3))

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






CREATE     Procedure dbo.usp_CustomVolatilityPointIsExist_Get
	@vcSymbolName varchar(8) = null
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	select distinct 
			CV.contractID as iContractID,
			CV.symbol as vcSymbolName,
			EC.expiryDate as dtExpiry,
			case when CSSP.volaSurfaceDataID is null then 0  
			else 1 end 'CustomVolatilityPointsCount'
		from OptionView OV
			join ContractView CV on OV.underlyingContractID = CV.contractID
			join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
			left join VolaSurfaceDataView VSD on OV.underlyingContractID = VSD.contractID and VSD.isDefaultSurface = 1
			left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
				and OV.expiryID = CSSP.expiryID
		where 
			CSSP.volaSurfaceDataID is null and 
			(CV.symbol = @vcSymbolName or @vcSymbolName is null) 
		order by CV.contractID,EC.expiryDate


Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE proc dbo.usp_DataInfoHH_Get
	@iDataInfoID int = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select 	dataInfoID as iDataInfoID, 
			keyName as vcKeyName, 
			keyValue as vcKeyValue
	from DataInfo
	where (@iDataInfoID is null or dataInfoID = @iDataInfoID)

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_ExchangeHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select
		exchangeID as iExchangeID,
		exchangeName as vcExchangeName,
		[Description] as vcDescription,
		ExchangeCode as vcExchangeCode,
		isUnderlying as tiIsUnderlying,
		isOption as tiIsOption,
		occCode as vcOccCode,
		actionDate as dtActionDate,
		actionID as tiActionID
	from Exchange
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_ExchangeHolidayHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select	exchangeID iExchangeID, 
		holidayID iHolidayID,
		actionDate as dtActionDate,
		actionID as tiActionID
	from ExchangeHoliday
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_ExpiryCalendarHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select 
		expCalendarID as iExpCalendarID,
		expCalendarName  as vcExpCalendarName,
		expCalendarDesc  as vcExpCalendarDesc,
		actionDate as dtActionDate,
		actionID as tiActionID
	from ExpiryCalendar
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROCEDURE dbo.usp_ExpiryCalendar_Del 
	@iExpCalendarID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iExpCalendarID <= 3 
	begin
		Raiserror('You can''t delete system defined calendar!',16,1)
		Return(-1)
	end

	begin tran
--		delete from ExpiryInCalendar where expCalendarID  = @iExpCalendarID 
		update ExpiryInCalendar set actionID = 3 where expCalendarID  = @iExpCalendarID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
	
--		delete from ExpiryCalendar where expCalendarID  = @iExpCalendarID 
		update ExpiryCalendar set actionID = 3 where expCalendarID  = @iExpCalendarID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
	commit tran


	set @error = @@error

finish:
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




CREATE  PROCEDURE dbo.usp_ExpiryCalendar_Get 
	@iExpCalendarID int = null,
	@dtActionDate datetime = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select 
		expCalendarID as iExpCalendarID,
		expCalendarName  as vcExpCalendarName,
		expCalendarDesc  as vcExpCalendarDesc,
		ExpiryCalendar.actionDate as dtActionDate,
		ExpiryCalendar.actionID as tiActionID
		from ExpiryCalendar 
		where (expCalendarID = @iExpCalendarID or @iExpCalendarID is null)
			and (ExpiryCalendar.actionDate >= @dtActionDate or @dtActionDate is null and ExpiryCalendar.actionID < 3 )

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

CREATE PROC dbo.usp_ExpiryHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select expiryID as iExpiryID, 
		expiryPeriod as vcExpiryPeriod, 
		actionDate as dtActionDate,
		actionID as tiActionID
	from Expiry
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_ExpiryInCalendarHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select expCalendarID as iExpCalendarID,
		expiryID as iExpiryID, 
		expiryDate as dtExpiryDate, 
		vegaWeight as fVegaWeight, 
		actionDate as dtActionDate,
		actionID as tiActionID
	from ExpiryInCalendar
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_ExpiryInCalendar_Get
	@iExpiryID int = null,
	@iExpCalendarID int = null,
	@dtActionDate datetime = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select 
		expCalendarID as iExpCalendarID,
		expiryID as iExpiryID, 
		expiryDate as dtExpiryDate, 
		vegaWeight as fVegaWeight, 
		actionDate as dtActionDate,
		actionID as tiActionID
	from ExpiryInCalendar EC 
	where ((EC.expiryID = @iExpiryID or @iExpiryID is null) and (expCalendarID = @iExpCalendarID or @iExpCalendarID is null))
		and (EC.actionDate >= @dtActionDate or @dtActionDate is null and EC.actionID < 3)

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



CREATE        Procedure dbo.usp_Expiry_Get
	@iExpiryID int = null,
	@vcExpiryPeriod varchar(30) = null,
	@dtActionDate datetime = null,
	@iExpCalendarID int = 1
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select Expiry.expiryID as iExpiryID, 
		expiryPeriod as vcExpiryPeriod, 
		EC.expiryDate as dtExpiryDate,
		EC.vegaWeight as fVegaWeight,
		Expiry.actionDate as dtActionDate,
		Expiry.actionID as tiActionID
		from Expiry
				join ExpiryInCalendar EC on Expiry.expiryID = EC.expiryID and EC.expCalendarID = @iExpCalendarID

		where ((expiryPeriod = @vcExpiryPeriod or @vcExpiryPeriod is null) and 
				(Expiry.expiryID = @iExpiryID or @iExpiryID is null))
			and (Expiry.actionDate>=@dtActionDate or @dtActionDate is null and Expiry.actionID<3)

	order by expiryDate

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

CREATE PROC dbo.usp_HolidayHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select holidayID as iHolidayID, 
		holidayName as vcHolidayName, 
		holidayDate as dtHolidayDate,
		actionDate as dtActionDate,
		actionID as tiActionID
	from Holiday
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_IRCurveHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select curveID as iCurveID,
		curveName as vcCurveName, 
		curveTypeID as iCurveTypeID, 
		ruleTypeID as iRuleTypeID, 
		positionThreshold as mPositionThreshold,
		actionDate as dtActionDate,
		actionID as tiActionID
	from IRCurve
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_IRPointHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
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
	from IRPoint
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROC dbo.usp_IndexDefinitionHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select indexID as iIndexID,
		stockID as iStockID,
		weight as fWeight,
		beta as fBeta,
		actionDate as dtActionDate,
		actionID as tiActionID
	from IndexDefinition
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROC dbo.usp_IndexHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select i.indexID as iIndexID,
		i.yield as fYield,
		i.isHTB as tiIsHTB,
		i.CalcOptionType as tiCalcOptionType,
		i.CalcModelType as tiCalcModelType,
		i.isBasket as tiIsBasket,
		i.isActive as tiIsActive,
		i.actionDate as dtActionDate,
		i.actionID as tiActionID
	from [Index] i
	where (i.actionDate > @dtActionDate or @dtActionDate is null)
	order by i.actionDate

return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROC dbo.usp_OptionHH_Get
	@dtActionDate datetime = null,
	@iUnderlyingContractID int
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select o.optionID as iOptionID, 
		o.underlyingContractID as iUnderlyingContractID,
		o.strike as fStrike, 
		o.expiryID as iExpiryID,
		o.isCall as tiIsCall,
		o.actionDate as dtActionDate,
		o.actionID as tiActionID
	from [Option] o
	where
		o.underlyingContractID = @iUnderlyingContractID
		and (o.actionDate > @dtActionDate or @dtActionDate is null)
	order by o.actionDate

return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





CREATE  Procedure dbo.usp_OptionVM_Get
	@iOptionID int = null,			-- option's identifier
	@iUnderlyingID int,				-- underlying's identifier
	@dtExpDateMin datetime = null,	-- expiration's date
	@bIsCall bit = null,
	@iSortOrder int = 1	--			-- 1, 2 or 3. see comments below
as
/*
	Created by Andrey Sinkin
	This procedure returns option's data
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iSortOrder = 1	-- sort by symbol
		select	O.OptionID as iOptionID, 
				O.underlyingContractID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as  dtExpDate,
				C.symbol as vcSymbolName, 
				null as iExchangeID,
				C.contractName as vcDescription,
				CP.lotSize as iLotSize
				from OptionView O
						join ContractView C on O.OptionID = C.contractID
						join ContractPrice CP on CP.contractID = C.contractID and exchangeID is null
						join Contract C1 on O.underlyingContractID = C1.contractID
						join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where O.underlyingContractID = @iUnderlyingID
						and (@iOptionID is null or O.OptionID = @iOptionID)
						and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
						and (@bIsCall is null or O.isCall = @bIsCall)
				order by C.symbol
	else
	if @iSortOrder = 2	-- sort by isCall+year(expiry)+month(expiry)+strike
		select	O.OptionID as iOptionID, 
				O.underlyingContractID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
				C.symbol as vcSymbolName, 
				null iExchangeID,
				C.contractName as vcDescription, 
				ContractPrice.lotSize as iLotSize
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where O.underlyingContractID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by isCall, year(EC.expiryDate),  month(EC.expiryDate), strike
	else
	if @iSortOrder = 3	-- sort by expiry+strike+symbol(without two last symbols)+isCall
		select 	O.OptionID iOptionID,
				O.underlyingContractID iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
				C.symbol vcSymbolName, 
				null iExchangeID,
				C.contractName vcDescription,	
				ContractPrice.lotSize iLotSize
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where O.underlyingContractID = @iUnderlyingID
					and (@iOptionID is null or EC.expiryDate = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by EC.expiryDate, strike, left(C.symbol, len(C.symbol) - 2), isCall desc
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








CREATE	proc dbo.usp_OptionWithPrice_Get
	  @iUnderlyingID int = Null,		 
	  @dtMinExpiry datetime = Null,
	  @iOptionID int = Null,
	  @bIsBestPriceOnly bit = 0
  as
   select	C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			O.underlyingContractID as iUnderlyingContractID,
			C2.contractTypeID as iUnderlyingContractTypeID,
			O.strike as fStrike, 
			EC.expiryDate as dtExpiry,
			O.isCall as bIsCall,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.lotSize as iLotSize, 
			CP.priceLast as fPriceLast,
			CP.priceClose as fPriceClose,
			CP.priceOpen as fPriceOpen,
			CP.priceLow as fPriceLow, 
			CP.priceHigh as fPriceHigh,
			CP.volume as iVolume,
			CP.openInterest as iOpenInterest, 
			E.exchangeID as iExchangeID,
			E.exchangeCode as vcExchangeCode,
			CP.priceTheo as fPriceTheo 
			  from ContractView c  
					join OptionView O on O.optionID = C.contractID
					join ContractView c2 on C2.contractID = O.underlyingContractID
					join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and c2.expCalendarID = EC.expCalendarID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where c.contractTypeID=3
					and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
					and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
					and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
					and (@bIsBestPriceOnly = 0 or @bIsBestPriceOnly = 1 and CP.exchangeID is null)
			order by o.underlyingContractID, year(EC.expiryDate), month(EC.expiryDate), o.strike,E.exchangeCode, o.isCall desc
 
  if (@@error <> 0)
	   return (-1)
	 else  
	   Return(0)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE         proc dbo.usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
	@dtActionDate datetime = null,
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
	if isnull(@tiWithPosition, 0) = 0
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
			from ContractView C  
				inner join [Option] O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
			order by o.underlyingContractID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
			from ContractView c  
				inner join [Option] O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
	end
	else
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
			from ContractView C  
				inner join [Option] O on O.optionID = C.contractID
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0 and (tr.traderID = @iTraderID or @iTraderID is Null)
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,CP.lotSize
			order by o.underlyingContractID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
			from ContractView c  
				inner join [Option] O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null						
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0 and (tr.traderID = @iTraderID or @iTraderID is Null)
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,CP.lotSize
	end


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





CREATE          Procedure dbo.usp_Option_Import
	@iUnderlyingContractID int = null,  --underlying contractID
	@vcSymbol varchar(8),  -- option symbol
	@fPriceAsk float, 
	@fPriceBid float, 
	@iVolume int,
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iOpenInterest int,
	@iExchangeID int = Null, -- the exchange identifier (special for ContractPrice update)
	@vcUnderlyingSymbol varchar(8) = null,  -- stock symbol
	@iLotSize int = null
as
/*
	Created by ... Redesigned by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int

	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			Declare @ERR_MSG varchar(8000)
			set @ERR_MSG = 'Incorrect expiration! '  + '"' +  Convert(varchar(50),@dtExpiry) + '"' + ' for symbol=' + '"' + @vcSymbol + '"' +  ' UnderlyingSymbol=' + '"' + @vcUnderlyingSymbol + '"' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- check the consistensy of input parameters (for stock)
	--===================================================================			
	declare @iUnderlyingFirstContractID int
		set @iUnderlyingFirstContractID = @iUnderlyingContractID
	--select underlying ID
	select @iUnderlyingContractID = contractID  
		from Contract 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3
	if @iUnderlyingFirstContractID <> @iUnderlyingContractID
		begin
			Raiserror ('Wrong underlying contract ID in usp_Option_Import!', 16, 1)
			Return (-1)
		end  
	--select underlying option symbol
	if @vcUnderlyingSymbol is null
		select @vcUnderlyingSymbol=Contract.Symbol 
			from Contract (nolock) 	
			where Contract.contractID = @iUnderlyingContractID and Contract.contractTypeID < 3

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if (@OptionID is Null) 
		select @OptionID = optionID
			from [Option] (nolock)
			where underlyingContractID = @iUnderlyingContractID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- try to find Option with same ticker in DB
	--===================================================================			
	declare @iIncorrectOptionID int

	select @iIncorrectOptionID = contractID 
		from Contract (nolock)
				join [Option] (nolock) on Contract.contractID = [Option].optionID
		where symbol = @vcSymbol and contractTypeID = 3 and 
			[Option].actionID < 3 and (@OptionID <> contractID) -- or @OptionID is null

	if @iIncorrectOptionID is not null
		begin
			-- mark as delete
			update [Option] set [Option].actionID = 3 
				where optionID = @iIncorrectOptionID and expiryID < @iLastExpiryID

			if @@rowcount = 0 
				begin
					--Raiserror ('Option with this symbol already exists!', 16, 1)
					--Return (-1)
					update [Option] set [Option].actionID = 3 
						where optionID = @iIncorrectOptionID
					update Contract set actionID = 3 
						where ContractID = @iIncorrectOptionID
				end   
			else
				update Contract set actionID = 3 
					where ContractID = @iIncorrectOptionID
		end

	--===================================================================			
	-- make the contract name
	--===================================================================			
--  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
--					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
--						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)

  if exists (select contractID from Contract(nolock) where contractID = @iUnderlyingContractID and contractTypeID > 2)
	 begin
	   RAISERROR ('Error. Can''t insert values to [Option]. The Underlying Contract is not stock or index!', 16, 1)
	   RETURN (-1)
	 end 

  if (@OptionID is Null) 
	begin
		Begin Transaction
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into ContractPrice (contractID,exchangeID, priceBid, priceAsk, volume,openInterest,lotSize)
				 values (@OptionID,@iExchangeID, @fPriceBid, @fPriceAsk, @iVolume,@iOpenInterest,@iLotSize)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID,lotSize)
						values(@OptionID,null,@iLotSize)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
		Commit Transaction
	end 
   else
	begin
		  update Contract
			   set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 
		  update ContractPrice
				set	priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					volume = @iVolume,
					openInterest=@iOpenInterest,
					lotSize = isnull(@iLotSize,lotSize)
				where contractID = @OptionID and isnull(exchangeID,0)=isnull(@iExchangeID,0)
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract price!', 16, 1)
				  RETURN (-1)
			   end 

			update [Option]
				set 
				IsCall = isnull(@iIsCall,IsCall),
				strike = isnull(@fStrike,Strike),
				expiryID = isnull(@iExpiryID,expiryID)
			where optionID = @OptionID

		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
				  RETURN (-1)
			   end 

	end 
	 
-- return @OptionID 
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




CREATE      proc dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(8) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@tiIsManualyEntered tinyint
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

   if exists (select contractID from ContractView where contractID = @iUnderlyingContractID and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 

	declare	@OptionID int, 
			@ContractName varchar(255),
			@iExpiryID int

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1


   if exists (select optionID from OptionView where underlyingContractID = @iUnderlyingContractID and 
					isCall = @iIsCall and 
					strike = @fStrike and 
					expiryID = @iExpiryID and
					(@iOptionID is Null or optionID <> @iOptionID))
		begin
		  RAISERROR ('Error. Can''t insert or update Option values. There are ambiguous Option in DB!', 16, 1)
		  RETURN (-1)
		end  

   set @ContractName = @vcSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
						 ' ' + cast(round(@fStrike, 2) as varchar)
 
	Begin Transaction
	   if (@iOptionID is Null)	
		 begin
			insert into Contract (contractTypeID, symbol, contractName)
				values (3, @vcSymbol, @ContractName)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  

			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				values (@iOptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@ContractName,contractName)
				where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]
				set underlyingContractID = @iUnderlyingContractID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end		

		if @tiIsManualyEntered in (0,1) 
			begin
--select @iOptionID as '@iOptionID*******************'
				update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

	Commit Transaction

    set @error = 0
finish:
if (@error <> 0) return (@error) else return @iOptionID









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO




CREATE PROCEDURE dbo.usp_PositionForReconcile_Get
	@tiByTrader tinyint = 0
AS
	if isnull(@tiByTrader, 0) = 0
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,

			cp.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where c.contractTypeID in (1, 2)
			and isnull(tr.isSimulated, 0) = 0
		group by c.contractID, c.contractTypeID, c.Symbol, cp.lotSize, cp.priceClose
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,

			cp.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract UC on o.underlyingContractID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
	
		where c.contractTypeID = 3
			and isnull(tr.isSimulated, 0) = 0
		group by C.contractID, C.contractTypeID, C.Symbol, 
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			cp.lotSize, cp.priceClose
	
		order by vcSymbol
	end
	else
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,

			cp.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join TraderView trdr on tr.traderID = trdr.traderID
		where c.contractTypeID in (1, 2)
			and isnull(tr.isSimulated, 0) = 0
		group by c.contractID, c.contractTypeID, c.Symbol, cp.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,

			cp.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract UC on o.underlyingContractID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join TraderView trdr on tr.traderID = trdr.traderID
	
		where c.contractTypeID = 3
			and isnull(tr.isSimulated, 0) = 0
		group by C.contractID, C.contractTypeID, C.Symbol, 
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			cp.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym
	
		order by vcSymbol
	end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO





CREATE    PROCEDURE dbo.usp_Position_Get
	@iUnderlyingID int = Null,
	@iTraderID int = Null,
	@iBookID int = Null,
	@iStrategyID int = Null,
	@dtMaxDate datetime = null
AS

	declare @dtToday datetime
		set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
	
		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		null as dtExpiry,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		null as fPosDailyTheoPrevDay,
		null as fPriceTheoClose
	
	from TradeView tr 
		join ContractView c  on tr.contractID = c.contractID
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
	where c.contractTypeID in (1, 2)
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield
	
	union

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,

		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		EC.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		sum((case when tr.tradeDate < @dtToday then isnull(cp.priceTheo, 0) else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyTheoPrevDay,
		cp.priceTheo as fPriceTheoClose

	from TradeView tr 
		join ContractView C on tr.contractID = C.contractID
		join ContractPrice CP on C.contractID = cp.contractID and exchangeID is null
		left join (OptionView o join Contract UC on o.underlyingContractID = UC.contractID) on c.contractID = o.optionID
		join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
		left join StockView s on uc.contractID = s.stockID
		left join IndexView i on uc.contractID = i.indexID

	where c.contractTypeID = 3
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)

	group by C.contractID, C.contractTypeID, C.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield,
		EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, cp.priceTheo

	order by vcSymbol







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_RMScenarioHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
    select rs.RMScenarioID as iRMScenarioID, 
		rs.ScenarioName as vcScenarioName, 
		rs.[Description] as vcDescription,
		points1 as iPoints1,
		step1 as fStep1,
		isAbs1 as tiIsAbs1,
		type1 as tiType1,
		points2 as iPoints2,
		step2 as fStep2,
		isAbs2 as tiIsAbs2,
		type2 as tiType2,
		actionDate as dtActionDate,
		actionID as tiActionID
	from RMScenario rs
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO







CREATE   Procedure dbo.usp_ReportMarketStructure_Get
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
	declare @error int		set @error = -1

	declare	@iLastExpiryID int
		select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc

	create table #FilteredStocks(stockID int)

	if @iStockID is null 
		begin
			insert into #FilteredStocks exec usp_FilterData_Get @iFilterID
		end

if @iStockID is not null and @iFilterID is null
	select
		O.underlyingContractID as iUnderlyingContractID,
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
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	O.expiryID > @iLastExpiryID and 
				underlyingContractID=@iStockID

else if @iStockID is not null and @iFilterID is not null
	select
		O.underlyingContractID as iUnderlyingContractID,
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
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	O.expiryID > @iLastExpiryID and
				underlyingContractID = @iStockID and 
				O.underlyingContractID in (select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is not null
	select
		O.underlyingContractID as iUnderlyingContractID,
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
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where O.expiryID > @iLastExpiryID and
			underlyingContractID in (Select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is null
	select
		O.underlyingContractID as iUnderlyingContractID,
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
				join Contract C1 on O.underlyingContractID = C1.contractID
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE          Procedure dbo.usp_ReportTrade_Get
	@iStockID int = Null, 
	@iFilterID int = Null,
	@dtBeginDate datetime = Null,
	@dtEndDate datetime = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from OptionView table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	create table #FilteredStocks(stockID int)
	
	-- for filtering by trades
	create table #T(tradeID int)
	if @iFilterID is not null
		begin
			declare @iTraderID int,
					@iBookID int,
					@iStrategyID int,
					@bTradeRowCount bit
			set @bTradeRowCount=0
			insert into #FilteredStocks
				exec usp_FilterData_Get @iFilterID, @iTraderID output, @iBookID output, @iStrategyID output
			if (@iTraderID<>0 or @iBookID<>0 or @iStrategyID<>0)
				begin
					set @bTradeRowCount = 1
					insert into #T
						exec usp_FilterTradeData_Get @iTraderID,@iBookID,@iStrategyID
				end
		end

	select	@dtBeginDate = convert(datetime,convert(varchar(10),@dtBeginDate,112)),
			@dtEndDate = convert(datetime,convert(varchar(10),@dtEndDate,112))
	select 
		O.underlyingContractID as iUnderlyingContractID,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		case T.isBuy when 0 then T.quantity * -1 else T.quantity end as iQuantity,
		T.tradeDate as dtTradeDate,
		T.price as fPriceTrade,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceLast as fPriceLast,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceTheo as fPriceTheo,
		T.traderID as iTraderID,
		T.bookID as iBookID,
		T.strategyID as iStrategyID,
		CP.priceClose as fPriceClose,
		Contract.symbol as vcSymbol,
		T.lotSize as iLotSize,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join TradeView T on O.optionID = T.contractID 
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
		where --EC.expiryDate > GetDate() and
			((
				(@iStockID is not null and @iFilterID is null and underlyingContractID=@iStockID) or 
				(@iStockID is null and @iFilterID is null) or
				(@iStockID is null and @iFilterID is not null and (underlyingContractID in (select stockID from #FilteredStocks)) and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0)  or 
				(@iStockID is not null and @iFilterID is not null 
					and underlyingContractID=@iStockID 
					and (underlyingContractID in (select stockID from #FilteredStocks)) 
					and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0))
			))
				and
				(
				(@dtBeginDate is  not null and @dtEndDate is not null and (convert(datetime,convert(varchar(10),T.tradeDate,112)) between @dtBeginDate and @dtEndDate)) or 
				(@dtBeginDate is not null and @dtEndDate is null and convert(datetime,convert(varchar(10),T.tradeDate,112)) >= @dtBeginDate) or
				(@dtBeginDate is null and @dtEndDate is not null and convert(datetime,convert(varchar(10),T.tradeDate,112)) <= @dtEndDate) or
				(@dtBeginDate is null and @dtEndDate is null)
				)
			)
	set @error = @@error
	if @iFilterID is not null
		begin
			drop table #FilteredStocks
		end
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


CREATE  PROC dbo.usp_StockHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select s.stockID as iStockID, 
		s.primaryExchangeID as iPrimaryExchangeID, 
		s.isHTB as tiIsHTB,
		s.DivAmt as fDivAmt, 
		s.DivFreq as iDivFreq,
		s.DivDate as dtDivDate,
		s.isDivCustom as tiIsDivCustom,
		s.divAmtCustom as fDivAmtCustom,
		s.divFreqCustom as iDivFreqCustom,
		s.divDateCustom as dtDivDateCustom,
		s.CalcOptionType as tiCalcOptionType,
		s.CalcModelType as tiCalcModelType,
		s.isActive as tiIsActive,
		s.actionDate as dtActionDate,
		s.actionID as tiActionID
	from Stock s
	where (s.actionDate > @dtActionDate or @dtActionDate is null)
	order by s.actionDate

return(@@error)

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
	if exists(select * from [Option](nolock) where underlyingContractID=@iStockID)
		begin
			Create table #Options_for_delete(OptionID int)
		
			insert into #Options_for_delete
				select optionID from [Option](nolock) where underlyingContractID=@iStockID
		
			if exists (select tradeID from TradeView where contractID in (select optionID from #Options_for_delete))
				begin
					Raiserror ('Error. Can''t delete Stock because of the existing Trade by Stock Options', 16, 1)
					Return (-1)
				end 
		end
	Begin Transaction
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = @bisRemovePermanent
			end

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID = @iStockID
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractExchangeLink set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	    delete ModelParameter where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
        --delete ContractInGroup where contractID = @iStockID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        --delete ContractPriceHistory where contractID = @iStockID
		--	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete ContractPrice where contractID = @iStockID

			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		if (@bisRemovePermanent = 1) 
			begin
				delete from ContractExchangeLink where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	        	delete IndexDefinition where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				delete TraderContract where contractID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		       	delete Stock where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	        	delete Contract where contractID = @iStockID
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_StrategyHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select 	strategyID as iStrategyID, 
			strategyName as vcStrategyName, 
			[description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
	from Strategy
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE     proc dbo.usp_TradeFeed_Del
        @vcExecID varchar(20),
		@dtTradeDate datetime
/*
	Created by Zaur Nuraliev
	Description: this procedure is mark as delete information in Trade table
*/
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	if not exists(select * from Trade where execID = @vcExecID and actionID > 0 and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112))
		begin
			--Raiserror('Record with this execID does not exists in DB!',16,1)
			--Return(-1)
			select null as iSeqNum
		end

	Declare @iSeqNum int,
		@iContractTypeID int

	select @iSeqNum = seqNum, @iContractTypeID = contractTypeID
		from Contract C join Trade T on C.contractID = T.contractID
		where execID = @vcExecID and T.actionID > 0 

--	if @iSeqNum is null or @iContractTypeID is null 
--		Return(-1)


	Begin tran
		Declare @iNewSeqNum int

			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated,
				execID)
			select tradeID,
				0,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated,
				@vcExecID
			from Trade
			where seqNum = @iSeqNum

			if @@error = 0
				begin
					set @iNewSeqNum = @@identity
					Commit Tran
					--Return @iNewSeqNum

					if @iContractTypeID < 3 
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
							tr.lotSize as iTradeLotSize,
							cp.lotSize as iUnderlyingLotSize,
							cp.lotSize as iLotSize,
							tr.isSimulated as tiIsSimulated,
							null as fPriceClose,
							null as fUnderlyingPriceClose
						from Trade tr 
							join Contract C  on tr.contractID = c.contractID
							join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
							left join StockView s on c.contractID = s.stockID
							left join IndexView i on c.contractID = i.indexID
							left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
							left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
						where tr.seqNum = @iSeqNum
				
					else --if @iContractTypeID =3
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
							tr.lotSize as iTradeLotSize,
							ucp.lotSize as iUnderlyingLotSize,
							cp.lotSize as iLotSize,
							tr.isSimulated as tiIsSimulated,
							cp.PriceClose as fPriceClose,
							ucp.priceClose as fUnderlyingPriceClose
						from Trade tr 
							join Contract C  on tr.contractID = c.contractID
							inner join [Option] O on C.contractID = O.optionID
							inner join Contract UC on O.underlyingContractID = UC.contractID
							inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
							inner join ContractPrice UCP on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
							join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
							left join StockView s on uc.contractID = s.stockID
							left join [IndexView] i on uc.contractID = i.indexID
							left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
							left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
						where tr.seqNum = @iSeqNum
				end
			else
				begin
					Rollback Tran
					Return (0)
				end




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE proc dbo.usp_TradeForReconcile_Get
as
		select
			tr.tradeID as iTradeID,
			tr.execID as vcExecID,
			
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.lotSize as iTradeLotSize

		from TradeView tr
			inner join Contract c on tr.contractID = c.contractID
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID in (1, 2)
			and isnull(tr.isSimulated, 0) = 0
			and isnull(tr.isPosition, 0) = 0
			and isnull(tr.execID, "") <> ""

	union

		select
			tr.tradeID as iTradeID,
			tr.execID as vcExecID,
			
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.lotSize as iTradeLotSize

		from TradeView tr 
			inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract UC on o.underlyingContractID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and isnull(tr.isSimulated, 0) = 0
			and isnull(tr.isPosition, 0) = 0
			and isnull(tr.execID, "") <> ""
		order by TradeDate

     Return(@@error)







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_TradeSeqHH_Get
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
		select
			seqNum as iSeqNum,
			actionDate as dtActionDate,
			tradeID as iTradeID,
			actionID as tiActionID,
			contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			BrokerCommissionID as iBrokerCommissionID,
			clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			lotSize as iLotSize,
			isSimulated as tiIsSimulated,
			execID as vcExecID
		from Trade
		where 
			(seqNum >= @iMinSeqNum or @iMinSeqNum is null)
			and (seqNum <= @iMaxSeqNum or @iMaxSeqNum is null)
		order by seqNum

return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO




CREATE  PROC dbo.usp_TradeSeq_Get
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null,
	@dtTradeDate datetime = null,
	@tiExtendedInfo tinyint = null,
	@iTraderID int = null
AS
	set @dtTradeDate = cast(floor(cast(@dtTradeDate as float)) as datetime)
	if @iMinSeqNum = 0 set @iMinSeqNum = Null
	if @iMaxSeqNum = 0 set @iMaxSeqNum = Null
	if @tiExtendedInfo is null or @tiExtendedInfo = 0
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.contractID as iContractID,
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
			tr.isPosition as tiIsPosition,
			tr.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated
		from Trade tr
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where 
			(@iMinSeqNum is null or @iMinSeqNum is not null and seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tradeDate as float)) as datetime) = @dtTradeDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by seqNum
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
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractId and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID in (1, 2)
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tr.tradeDate as float)) as datetime) = @dtTradeDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
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
			tr.lotSize as iTradeLotSize,
			ucp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join ContractView uc on o.underlyingContractID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID = 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tr.tradeDate as float)) as datetime) = @dtTradeDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by tr.seqNum
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO






CREATE        proc dbo.usp_Trade_Get
	@iTradeID int = null,
	@iContractID int = null,
	@tiExtendedInfo tinyint = 0,
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iBookID int = null,
	@iStrategyID int = null,
	@iContractGroupID int = null,
	@dtMinTradeDate datetime = null
as

 if @tiExtendedInfo is null or @tiExtendedInfo = 0

	select
		tradeID as iTradeID,
		contractID as iContractID,
		isClose as tiIsClose,
		tradeDate as dtTradeDate,
		quantity as iQuantity,
		price as fPrice,
		isBuy as tiIsBuy,
		bookID as iBookID,     
		strategyID as iStrategyID, 
		traderID as iTraderID,
		brokerID as iBrokerID,
		clearingBrokerID as iClearingBrokerID,
		brokerCommissionID as iBrokerCommissionID,
		clearingBrokerCommissionID as iClearingBrokerCommissionID,
		tradedIV as fTradedIV,
		spotReference as fSpotReference,
		isPosition as tiIsPosition,
		lotSize as iLotSize,
		isSimulated as tiIsSimulated
		from Trade 
			where 
			(@iTradeID is null or @iTradeID is not null and tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and contractID = @iContractID)
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tradeDate >= @dtMinTradeDate)
		group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition,lotSize,isSimulated
		having min(actionID) = 1
		order by TradeDate

 else if @iContractGroupID is null
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
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			cp.lotSize as iUnderlyingLotSize,
			
			tr.isSimulated as tiIsSimulated
		from Trade tr
			inner join Contract c on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID in (1, 2)
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,cp.priceClose,cp.lotSize
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
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			ucp.lotSize as iUnderlyingLotSize,
	
			tr.isSimulated as tiIsSimulated
		from Trade tr inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract UC on o.underlyingContractID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,
			cp.lotSize,cp.priceClose,ucp.lotSize,ucp.priceClose
		having min(tr.actionID) = 1
		order by TradeDate
  else
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
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			cp.lotSize as iUnderlyingLotSize,
			
			tr.isSimulated as tiIsSimulated
		from Trade tr
			inner join Contract c on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractInGroup cig on c.contractID = cig.contractID
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID in (1, 2)
			and cig.groupID = @iContractGroupID
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,cp.priceClose,cp.lotSize
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
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			ucp.lotSize as iUnderlyingLotSize,
			
			tr.isSimulated as tiIsSimulated
		from Trade tr 
			inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract uc on O.underlyingContractID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractInGroup cig on o.underlyingContractID = cig.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and cig.groupID = @iContractGroupID
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,
			cp.lotSize,cp.priceClose,ucp.lotSize, ucp.priceClose
		having min(tr.actionID) = 1
		order by TradeDate

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




CREATE Procedure dbo.usp_TraderContractForHH_Get 
	@iTraderID int = null, -- trader ID
	@vcAcronym varchar(12) = null, -- trader acronym
	@tiInfoType tinyint = 0, -- information required: 
								-- 0 - available underlyiings list,
								-- 1 - market structure updates,
								-- 2 - contracts for price feed,
								-- 3 - index definition,
								-- 4 - contract prices
	@iContractTypeID int = Null, -- contract type ID for @tiInfoType = 1, if Null or 0 then retrieve general contract updates
	@dtMinUpdateDate datetime = Null, -- minimal update date for @tiInfoType = 1, if Null then retrieve full actual info (w/o "deleted" records)
	@iContractID int = Null -- contract ID for @tiInfoType = 1
as
/*
	Created by Sharky
	Description: this procedure is for getting information for HendHeld initialize
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	if @iContractID = 0 set @iContractID = Null
	-- check tarder parameters
	if isnull(@iTraderID, 0) = 0
	begin
		if not @vcAcronym is null
			select top 1 @iTraderID = traderID
			from TraderView
			where acronym = @vcAcronym
		if isnull(@iTraderID, 0) = 0
		begin
			raiserror('Error. Invalid trader ID or acronym were specified.', 16, 1)
			return -1
		end
	end
	else if not exists(select 1 from Trader where traderID = @iTraderID)
	begin
		raiserror('Error. Invalid trader ID or acronym were specified.', 16, 1)
		return -1
	end
	set @tiInfoType = isnull(@tiInfoType, 0)
	if @tiInfoType = 0 -- available underlyings list
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			ce.exchangeID as iExchangeID,
			isUnderlying as iType1,
			isOption as iType2
		from TraderContractView tc
			join ContractView c on tc.contractID = c.contractID
			join ContractExchangeLinkView ce on c.contractID = ce.contractID
		where
			tc.traderID = @iTraderID
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			null as iExchangeID,
			1 as iType1,
			1 as iType2
		from TraderContractView tc
			join ContractView c on tc.contractID = c.contractID
		where
			tc.traderID = @iTraderID
	else if @tiInfoType = 1 -- market structure updates
	begin
		if @dtMinUpdateDate is not null -- updates info
		begin
			if isnull(@iContractTypeID, 0) = 0 -- general contract info
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					null as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate
	
				from TraderContractView tc
					join Contract c on tc.contractID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or c.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					cph.priceTheo as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate

				from TraderContractView tc
					join [Option] o on tc.contractID = o.underlyingContractID
					join Contract c on o.optionID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or c.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 3 -- options
				select
					o.optionID as iOptionID,
					o.underlyingContractID as iUnderlyingContractID,
					o.isCall as bIsCall,
					o.strike as fStrike,
					E.expiryDate as dtExpiry,
					o.actionID as tiActionID,
					o.actionDate as dtActionDate
					
				from TraderContractView tc
					join [Option] O on tc.contractID = o.underlyingContractID
					join Contract C on O.underlyingContractID = C.contractID
					join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or o.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 2 -- stocks
				select
					s.stockID as iStockID,
	
					s.divFreq as iDivFreq,
					s.divDate as dtDivDate,
					s.divAmt as fDivAmt,
					
					null as iBaseIndexID,
					s.primaryExchangeID as iPrimaryExchangeID,
					s.isHTB as bIsHTB,
	
					s.actionID as tiActionID,
					s.actionDate as dtActionDate,
					s.calcOptionType as tiCalcOptionType,
					s.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join Stock s on tc.contractID = s.stockID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or s.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or s.stockID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 1 -- indices
				select
					i.indexID as iIndexID,
	
					i.yield as fYield,
					i.isHTB as bIsHTB,
	
					i.actionID as tiActionID,
					i.actionDate as dtActionDate,

					i.calcOptionType as tiCalcOptionType,
					i.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join [Index] i on tc.contractID = i.indexID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or i.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or i.indexID = @iContractID)
				order by dtActionDate
			else
			begin
				raiserror('Error. Invalid contract type ID was specified.', 16, 1)
				return -1
			end
		end
		else
		begin
			if isnull(@iContractTypeID, 0) = 0 -- general contract info
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					null as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate
	
				from TraderContractView tc
					join ContractView c on tc.contractID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					cph.priceTheo as fPriceTheoClose,
					c.actionID as tiActionID,
					c.actionDate as dtActionDate

				from TraderContractView tc
					join OptionView o on tc.contractID = o.underlyingContractID
					join ContractView c on o.optionID = c.contractID
					left join ContractPrice cph on c.contractID = cph.contractID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 3 -- options
				select
					o.optionID as iOptionID,
					o.underlyingContractID as iUnderlyingContractID,
					o.isCall as bIsCall,
					o.strike as fStrike,
					E.expiryDate as dtExpiry,

					o.actionID as tiActionID,
					o.actionDate as dtActionDate
				from TraderContractView tc
					join OptionView o on tc.contractID = o.underlyingContractID
					join Contract C on O.underlyingContractID = C.contractID
					join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 2 -- stocks
				select
					s.stockID as iStockID,
	
					s.divFreq as iDivFreq,
					s.divDate as dtDivDate,
					s.divAmt as fDivAmt,
					
					null as iBaseIndexID,
					s.primaryExchangeID as iPrimaryExchangeID,
					s.isHTB as bIsHTB,
	
					s.actionID as tiActionID,
					s.actionDate as dtActionDate,

					s.calcOptionType as tiCalcOptionType,
					s.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join StockView s on tc.contractID = s.stockID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or s.stockID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 1 -- indices
				select
					i.indexID as iIndexID,
	
					i.yield as fYield,
					i.isHTB as bIsHTB,
	
					i.actionID as tiActionID,
					i.actionDate as dtActionDate,

					i.calcOptionType as tiCalcOptionType,
					i.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join IndexView i on tc.contractID = i.indexID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or i.indexID = @iContractID)
				order by dtActionDate
			else
			begin
				raiserror('Error. Invalid contract type ID was specified.', 16, 1)
				return -1
			end
		end
	end
	else if @tiInfoType = 2 -- contracts for price feed
	begin
		declare @dtToday datetime
		set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

		select distinct 
			C.ContractID as iContractID,
			C.ContractTypeID as iContractTypeID,
			C.Symbol as vcSymbol, 
			E.ExchangeCode as vcExchangeCode
		from TraderContractView tc	
			join ContractView c on tc.contractID = c.contractID
			join ContractExchangeLinkView CE on C.contractID = CE.contractID
			join ExchangeView E on CE.exchangeID = E.exchangeID
		where
			tc.traderID = @iTraderID and ce.isUnderlying = 1
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			null as vcExchangeCode
		from TraderContractView tc 
			join ContractView c on tc.contractID = c.contractID
		where 
			tc.traderID = @iTraderID
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			e.ExchangeCode as vcExchangeCode
		from TraderContractView tc	
			join OptionView o on tc.contractID = o.underlyingContractID
			join ContractView c on o.optionID = c.contractID
			join ContractExchangeLinkView CE on O.underlyingContractID = CE.contractID
			join ExchangeView e on ce.exchangeID = e.exchangeID
			join Contract C1 on O.underlyingContractID = C1.contractID
			join ExpiryInCalendarView E1 on E1.expCalendarID = C1.expCalendarID
		where
			tc.traderID = @iTraderID
			and ce.IsOption = 1
			and E1.expiryDate > @dtToday
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			null as vcExchangeCode
		from TraderContractView tc	
			join OptionView o on tc.contractID = o.underlyingContractID
			join ContractView c on o.optionID = c.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendarView E1 on E1.expCalendarID = C1.expCalendarID
		where
			tc.traderID = @iTraderID
			and E1.expiryDate > @dtToday
	end
	else if @tiInfoType = 3 -- index definition
	begin
		if @dtMinUpdateDate is not null -- updates info
			select
				i.indexID as iIndexID,
				i.stockID as iStockID,
				i.weight as fWeight,
				i.beta as fBeta,
	
				i.actionID as tiActionID,
				i.actionDate as dtActionDate

			from
				IndexDefinition i
			where
				i.indexID in (select contractID from TraderContractView where traderID = @iTraderID)
				and i.stockID in (select contractID from TraderContractView where traderID = @iTraderID)
				and (@dtMinUpdateDate is null or i.actionDate > @dtMinUpdateDate)
				order by dtActionDate
		else
			select
				i.indexID as iIndexID,
				i.stockID as iStockID,
				i.weight as fWeight,
				i.beta as fBeta,
	
				i.actionID as tiActionID,
				i.actionDate as dtActionDate
			from
				IndexDefinitionView i
			where
				i.indexID in (select contractID from TraderContractView where traderID = @iTraderID)
				and i.stockID in (select contractID from TraderContractView where traderID = @iTraderID)
				order by dtActionDate
	end
	else if @tiInfoType = 4 -- contract prices
	begin
		select
			c.contractID as iContractID,
			cph.priceClose as fPriceClose,
			null as fPriceTheoClose
		from TraderContractView tc
			join Contract c on tc.contractID = c.contractID
			left join ContractPrice cph on c.contractID = cph.contractID 
		where
			tc.traderID = @iTraderID
		union
		select
			c.contractID as iContractID,
			cph.priceClose as fPriceClose,
			cph.priceTheo as fPriceTheoClose
		from TraderContractView tc
			join [Option] o on tc.contractID = o.underlyingContractID
			join Contract c on o.optionID = c.contractID
			left join ContractPrice cph on c.contractID = cph.contractID
		where
			tc.traderID = @iTraderID
	end
	else
	begin
		raiserror('Error. Invalid information type was specified.', 16, 1)
		return -1
	end
	return @@error




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_TraderContractHH_Get
	@dtActionDate datetime = null,
	@iTraderID int
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select
		contractID as iContractID,
		actionDate as dtActionDate,
		actionID as tiActionID 
	from TraderContract
	where (actionDate >= @dtActionDate or @dtActionDate is null)
		and traderID = @iTraderID
	order by actionDate

return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_TraderHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select traderID as iTraderID, 
			acronym as vcAcronym, 
			traderName as vcTraderName, 
			clearingTraderCode as vcClearingTraderCode,
			traderRoleID as iTraderRoleID,
			traderRefID as iTraderRefID,
			actionDate as dtActionDate,
			actionID as tiActionID 
	from Trader
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





CREATE  proc dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
	set nocount on
	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end

Return (0)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROC dbo.usp_VolaSurfaceDataHH_Get
	@dtActionDate datetime = null,
	@iUnderlyingContractID int
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------

	select	volaSurfaceDataID as iVolaSurfaceDataID, 
			vsd.contractID as iContractID, 
			baseUnderlinePrice as fBaseUnderlinePrice, 
			smileAccelerator as fSmileAccelerator,
			interpolationFactor as fInterpolationFactor,
			strikeMoneynessHi as fStrikeMoneynessHi,
			strikeMoneynessLo as fStrikeMoneynessLo,
			surfaceName as vcSurfaceName,
			isPriceOverride as bIsPriceOverride,
			isDiscreteAcceleration as bIsDiscreteAcceleration,
			isDefaultSurface as bIsDefaultSurface,
			ATMVolatility as fATMVolatility,
			vsd.actionDate as dtActionDate,
			vsd.actionID as tiActionID
	from VolaSurfaceData vsd
	where vsd.contractID = @iUnderlyingContractID
		and (vsd.actionDate > @dtActionDate or @dtActionDate is null)
	order by vsd.actionDate

return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO






CREATE       proc dbo.usp_allStockOptionEOD_Get
	@iStockID int = null
as
	if @iStockID is null 
		-- all options
	 	 select	C.contractID as iContractID,
				C.Symbol as vcSymbol,  
				null as vcExchangeCode, 
				null as iExchangeID, 
				O.isCall as bIsCall, 
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry
         	from ContractView C (nolock)
					join OptionView O (nolock) on O.optionID = C.contractID
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
	   		where EC.expiryID > (select top 1 expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc)
	else
	-- get list of NULL exchanges

	 	select C.contractID as iContractID,
				C.Symbol as vcSymbol,  
				null as vcExchangeCode, 
				null as iExchangeID, 
				O.isCall as bIsCall, 
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry
            from Contract C (nolock)
					join OptionView O (nolock) on O.optionID = C.contractID
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
	   		where O.underlyingContractID = @iStockID
				and EC.expiryID > (select top 1 expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc)
		
Return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_New]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE   PROCEDURE dbo.usp_TradeFeed_New
	@vcTraderAcronym varchar(12),
	@vcExecID varchar(20),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(8),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(8) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into table_name table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if exists(select * from Trade where execID = @vcExecID and actionID > 0 and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112))
		begin
			--Raiserror('Record with same execID alreadty exists in DB!',16,1)
			Select null as iSeqNum
			Return(-1)

		end

	Begin Transaction

	Declare @iTradeContractID int, @iStructureMissed int
	set @iStructureMissed = 0

	--check underlying symbol
	Declare @iUnderlyingContractID int

		if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
			set @vcUnderlyingSymbol = @vcContractSymbol

	select @iUnderlyingContractID = contractID from ContractView where symbol = @vcUnderlyingSymbol and contractTypeID in(1,2)


	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			if @iTradeLotSize is null set @iTradeLotSize = 1
			insert into ContractPrice (contractID,exchangeID,lotSize) values(@iUnderlyingContractID,null,@iTradeLotSize)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			set @iStructureMissed = 1

/*			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol,@iContractTypeID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity
			if @iContractTypeID = 1
				begin
					insert into [Index](indexID) values(@iUnderlyingContractID)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else if @iContractTypeID = 2
				begin
					insert into Stock (stockID) values(@iUnderlyingContractID)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			if @iContractTypeID < 3 
				begin	
					if @iTradeLotSize is null set @iTradeLotSize = 1
					insert into ContractPrice (contractID,exchangeID,lotSize) values(@iUnderlyingContractID,null,@iTradeLotSize)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				insert into ContractPrice (contractID,exchangeID) values(@iUnderlyingContractID,null)
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end*/
		end
	else
		begin
			update Stock set isActive = 1 where stockID = @iUnderlyingContractID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					@iExpiryID int
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			select @iOptionID = optionID
				from OptionView (nolock)
				where underlyingContractID = @iUnderlyingContractID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3
	
			if @iOptionID is null
			--create new option
				begin
					if isnull(@vcContractSymbol, '') <> ''
					begin
						Declare @vcContractName varchar(255)
						  	set @vcContractName = @vcUnderlyingSymbol + ' ' + datename(month, @dtExpiry) + 
										' ' + datename(year, @dtExpiry) + ' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
											 ' ' + cast(round(@fStrike, 2) as varchar)
		
						insert into Contract (contractTypeID, symbol, contractName) values (3, @vcContractSymbol,@vcContractName)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
							set @iOptionID = @@identity
				
						insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
							 values (@iOptionID, @iUnderlyingContractID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID,lotSize) values(@iOptionID,null,@iTradeLotSize)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
					end
					else
					begin
						Select null as iSeqNum
						Return(-2)
					end
				end

			set @iStructureMissed = @iStructureMissed + 2
			set @iTradeContractID = @iOptionID
		end
		else
			set @iTradeContractID = @iUnderlyingContractID


	--work with Trader
	Declare @iTraderID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym

	if @iTraderID is null
		begin
			insert into Trader(acronym,traderName) values(@vcTraderAcronym,@vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingContractID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingContractID)
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	--create new trade
	if @iTradeLotSize is null
		select @iTradeLotSize = lotSize from ContractPrice 
			where exchangeID is null and 
					(contractID = @iOptionID and @iContractTypeID = 3) or
					(contractID = @iUnderlyingContractID and @iContractTypeID < 3)

	if @iTradeLotSize is null
		if @iContractTypeID = 3 
			set @iTradeLotSize = 100
		else if @iContractTypeID < 3 
			set @iTradeLotSize = 1

	Declare @iNewTradeID int
		select @iNewTradeID = max(tradeID) + 1 from Trade

	if @iNewTradeID is null 
			set @iNewTradeID = 1
	
	insert into Trade (
		execID,
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		traderID, 
		isBuy, 
		isPosition,
		lotSize,
		isSimulated,
		brokerCommissionID,
		clearingBrokerCommissionID)
	values (
		@vcExecID,
		@iNewTradeID,
		1,
		@iTradeContractID, 
		0, 
		@dtTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iTraderID, 
		@tiIsBuy, 
		0,
		@iTradeLotSize,
		0,
		null,
		null)

	Declare @iSeqNum int 
		set @iSeqNum = @@identity

		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction

 --if all OK
	if @iContractTypeID < 3 
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
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			null as fPriceClose,
			null as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

	else if @iContractTypeID =3
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
			tr.lotSize as iTradeLotSize,
			ucp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			CP.PriceClose as fPriceClose,
			UCP.priceClose as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			inner join [Option] O on C.contractID = O.optionID
			inner join Contract UC on O.underlyingContractID = UC.contractID
			inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
			inner join ContractPrice UCP on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

    set @error = 0

finish:
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.33.003')
	begin
		update DataInfo set keyValue = '3.33.003' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.33.003'
	end


