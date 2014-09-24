/*
PLEASE DO NOT FORGET:
	1. Run this script at the destiation DB!
	2. Keep "SQL SERVER AGENT" running before or after execution of this script!
	(You must markup "SQL SERVER AGENT" as 'Auto-Start Service when OS starts'
	to provide execution of all scheduled jobs at this server)	

What make this script:
	Changes in job 'DB cleanup':
	- was added step, which mark as deleted all expired options before DB cleanup 

DB version 3.34.008
*/
set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FixMarketStructure]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FixMarketStructure]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[job_FixMarketStructure]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[job_FixMarketStructure]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

Create procedure dbo.job_FixMarketStructure
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
		select O.OptionID--,A.MSUpdateDate,O.actionDate
			from [Option] O
				join (select contractID,MSUpdateDate from Contract where MSUpdateDate is not null and actionID < 3) A 
					on O.underlyingContractID = A.contractID 
					and O.actionDate < A.MSUpdateDate 
			where O.actionID < 3 and O.isManualyEntered = 0 and 
					datediff(hour,O.actionDate, A.MSUpdateDate) > @iNumberOfHours and 
					(A.contractID = @iContractID or @iContractID is null)

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
--=========================================================================================================
--=========================================================================================================
-- Script generated on 02.07.2002 13:11
-- By: sa
-- Server: CROCODILE


BEGIN TRANSACTION            
  Declare @JobID BINARY(16)  
  Declare 	@ReturnCode INT,
			@db_name varchar(8000)    

  	set @ReturnCode = 0     
	--here you can change the DB name on a which will be executed this job
	set @db_name = db_name()

if (Select count(*) from msdb.dbo.syscategories where name = N'Database Maintenance') < 1 
  execute msdb.dbo.sp_add_category @name = N'Database Maintenance'

  -- Delete the job with the same name (if it exists)
  Select @JobID = job_id     
  from msdb.dbo.sysjobs    
  where (name = N'DB cleanup')       
  if (@JobID is not null)    
	  begin  
	  -- Check if the job is a multi-server job  
	  if (exists (select * 
	              from msdb.dbo.sysjobservers 
	              where (job_id = @JobID) and (server_id <> 0))) 
		  begin 
		    -- There is, so abort the script 
		    Raiserror (N'Unable to import job ''DB cleanup'' since there is already a multi-server job with this name.', 16, 1) 
		    goto QuitWithRollback  
		  end 
	  else 
	    -- Delete the [local] job 
	    execute msdb.dbo.sp_delete_job @job_name = N'DB cleanup' 
	    select @JobID = null
	  end 

begin 

  -- Add the job
  execute @ReturnCode = msdb.dbo.sp_add_job @job_id = @JobID OUTPUT , 
								@job_name = N'DB cleanup', 
								@owner_login_name = N'sa', 
								@description = N'run the stored procedure for DB cleanup (delete records with actionID = 3)', 
								@category_name = N'Database Maintenance', 
								@enabled = 1, 
								@notify_level_email = 0, 
								@notify_level_page = 0, 
								@notify_level_netsend = 0, 
								@notify_level_eventlog = 2, 
								@delete_level= 0
  if (@@error <> 0 OR @ReturnCode <> 0) goto QuitWithRollback 

  -- Add the job steps
  execute @ReturnCode = msdb.dbo.sp_add_jobstep @job_id = @JobID, 
								@step_id = 1, 
								@step_name = N'Fix market structure', 
								@command = N'exec job_FixMarketStructure', 
								@database_name = @db_name, 
								@server = N'', 
								@database_user_name = N'', 
								@subsystem = N'TSQL', 
								@cmdexec_success_code = 0, 
								@flags = 0, 
								@retry_attempts = 0, 
								@retry_interval = 1, 
								@output_file_name = N'', 
								@on_success_step_id = 0, 
								@on_success_action = 3, 
								@on_fail_step_id = 0, 
								@on_fail_action = 3
  if (@@error <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  execute @ReturnCode = msdb.dbo.sp_add_jobstep @job_id = @JobID, 
								@step_id = 2, 
								@step_name = N'Cleanup DB', 
								@command = N'exec job_Cleanup_DB', 
								@database_name = @db_name, 
								@server = N'', 
								@database_user_name = N'', 
								@subsystem = N'TSQL', 
								@cmdexec_success_code = 0, 
								@flags = 0, 
								@retry_attempts = 0, 
								@retry_interval = 1, 
								@output_file_name = N'', 
								@on_success_step_id = 0, 
								@on_success_action = 1, 
								@on_fail_step_id = 0, 
								@on_fail_action = 2
  if (@@error <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  execute @ReturnCode = msdb.dbo.sp_update_job @job_id = @JobID, @start_step_id = 1 
	  if (@@error <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the job schedules
  execute @ReturnCode = msdb.dbo.sp_add_jobschedule @job_id = @JobID, 
								@name = N'DB cleanup schedule', 
								@enabled = 1, 
								@freq_type = 32, 
								@active_start_time = 0, 
								@freq_interval = 1, 
								@freq_subday_type = 1, 
								@freq_subday_interval = 0, 
								@freq_relative_interval = 1, 
								@freq_recurrence_factor = 1, 
								@active_end_date = 99991231, 
								@active_end_time = 235959
  if (@@error <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the Target Servers
  execute @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @JobID, @server_name = N'(local)' 
  if (@@error <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

end
COMMIT TRANSACTION          
GOTO   EndSave              
QuitWithRollback:
  IF (@@TRANCOUNT > 0) rollback transaction 
EndSave: 
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.34.008')
	begin
		update DataInfo set keyValue = '3.34.008' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.34.008'
	end



