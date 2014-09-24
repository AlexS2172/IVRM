/******
replace following to generate JOB
#IVRM_JOB_NAME#  --- sample IVRM(1,0) EOD->ETS_ASP_EOD
#IVRM_DATABASE#  --- sample ETS_ASP_CFI
#NOTIFICATION_EMAIL# --- sample IVTRUBLE@EGARTECH.COM
******/

USE [msdb]
GO

/****** Object:  Job [#IVRM_JOB_NAME# ]    Script Date:  ******/
BEGIN TRANSACTION
DECLARE @ReturnCode INT
SELECT @ReturnCode = 0
/****** Object:  JobCategory [[Uncategorized (Local)]]]    Script Date: 10/07/2009 10:18:20 ******/
IF NOT EXISTS (SELECT name FROM msdb.dbo.syscategories WHERE name=N'[Uncategorized (Local)]' AND category_class=1)
BEGIN
EXEC @ReturnCode = msdb.dbo.sp_add_category @class=N'JOB', @type=N'LOCAL', @name=N'[Uncategorized (Local)]'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback

END

DECLARE @jobId BINARY(16)
EXEC @ReturnCode =  msdb.dbo.sp_add_job @job_name=N'#IVRM_JOB_NAME#',
        @enabled=1,
        @notify_level_eventlog=0,
        @notify_level_email=0,
        @notify_level_netsend=0,
        @notify_level_page=0,
        @delete_level=0,
        @description=N'No description available.',
        @category_name=N'[Uncategorized (Local)]',
        @owner_login_name=N'sa', @job_id = @jobId OUTPUT
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback

/****** Object:  Step [Update From Eod]    Script Date: 10/07/2009 10:18:20 ******/
EXEC @ReturnCode = msdb.dbo.sp_add_jobstep @job_id=@jobId, @step_name=N'Update From Eod',
        @step_id=1,
        @cmdexec_success_code=0,
        @on_success_action=3,
        @on_success_step_id=0,
        @on_fail_action=4,
        @on_fail_step_id=3,
        @retry_attempts=0,
        @retry_interval=0,
        @os_run_priority=0, @subsystem=N'TSQL',
        @command=N'declare @error int

exec @error = dbo.usp_AspEodUpdateMarketStructure_Execute

if @error <> 0 begin
    RAISERROR (''Error while executing main step'', 16, 1)
end
',
        @database_name=N'#IVRM_DATABASE#',
        @flags=0
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback

/****** Object:  Step [Send Mail On Success]    Script Date: 10/07/2009 10:18:20 ******/
EXEC @ReturnCode = msdb.dbo.sp_add_jobstep @job_id=@jobId, @step_name=N'Send Mail On Success',
        @step_id=2,
        @cmdexec_success_code=0,
        @on_success_action=1,
        @on_success_step_id=0,
        @on_fail_action=3,
        @on_fail_step_id=0,
        @retry_attempts=0,
        @retry_interval=0,
        @os_run_priority=0, @subsystem=N'TSQL',
        @command=N'master.dbo.sp_cdonts_sendmail
''#NOTIFICATION_EMAIL#'',
''IVRM EOD JOB COMPLETE SUCCESS ON #IVRM_DATABASE#'',
''.''',
        @database_name=N'#IVRM_DATABASE#',
        @flags=0
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback

/****** Object:  Step [Send Mail on Failure]    Script Date: 10/07/2009 10:18:20 ******/
EXEC @ReturnCode = msdb.dbo.sp_add_jobstep @job_id=@jobId, @step_name=N'Send Mail On Failure',
        @step_id=3,
        @cmdexec_success_code=0,
        @on_success_action=2,
        @on_success_step_id=0,
        @on_fail_action=2,
        @on_fail_step_id=0,
        @retry_attempts=0,
        @retry_interval=0,
        @os_run_priority=0, @subsystem=N'TSQL',
        @command=N'master.dbo.sp_cdonts_sendmail
''#NOTIFICATION_EMAIL#'',
''IVRM EOD JOB FAIL ON #IVRM_DATABASE#'',
''Error occured while updating database.'',
1,
 ''select event from #IVRM_DATABASE#.dbo.AspEodMsUpdateLog where datediff(day, eventDate, GETDATE()) = 0''',
        @database_name=N'#IVRM_DATABASE#',
        @flags=0
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_update_job @job_id = @jobId, @start_step_id = 1
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @jobId, @server_name = N'(local)'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
COMMIT TRANSACTION
GOTO EndSave
QuitWithRollback:
    IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION
EndSave: