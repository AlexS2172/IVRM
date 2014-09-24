-- =============================================
-- Create tables Current_PeQuote_SnapShot, Last_PeQuote_SnapShot
-- =============================================
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'Current_PeQuote_SnapShot' 
	   AND	  type = 'U')
    DROP TABLE Current_PeQuote_SnapShot
GO

CREATE TABLE Current_PeQuote_SnapShot (
	execID			varchar(20)	NOT NULL, 
	trader_acronym		varchar(12)	NOT NULL, 
	isBuy			tinyint		NOT NULL, 
	quantity		int		NOT NULL, 
	price			float		NOT NULL, 
	trade_date		datetime	NOT NULL, 
	contract_symbol		varchar(20)	NOT NULL, 
	contract_type		int		NOT NULL, 
	isCall			tinyint		NOT NULL, 
	strike			float		NOT NULL, 
	expiry_date		datetime	NOT NULL, 
	opt_root_symbol		varchar(20)	NOT NULL,
	isNewTrade		tinyint		NOT NULL
 
PRIMARY KEY (execID) )
GO

------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'Last_PeQuote_SnapShot' 
	   AND	  type = 'U')
    DROP TABLE Last_PeQuote_SnapShot
GO

CREATE TABLE Last_PeQuote_SnapShot (
	execID			varchar(20)	NOT NULL, 
	trader_acronym		varchar(12)	NOT NULL, 
	isBuy			tinyint		NOT NULL, 
	quantity		int		NOT NULL, 
	price			float		NOT NULL, 
	trade_date		datetime	NOT NULL, 
	contract_symbol		varchar(20)	NOT NULL, 
	contract_type		int		NOT NULL, 
	isCall			tinyint		NOT NULL, 
	strike			float		NOT NULL, 
	expiry_date		datetime	NOT NULL, 
	opt_root_symbol		varchar(20)	NOT NULL,
	isNewTrade		tinyint		NOT NULL
 
PRIMARY KEY (execID) )
GO