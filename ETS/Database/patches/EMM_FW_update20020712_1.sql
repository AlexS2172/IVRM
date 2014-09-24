/*
PLEASE DO NOT FORGET:
	1. Run this script at the destiation DB!
	2. Keep "SQL SERVER AGENT" running before or after execution of this script!
	(You must markup "SQL SERVER AGENT" as 'Auto-Start Service when OS starts'
	to provide execution of all scheduled jobs at this server)	

What make this script:
	this script recreate 2 DB maintenance jobs synchronous

DB version 3.36.014
*/


-- Script generated on 12.07.2002 19:42
BEGIN TRANSACTION            
  	DECLARE @JobID BINARY(16)
	DECLARE @DBNAME varchar(255)  
		SELECT @DBNAME = db_name()
	DECLARE @ReturnCode INT    
		SELECT @ReturnCode = 0     
IF (SELECT COUNT(*) FROM msdb.dbo.syscategories WHERE name = N'Database Maintenance') < 1 
  EXECUTE msdb.dbo.sp_add_category @name = N'Database Maintenance'

  -- Delete the job with the same name (if it exists)
  SELECT @JobID = job_id     
  FROM   msdb.dbo.sysjobs    
  WHERE (name = N'DB cleanup')       
  IF (@JobID IS NOT NULL)    
  BEGIN  
  -- Check if the job is a multi-server job  
  IF (EXISTS (SELECT  * 
              FROM    msdb.dbo.sysjobservers 
              WHERE   (job_id = @JobID) AND (server_id <> 0))) 
  BEGIN 
    -- There is, so abort the script 
    RAISERROR (N'Unable to import job ''DB cleanup'' since there is already a multi-server job with this name.', 16, 1) 
    GOTO QuitWithRollback  
  END 
  ELSE 
    -- Delete the [local] job 
    EXECUTE msdb.dbo.sp_delete_job @job_name = N'DB cleanup' 
    SELECT @JobID = NULL
  END 

BEGIN 

  -- Add the job
  EXECUTE @ReturnCode = msdb.dbo.sp_add_job @job_id = @JobID OUTPUT ,
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
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the job steps
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobstep @job_id = @JobID, 
											@step_id = 1, 
											@step_name = N'Fix market structure', 
											@command = N'exec job_FixMarketStructure', 
											@database_name = @DBNAME, 
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
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobstep @job_id = @JobID, 
											@step_id = 2, 
											@step_name = N'Cleanup DB', 
											@command = N'exec job_Cleanup_DB', 
											@database_name = @DBNAME, 
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
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 
  EXECUTE @ReturnCode = msdb.dbo.sp_update_job @job_id = @JobID, @start_step_id = 1 

  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the job schedules
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobschedule @job_id = @JobID, 
											@name = N'DB cleanup schedule', 
											@enabled = 1, 
											@freq_type = 32, 
											@active_start_date = 20020702, 
											@active_start_time = 0, 
											@freq_interval = 1, 
											@freq_subday_type = 1, 
											@freq_subday_interval = 0, 
											@freq_relative_interval = 1, 
											@freq_recurrence_factor = 1, 
											@active_end_date = 99991231, 
											@active_end_time = 235959
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the Target Servers
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @JobID, @server_name = N'(local)' 
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

Print 'job ''DB cleanup'' has been recreated for the ' + upper(@DBNAME) + ' database'

END
--==================================================================================================
SELECT @JobID = null,@ReturnCode = 0

IF (SELECT COUNT(*) FROM msdb.dbo.syscategories WHERE name = N'Database Maintenance') < 1 
  EXECUTE msdb.dbo.sp_add_category @name = N'Database Maintenance'

  -- Delete the job with the same name (if it exists)
  SELECT @JobID = job_id     
  FROM   msdb.dbo.sysjobs    
  WHERE (name = N'Speed up operations with DB')       
  IF (@JobID IS NOT NULL)    
  BEGIN  
  -- Check if the job is a multi-server job  
  IF (EXISTS (SELECT  * 
              FROM    msdb.dbo.sysjobservers 
              WHERE   (job_id = @JobID) AND (server_id <> 0))) 
  BEGIN 
    -- There is, so abort the script 
    RAISERROR (N'Unable to import job ''Speed up operations with DB'' since there is already a multi-server job with this name.', 16, 1) 
    GOTO QuitWithRollback  
  END 
  ELSE 
    -- Delete the [local] job 
    EXECUTE msdb.dbo.sp_delete_job @job_name = N'Speed up operations with DB' 
    SELECT @JobID = NULL
  END 

BEGIN 

  -- Add the job
  EXECUTE @ReturnCode = msdb.dbo.sp_add_job @job_id = @JobID OUTPUT , 
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
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the job steps
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobstep @job_id = @JobID, 
								@step_id = 1, 
								@step_name = N'execution of stored procedure', 
								@command = N'exec job_ReorganizeMainTables', 
								@database_name = @DBNAME, 
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
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 
  EXECUTE @ReturnCode = msdb.dbo.sp_update_job @job_id = @JobID, @start_step_id = 1 

  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the job schedules
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobschedule @job_id = @JobID, 
								@name = N'Scheduled reorganization of indices structure and statistics update', 
								@enabled = 1, 
								@freq_type = 8, 
								@active_start_date = 20020614, 
								@active_start_time = 40000, 
								@freq_interval = 1, 
								@freq_subday_type = 1, 
								@freq_subday_interval = 0, 
								@freq_relative_interval = 0, 
								@freq_recurrence_factor = 1, 
								@active_end_date = 99991231, 
								@active_end_time = 235959
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

  -- Add the Target Servers
  EXECUTE @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @JobID, @server_name = N'(local)' 
  IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback 

Print 'job ''Speed up operations with DB'' has been recreated for the ' + upper(@DBNAME) + ' database'
END
COMMIT TRANSACTION          
GOTO   EndSave              
QuitWithRollback:
  IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION 
EndSave: 
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.36.014')
	begin
		update DataInfo set keyValue = '3.36.014' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.36.014'
	end






