use ClearingDb

/*******************************************************************************************/
/*******************************[ 	STORED PROCEDURES	]***************************/
/*******************************************************************************************/

/*******************************[ 	  GetCurrentDate	]***************************/
IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'sp_GetCurrentDate' AND type = 'P')
   DROP PROCEDURE sp_GetCurrentDate

GO

CREATE PROCEDURE sp_GetCurrentDate @DayOffset integer output
AS 
declare @Year integer , @DayOfYear integer, @Now smalldatetime
Set @Now = GETDATE ( )
Set @Year = DATENAME ( year ,  @Now)
Set @DayOfYear = DATENAME ( dayofyear , @Now )
Set @DayOffset = @Year * 366 + @DayOfYear

GO


/*******************************[ 	  InsertTrade		]***************************/



IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'sp_InsertTrade' AND type = 'P')
   DROP PROCEDURE sp_InsertTrade

GO

CREATE PROCEDURE sp_InsertTrade 	@iCacheID     int ,
					@ins_upd_del_flag tinyint,
					@put_call	 tinyint,
					@buy_sell tinyint,
					@origin   tinyint,
					@open_close tinyint,
					@trade_id   varchar(10),
					@user_id    varchar(10),
					@trading_sym varchar(7),
					@base_sym	varchar(7),
					@parent_account varchar(11),
					@account varchar(11),
					@q_account varchar(11),
					@security_type varchar(3),
					@security_subtype varchar(3),
					@clearing_firm varchar(5),
					@cusip varchar(10),
					@basket_id varchar(6),
					@giveup_firm varchar(5),	
					@contra_firm varchar(5),
					@exec_broker varchar(4),
					@contra_broker varchar(4),
					@mmcs_sub_acct varchar(3),
					@exercise_multiplier float(53),
					@extended_premium float(53),
					@premium float(53),
					@strike float(53),
					@price float(53),
					@quantity int,
					@tag int,
					@expiration_date int,
					@execution_date int,
					@last_upd_date int,
					@contract_date int,
					@source_system int ,
					@trade_id_num int ,
					@trade_group_id int ,
					@settlement_date int,
					@entry_firm varchar(5),
					@matched_flag tinyint,
					@spread_ind tinyint,
					@premium_multiplier float(53),
					@source_code varchar(3),
					@parent_acct_intrl_id varchar(14),
					@acct_intrl_id varchar(14),
					@q_acct_intrl_id varchar(14),
					@cap_interval float(53),
					@trade_exchange int ,
					@settled_unsettled_ind tinyint,
					@stock_origin tinyint,
					@mm_card_number varchar(8),
					@basket_tag int ,
					@basket_price float(53),
					@short_sale tinyint,
					@commission_code varchar(3),
					@commission float(53),
					@same_day_exer tinyint,
					@terminal_name varchar(11),
					@portfolio_name	varchar(255)
AS

declare @Now integer
exec sp_GetCurrentDate @Now output

if exists (select * from Trades where Trades.iDateOfTrade = @Now and Trades.iCacheID = @iCacheID)
	return

insert into Trades values
(
	@Now,
	@iCacheID,
	@ins_upd_del_flag,
	@put_call	 ,
	@buy_sell ,
	@origin  , 
	@open_close ,
	@trade_id   ,
	@user_id    ,
	@trading_sym ,
	@base_sym	,
	@parent_account ,
	@account ,
	@q_account ,
	@security_type ,
	@security_subtype ,
	@clearing_firm ,
	@cusip ,
	@basket_id ,
	@giveup_firm ,	
	@contra_firm ,
	@exec_broker ,
	@contra_broker ,
	@mmcs_sub_acct ,
	@exercise_multiplier ,
	@extended_premium ,
	@premium ,
	@strike ,
	@price ,
	@quantity ,
	@tag,
	@expiration_date ,
	@execution_date ,
	@last_upd_date ,
	@contract_date ,
	@source_system ,
	@trade_id_num ,
	@trade_group_id ,
	@settlement_date ,
	@entry_firm ,
	@matched_flag ,
	@spread_ind ,
	@premium_multiplier ,
	@source_code ,
	@parent_acct_intrl_id ,
	@acct_intrl_id ,
	@q_acct_intrl_id ,
	@cap_interval ,
	@trade_exchange ,
	@settled_unsettled_ind ,
	@stock_origin ,
	@mm_card_number ,
	@basket_tag ,
	@basket_price ,
	@short_sale ,
	@commission_code ,
	@commission ,
	@same_day_exer ,
	@terminal_name,
	@portfolio_name
)

GO


/*******************************[ 	  GetLastCacheID	]***************************/

IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'sp_GetLastCacheID' AND type = 'P')
   DROP PROCEDURE sp_GetLastCacheID

GO

CREATE PROCEDURE sp_GetLastCacheID
AS
declare @Now integer
exec sp_GetCurrentDate @Now output

select count(*) from Trades where Trades.iDateOfTrade = @Now

GO


