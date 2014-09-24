-- fix for "Can not delete record from RMScenario table"
set nocount on
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
ALTER   Procedure dbo.usp_RMScenario_Del
	@iRMScenarioID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	update RMScenario set actionID=3 where RMScenarioID = @iRMScenarioID
	--delete RMScenario Where RMScenarioID = @iRMScenarioID

	set @error = @@error
        Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--============================================================================================================
--change DB version
--============================================================================================================
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.22')
	begin
		update DataInfo set keyValue = '3.22' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.22'
	end

