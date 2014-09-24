/*
fix for MLine00001086
*/

/*PREPARATIONS FOR ERROR HANDLING*/
SET NOCOUNT ON
GO
IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#tmpErrors')) DROP TABLE #tmpErrors
GO
CREATE TABLE #tmpErrors (Error int)
GO
SET XACT_ABORT ON
GO
/*SCRIPTS MAIN SECTION*/
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.39.056' or (keyValue = '3.39.057')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.39.057 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.39.057' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
ALTER                 PROCEDURE dbo.usp_LogCA_Rollback
	@iCorpActionID int,
	@isDebugMode bit = 0 
AS
/*
	Created by: Zaur Nuraliev
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------	------------------------
	declare @error int		set @error = 0
	declare @rowcount int
	declare @error_descr varchar(1000)	set @error_descr = ''

	declare @start_time datetime set @start_time = GetDate()
	declare @iStop bit set @iStop = 0

	declare @iRowID int,
			@vcTableName varchar(50),
			@vcFieldName varchar(50),
			@vcFieldValue varchar(250),
			@vcFieldType varchar(50),
			@tiActionID tinyint,
			@iCorpActionLogID int,
			@vcKeyRow varchar(50),
			@dtActionDate datetime 

	declare @vcString1 varchar(8000),
			@vcString2 varchar(8000),
			@vcString3 varchar(8000)

	declare @iPreviousRowID int,
			@iInsertedColumnsCount int, 
			@iLoopCounter int,
			@bisMarked_as_delete bit

	select @vcString1 ='',@vcString2 ='',@vcString3 =''


	if object_ID('tempdb..#result') is not null drop table #result
		create table #result(stopID int)

	Begin Transaction LogCA_Rollback
		DECLARE log_reader CURSOR
		READ_ONLY
		FOR select rowID, tableName, fieldName, fieldValue, fieldType, actionID,corpActionLogID,keyRow,actionDate
				from CorporateActionLog
				where corpActionID = @iCorpActionID
			order by corpActionLogID desc
		Open log_reader
			fetch next from log_reader into @iRowID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType, @tiActionID, @iCorpActionLogID,@vcKeyRow,@dtActionDate
		while (@@fetch_status <> -1)
		begin
			if (@@fetch_status <> -2)
			BEGIN

			--correct null value in @vcFieldValue
				if @vcFieldValue is null set @vcFieldValue = 'null' else set @vcFieldValue = '''' +  @vcFieldValue + ''''

			-- read from CorporateActionLog
				if @tiActionID = 1 
					begin
					if @vcTableName = 'Trade'
						begin
							declare @iTradeID int
							select @iTradeID=tradeID from Trade where seqNum=convert(int,@iRowID)
								if @@error <> 0 begin set @error = -1 BREAK end 
							if @iTradeID is null
							begin
								set @error_descr = 'Record from Trade table with seqNum=' + convert(varchar(25),@iRowID) +
								' was deleted after Corporate action'

								set @error = - 1 BREAK 
							end
							delete from Trade where seqNum=convert(int,@iRowID)
							select @error = @@error, @rowcount = @@rowcount

							if @error <> 0 BREAK
							
							if @rowcount=0
							begin
								set @error_descr = 'Record from Trade table with seqNum=' + convert(varchar(25),@iRowID) +
								' was deleted after Corporate action'

								set @error = - 1 BREAK 
							end
							if exists(select * from Trade where tradeID=@iTradeID and actionID=0)
							begin
								set @error_descr = 'Record from Trade table with tradeID=' + convert(varchar(25),@iTradeID) +
								' was deleted after Corporate action'

								set @error = - 1 BREAK 
							end
						end 
					else
						begin
						--perform rollback of insert
							set @vcString1 = "Update " + @vcTableName + ' set actionID=3 where ' +  @vcKeyRow + '=' + convert(varchar(25),@iRowID)
							if @isDebugMode =1
								begin
									select @vcString1 as 'String for delete'
								end
							else
								begin
									exec(@vcString1)
										if @@error <> 0 begin set @error = -1 BREAK end 
								end							
						end
					end
				else if @tiActionID = 2
					begin
					--check ability of update operation
						set @vcString1 = 'if (select datediff(ss,' + '''' + convert(varchar(19),@dtActionDate,120) + '''' +
											',convert(varchar(19),actionDate,120)) from ' + @vcTableName +  
											' where ' +  @vcKeyRow + '=' + convert(varchar(25),@iRowID) + ')>0 and ' + 
											'(select datediff(ss,convert(varchar(19),actionDate,120),' + 
											'''' + convert(varchar(19),@start_time,120) + '''' +
											') from ' + @vcTableName + ' where ' +  @vcKeyRow + '=' + convert(varchar(25),@iRowID) + ')>0' +
											' begin insert into #result select -1 end'				

						--select @vcString1 as 'string for check ability of update operation'
						exec(@vcString1)
						if exists(select * from #result) 
						begin 
							set @error_descr = 'Record from ' + @vcTableName + ' table with ' +  @vcKeyRow + '=' + convert(varchar(25),@iRowID) +
							' was updated after Corporate action'

							set @error = - 1 BREAK 
						end 

					--perform rollback of update
						set @vcString1 = 'Update ' + @vcTableName + ' set ' + @vcFieldName + '=' + 
										'convert(' + @vcFieldType + ',' + @vcFieldValue + ')' +
										' where ' + @vcKeyRow + '=' + convert(varchar(25),@iRowID)
						if @isDebugMode =1
							begin
								select @vcString1 as 'String for update'
							end
						else
							begin
								exec(@vcString1)
									if @@error != 0 begin set @error = -1 BREAK end 
							end
					end
				else if @tiActionID = 3
					begin
					--perform rollback of delete

					--check ability of insert operation
						set @vcString1 = 'if (select datediff(ss,' + '''' + convert(varchar(19),@dtActionDate,120) + '''' +
											',convert(varchar(19),actionDate,120)) from ' + @vcTableName +  
											' where ' +  @vcKeyRow + '=' + convert(varchar(25),@iRowID) + ')>0 and ' + 
											'(select datediff(ss,convert(varchar(19),actionDate,120),' + 
											'''' + convert(varchar(19),@start_time,120) + '''' +
											') from ' + @vcTableName + ' where ' +  @vcKeyRow + '=' + convert(varchar(25),@iRowID) + ')>0' +
											' begin insert into #result select -1 end'

						exec(@vcString1)
						if exists(select * from #result)
						begin 
							set @error_descr = 'Record from ' + @vcTableName + ' table with ' +  @vcKeyRow + '=' + convert(varchar(25),@iRowID) +
							' was updated after Corporate action'

							set @error = - 1 BREAK 
						end 

						--if row has been deleted
						select @iInsertedColumnsCount = count(*) 
							from CorporateActionLog where corpActionID = @iCorpActionID 
										and tableName = @vcTableName
										and rowID = @iRowID

						set @iLoopCounter = isnull(@iLoopCounter,0) + 1

--						select @vcString2 = @vcString2 + @vcFieldName + case when @iInsertedColumnsCount <> @iLoopCounter then ',' else '' end
--						select @vcString3 = @vcString3 + 'convert(' + @vcFieldType + ',' + @vcFieldValue + ')' + case when @iInsertedColumnsCount <> @iLoopCounter then ',' else '' end
						if @vcFieldName != @vcKeyRow
						begin
							select @vcString2 = @vcString2 + @vcFieldName + case when @iInsertedColumnsCount - 1 > @iLoopCounter then ',' else '' end
							select @vcString3 = @vcString3 + 'convert(' + @vcFieldType + ',' + @vcFieldValue + ')' + case when @iInsertedColumnsCount - 1 > @iLoopCounter  then ',' else '' end
						end

						if @iInsertedColumnsCount = @iLoopCounter
							begin --1
								set @iLoopCounter = 0

								--perform insert if row has been deleted
								set @vcString1 = 'if not exists(select * from ' + @vcTableName + 
													' where ' + @vcKeyRow + '=' + convert(varchar(25),@iRowID) + ') begin' +
													' insert into ' + @vcTableName + '(' +  @vcString2 + ') select ' + @vcString3 + ' end'
								if @isDebugMode =1
									begin
										select @vcString1 as 'string for insert'
									end
								else 
									begin
										execute(@vcString1)
											if @@error != 0 begin set @error = -1 BREAK end 
									end

								--if row has been marked as deleted (have actionID = 3)
								set @vcString1 = 'if exists(select * from ' + @vcTableName + 
													' where ' + @vcKeyRow + '=' + convert(varchar(25),@iRowID) + ')' +
													' begin ' + 'ALTER TABLE ' + @vcTableName + ' DISABLE TRIGGER tU_' + @vcTableName +
													' update '  + @vcTableName + ' set actionID=2, actionDate=GetDate() where ' + @vcKeyRow + '=' + convert(varchar(25),@iRowID) +
													' ALTER TABLE ' + @vcTableName + ' ENABLE TRIGGER tU_' + @vcTableName +
													' end'
								if @isDebugMode =1
									begin	
										select @vcString1 as 'string for update'
									end
								else
									begin
										execute(@vcString1)
											if @@error != 0 begin set @error = -1 BREAK end 
									end
							select @vcString2 = '',@vcString3 = ''
							end --1
					end

			END
			fetch next from log_reader into @iRowID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType, @tiActionID, @iCorpActionLogID,@vcKeyRow,@dtActionDate
		end
		Close log_reader
		Deallocate log_reader

		if @isDebugMode = 0 and @error != -1
			begin
				delete from CorporateActionLog where corpActionID = @iCorpActionID
					if @@error != 0 begin set @error = -1 end
				delete CorporateAction where corpActionID = @iCorpActionID
					if @@error != 0 begin set @error = -1 end
			end

		if @error = -1
			begin
				if @@TranCount > 0 begin Rollback Transaction LogCA_Rollback goto finish end
			end

	Commit Transaction LogCA_Rollback


    set @error = 0
finish:
	if @@error = 0 and @error !=0 
		begin	
			Raiserror('Rollback of Corporate Action could not be performed! %s',16,1,@error_descr)
		end 
	Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.39.057')
	begin
		update DataInfo set keyValue = '3.39.057' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
IF EXISTS (SELECT * FROM #tmpErrors)
BEGIN
	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
END
GO
IF @@TRANCOUNT > 0 
BEGIN
	select 'DB VERSION WAS CHANGED TO 3.39.057' '3.39.057 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO