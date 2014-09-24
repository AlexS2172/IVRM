/*
	script for next release of ETS 
*/
set nocount on
GO
--==========================================================================================================
-- changes in ContractExchangeLink table
--==========================================================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractExchangeLink' and COLUMN_NAME='isUnderlying')
	begin
		Alter Table ContractExchangeLink
			Add isUnderlying tinyint constraint DF_isUnderlying default(1)
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractExchangeLink' and COLUMN_NAME='isOption')
	begin
		Alter Table ContractExchangeLink
			Add isOption tinyint constraint DF_isOption default(1)
	end
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractExchangeLink' and COLUMN_NAME='exchangeLinkType')
	execute ('update ContractExchangeLink set isUnderlying = Right(exchangeLinkType,1), isOption = Left(exchangeLinkType,1)')
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractExchangeLink' and COLUMN_NAME='exchangeLinkType')
	Alter Table ContractExchangeLink Drop Column exchangeLinkType
GO
--==========================================================================================================
-- changes in Exchange table
--==========================================================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Exchange' and COLUMN_NAME='isUnderlying')
	begin
		Alter Table Exchange
			Add isUnderlying tinyint constraint DF_Exchange_isUnderlying default(1)
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Exchange' and COLUMN_NAME='isOption')
	begin
		Alter Table Exchange
			Add isOption tinyint constraint DF_Exchange_isOption default(1)
	end
GO
--==========================================================================================================
-- fill up Exchange table
--==========================================================================================================
	if not exists(select * from ExchangeView where ExchangeName = 'NASDAQ')
		insert into ExchangeView(ExchangeName, [description], ExchangeCode) 
			values('NASDAQ','NASDAQ Stock ExchangeView', 'S')
	if not exists(select * from ExchangeView where ExchangeName = 'NYSE')
		insert into ExchangeView (ExchangeName, [description], ExchangeCode)
			values('NYSE','New York Stock ExchangeView', 'N')
	if not exists(select * from ExchangeView where ExchangeName = 'AMEX')
			insert into ExchangeView (ExchangeName, [description], ExchangeCode)
				values('AMEX','AMEX Stock ExchangeView','A')
	if not exists(select * from ExchangeView where ExchangeName = 'CBOE')
			insert into ExchangeView (ExchangeName, [description], ExchangeCode)
				values('CBOE','Chicago Board of Option ExchangeView','H')
	if not exists(select * from ExchangeView where ExchangeName = 'PHLX')
			insert into ExchangeView (ExchangeName, [description], ExchangeCode)
				values('PHLX','Philadelphia Stock ExchangeView','X')
	if not exists(select * from ExchangeView where ExchangeName = 'PSE')
		insert into ExchangeView (ExchangeName, [description], ExchangeCode)
			values('PSE','Pacific Stock ExchangeView (Options)', 'P')
	if not exists(select * from ExchangeView where ExchangeName = 'ISE')
			insert into ExchangeView (ExchangeName, [description], ExchangeCode)
				values('ISE','International Security ExchangeView','K')
GO
	if not exists(select * from DataInfo where dataInfoID = 6)
	        insert into DataInfo 
	            select 6,'lastCleanupDate',GetDate() 
GO
--==========================================================================================================
-- Views
--==========================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExchangeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExchangeView]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE VIEW dbo.ExchangeView
AS
	Select 	exchangeID, 
			exchangeName, 
			[description], 
			exchangeCode, 
			actionID, 
			actionDate, 
			isUnderlying, 
			isOption
		from Exchange
		where actionID < 3
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--==========================================================================================================
-- SP's
--==========================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DatabaseInitialization]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DatabaseInitialization]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  procedure dbo.usp_DatabaseInitialization
as
--Description: this procedure fill up key references tables
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

Begin Transaction

	if not exists(Select * From DataInfo where dataInfoID=1)
		begin
			insert into DataInfo (dataInfoID, keyName,keyValue) values(1,'version','3.23')
		end
	if not exists(Select * From DataInfo where dataInfoID=4)
		begin
			insert into DataInfo (dataInfoID, keyName,keyValue) values(4,'wtd vega maturity','60')
		end
	if not exists(Select * From DataInfo where dataInfoID=5)
		begin
			insert into DataInfo (dataInfoID, keyName,keyValue) values(5,'lastTradeTime','1011646800')
		end

	if not exists(select * from TraderRole)
		DBCC CHECKIDENT ('TraderRole', RESEED,0)

	set identity_insert TraderRole on
	if not exists(Select * From TraderRole where traderRoleID=1)
		begin
			insert into TraderRole (traderRoleID,roleName) values(1,'Administrator')
		end
	if not exists(Select * From TraderRole where traderRoleID=2)
		begin
			insert into TraderRole (traderRoleID,roleName) values(2,'Head RM')
		end
	if not exists(Select * From TraderRole where traderRoleID=3)
		begin
			insert into TraderRole (traderRoleID,roleName) values(3,'RM')
		end
	if not exists(Select * From TraderRole where traderRoleID=4)
		begin
			insert into TraderRole (traderRoleID,roleName) values(4,'Trader')
		end
	set identity_insert TraderRole off

	if not exists(select * from Tenor)
		DBCC CHECKIDENT ('Tenor', RESEED,0)

	set identity_insert Tenor on
	if not exists(Select * From Tenor where tenorID=1)
		begin
			insert into Tenor (tenorID, value,valueIV) values(1,10,30)
		end
	if not exists(Select * From Tenor where tenorID=2)
		begin
			insert into Tenor (tenorID, value,valueIV) values(2,20,30)
		end
	if not exists(Select * From Tenor where tenorID=3)
		begin
			insert into Tenor (tenorID, value,valueIV) values(3,30,30)
		end
	if not exists(Select * From Tenor where tenorID=4)
		begin
			insert into Tenor (tenorID, value,valueIV) values(4,60,60)
		end
	if not exists(Select * From Tenor where tenorID=5)
		begin
			insert into Tenor (tenorID, value,valueIV) values(5,90,90)
		end
	if not exists(Select * From Tenor where tenorID=6)
		begin
			insert into Tenor (tenorID, value,valueIV) values(6,120,120)
		end
	if not exists(Select * From Tenor where tenorID=7)
		begin
			insert into Tenor (tenorID, value,valueIV) values(7,150,150)
		end
	if not exists(Select * From Tenor where tenorID=8)
		begin
			insert into Tenor (tenorID, value,valueIV) values(8,180,180)
		end
	set identity_insert Tenor off

	set identity_insert FilterField on
	if not exists(Select * From FilterField where filterFieldID = 1)
		begin
			insert into FilterField (filterFieldID,fieldName) values(1,'traderID')
		end
	if not exists(Select * From FilterField where filterFieldID = 2)
		begin
			insert into FilterField (filterFieldID,fieldName) values(2,'bookID')
		end
	if not exists(Select * From FilterField where filterFieldID = 3)
		begin
			insert into FilterField (filterFieldID,fieldName) values(3,'strategyID')
		end
	if not exists(Select * From FilterField where filterFieldID = 4)
		begin
			insert into FilterField (filterFieldID,fieldName) values(4,'groupID')
		end
	set identity_insert FilterField off

	set identity_insert RuleItemType on
	if not exists(Select * From RuleItemType where ruleItemTypeID = 1)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(1,'int')
		end
	if not exists(Select * From RuleItemType where ruleItemTypeID = 2)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(2,'double')
		end
	if not exists(Select * From RuleItemType where ruleItemTypeID = 3)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(3,'char')
		end
	if not exists(Select * From RuleItemType where ruleItemTypeID = 4)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(4,'string')
		end
	if not exists(Select * From RuleItemType where ruleItemTypeID = 5)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(5,'time')
		end
	set identity_insert RuleItemType off

	set identity_insert GenerationRule on
	if not exists(Select * From GenerationRule where ruleID = 1)
		begin
			insert into GenerationRule (ruleID,ruleName,interpolationFactor,callShift,putShift,smileAccelerator,strikeIncrement, strikeMoneynessLo,strikeMoneynessHi,isDefaultRule,requiredPoints,ruleType) values(1,'<Default>',0.52,500.0,500.0,0,2,10,150.0,1,9,0)
		end
	set identity_insert GenerationRule off