/*******************************[ 	  GetAllTrades		]***************************/

IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'sp_GetAllTrades' AND type = 'P')
   DROP PROCEDURE sp_GetAllTrades

GO

CREATE PROCEDURE sp_GetAllTrades
AS
declare @Now integer
exec sp_GetCurrentDate @Now output

select * from Trades where Trades.iDateOfTrade = @Now

GO


/*******************************[ 	  GetTradeByCacheID	]***************************/

IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'sp_GetTradeByCacheID' AND type = 'P')
   DROP PROCEDURE sp_GetTradeByCacheID

GO

CREATE PROCEDURE sp_GetTradeByCacheID 	@iCacheID     int
AS
declare @Now integer
exec sp_GetCurrentDate @Now output

select * from Trades where Trades.iDateOfTrade = @Now and Trades.iCacheID = @iCacheID

GO

/*******************************[ 	  GetLastTradeTime	]***************************/

IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'sp_GetLastTradeTime' AND type = 'P')
   DROP PROCEDURE sp_GetLastTradeTime

GO

CREATE PROCEDURE sp_GetLastTradeTime
AS
select * from LastTradeTime 

GO

/*******************************[ 	  SetLastTradeTime	]***************************/

IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'sp_SetLastTradeTime' AND type = 'P')
   DROP PROCEDURE sp_SetLastTradeTime

GO

CREATE PROCEDURE sp_SetLastTradeTime 	@iLastTradeTime     int
AS
update LastTradeTime set iLastTradeTime = @iLastTradeTime

GO

/*******************************[ 	  GetAllPortfolioRules		]***************************/
IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'sp_GetAllPortfolioRules' AND type = 'P')
   DROP PROCEDURE sp_GetAllPortfolioRules

GO

CREATE PROCEDURE sp_GetAllPortfolioRules
AS
select R.PortfolioName , I.ItemName, F.Mask, T.TypeName
	from PortfolioRules as R, PortfolioRuleItems as I, PortfolioRuleItemTypes as T,
	     PortfolioFilterGroups as G, PortfolioFilters as F
	where 	R.RuleID = I.RuleID and T.TypeID = I.TypeID
		and I.FilterGroupID = G.FilterGroupID and G.FilterGroupID = F.FilterGroupID
	order by R.Priority

GO

/*******************************************************************************************/
/*******************************[ 	  TABLES		]***************************/
/*******************************************************************************************/

/*******************************[ 	  Trades		]***************************/



IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'Trades' AND type = 'U')
   DROP TABLE Trades

GO

CREATE TABLE Trades
(
	iDateOfTrade int ,
	iCacheID     int ,
	ins_upd_del_flag tinyint,
	put_call	 tinyint,
	buy_sell tinyint,
	origin   tinyint,
	open_close tinyint,
	trade_id   varchar(10),
	user_id    varchar(10),
	trading_sym varchar(7),
	base_sym	varchar(7),
	parent_account varchar(11),
	account varchar(11),
	q_account varchar(11),
	security_type varchar(3),
	security_subtype varchar(3),
	clearing_firm varchar(5),
	cusip varchar(10),
	basket_id varchar(6),
	giveup_firm varchar(5),	
	contra_firm varchar(5),
	exec_broker varchar(4),
	contra_broker varchar(4),
	mmcs_sub_acct varchar(3),
	exercise_multiplier float(53),
	extended_premium float(53),
	premium float(53),
	strike float(53),
	price float(53),
	quantity int,
	tag int,
	expiration_date int,
	execution_date int,
	last_upd_date int,
	contract_date int,
	source_system int ,
	trade_id_num int ,
	trade_group_id int ,
	settlement_date int,
	entry_firm varchar(5),
	matched_flag tinyint,
	spread_ind tinyint,
	premium_multiplier float(53),
	source_code varchar(3),
	parent_acct_intrl_id varchar(14),
	acct_intrl_id varchar(14),
	q_acct_intrl_id varchar(14),
	cap_interval float(53),
	trade_exchange int ,
	settled_unsettled_ind tinyint,
	stock_origin tinyint,
	mm_card_number varchar(8),
	basket_tag int ,
	basket_price float(53),
	short_sale tinyint,
	commission_code varchar(3),
	commission float(53),
	same_day_exer tinyint,
	terminal_name varchar(11),	
	portfolio_name	varchar(255)

	constraint PK3 primary key (iDateOfTrade, iCacheID)
)

/*******************************[ 	  LastTradeTime		]***************************/



IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'LastTradeTime' AND type = 'U')
   DROP TABLE LastTradeTime

GO

CREATE TABLE LastTradeTime
(
	iLastTradeTime int
)

insert into LastTradeTime values (0)

/*******************************[ 	  Portfolio Rules		]***************************/

IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'PortfolioRuleItems' AND type = 'U')
   DROP TABLE PortfolioRuleItems

GO

IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'PortfolioFilters' AND type = 'U')
   DROP TABLE PortfolioFilters

GO

IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'PortfolioFilterGroups' AND type = 'U')
   DROP TABLE PortfolioFilterGroups

GO

IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'PortfolioRuleItemTypes' AND type = 'U')
   DROP TABLE PortfolioRuleItemTypes

GO


IF EXISTS (SELECT name FROM sysobjects 
         WHERE name = 'PortfolioRules' AND type = 'U')
   DROP TABLE PortfolioRules

GO

CREATE TABLE PortfolioRules
(
	RuleID 		int 		PRIMARY KEY,
	Priority 	int,
	PortfolioName  	varchar(255)	NOT NULL
)

GO

CREATE TABLE PortfolioRuleItemTypes
(
	TypeID 		int 		IDENTITY(1,1) PRIMARY KEY,
	TypeName  	varchar(255)	NOT NULL
)

GO

CREATE TABLE PortfolioFilterGroups
(
	FilterGroupID 	int 		IDENTITY(1,1) PRIMARY KEY,
	Name  		varchar(255)	NOT NULL
)

GO

CREATE TABLE PortfolioFilters
(
	FilterID 	int 		IDENTITY(1,1) PRIMARY KEY,
	FilterGroupID	int		REFERENCES PortfolioFilterGroups(FilterGroupID),
	Mask  		varchar(255)	NOT NULL
)

GO

insert into PortfolioRuleItemTypes values('INT')
GO
insert into PortfolioRuleItemTypes values('DOUBLE')
GO
insert into PortfolioRuleItemTypes values('CHAR')
GO
insert into PortfolioRuleItemTypes values('STRING')
GO
insert into PortfolioRuleItemTypes values('TIME')
GO

CREATE TABLE PortfolioRuleItems
(
	RuleItemID 	int 		IDENTITY(1,1) PRIMARY KEY,
	ItemName  	varchar(255)	NOT NULL,
	FilterGroupID	int		REFERENCES PortfolioFilterGroups(FilterGroupID),
	RuleID		int		REFERENCES PortfolioRules(RuleID),
	TypeID		int		REFERENCES PortfolioRuleItemTypes(TypeID)
)

GO

/********************************************/
/******* Test portfolio - code to delete ****/
/********************************************/

/*Create rules - priorities and portfolio names*/
if not exists (select * from PortfolioRules where PortfolioName = 'Customers')
	insert into PortfolioRules values (1, 1, 'Customers')
go

if not exists (select * from PortfolioRules where PortfolioName = 'spec1')
	insert into PortfolioRules values (2, 2, 'spec1')
go

if not exists (select * from PortfolioRules where PortfolioName = 'spec2')
	insert into PortfolioRules values (3, 3, 'spec2')
go

if not exists (select * from PortfolioRules where PortfolioName = 'spec3')
	insert into PortfolioRules values (4, 4, 'spec3')
go

if not exists (select * from PortfolioRules where PortfolioName = 'spec4')
	insert into PortfolioRules values (5, 5, 'spec4')
go

delete from PortfolioFilterGroups
GO
/*create filter groups*/
insert into PortfolioFilterGroups values('MSFT_AND_IBM')	/*1*/
GO
insert into PortfolioFilterGroups values('TRADER_A')	/*2*/
GO
insert into PortfolioFilterGroups values('TRADER_B')	/*3*/
GO
insert into PortfolioFilterGroups values('ACCOUNT_A')	/*4*/
GO
insert into PortfolioFilterGroups values('ACCOUNT_B')	/*5*/
GO
insert into PortfolioFilterGroups values('ORIGIN_C')	/*6*/
GO


delete from PortfolioFilters
GO
/*create filters*/
insert into PortfolioFilters values(1, 'MSFT')
GO
insert into PortfolioFilters values(1, 'IBM')
GO
insert into PortfolioFilters values(2, 'A')
GO
insert into PortfolioFilters values(3, 'B')
GO
insert into PortfolioFilters values(4, 'A')
GO
insert into PortfolioFilters values(5, 'B')
GO
insert into PortfolioFilters values(6, 'C C')
GO

/*bind all*/

insert into PortfolioRuleItems values ('origin', 6, 1, 3)/*CHAR*/
go

/*spec1*/
insert into PortfolioRuleItems values ('user_id', 3, 2, 4)/*STRING*/
go

/*spec2*/
insert into PortfolioRuleItems values ('trading_sym', 1, 3, 4)
go
insert into PortfolioRuleItems values ('user_id', 2, 3, 4)
go
insert into PortfolioRuleItems values ('account', 4, 3, 4)
go

/*spec3*/
insert into PortfolioRuleItems values ('trading_sym', 1, 4, 4)
go
insert into PortfolioRuleItems values ('user_id', 2, 4, 4)
go
insert into PortfolioRuleItems values ('account', 5, 4, 4)
go

/*spec4*/
insert into PortfolioRuleItems values ('user_id', 2, 5, 4)
go

/*sp_GetAllTrades*/
/*sp_GetAllPortfolioRules*/
sp_GetAllPortfolioRules