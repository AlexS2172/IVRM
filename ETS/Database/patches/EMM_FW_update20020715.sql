/*
fix for Defect  MLine00000570 ETSMM Submitted

Headline: After runing script for DB cleanup I lost any options

DB version 3.36.015
*/
set nocount on
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
ALTER  procedure dbo.job_FixMarketStructure
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
					join Contract UC on O.underlyingContractID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where E.expiryDate < GetDate() 
					and O.isManualyEntered = 0
					and O.actionID < 3
					and O.optionID not in (select distinct ContractID from Trade)
/*
		select O.OptionID--,A.MSUpdateDate,O.actionDate
			from [Option] O
				join (select contractID,MSUpdateDate from Contract where MSUpdateDate is not null and actionID < 3) A 
					on O.underlyingContractID = A.contractID 
					and O.actionDate < A.MSUpdateDate 
			where O.actionID < 3 and O.isManualyEntered = 0 and 
					datediff(hour,O.actionDate, A.MSUpdateDate) > @iNumberOfHours and 
					(A.contractID = @iContractID or @iContractID is null)
*/

	exec usp_Option_Del
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

    set @error = 0
finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.36.015')
	begin
		update DataInfo set keyValue = '3.36.015' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.36.015'
	end