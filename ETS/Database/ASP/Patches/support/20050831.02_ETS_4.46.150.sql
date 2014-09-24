SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.149'  or (keyValue = '3.46.150') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.150 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.150' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='IRPoint' and COLUMN_NAME='neutralRate')
begin
	ALTER TABLE IRPoint ADD neutralRate float NULL
end
GO
--if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='IRPoint' and COLUMN_NAME='neutralRate')
--begin
	update IRPoint set neutralRate = (shortRate + longRate)/2
	ALTER TABLE IRPoint ALTER COLUMN neutralRate float NOT NULL
	
--end
GO
ALTER View IRPointView
as
	Select 
		IRpointID,
		curveID,
		periodTypeID,
		num,
		shortRate,
		HTBRate,
		longRate,
		actionID,
		actionDate,
		neutralRate
	from IRPoint
	where actionID < 3
GO

ALTER  PROC dbo.usp_IRPoint_Get
	@iCurveID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	if @iCurveID is not null
		select
			IRpointID as iIRpointID, 
			curveID as iCurveID, 
			periodTypeID as iPeriodTypeID, 
			num as iNum, 
			shortRate as fShortRate, 
			HTBRate as fHTBRate, 
			longRate as fLongRate,
			actionDate as dtActionDate,
			actionID as tiActionID,
			neutralRate as neutralRate
		from
			IRPointView
		where
			curveID = @iCurveID
		order by periodTypeID, num 
	else
		select
			IRpointID as iIRpointID, 
			curveID as iCurveID, 
			periodTypeID as iPeriodTypeID, 
			num as iNum, 
			shortRate as fShortRate, 
			HTBRate as fHTBRate, 
			longRate as fLongRate,
			actionDate as dtActionDate,
			actionID as tiActionID,
			neutralRate as neutralRate
		from
			IRPointView
		order by periodTypeID, num 

	set @error = @@error
Return(@error)
GO
ALTER   Procedure dbo.usp_IRPoint_Save
	@iIRpointID int = Null,
	@iCurveID int = Null,
	@iPeriodTypeID int = Null,
	@iNum int = Null,
	@fShortRate float = Null,
	@fHTBRate float = Null,
	@fLongRate float = Null,
	@fNeutralRate float = Null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from IRPoint where Num = @iNum and  PeriodTypeID=@iPeriodTypeID and actionID < 3 and (@iIRpointID <> IRpointID or @iIRpointID is null))
		begin
			Raiserror ('IRPoint with this period already exists!', 16, 1)
			Return (-1)
		end   
	if @iCurveID is not Null and @iIRpointID is Null
		begin
			insert into IRPoint
					(
					curveID,
					periodTypeID,
					num,
					shortRate,
					HTBRate,
					longRate,
					neutralRate
					) 
				values (
					@iCurveID,
					@iPeriodTypeID,
					@iNum,
					@fShortRate,
					@fHTBRate,
					@fLongRate,
					isNull(@fNeutralRate, (@fShortRate+@fLongRate)/2)
										)
			select @iIRpointID = @@identity
		end
	else if @iIRpointID is not Null
		begin
			update IRPoint set
				curveID = isNull(@iCurveID, curveID),
				periodTypeID = isNull(@iPeriodTypeID, periodTypeID),
				num = isNull(@iNum, num),
				shortRate = isNull(@fShortRate, shortRate),
				HTBRate = isNull(@fHTBRate, HTBRate),
				longRate = isNull(@fLongRate, longRate),
				neutralRate = isNull(@fNeutralRate, neutralRate)
			where IRpointID = @iIRpointID
		end
	set @error = @@error
	if (@error = 0) set @error = @iCurveID
Return(@error)
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoint_Update]
GO

CREATE PROC dbo.usp_IRPoint_Update
	@iPeriodTypeID int = Null,
	@iNum int = Null,
	@fShortRate float = Null,
	@fLongRate float = Null,
	@fNeutralRate float = Null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from IRPoint where Num = @iNum and  PeriodTypeID=@iPeriodTypeID )
	begin
		update IRPoint set
			actionID = 1,
			shortRate = isNull(@fShortRate, shortRate),
			longRate = isNull(@fLongRate, longRate),
			neutralRate = isNull(@fNeutralRate, neutralRate)
		where @iNum = num AND @iPeriodTypeID = periodTypeID
	end	
 	else
		begin
			declare @curveID  int
	
			DECLARE curs1 CURSOR FOR
			select curveID
			from IRCurve
			
			OPEN curs1
			FETCH NEXT FROM curs1 
			INTO @curveID
			
			WHILE @@FETCH_STATUS = 0
			BEGIN
				exec dbo.usp_IRPoint_Save null,@curveID,@iPeriodTypeID,@iNum, @fShortRate,0,@fLongRate,@fNeutralRate
				FETCH NEXT FROM curs1 INTO @curveID
			END
		
			CLOSE curs1
			DEALLOCATE curs1

		end
	set @error = @@error
Return(@error)

GO

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoints_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoints_Import]
GO

CREATE  PROC dbo.usp_IRPoints_Import
	@txXmlUpdateData as ntext
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		
	Declare @idoc int
	--Create an internal representation of the XML document.
	Exec sp_xml_preparedocument @idoc OUTPUT, @txXmlUpdateData
	
	-- SELECT statement using OPENXML rowset provider

	update IRPoint set actionID=3

	declare @Num  int,
		@NeutralRate float,
		@ShortRate float,
		@LongRate float

	DECLARE curs CURSOR FOR
	SELECT Num, NeutralRate, ShortRate, LongRate
	FROM       OPENXML (@idoc, '/IRCurve/IRPoint')
			WITH (Num  int,
				NeutralRate float,
				ShortRate float,
				LongRate float)
	
	OPEN curs
	FETCH NEXT FROM curs 
	INTO @Num,
		@NeutralRate,
		@ShortRate,
		@LongRate
	WHILE @@FETCH_STATUS = 0
	BEGIN
		exec dbo.usp_IRPoint_Update 1, @Num, @ShortRate, @LongRate, @NeutralRate 
		FETCH NEXT FROM curs 
		INTO @Num,
			@NeutralRate,
			@ShortRate,
			@LongRate
	END
	CLOSE curs
	DEALLOCATE curs

	--And finaly to release allocated XML datatable - following code must be executed
	Exec sp_xml_removedocument @idoc
	set @error = @@error
Return(@error)
GO
ALTER   Trigger tU_IRPoint ON dbo.IRPoint
    for Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	-- 1st step - update actionDate if record not already deleted
	update IRPoint
		set actionDate = GetDate()
	from IRPoint IRP 
			join inserted on IRP.IRPointID = inserted.IRPointID
			join deleted on IRP.IRPointID = deleted.IRPointID
	where deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	update IRPoint
		set actionID=2
	from IRPoint IRP
			join inserted on IRP.IRPointID = inserted.IRPointID
			join deleted on IRP.IRPointID = deleted.IRPointID
	where deleted.actionID<3 and inserted.actionID = 1
/*
	-- 3d step -disable restore of deleted record
	update IRPoint 
		set actionID = 3
	from IRPoint IRP
			join inserted on IRP.IRPointID = inserted.IRPointID
			join deleted on IRP.IRPointID = deleted.IRPointID
	where deleted.actionID=3
*/
GO

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.150')
	begin
		update DataInfo set keyValue = '3.46.150' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.150' '3.46.150 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

