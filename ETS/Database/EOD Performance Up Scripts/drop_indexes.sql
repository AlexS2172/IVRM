-- delete indexes (-10% update speed) 
drop index contract.IX_Contract_symbol_contractID_contractTypeID_actionID
drop index contract.IX_Contract_symbol_contractTypeID_actionID
drop index CustomStrikeSkewPoint.actionID_volaSurfaceDataID_ind
drop index CustomStrikeSkewPoint.IX_CustomStrikeSkewPoint
drop index CustomStrikeSkewPoint.IX_CustomStrikeSkewPoint_9
drop index CustomStrikeSkewPoint.CustomStrikeSkewPoint3
drop index [option].I_IsCall

-- create indexes
create index IX_IndexBeta_IndexID on IndexBeta(IndexID)
create index IX_IndexBeta_ContractID on IndexBeta(ContractID)
create index IX_ContractExchangeLink_ContractID on ContractExchangeLink(ContractID)
