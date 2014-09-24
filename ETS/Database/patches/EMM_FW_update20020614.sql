/*
PLEASE DO NOT FORGET:
	1. Run this script at the destiation DB!
	2. Run the "SQL SERVER AGENT" before or after execution of this script!

This script create 2 jobs:
	1. 'Speed up operations with DB'- occurs every 1 week(s) on Sunday, at 4:00:00
	2. 'DB cleanup' job  		- occurs every 1 month(s) on the 1st Sunday on that month at 0:00:00
and change DB version to 3.32.002

*/
set nocount on
GO
--===========================================================================================================
-- 'Speed up operations with DB' job (execution of job_ReorganizeMainTables)
--===========================================================================================================
Declare @iRetCode int,
	@db_name varchar(8000)

--here you can change the DB name on a whic will be executed job_ReorganizeMainTables SP
set @db_name = db_name()

BEGIN TRANSACTION            
  Declare 	@JobID binary(16),  
		@ReturnCode int

	Set @ReturnCode = 0     

	if (select count(*) from msdb.dbo.syscategories where name = N'Database Maintenance') < 1 
		execute msdb.dbo.sp_add_category @name = N'Database Maintenance'
  -- Delete the job with the same name (if it exists)
  select @JobID = job_id from msdb.dbo.sysjobs where (name = N'Speed up operations with DB')       
  if (@JobID is not null)    
	begin  
		  -- Check if the job is a multi-server job  
		if(exists (select * from msdb.dbo.sysjobservers where (job_id = @JobID) and (server_id <> 0))) 
			begin 
    		-- There is, so abort the script 
		    Raiserror (N'Unable to import job ''Speed up operations with DB'' since there is already a multi-server job with this name.', 16, 1) 
		    goto QuitWithRollback  
	end 
  else 
    -- Delete the [local] job 
    execute msdb.dbo.sp_delete_job @job_name = N'Speed up operations with DB' 
    select @JobID = NULL
  end 

begin 
  -- Add the job 
		execute @ReturnCode = msdb.dbo.sp_add_job 
			@job_id = @JobID OUTPUT , 
			@job_name = N'Speed up operations with DB', 
			@owner_login_name = N'sa', 
			@description = N'Reorganize indices structure and statistics update - For speed up operations with DB', 
			@category_name = N'Database Maintenance', 
			@enabled = 1, 
			@notify_level_email = 0, 
			@notify_level_page = 0, 
			@notify_level_netsend = 0, 
			@notify_level_eventlog = 2, 
			@delete_level= 0
  if (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
  -- Add the job steps 
		execute @ReturnCode = msdb.dbo.sp_add_jobstep 
			@job_id = @JobID, 
			@step_id = 1, 
			@step_name = N'execution of stored procedure', 
			@command = N'exec job_ReorganizeMainTables', 
			@database_name = @db_name, 
			@server = N'', 
			@database_user_name = N'', 
			@subsystem = N'TSQL', 
			@cmdexec_success_code = 0, 
			@flags = 0, 
			@retry_attempts = 0, 
			@retry_interval = 0, 
			@output_file_name = N'', 
			@on_success_step_id = 0, 
			@on_success_action = 1, 
			@on_fail_step_id = 0, 
			@on_fail_action = 2

  if (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  execute @ReturnCode = msdb.dbo.sp_update_job @job_id = @JobID, @start_step_id = 1 
  if (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 
  -- Add the job schedules 
		execute @ReturnCode = msdb.dbo.sp_add_jobschedule 
			@job_id = @JobID, 
			@name = N'Scheduled reorganization of indices structure and statistics update', 
			@enabled = 1, 
			@freq_type = 8, 
			@active_start_time = 40000,
			@freq_interval = 1, 
			@freq_subday_type = 1, 
			@freq_subday_interval = 0, 
			@freq_relative_interval = 0, 
			@freq_recurrence_factor = 1, 
			@active_end_date = 99991231, 
			@active_end_time = 235959

  if (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 
  -- Add the Target Servers
  execute @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @JobID, @server_name = N'(local)' 
  if (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

END
COMMIT TRANSACTION          
GOTO   EndSave              
QuitWithRollback:
  IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION 
EndSave: 
GO
--===========================================================================================================
-- 'DB cleanup' job (execution of job_Cleanup_DB)
--===========================================================================================================
Declare @iRetCode int,
		@db_name varchar(8000)

--here you can change the DB name on a which will be executed 
set @db_name = db_name()

BEGIN TRANSACTION            
  Declare 	@JobID binary(16),  
  			@ReturnCode int

  Set @ReturnCode = 0     

if (select count(*) from msdb.dbo.syscategories where name = N'[Uncategorized (Local)]') < 1 
  execute msdb.dbo.sp_add_category @name = N'[Uncategorized (Local)]'
  -- Delete the job with the same name (if it exists)
  Select @JobID = job_id from   msdb.dbo.sysjobs where (name = N'DB cleanup')       

  if (@JobID is not null)    
	  begin  
	  -- Check if the job is a multi-server job  
	  if exists (select * from msdb.dbo.sysjobservers where job_id = @JobID and server_id <> 0) 
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
		execute @ReturnCode = msdb.dbo.sp_add_job 
			@job_id = @JobID output, 
			@job_name = N'DB cleanup', 
			@owner_login_name = N'sa', 
			@description = N'run the stored procedure for DB cleanup (delete records with actionID = 3)', 
			@category_name = N'[Uncategorized (Local)]', 
			@enabled = 1, 
			@notify_level_email = 0, 
			@notify_level_page = 0, 
			@notify_level_netsend = 0, 
			@notify_level_eventlog = 2, 
			@delete_level= 0
	if (@@error <> 0 or @ReturnCode <> 0) goto QuitWithRollback
  -- Add the job steps	
		execute @ReturnCode = msdb.dbo.sp_add_jobstep 
			@job_id = @JobID, 
			@step_id = 1, 
			@step_name = N'SP execution', 
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
	if (@@error <> 0 or @ReturnCode <> 0) goto QuitWithRollback 

	execute @ReturnCode = msdb.dbo.sp_update_job @job_id = @JobID, @start_step_id = 1 

	if (@@error <> 0 or @ReturnCode <> 0) goto QuitWithRollback 
  -- Add the job schedules
		execute @ReturnCode = msdb.dbo.sp_add_jobschedule 
			@job_id = @JobID, 
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
	if (@@error <> 0 or @ReturnCode <> 0) goto QuitWithRollback
  -- Add the Target Servers
		execute @ReturnCode = msdb.dbo.sp_add_jobserver 
			@job_id = @JobID, 
			@server_name = N'(local)' 
	if (@@error <> 0 or @ReturnCode <> 0) goto QuitWithRollback 
end
COMMIT TRANSACTION          
GOTO   EndSave              
QuitWithRollback:
  IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION 
EndSave:
 
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.32.002')
	begin
		update DataInfo set keyValue = '3.32.002' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.32.002'
	end