--============================================================================================================
--fill up Expiry table
--============================================================================================================
if not exists (select * from Expiry)
	begin
		set nocount on
		insert into Expiry(expiryID,expiryPeriod) values (1,'JAN2000')
		insert into Expiry(expiryID,expiryPeriod) values (2,'FEB2000')
		insert into Expiry(expiryID,expiryPeriod) values (3,'MAR2000')
		insert into Expiry(expiryID,expiryPeriod) values (4,'APR2000')
		insert into Expiry(expiryID,expiryPeriod) values (5,'MAY2000')
		insert into Expiry(expiryID,expiryPeriod) values (6,'JUN2000')
		insert into Expiry(expiryID,expiryPeriod) values (7,'JUL2000')
		insert into Expiry(expiryID,expiryPeriod) values (8,'AUG2000')
		insert into Expiry(expiryID,expiryPeriod) values (9,'SEP2000')
		insert into Expiry(expiryID,expiryPeriod) values (10,'OCT2000')
		insert into Expiry(expiryID,expiryPeriod) values (11,'NOV2000')
		insert into Expiry(expiryID,expiryPeriod) values (12,'DEC2000')
		insert into Expiry(expiryID,expiryPeriod) values (13,'JAN2001')
		insert into Expiry(expiryID,expiryPeriod) values (14,'FEB2001')
		insert into Expiry(expiryID,expiryPeriod) values (15,'MAR2001')
		insert into Expiry(expiryID,expiryPeriod) values (16,'APR2001')
		insert into Expiry(expiryID,expiryPeriod) values (17,'MAY2001')
		insert into Expiry(expiryID,expiryPeriod) values (18,'JUN2001')
		insert into Expiry(expiryID,expiryPeriod) values (19,'JUL2001')
		insert into Expiry(expiryID,expiryPeriod) values (20,'AUG2001')
		insert into Expiry(expiryID,expiryPeriod) values (21,'SEP2001')
		insert into Expiry(expiryID,expiryPeriod) values (22,'OCT2001')
		insert into Expiry(expiryID,expiryPeriod) values (23,'NOV2001')
		insert into Expiry(expiryID,expiryPeriod) values (24,'DEC2001')
		insert into Expiry(expiryID,expiryPeriod) values (25,'JAN2002')
		insert into Expiry(expiryID,expiryPeriod) values (26,'FEB2002')
		insert into Expiry(expiryID,expiryPeriod) values (27,'MAR2002')
		insert into Expiry(expiryID,expiryPeriod) values (28,'APR2002')
		insert into Expiry(expiryID,expiryPeriod) values (29,'MAY2002')
		insert into Expiry(expiryID,expiryPeriod) values (30,'JUN2002')
		insert into Expiry(expiryID,expiryPeriod) values (31,'JUL2002')
		insert into Expiry(expiryID,expiryPeriod) values (32,'AUG2002')
		insert into Expiry(expiryID,expiryPeriod) values (33,'SEP2002')
		insert into Expiry(expiryID,expiryPeriod) values (34,'OCT2002')
		insert into Expiry(expiryID,expiryPeriod) values (35,'NOV2002')
		insert into Expiry(expiryID,expiryPeriod) values (36,'DEC2002')
		insert into Expiry(expiryID,expiryPeriod) values (37,'JAN2003')
		insert into Expiry(expiryID,expiryPeriod) values (38,'FEB2003')
		insert into Expiry(expiryID,expiryPeriod) values (39,'MAR2003')
		insert into Expiry(expiryID,expiryPeriod) values (40,'APR2003')
		insert into Expiry(expiryID,expiryPeriod) values (41,'MAY2003')
		insert into Expiry(expiryID,expiryPeriod) values (42,'JUN2003')
		insert into Expiry(expiryID,expiryPeriod) values (43,'JUL2003')
		insert into Expiry(expiryID,expiryPeriod) values (44,'AUG2003')
		insert into Expiry(expiryID,expiryPeriod) values (45,'SEP2003')
		insert into Expiry(expiryID,expiryPeriod) values (46,'OCT2003')
		insert into Expiry(expiryID,expiryPeriod) values (47,'NOV2003')
		insert into Expiry(expiryID,expiryPeriod) values (48,'DEC2003')
		insert into Expiry(expiryID,expiryPeriod) values (49,'JAN2004')
		insert into Expiry(expiryID,expiryPeriod) values (50,'FEB2004')
		insert into Expiry(expiryID,expiryPeriod) values (51,'MAR2004')
		insert into Expiry(expiryID,expiryPeriod) values (52,'APR2004')
		insert into Expiry(expiryID,expiryPeriod) values (53,'MAY2004')
		insert into Expiry(expiryID,expiryPeriod) values (54,'JUN2004')
		insert into Expiry(expiryID,expiryPeriod) values (55,'JUL2004')
		insert into Expiry(expiryID,expiryPeriod) values (56,'AUG2004')
		insert into Expiry(expiryID,expiryPeriod) values (57,'SEP2004')
		insert into Expiry(expiryID,expiryPeriod) values (58,'OCT2004')
		insert into Expiry(expiryID,expiryPeriod) values (59,'NOV2004')
		insert into Expiry(expiryID,expiryPeriod) values (60,'DEC2004')
		insert into Expiry(expiryID,expiryPeriod) values (61,'JAN2005')
		insert into Expiry(expiryID,expiryPeriod) values (62,'FEB2005')
		insert into Expiry(expiryID,expiryPeriod) values (63,'MAR2005')
		insert into Expiry(expiryID,expiryPeriod) values (64,'APR2005')
		insert into Expiry(expiryID,expiryPeriod) values (65,'MAY2005')
		insert into Expiry(expiryID,expiryPeriod) values (66,'JUN2005')
		insert into Expiry(expiryID,expiryPeriod) values (67,'JUL2005')
		insert into Expiry(expiryID,expiryPeriod) values (68,'AUG2005')
		insert into Expiry(expiryID,expiryPeriod) values (69,'SEP2005')
		insert into Expiry(expiryID,expiryPeriod) values (70,'OCT2005')
		insert into Expiry(expiryID,expiryPeriod) values (71,'NOV2005')
		insert into Expiry(expiryID,expiryPeriod) values (72,'DEC2005')
		insert into Expiry(expiryID,expiryPeriod) values (73,'JAN2006')
		insert into Expiry(expiryID,expiryPeriod) values (74,'FEB2006')
		insert into Expiry(expiryID,expiryPeriod) values (75,'MAR2006')
		insert into Expiry(expiryID,expiryPeriod) values (76,'APR2006')
		insert into Expiry(expiryID,expiryPeriod) values (77,'MAY2006')
		insert into Expiry(expiryID,expiryPeriod) values (78,'JUN2006')
		insert into Expiry(expiryID,expiryPeriod) values (79,'JUL2006')
		insert into Expiry(expiryID,expiryPeriod) values (80,'AUG2006')
		insert into Expiry(expiryID,expiryPeriod) values (81,'SEP2006')
		insert into Expiry(expiryID,expiryPeriod) values (82,'OCT2006')
		insert into Expiry(expiryID,expiryPeriod) values (83,'NOV2006')
		insert into Expiry(expiryID,expiryPeriod) values (84,'DEC2006')
		insert into Expiry(expiryID,expiryPeriod) values (85,'JAN2007')
		insert into Expiry(expiryID,expiryPeriod) values (86,'FEB2007')
		insert into Expiry(expiryID,expiryPeriod) values (87,'MAR2007')
		insert into Expiry(expiryID,expiryPeriod) values (88,'APR2007')
		insert into Expiry(expiryID,expiryPeriod) values (89,'MAY2007')
		insert into Expiry(expiryID,expiryPeriod) values (90,'JUN2007')
		insert into Expiry(expiryID,expiryPeriod) values (91,'JUL2007')
		insert into Expiry(expiryID,expiryPeriod) values (92,'AUG2007')
		insert into Expiry(expiryID,expiryPeriod) values (93,'SEP2007')
		insert into Expiry(expiryID,expiryPeriod) values (94,'OCT2007')
		insert into Expiry(expiryID,expiryPeriod) values (95,'NOV2007')
		insert into Expiry(expiryID,expiryPeriod) values (96,'DEC2007')
		insert into Expiry(expiryID,expiryPeriod) values (97,'JAN2008')
		insert into Expiry(expiryID,expiryPeriod) values (98,'FEB2008')
		insert into Expiry(expiryID,expiryPeriod) values (99,'MAR2008')
		insert into Expiry(expiryID,expiryPeriod) values (100,'APR2008')
		insert into Expiry(expiryID,expiryPeriod) values (101,'MAY2008')
		insert into Expiry(expiryID,expiryPeriod) values (102,'JUN2008')
		insert into Expiry(expiryID,expiryPeriod) values (103,'JUL2008')
		insert into Expiry(expiryID,expiryPeriod) values (104,'AUG2008')
		insert into Expiry(expiryID,expiryPeriod) values (105,'SEP2008')
		insert into Expiry(expiryID,expiryPeriod) values (106,'OCT2008')
		insert into Expiry(expiryID,expiryPeriod) values (107,'NOV2008')
		insert into Expiry(expiryID,expiryPeriod) values (108,'DEC2008')
		insert into Expiry(expiryID,expiryPeriod) values (109,'JAN2009')
		insert into Expiry(expiryID,expiryPeriod) values (110,'FEB2009')
		insert into Expiry(expiryID,expiryPeriod) values (111,'MAR2009')
		insert into Expiry(expiryID,expiryPeriod) values (112,'APR2009')
		insert into Expiry(expiryID,expiryPeriod) values (113,'MAY2009')
		insert into Expiry(expiryID,expiryPeriod) values (114,'JUN2009')
		insert into Expiry(expiryID,expiryPeriod) values (115,'JUL2009')
		insert into Expiry(expiryID,expiryPeriod) values (116,'AUG2009')
		insert into Expiry(expiryID,expiryPeriod) values (117,'SEP2009')
		insert into Expiry(expiryID,expiryPeriod) values (118,'OCT2009')
		insert into Expiry(expiryID,expiryPeriod) values (119,'NOV2009')
		insert into Expiry(expiryID,expiryPeriod) values (120,'DEC2009')
		insert into Expiry(expiryID,expiryPeriod) values (121,'JAN2010')
		insert into Expiry(expiryID,expiryPeriod) values (122,'FEB2010')
		insert into Expiry(expiryID,expiryPeriod) values (123,'MAR2010')
		insert into Expiry(expiryID,expiryPeriod) values (124,'APR2010')
		insert into Expiry(expiryID,expiryPeriod) values (125,'MAY2010')
		insert into Expiry(expiryID,expiryPeriod) values (126,'JUN2010')
		insert into Expiry(expiryID,expiryPeriod) values (127,'JUL2010')
		insert into Expiry(expiryID,expiryPeriod) values (128,'AUG2010')
		insert into Expiry(expiryID,expiryPeriod) values (129,'SEP2010')
		insert into Expiry(expiryID,expiryPeriod) values (130,'OCT2010')
		insert into Expiry(expiryID,expiryPeriod) values (131,'NOV2010')
		insert into Expiry(expiryID,expiryPeriod) values (132,'DEC2010')
		insert into Expiry(expiryID,expiryPeriod) values (133,'JAN2011')
		insert into Expiry(expiryID,expiryPeriod) values (134,'FEB2011')
		insert into Expiry(expiryID,expiryPeriod) values (135,'MAR2011')
		insert into Expiry(expiryID,expiryPeriod) values (136,'APR2011')
		insert into Expiry(expiryID,expiryPeriod) values (137,'MAY2011')
		insert into Expiry(expiryID,expiryPeriod) values (138,'JUN2011')
		insert into Expiry(expiryID,expiryPeriod) values (139,'JUL2011')
		insert into Expiry(expiryID,expiryPeriod) values (140,'AUG2011')
		insert into Expiry(expiryID,expiryPeriod) values (141,'SEP2011')
		insert into Expiry(expiryID,expiryPeriod) values (142,'OCT2011')
		insert into Expiry(expiryID,expiryPeriod) values (143,'NOV2011')
		insert into Expiry(expiryID,expiryPeriod) values (144,'DEC2011')
		insert into Expiry(expiryID,expiryPeriod) values (145,'JAN2012')
		insert into Expiry(expiryID,expiryPeriod) values (146,'FEB2012')
		insert into Expiry(expiryID,expiryPeriod) values (147,'MAR2012')
		insert into Expiry(expiryID,expiryPeriod) values (148,'APR2012')
		insert into Expiry(expiryID,expiryPeriod) values (149,'MAY2012')
		insert into Expiry(expiryID,expiryPeriod) values (150,'JUN2012')
		insert into Expiry(expiryID,expiryPeriod) values (151,'JUL2012')
		insert into Expiry(expiryID,expiryPeriod) values (152,'AUG2012')
		insert into Expiry(expiryID,expiryPeriod) values (153,'SEP2012')
		insert into Expiry(expiryID,expiryPeriod) values (154,'OCT2012')
		insert into Expiry(expiryID,expiryPeriod) values (155,'NOV2012')
		insert into Expiry(expiryID,expiryPeriod) values (156,'DEC2012')
		insert into Expiry(expiryID,expiryPeriod) values (157,'JAN2013')
		insert into Expiry(expiryID,expiryPeriod) values (158,'FEB2013')
		insert into Expiry(expiryID,expiryPeriod) values (159,'MAR2013')
		insert into Expiry(expiryID,expiryPeriod) values (160,'APR2013')
		insert into Expiry(expiryID,expiryPeriod) values (161,'MAY2013')
		insert into Expiry(expiryID,expiryPeriod) values (162,'JUN2013')
		insert into Expiry(expiryID,expiryPeriod) values (163,'JUL2013')
		insert into Expiry(expiryID,expiryPeriod) values (164,'AUG2013')
		insert into Expiry(expiryID,expiryPeriod) values (165,'SEP2013')
		insert into Expiry(expiryID,expiryPeriod) values (166,'OCT2013')
		insert into Expiry(expiryID,expiryPeriod) values (167,'NOV2013')
		insert into Expiry(expiryID,expiryPeriod) values (168,'DEC2013')
		insert into Expiry(expiryID,expiryPeriod) values (169,'JAN2014')
		insert into Expiry(expiryID,expiryPeriod) values (170,'FEB2014')
		insert into Expiry(expiryID,expiryPeriod) values (171,'MAR2014')
		insert into Expiry(expiryID,expiryPeriod) values (172,'APR2014')
		insert into Expiry(expiryID,expiryPeriod) values (173,'MAY2014')
		insert into Expiry(expiryID,expiryPeriod) values (174,'JUN2014')
		insert into Expiry(expiryID,expiryPeriod) values (175,'JUL2014')
		insert into Expiry(expiryID,expiryPeriod) values (176,'AUG2014')
		insert into Expiry(expiryID,expiryPeriod) values (177,'SEP2014')
		insert into Expiry(expiryID,expiryPeriod) values (178,'OCT2014')
		insert into Expiry(expiryID,expiryPeriod) values (179,'NOV2014')
		insert into Expiry(expiryID,expiryPeriod) values (180,'DEC2014')
		insert into Expiry(expiryID,expiryPeriod) values (181,'JAN2015')
		insert into Expiry(expiryID,expiryPeriod) values (182,'FEB2015')
		insert into Expiry(expiryID,expiryPeriod) values (183,'MAR2015')
		insert into Expiry(expiryID,expiryPeriod) values (184,'APR2015')
		insert into Expiry(expiryID,expiryPeriod) values (185,'MAY2015')
		insert into Expiry(expiryID,expiryPeriod) values (186,'JUN2015')
		insert into Expiry(expiryID,expiryPeriod) values (187,'JUL2015')
		insert into Expiry(expiryID,expiryPeriod) values (188,'AUG2015')
		insert into Expiry(expiryID,expiryPeriod) values (189,'SEP2015')
		insert into Expiry(expiryID,expiryPeriod) values (190,'OCT2015')
		insert into Expiry(expiryID,expiryPeriod) values (191,'NOV2015')
		insert into Expiry(expiryID,expiryPeriod) values (192,'DEC2015')
		insert into Expiry(expiryID,expiryPeriod) values (193,'JAN2016')
		insert into Expiry(expiryID,expiryPeriod) values (194,'FEB2016')
		insert into Expiry(expiryID,expiryPeriod) values (195,'MAR2016')
		insert into Expiry(expiryID,expiryPeriod) values (196,'APR2016')
		insert into Expiry(expiryID,expiryPeriod) values (197,'MAY2016')
		insert into Expiry(expiryID,expiryPeriod) values (198,'JUN2016')
		insert into Expiry(expiryID,expiryPeriod) values (199,'JUL2016')
		insert into Expiry(expiryID,expiryPeriod) values (200,'AUG2016')
		insert into Expiry(expiryID,expiryPeriod) values (201,'SEP2016')
		insert into Expiry(expiryID,expiryPeriod) values (202,'OCT2016')
		insert into Expiry(expiryID,expiryPeriod) values (203,'NOV2016')
		insert into Expiry(expiryID,expiryPeriod) values (204,'DEC2016')
		insert into Expiry(expiryID,expiryPeriod) values (205,'JAN2017')
		insert into Expiry(expiryID,expiryPeriod) values (206,'FEB2017')
		insert into Expiry(expiryID,expiryPeriod) values (207,'MAR2017')
		insert into Expiry(expiryID,expiryPeriod) values (208,'APR2017')
		insert into Expiry(expiryID,expiryPeriod) values (209,'MAY2017')
		insert into Expiry(expiryID,expiryPeriod) values (210,'JUN2017')
		insert into Expiry(expiryID,expiryPeriod) values (211,'JUL2017')
		insert into Expiry(expiryID,expiryPeriod) values (212,'AUG2017')
		insert into Expiry(expiryID,expiryPeriod) values (213,'SEP2017')
		insert into Expiry(expiryID,expiryPeriod) values (214,'OCT2017')
		insert into Expiry(expiryID,expiryPeriod) values (215,'NOV2017')
		insert into Expiry(expiryID,expiryPeriod) values (216,'DEC2017')
		insert into Expiry(expiryID,expiryPeriod) values (217,'JAN2018')
		insert into Expiry(expiryID,expiryPeriod) values (218,'FEB2018')
		insert into Expiry(expiryID,expiryPeriod) values (219,'MAR2018')
		insert into Expiry(expiryID,expiryPeriod) values (220,'APR2018')
		insert into Expiry(expiryID,expiryPeriod) values (221,'MAY2018')
		insert into Expiry(expiryID,expiryPeriod) values (222,'JUN2018')
		insert into Expiry(expiryID,expiryPeriod) values (223,'JUL2018')
		insert into Expiry(expiryID,expiryPeriod) values (224,'AUG2018')
		insert into Expiry(expiryID,expiryPeriod) values (225,'SEP2018')
		insert into Expiry(expiryID,expiryPeriod) values (226,'OCT2018')
		insert into Expiry(expiryID,expiryPeriod) values (227,'NOV2018')
		insert into Expiry(expiryID,expiryPeriod) values (228,'DEC2018')
		insert into Expiry(expiryID,expiryPeriod) values (229,'JAN2019')
		insert into Expiry(expiryID,expiryPeriod) values (230,'FEB2019')
		insert into Expiry(expiryID,expiryPeriod) values (231,'MAR2019')
		insert into Expiry(expiryID,expiryPeriod) values (232,'APR2019')
		insert into Expiry(expiryID,expiryPeriod) values (233,'MAY2019')
		insert into Expiry(expiryID,expiryPeriod) values (234,'JUN2019')
		insert into Expiry(expiryID,expiryPeriod) values (235,'JUL2019')
		insert into Expiry(expiryID,expiryPeriod) values (236,'AUG2019')
		insert into Expiry(expiryID,expiryPeriod) values (237,'SEP2019')
		insert into Expiry(expiryID,expiryPeriod) values (238,'OCT2019')
		insert into Expiry(expiryID,expiryPeriod) values (239,'NOV2019')
		insert into Expiry(expiryID,expiryPeriod) values (240,'DEC2019')
		insert into Expiry(expiryID,expiryPeriod) values (241,'JAN2020')
		insert into Expiry(expiryID,expiryPeriod) values (242,'FEB2020')
		insert into Expiry(expiryID,expiryPeriod) values (243,'MAR2020')
		insert into Expiry(expiryID,expiryPeriod) values (244,'APR2020')
		insert into Expiry(expiryID,expiryPeriod) values (245,'MAY2020')
		insert into Expiry(expiryID,expiryPeriod) values (246,'JUN2020')
		insert into Expiry(expiryID,expiryPeriod) values (247,'JUL2020')
		insert into Expiry(expiryID,expiryPeriod) values (248,'AUG2020')
		insert into Expiry(expiryID,expiryPeriod) values (249,'SEP2020')
		insert into Expiry(expiryID,expiryPeriod) values (250,'OCT2020')
		insert into Expiry(expiryID,expiryPeriod) values (251,'NOV2020')
		insert into Expiry(expiryID,expiryPeriod) values (252,'DEC2020')
	
	end
