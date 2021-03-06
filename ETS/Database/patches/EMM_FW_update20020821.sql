/*
	patch for MLine00000623
	DB version: 3.37.023
*/
set nocount on
GO
--======================================================================================
-- remove unusefull unique index from ContractExchangeLink table
--======================================================================================
GO
if INDEXPROPERTY (object_id('ContractExchangeLink') , 'IX_ContractExchangeLink','IndexID') is not null
	DROP INDEX ContractExchangeLink.IX_ContractExchangeLink
GO
--============================================================================================================
--change DB version
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.37.023')
	begin
		update DataInfo set keyValue = '3.37.023' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.37.023'
	end