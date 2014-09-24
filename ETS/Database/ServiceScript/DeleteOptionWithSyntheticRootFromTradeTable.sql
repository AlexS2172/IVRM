-- Get SU Options with 'select *' and delete them with 'delete Trade'
--delete Trade
select *
FROM Trade JOIN OptionRoot 
   ON OptionRoot.OptionrootID = Trade.OptionrootID
WHERE OptionRoot.isSynthetic = 1