--============================================================================================================
--fill up ExpiryCalendar table
--============================================================================================================
if not exists (select * from ExpiryCalendar) 
	begin
		insert into ExpiryCalendar (expCalendarName,expCalendarDesc) values('Third Saturday','Third saturday of the month')
		insert into ExpiryCalendar (expCalendarName,expCalendarDesc) values('Third Friday','Third friday of the month')
		insert into ExpiryCalendar (expCalendarName,expCalendarDesc) values('Third Thursday','Third thursday of the month')
	end

--============================================================================================================
--fill up ExpiryInCalendar table
--============================================================================================================
if not exists (select * from ExpiryInCalendar) 
	begin
		set nocount on
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,1,'2000-01-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,2,'2000-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,3,'2000-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,4,'2000-04-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,5,'2000-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,6,'2000-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,7,'2000-07-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,8,'2000-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,9,'2000-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,10,'2000-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,11,'2000-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,12,'2000-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,13,'2001-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,14,'2001-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,15,'2001-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,16,'2001-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,17,'2001-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,18,'2001-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,19,'2001-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,20,'2001-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,21,'2001-09-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,22,'2001-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,23,'2001-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,24,'2001-12-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,25,'2002-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,26,'2002-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,27,'2002-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,28,'2002-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,29,'2002-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,30,'2002-06-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,31,'2002-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,32,'2002-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,33,'2002-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,34,'2002-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,35,'2002-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,36,'2002-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,37,'2003-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,38,'2003-02-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,39,'2003-03-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,40,'2003-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,41,'2003-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,42,'2003-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,43,'2003-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,44,'2003-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,45,'2003-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,46,'2003-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,47,'2003-11-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,48,'2003-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,49,'2004-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,50,'2004-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,51,'2004-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,52,'2004-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,53,'2004-05-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,54,'2004-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,55,'2004-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,56,'2004-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,57,'2004-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,58,'2004-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,59,'2004-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,60,'2004-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,61,'2005-01-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,62,'2005-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,63,'2005-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,64,'2005-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,65,'2005-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,66,'2005-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,67,'2005-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,68,'2005-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,69,'2005-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,70,'2005-10-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,71,'2005-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,72,'2005-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,73,'2006-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,74,'2006-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,75,'2006-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,76,'2006-04-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,77,'2006-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,78,'2006-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,79,'2006-07-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,80,'2006-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,81,'2006-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,82,'2006-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,83,'2006-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,84,'2006-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,85,'2007-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,86,'2007-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,87,'2007-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,88,'2007-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,89,'2007-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,90,'2007-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,91,'2007-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,92,'2007-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,93,'2007-09-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,94,'2007-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,95,'2007-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,96,'2007-12-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,97,'2008-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,98,'2008-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,99,'2008-03-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,100,'2008-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,101,'2008-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,102,'2008-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,103,'2008-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,104,'2008-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,105,'2008-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,106,'2008-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,107,'2008-11-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,108,'2008-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,109,'2009-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,110,'2009-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,111,'2009-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,112,'2009-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,113,'2009-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,114,'2009-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,115,'2009-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,116,'2009-08-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,117,'2009-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,118,'2009-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,119,'2009-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,120,'2009-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,121,'2010-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,122,'2010-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,123,'2010-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,124,'2010-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,125,'2010-05-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,126,'2010-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,127,'2010-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,128,'2010-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,129,'2010-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,130,'2010-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,131,'2010-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,132,'2010-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,133,'2011-01-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,134,'2011-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,135,'2011-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,136,'2011-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,137,'2011-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,138,'2011-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,139,'2011-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,140,'2011-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,141,'2011-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,142,'2011-10-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,143,'2011-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,144,'2011-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,145,'2012-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,146,'2012-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,147,'2012-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,148,'2012-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,149,'2012-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,150,'2012-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,151,'2012-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,152,'2012-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,153,'2012-09-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,154,'2012-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,155,'2012-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,156,'2012-12-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,157,'2013-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,158,'2013-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,159,'2013-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,160,'2013-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,161,'2013-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,162,'2013-06-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,163,'2013-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,164,'2013-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,165,'2013-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,166,'2013-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,167,'2013-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,168,'2013-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,169,'2014-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,170,'2014-02-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,171,'2014-03-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,172,'2014-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,173,'2014-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,174,'2014-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,175,'2014-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,176,'2014-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,177,'2014-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,178,'2014-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,179,'2014-11-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,180,'2014-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,181,'2015-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,182,'2015-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,183,'2015-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,184,'2015-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,185,'2015-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,186,'2015-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,187,'2015-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,188,'2015-08-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,189,'2015-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,190,'2015-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,191,'2015-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,192,'2015-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,193,'2016-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,194,'2016-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,195,'2016-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,196,'2016-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,197,'2016-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,198,'2016-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,199,'2016-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,200,'2016-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,201,'2016-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,202,'2016-10-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,203,'2016-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,204,'2016-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,205,'2017-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,206,'2017-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,207,'2017-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,208,'2017-04-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,209,'2017-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,210,'2017-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,211,'2017-07-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,212,'2017-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,213,'2017-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,214,'2017-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,215,'2017-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,216,'2017-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,217,'2018-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,218,'2018-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,219,'2018-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,220,'2018-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,221,'2018-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,222,'2018-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,223,'2018-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,224,'2018-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,225,'2018-09-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,226,'2018-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,227,'2018-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,228,'2018-12-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,229,'2019-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,230,'2019-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,231,'2019-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,232,'2019-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,233,'2019-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,234,'2019-06-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,235,'2019-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,236,'2019-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,237,'2019-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,238,'2019-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,239,'2019-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,240,'2019-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,241,'2020-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,242,'2020-02-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,243,'2020-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,244,'2020-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,245,'2020-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,246,'2020-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,247,'2020-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,248,'2020-08-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,249,'2020-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,250,'2020-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,251,'2020-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,252,'2020-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,1,'2000-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,2,'2000-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,3,'2000-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,4,'2000-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,5,'2000-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,6,'2000-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,7,'2000-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,8,'2000-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,9,'2000-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,10,'2000-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,11,'2000-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,12,'2000-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,13,'2001-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,14,'2001-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,15,'2001-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,16,'2001-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,17,'2001-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,18,'2001-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,19,'2001-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,20,'2001-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,21,'2001-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,22,'2001-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,23,'2001-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,24,'2001-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,25,'2002-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,26,'2002-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,27,'2002-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,28,'2002-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,29,'2002-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,30,'2002-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,31,'2002-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,32,'2002-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,33,'2002-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,34,'2002-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,35,'2002-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,36,'2002-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,37,'2003-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,38,'2003-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,39,'2003-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,40,'2003-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,41,'2003-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,42,'2003-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,43,'2003-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,44,'2003-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,45,'2003-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,46,'2003-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,47,'2003-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,48,'2003-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,49,'2004-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,50,'2004-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,51,'2004-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,52,'2004-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,53,'2004-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,54,'2004-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,55,'2004-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,56,'2004-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,57,'2004-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,58,'2004-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,59,'2004-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,60,'2004-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,61,'2005-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,62,'2005-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,63,'2005-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,64,'2005-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,65,'2005-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,66,'2005-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,67,'2005-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,68,'2005-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,69,'2005-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,70,'2005-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,71,'2005-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,72,'2005-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,73,'2006-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,74,'2006-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,75,'2006-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,76,'2006-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,77,'2006-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,78,'2006-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,79,'2006-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,80,'2006-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,81,'2006-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,82,'2006-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,83,'2006-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,84,'2006-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,85,'2007-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,86,'2007-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,87,'2007-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,88,'2007-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,89,'2007-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,90,'2007-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,91,'2007-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,92,'2007-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,93,'2007-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,94,'2007-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,95,'2007-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,96,'2007-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,97,'2008-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,98,'2008-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,99,'2008-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,100,'2008-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,101,'2008-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,102,'2008-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,103,'2008-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,104,'2008-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,105,'2008-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,106,'2008-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,107,'2008-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,108,'2008-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,109,'2009-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,110,'2009-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,111,'2009-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,112,'2009-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,113,'2009-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,114,'2009-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,115,'2009-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,116,'2009-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,117,'2009-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,118,'2009-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,119,'2009-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,120,'2009-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,121,'2010-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,122,'2010-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,123,'2010-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,124,'2010-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,125,'2010-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,126,'2010-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,127,'2010-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,128,'2010-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,129,'2010-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,130,'2010-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,131,'2010-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,132,'2010-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,133,'2011-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,134,'2011-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,135,'2011-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,136,'2011-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,137,'2011-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,138,'2011-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,139,'2011-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,140,'2011-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,141,'2011-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,142,'2011-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,143,'2011-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,144,'2011-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,145,'2012-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,146,'2012-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,147,'2012-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,148,'2012-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,149,'2012-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,150,'2012-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,151,'2012-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,152,'2012-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,153,'2012-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,154,'2012-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,155,'2012-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,156,'2012-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,157,'2013-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,158,'2013-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,159,'2013-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,160,'2013-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,161,'2013-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,162,'2013-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,163,'2013-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,164,'2013-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,165,'2013-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,166,'2013-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,167,'2013-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,168,'2013-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,169,'2014-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,170,'2014-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,171,'2014-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,172,'2014-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,173,'2014-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,174,'2014-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,175,'2014-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,176,'2014-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,177,'2014-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,178,'2014-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,179,'2014-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,180,'2014-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,181,'2015-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,182,'2015-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,183,'2015-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,184,'2015-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,185,'2015-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,186,'2015-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,187,'2015-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,188,'2015-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,189,'2015-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,190,'2015-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,191,'2015-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,192,'2015-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,193,'2016-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,194,'2016-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,195,'2016-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,196,'2016-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,197,'2016-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,198,'2016-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,199,'2016-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,200,'2016-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,201,'2016-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,202,'2016-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,203,'2016-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,204,'2016-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,205,'2017-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,206,'2017-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,207,'2017-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,208,'2017-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,209,'2017-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,210,'2017-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,211,'2017-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,212,'2017-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,213,'2017-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,214,'2017-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,215,'2017-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,216,'2017-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,217,'2018-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,218,'2018-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,219,'2018-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,220,'2018-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,221,'2018-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,222,'2018-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,223,'2018-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,224,'2018-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,225,'2018-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,226,'2018-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,227,'2018-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,228,'2018-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,229,'2019-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,230,'2019-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,231,'2019-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,232,'2019-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,233,'2019-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,234,'2019-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,235,'2019-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,236,'2019-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,237,'2019-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,238,'2019-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,239,'2019-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,240,'2019-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,241,'2020-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,242,'2020-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,243,'2020-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,244,'2020-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,245,'2020-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,246,'2020-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,247,'2020-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,248,'2020-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,249,'2020-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,250,'2020-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,251,'2020-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,252,'2020-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,1,'2000-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,2,'2000-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,3,'2000-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,4,'2000-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,5,'2000-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,6,'2000-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,7,'2000-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,8,'2000-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,9,'2000-09-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,10,'2000-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,11,'2000-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,12,'2000-12-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,13,'2001-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,14,'2001-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,15,'2001-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,16,'2001-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,17,'2001-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,18,'2001-06-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,19,'2001-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,20,'2001-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,21,'2001-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,22,'2001-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,23,'2001-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,24,'2001-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,25,'2002-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,26,'2002-02-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,27,'2002-03-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,28,'2002-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,29,'2002-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,30,'2002-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,31,'2002-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,32,'2002-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,33,'2002-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,34,'2002-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,35,'2002-11-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,36,'2002-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,37,'2003-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,38,'2003-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,39,'2003-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,40,'2003-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,41,'2003-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,42,'2003-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,43,'2003-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,44,'2003-08-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,45,'2003-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,46,'2003-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,47,'2003-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,48,'2003-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,49,'2004-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,50,'2004-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,51,'2004-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,52,'2004-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,53,'2004-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,54,'2004-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,55,'2004-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,56,'2004-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,57,'2004-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,58,'2004-10-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,59,'2004-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,60,'2004-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,61,'2005-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,62,'2005-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,63,'2005-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,64,'2005-04-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,65,'2005-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,66,'2005-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,67,'2005-07-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,68,'2005-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,69,'2005-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,70,'2005-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,71,'2005-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,72,'2005-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,73,'2006-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,74,'2006-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,75,'2006-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,76,'2006-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,77,'2006-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,78,'2006-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,79,'2006-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,80,'2006-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,81,'2006-09-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,82,'2006-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,83,'2006-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,84,'2006-12-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,85,'2007-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,86,'2007-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,87,'2007-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,88,'2007-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,89,'2007-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,90,'2007-06-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,91,'2007-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,92,'2007-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,93,'2007-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,94,'2007-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,95,'2007-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,96,'2007-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,97,'2008-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,98,'2008-02-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,99,'2008-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,100,'2008-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,101,'2008-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,102,'2008-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,103,'2008-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,104,'2008-08-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,105,'2008-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,106,'2008-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,107,'2008-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,108,'2008-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,109,'2009-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,110,'2009-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,111,'2009-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,112,'2009-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,113,'2009-05-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,114,'2009-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,115,'2009-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,116,'2009-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,117,'2009-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,118,'2009-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,119,'2009-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,120,'2009-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,121,'2010-01-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,122,'2010-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,123,'2010-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,124,'2010-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,125,'2010-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,126,'2010-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,127,'2010-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,128,'2010-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,129,'2010-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,130,'2010-10-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,131,'2010-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,132,'2010-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,133,'2011-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,134,'2011-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,135,'2011-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,136,'2011-04-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,137,'2011-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,138,'2011-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,139,'2011-07-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,140,'2011-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,141,'2011-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,142,'2011-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,143,'2011-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,144,'2011-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,145,'2012-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,146,'2012-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,147,'2012-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,148,'2012-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,149,'2012-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,150,'2012-06-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,151,'2012-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,152,'2012-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,153,'2012-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,154,'2012-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,155,'2012-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,156,'2012-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,157,'2013-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,158,'2013-02-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,159,'2013-03-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,160,'2013-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,161,'2013-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,162,'2013-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,163,'2013-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,164,'2013-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,165,'2013-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,166,'2013-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,167,'2013-11-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,168,'2013-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,169,'2014-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,170,'2014-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,171,'2014-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,172,'2014-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,173,'2014-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,174,'2014-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,175,'2014-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,176,'2014-08-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,177,'2014-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,178,'2014-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,179,'2014-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,180,'2014-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,181,'2015-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,182,'2015-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,183,'2015-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,184,'2015-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,185,'2015-05-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,186,'2015-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,187,'2015-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,188,'2015-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,189,'2015-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,190,'2015-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,191,'2015-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,192,'2015-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,193,'2016-01-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,194,'2016-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,195,'2016-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,196,'2016-04-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,197,'2016-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,198,'2016-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,199,'2016-07-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,200,'2016-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,201,'2016-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,202,'2016-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,203,'2016-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,204,'2016-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,205,'2017-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,206,'2017-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,207,'2017-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,208,'2017-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,209,'2017-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,210,'2017-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,211,'2017-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,212,'2017-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,213,'2017-09-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,214,'2017-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,215,'2017-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,216,'2017-12-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,217,'2018-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,218,'2018-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,219,'2018-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,220,'2018-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,221,'2018-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,222,'2018-06-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,223,'2018-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,224,'2018-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,225,'2018-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,226,'2018-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,227,'2018-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,228,'2018-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,229,'2019-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,230,'2019-02-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,231,'2019-03-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,232,'2019-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,233,'2019-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,234,'2019-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,235,'2019-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,236,'2019-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,237,'2019-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,238,'2019-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,239,'2019-11-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,240,'2019-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,241,'2020-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,242,'2020-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,243,'2020-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,244,'2020-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,245,'2020-05-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,246,'2020-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,247,'2020-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,248,'2020-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,249,'2020-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,250,'2020-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,251,'2020-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,252,'2020-12-17')
	end

	-- fill up Exchange table
	if not exists(select * from ExchangeView where ExchangeName = 'NASDAQ')
		insert into ExchangeView(ExchangeName, [description], ExchangeCode) 
			values('NASDAQ','NASDAQ Stock ExchangeView', 'S')
	if not exists(select * from ExchangeView where ExchangeName = 'NYSE')
		insert into ExchangeView (ExchangeName, [description], ExchangeCode)
			values('NYSE','New York Stock ExchangeView', 'N')
	if not exists(select * from ExchangeView where ExchangeName = 'AMEX')
			insert into ExchangeView (ExchangeName, [description], ExchangeCode)
				values('AMEX','AMEX Stock ExchangeView','A')
	if not exists(select * from ExchangeView where ExchangeName = 'CBOE')
			insert into ExchangeView (ExchangeName, [description], ExchangeCode)
				values('CBOE','Chicago Board of Option ExchangeView','H')
	if not exists(select * from ExchangeView where ExchangeName = 'PHLX')
			insert into ExchangeView (ExchangeName, [description], ExchangeCode)
				values('PHLX','Philadelphia Stock ExchangeView','X')
	if not exists(select * from ExchangeView where ExchangeName = 'PSE')
		insert into ExchangeView (ExchangeName, [description], ExchangeCode)
			values('PSE','Pacific Stock ExchangeView (Options)', 'P')
	if not exists(select * from ExchangeView where ExchangeName = 'ISE')
			insert into ExchangeView (ExchangeName, [description], ExchangeCode)
				values('ISE','International Security ExchangeView','K')

