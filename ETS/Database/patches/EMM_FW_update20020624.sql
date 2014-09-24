/*
	fix for MLine00000490
*/

set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Holiday_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Holiday_Save]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   proc dbo.usp_Holiday_Save
		@iHolidayID int,
		@dtHolidayDate smalldatetime,
		@vcHolidayName varchar(30)
 as
  
	if exists (select * from Holiday where holidayDate = @dtHolidayDate and actionID < 3 and (@iHolidayID <> holidayID or @iHolidayID is null))
		begin
			Raiserror ('Row in Holiday table with same holiday date already exists!', 16, 1)
			Return (-1)
		end   

/*
	if exists (select * from Holiday where holidayName = @vcHolidayName and actionID < 3 and (@iHolidayID <> holidayID or @iHolidayID is null))
		begin
			Raiserror ('Row in Holiday table with same holiday name already exists!', 16, 1)
			Return (-1)
		end  
*/

   if (@iHolidayID is Null)  
	 begin
		 insert into Holiday (holidayName, holidayDate)
				values (@vcHolidayName, @dtHolidayDate)
		 if (@@error <> 0)
			return (-1)
		  else	
			return @@identity
	 end
   else 
	 begin
		update Holiday 
			 set holidayName = @vcHolidayName,
				  holidayDate = @dtHolidayDate		  
		  where holidayID = @iHolidayID
		if (@@error <> 0)
		   return (-1)
		 else  
		   return 0
	 end				
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.34.005')
	begin
		update DataInfo set keyValue = '3.34.005' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.34.005'
	end