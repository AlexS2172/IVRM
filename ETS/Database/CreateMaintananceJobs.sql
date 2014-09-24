set nocount on
set quoted_identifier, ansi_nulls, ansi_warnings, arithabort, concat_null_yields_null, ansi_padding on
set numeric_roundabort off
set transaction isolation level read uncommitted
set xact_abort on

if is_srvrolemember('sysadmin')=0
begin
	raiserror('You must be a member of "sysadmin" fixed server role to perform this operation!',16,5)
	goto QuitWithRollback
end

begin transaction            
	declare @job_id binary(16)  
		,@db_name sysname
		,@ret_code int    
		,@job1_name sysname
		,@job2_name sysname
		,@comment nvarchar(4000)


	select @db_name = db_name()	--'ETS'
		,@ret_code = 0
		,@job1_name = N'DB cleanup ("' + @db_name + N'")'
		,@job2_name = N'Speed up operations with DB ("' + @db_name + N'")'


--	Delete required jobs if exists.

--	Delete first Job
	if (select count(*) from msdb.dbo.syscategories where name = N'Database Maintenance') < 1 
		exec msdb.dbo.sp_add_category @name = N'Database Maintenance'

	-- Delete the job with the same name (if it exists)
	select @job_id = job_id     
	from   msdb.dbo.sysjobs    
	where (name = @job1_name)

	if (@job_id is not null)    
	begin
		-- check if the job is a multi-server job  
		if (exists (select  * 
					from    msdb.dbo.sysjobservers 
					where   (job_id = @job_id) and (server_id <> 0))) 
		begin 
			-- there is, so abort the script 
			set @comment = N'Unable to import job "' + @job1_name + '" since there is already a multi-server job with this name.'
			raiserror (@comment, 16, 1) 
			goto QuitWithRollback  
		end 

		-- Delete the [local] job 
		exec msdb.dbo.sp_delete_job @job_name = @job1_name
		select @job_id = null
	end

--	Delete second job
	set @job_id = null

	select @job_id = job_id     
	from   msdb.dbo.sysjobs    
	where (name = @job2_name)

	if (@job_id is not null)    
	begin  
		-- Check if the job is a multi-server job  
		if (exists (select  * 
					from    msdb.dbo.sysjobservers 
					where   (job_id = @job_id) and (server_id <> 0))) 
		begin 
			-- There is, so abort the script 
			set @comment = N'Unable to import job "' + @job2_name + '" since there is already a multi-server job with this name.'
			raiserror (@comment, 16, 1) 
			goto QuitWithRollback  
		end 

		-- Delete the [local] job 
		exec msdb.dbo.sp_delete_job @job_name = @job2_name

		select @job_id = null
	end 


--	Adding jobs, steps and shedules


--	Adding first job

	select @comment = N'run the stored procedure for DB ("' + @db_name + '") cleanup (delete records with actionID = 3)'
		,@job_id = null
	-- Add the job
	exec @ret_code = msdb.dbo.sp_add_job @job_id = @job_id output, @job_name = @job1_name, @owner_login_name = N'sa', @description = @comment, @category_name = N'Database Maintenance', @enabled = 1, @notify_level_email = 0, @notify_level_page = 0, @notify_level_netsend = 0, @notify_level_eventlog = 2, @delete_level= 0
	if (@@error <> 0 or @ret_code <> 0) goto QuitWithRollback
	
	-- Add the job steps
	exec @ret_code = msdb.dbo.sp_add_jobstep @job_id = @job_id, @step_id = 1, @step_name = N'Fix market structure', @command = N'exec dbo.job_FixMarketStructure', @database_name = @db_name, @server = N'', @database_user_name = N'', @subsystem = N'TSQL', @cmdexec_success_code = 0, @flags = 0, @retry_attempts = 0, @retry_interval = 1, @output_file_name = N'', @on_success_step_id = 0, @on_success_action = 3, @on_fail_step_id = 0, @on_fail_action = 3
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 
	set @comment = N'Cleanup DB ("' + @db_name + '")'
	exec @ret_code = msdb.dbo.sp_add_jobstep @job_id = @job_id, @step_id = 2, @step_name = @comment, @command = N'exec job_Cleanup_DB', @database_name = @db_name, @server = N'', @database_user_name = N'', @subsystem = N'TSQL', @cmdexec_success_code = 0, @flags = 0, @retry_attempts = 0, @retry_interval = 1, @output_file_name = N'', @on_success_step_id = 0, @on_success_action = 1, @on_fail_step_id = 0, @on_fail_action = 2
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 
	exec @ret_code = msdb.dbo.sp_update_job @job_id = @job_id, @start_step_id = 1 
	
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 
	
	-- Add the job schedules
	set @comment = N'DB ("' + @db_name + '") cleanup schedule'
	exec @ret_code = msdb.dbo.sp_add_jobschedule @job_id = @job_id, @name = @comment, @enabled = 1, @freq_type = 32, @active_start_date = 20020702, @active_start_time = 0, @freq_interval = 1, @freq_subday_type = 1, @freq_subday_interval = 0, @freq_relative_interval = 1, @freq_recurrence_factor = 1, @active_end_date = 99991231, @active_end_time = 235959
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 
	
	-- Add the Target Servers
	exec @ret_code = msdb.dbo.sp_add_jobserver @job_id = @job_id, @server_name = N'(local)' 
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 


--	Adding second job

	select @comment = N'Reorganize indices structure and statistics update - For speed up operations with DB ("' + @db_name + '")'
		,@job_id = null
	exec @ret_code = msdb.dbo.sp_add_job @job_id = @job_id output , @job_name = @job2_name, @owner_login_name = N'sa', @description = @comment, @category_name = N'Database Maintenance', @enabled = 1, @notify_level_email = 0, @notify_level_page = 0, @notify_level_netsend = 0, @notify_level_eventlog = 2, @delete_level= 0
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 
	
	-- Add the job steps
	exec @ret_code = msdb.dbo.sp_add_jobstep @job_id = @job_id, @step_id = 1, @step_name = N'execution of stored procedure', @command = N'exec dbo.job_ReorganizeMainTables', @database_name = @db_name, @server = N'', @database_user_name = N'', @subsystem = N'TSQL', @cmdexec_success_code = 0, @flags = 0, @retry_attempts = 0, @retry_interval = 0, @output_file_name = N'', @on_success_step_id = 0, @on_success_action = 1, @on_fail_step_id = 0, @on_fail_action = 2
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 
	exec @ret_code = msdb.dbo.sp_update_job @job_id = @job_id, @start_step_id = 1 
	
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 
	
	-- Add the job schedules
	exec @ret_code = msdb.dbo.sp_add_jobschedule @job_id = @job_id, @name = N'Scheduled reorganization of indices structure and statistics update', @enabled = 1, @freq_type = 8, @active_start_date = 20020614, @active_start_time = 40000, @freq_interval = 1, @freq_subday_type = 1, @freq_subday_interval = 0, @freq_relative_interval = 0, @freq_recurrence_factor = 1, @active_end_date = 99991231, @active_end_time = 235959
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 
	
	-- Add the Target Servers
	exec @ret_code = msdb.dbo.sp_add_jobserver @job_id = @job_id, @server_name = N'(local)' 
	if (@@error <> 0 OR @ret_code <> 0) goto QuitWithRollback 

commit transaction          
goto   EndSave              
QuitWithRollback:
  if (@@trancount > 0) rollback transaction 
EndSave: 