Commit Transaction
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinitionBeta_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinitionBeta_Clear]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_New]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  Procedure dbo.usp_IndexDefinitionBeta_Clear 
	@iIndexID int = null, --the index identifier
	@iStockID int = null --the stock identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into IndexDefinition table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction
		update IndexDefinition
			set beta = null
			where 
				(@iIndexID is null or indexID = @iIndexID)
				and (@iStockID is null or stockID = @iStockID)
	
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

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
							join ExpiryInCalendar EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
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

CREATE PROCEDURE dbo.usp_TradeFeed_New
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

			insert into Stock (stockID) values(@iUnderlyingContractID)
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
			join ExpiryInCalendar EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
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
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Save]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE procedure dbo.usp_Exchange_Get
	@iExchangeID int = null,
	@dtActionDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Exchange table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
  
	select
		exchangeID as iExchangeID,
		exchangeName as vcExchangeName,
		[Description] as vcDescription,
		ExchangeCode as vcExchangeCode,
		actionDate as dtActionDate,
		actionID as tiActionID,
		isUnderlying as tiIsUnderlying,
		isOption as tiIsOption
	from Exchange
	where (exchangeID=@iExchangeID or @iExchangeID is null) and
			(actionDate >= @dtActionDate or @dtActionDate is null and actionID<3)

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
CREATE    proc dbo.usp_Exchange_Save
		@iExchangeID int = null output,
		@vcExchangeName varchar(30) = null,
		@vcDescription varchar(120) = null,
		@vcExchangeCode varchar(10) = null,
		@tiIsUnderlying tinyint = null,
		@tiIsOption tinyint = null
  as
	if exists (select * from Exchange where ExchangeName = @vcExchangeName and actionID < 3 and (@iExchangeID <> ExchangeID or @iExchangeID is null))
		begin
			Raiserror ('Exchange with this name already exists!', 16, 1)
			Return (-1)
		end   
	if exists (select * from Exchange where ExchangeCode = @vcExchangeCode and actionID < 3 and (@iExchangeID <> ExchangeID or @iExchangeID is null))
		begin
			Raiserror ('Exchange with this code already exists!', 16, 1)
			Return (-1)
		end   
   
	if @iExchangeID is null
	begin
		insert into Exchange (
					exchangeName, 
					[Description],
					exchangeCode, 
					isUnderlying, 
					isOption)
				values (
					@vcExchangeName, 
					@vcDescription,
					@vcExchangeCode, 
					isnull(@tiIsUnderlying, 1), 
					isnull(@tiIsOption, 1))

			if (@@error <> 0) return (-1)

		set @iExchangeID = @@identity
	 end
   else 
	 begin
		update Exchange
			 set  
				exchangeName = isnull(@vcExchangeName,exchangeName),
				[Description] = isnull(@vcDescription,[Description]),
				exchangeCode = isnull(@vcExchangeCode,exchangeCode),
				isUnderlying = isnull(@tiIsUnderlying, isUnderlying),
				isOption = isnull(@tiIsOption, isOption)
			where exchangeID = @iExchangeID

		if (@@error <> 0)
		   return (-1)
		 else  
		   return (0)
	 end				
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractExchangeLink_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractExchangeLink_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Save]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE proc usp_ContractExchangeLink_Get
	@iContractID int = Null
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select contractID as iContractID,
			exchangeID as iExchangeID,
			isUnderlying as tiIsUnderlying,
			isOption as tiIsOption,
			ContractExchangeLinkID as iContractExchangeLinkID
		from ContractExchangeLink
		where contractID=@iContractID or (@iContractID is null)
		
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
--				case when isnull(convert(bit,left(CP.exchangeLinkType,1)),0) <> 0 then e.isUnderlying else 0 end as iType1,
--				case when isnull(convert(bit,right(CP.exchangeLinkType,1)),0) <> 0 then e.isOption else 0 end as iType2,
				e.isUnderlying as tiIsUnderlyingExchange,
				e.isOption as tiIsOptionExchange
			from ContractExchangeLink CP, ExchangeView E
			where CP.contractID = @iContractID and CP.exchangeID =* E.exchangeID
			order by E.ExchangeName
	else
		select cp.exchangeID as iExchangeID,
				case when isnull(CP.isUnderlying,0) <> 0  then isnull(e.isUnderlying,0) else 0 end as iType1,
				case when isnull(CP.isOption,0) <> 0 then isnull(e.isOption,0) else 0 end as iType2
--				case when isnull(convert(bit,left(CP.exchangeLinkType,1)),0) <> 0 then e.isUnderlying else 0 end as iType1,
--				case when isnull(convert(bit,right(CP.exchangeLinkType,1)),0) <> 0 then e.isOption else 0 end as iType2
			from ContractExchangeLink CP
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
		if not exists(select * from ContractExchangeLink where isnull(exchangeID,0)=isnull(@iExchangeID,0) and contractID=@iContractID) 
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
--=====================================================================================================
--SP's for DB cleanup
--=====================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[job_Cleanup_DB]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[job_Cleanup_DB]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Del]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Del]
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
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
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

CREATE Procedure dbo.job_Cleanup_DB

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
		update DataInfo set keyValue = GetDate() where dataInfoID = 6

	Commit Transaction
    set @error = 0
finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingByGroup_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Underlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Underlying_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE           proc dbo.usp_UnderlyingByGroup_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@tiWithPosition, 0) = 0
	begin
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
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
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
			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
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
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
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
			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				s.isActive as tiIsActive
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize, s.isActive
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				i.isActive as tiIsActive
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				s.isActive as tiIsActive
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize, s.isActive
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				i.isActive as tiIsActive
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive
			order by c.symbol	 
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

CREATE            proc dbo.usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@iGroupID, 0) <> 0
	begin
		exec usp_UnderlyingByGroup_Get @iContractID,@iTraderID,@iGroupID,@tiWithPosition	
		Return(0)
	end 

	if isnull(@tiWithPosition, 0) = 0
	begin
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
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive
			from ContractView c 
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
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
			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive
			from ContractView c 
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
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
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
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
			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				s.isActive as tiIsActive
			from ContractView c 
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where
				c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize, s.isActive
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,

				i.isActive as tiIsActive
			from ContractView c 
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where
				c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,

				s.isActive as tiIsActive
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where 
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize, s.isActive
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,

				i.isActive as tiIsActive
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive
			order by c.symbol	 
	end

	Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.29')
	begin
		update DataInfo set keyValue = '3.29' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.29'
	end
