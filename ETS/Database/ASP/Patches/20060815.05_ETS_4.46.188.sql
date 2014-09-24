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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.187'  or (keyValue = '3.46.188') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.188 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.188' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*BEGIN DATA MODIFICATION SECTION*/

/*FutUnd_4_Funcs***************************/
--Created by Nikita Kychkin
--14.08.2006
/*******************************************/

/*==========================================================================================*/
/*FUNCTIONS*/
/*==========================================================================================*/
/*fn_GetMaturityID*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[fn_GetMaturityID]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[fn_GetMaturityID]
go
create function fn_GetMaturityID (@dtDate datetime)  
	returns int
as  
begin 
	return (year(@dtDate) - 2000) * 12 + datepart(month, @dtDate)
end
go

/*fn_NameContr*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[fn_NameContr]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[fn_NameContr]
go
create function fn_NameContr (@CName nvarchar(20), @TypeY int)
RETURNS nvarchar(3)
as
Begin
	Declare @yy nvarchar(2)
	Declare @m nvarchar(1)
	Declare @Result nvarchar(3)

	Set @m = case Upper(SUBSTRING(@CName,1,3))
		    when 'JAN' then 'F' --'Jan'
		    when 'FEB' then 'G'	--'Feb'
		    when 'MAR' then 'H'	--'Mar'
		    when 'Apr' then 'J'	--'Apr'
		    when 'MAY' then 'K'	--'May'
		    when 'JUN' then 'M'	--'Jun'
		    when 'JUL' then 'N'	--'Jul'
		    when 'AUG' then 'Q'	--'Aug'
		    when 'SEP' then 'U'	--'Sep'
		    when 'OCT' then 'V'	--'Oct'
		    when 'NOV' then 'X'	--'Nov'
		    when 'DEC' then 'Z'	--'Dec'
		  end
	
	Set @yy = SUBSTRING(@CName,Len(@CName)-1,2)
	if @TypeY = 1 
		Begin
		 if SUBSTRING(@yy,1,1) = '0' 
			Set @yy  = SUBSTRING(@yy,2,1)	 
		End 
	Set @Result = @yy + @m
	Return(@Result)
end
go

/*fn_OptS2FutS*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[fn_OptS2FutS]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[fn_OptS2FutS]
go
create function fn_OptS2FutS (@OptCName nvarchar(50))
RETURNS nvarchar (10)
as
Begin
	Declare @i int
	Declare @k int
	Declare @Result nvarchar(10)
	
	Set @k = 1
	Set @i = 0
	While (@i < 1)
		Begin
			if SUBSTRING(@OptCName,@k,1) = '/'
			   Set @i = @i + 1
			if @i = 1 
			   Begin
				Set @Result = SUBSTRING(@OptCName,1,@k-1)
			   End
			Set @k = @k +1 			
		End	
	Return(@Result)
end
go

/*GetBusDay*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[GetBusDay]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[GetBusDay]
go
create FUNCTION GetBusDay (@Dt as datetime, @Curr_ID as int)  
RETURNS datetime AS  
BEGIN 
	Declare @BusDay as datetime
	Declare @tmpDt as datetime
	Declare @Flag as int

	Set @Flag = 0
	while @Flag = 0
	Begin 	
		if exists(Select Jdate from holidays where @Curr_ID = Curr_ID and cast(Jdate + 25566 as datetime) = 
					DateADD(Day,Case  when Datename(weekday,@Dt) =  'SunDay' then 1 
					   		  when Datename(weekday,@Dt) =  'Saturday' then 2 
						   				     else 0 end, @Dt)
												)
			Set @Dt =  DateADD(Day, 1, @Dt)
		else
		Begin
			Set @Flag =1 
			Set @BusDay = DateADD(Day,Case  when Datename(weekday,@Dt) =  'SunDay' then 1 
				                       when Datename(weekday,@Dt) =  'Saturday' then 2 
						   			       else 0 end, @Dt )
		End
	end
	RETURN(@BusDay)
END
go

GO
/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.188')
	begin
		update DataInfo set keyValue = '3.46.188' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.188' '3.46.188 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
