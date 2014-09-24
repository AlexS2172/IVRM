/*
	this script adds necessaries stocks.
	this script work correctly only at 3.06 version (or higher) of DB 
*/

--===================================================================================================
--insert new stocks
--===================================================================================================
set nocount on

Declare @vcSymbol varchar(8)

set @vcSymbol = 'ACF'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ACXM'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ADBE'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ADLR'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ADRX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='AEM'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='AEP'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='AFCI'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ALLY'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ALO'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='AMGN'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='AW'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='BEAV'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='BMET'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CA'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CCUR'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CHTR'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CLB'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CMNT'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CMVT'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='COCO'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='COHU'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CPN'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CREE'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CSGS'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='CTV'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='DCN'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='DOX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='EDSN'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ELNK'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='EMLX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ESRX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='FCX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='FNF'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='FPL'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='GE'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='GENZ'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='GETY'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='GNSS'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='GPS'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='HC'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='HDWR'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='HGSI'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='HLYW'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='HRB'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='HTCH'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='IMCL'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='INFA'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='INTU'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ISSX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ITT'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='JCP'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='JNPR'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='MU'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='NETE'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='OPMR'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PATH'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PAYX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PCP'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PCS'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PCSA'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PFE'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PKI'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PPD'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PRX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PSTI'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='PZL'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='Q'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='RFMD'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='RIG'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='ROIL'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='SBL'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='SBYN'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='SEAC'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='SEPR'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='SPOT'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='TRMS'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='TTN'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='TZIX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='UAL'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='UNM'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='UOPX'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='VARI'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='VAST'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='VGR'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='VPHM'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='VRTY'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='WIND'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='WMB'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='WWCA'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='XTO'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol = 'SLB'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='SII'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='TDW'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='BJS'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
GO
--===================================================================================================
--update expCalendarID of all newly added  stocks in DB
--===================================================================================================
update Contract set expCalendarID = 1 where  contractTypeID = 2 
GO
--===================================================================================================
--update statistics of all DB tables 
--===================================================================================================
exec sp_updateStats



