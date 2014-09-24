/*
this script for fix "MLine00000311 Calendar captions" and "MLine00000374 Problems with saving vola"
*/
set nocount on 
GO
update expiryCalendar set expCalendarName = '3rd Sat' where expCalendarID = 1
update expiryCalendar set expCalendarName = '3rd Fri' where expCalendarID = 2
update expiryCalendar set expCalendarName = '3rd Thu' where expCalendarID = 3
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
ALTER        Procedure dbo.usp_CustomStrikeSkewPoint_Save 
	@iCustomStrikeSkewPointID int = Null output,
	@iVolaSurfaceDataID int = Null,
	@iType int = Null,
	@fStrike float = Null,
	@fVolatility float = Null,
	@dtExpDate datetime = Null,
	@iIsBasePoint int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into CustomStrikeSkewPoint table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)


	if exists (select * from CustomStrikeSkewPoint where 
						volaSurfaceDataID = @iVolaSurfaceDataID and 
						strike = @fStrike and 
						type = @iType and 
						expiryID = @iExpiryID
				and actionID < 3 and (@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null))
		begin
			--Raiserror ('Row in CustomStrikeSkewPoint table  with same volaSurfaceDataID, strike, type and expDate already exists!', 16, 1)
			--Return (-1)
			select @iCustomStrikeSkewPointID = customStrikeSkewPointID 
				from CustomStrikeSkewPoint 
				where	volaSurfaceDataID = @iVolaSurfaceDataID and 
						strike = @fStrike and 
						type = @iType and 
						expiryID = @iExpiryID
						and actionID < 3 
						and (@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null)
		end 
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					expiryID,
					isBasePoint) 
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						@iExpiryID,
						@iIsBasePoint)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iCustomStrikeSkewPointID=@@IDENTITY
			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID),
					isBasePoint = isnull(@iIsBasePoint,isBasePoint)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

    set @error = 0

finish:
Return(@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

set nocount on 
--============================================================================================================
--change DB version
--============================================================================================================
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.16')
	begin
		update DataInfo set keyValue = '3.16' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.16'
	end
