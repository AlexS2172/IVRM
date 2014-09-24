/*
	Update database (version 3.00) for ETSMM Vola Manager
	Cleanup of DB structure (this script remove 2 useless tables from DB and change 3 SP's)
*/
if exists (select * from dbo.sysobjects where id = object_id(N'[InterestRateCurve]') and OBJECTPROPERTY(id, N'IsTable') = 1)
drop table [InterestRateCurve]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[PeriodType]') and OBJECTPROPERTY(id, N'IsTable') = 1)
drop table [PeriodType]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Del]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_InterestRateCurve_Del
	@iCurvePointID int	-- InterestRateCurve's identifier
as
/*
	Created by Andrey Sinkin
	this procedure deletes data from InterestRateCurve table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	-- declare @error int	set @error = -1

	--delete InterestRateCurve where curvePointID = @iCurvePointID
	exec usp_IRPoint_Del @iIRpointID = @iCurvePointID

	Return(@@error)
GO

CREATE Procedure dbo.usp_InterestRateCurve_Get
	@iCurvePointID int = null,	-- InterestRateCurve identifier
	@bAdvancedMode bit = 0		-- flag for advanced mode
as
/*
	Created by Andrey Sinkin
	this procedure returns data from InterestRateCurve table (now from IRPoint table)
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @date datetime		set @date = getdate()

	if @bAdvancedMode = 0
		select 	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				longRate as fRateAsk, 
				shortRate as fRateBid
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iPeriodTypeID, iNumber
	else
		select	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				longRate as fRateAsk, 
				shortRate as fRateBid,
			case periodTypeID
				when 1 then datediff(day, getdate(), dateadd(day, num, @date))
				when 2 then datediff(day, getdate(), dateadd(week, num, @date))
				when 3 then datediff(day, getdate(), dateadd(month, num, @date))
				when 4 then datediff(day, getdate(), dateadd(year, num, @date))
			end iDaysFromToday
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iDaysFromToday

/*	if @bAdvancedMode = 0
		select 	curvePointID iCurvePointID,	periodTypeID iPeriodTypeID, number iNumber, rateAsk fRateAsk, rateBid fRateBid
			from InterestRateCurve
			where @iCurvePointID is null or curvePointID = @iCurvePointID
			order by iPeriodTypeID, iNumber
	else
		select curvePointID iCurvePointID, periodTypeID iPeriodTypeID, number iNumber, rateAsk fRateAsk, rateBid fRateBid,
			case periodTypeID
				when 1 then datediff(day, getdate(), dateadd(day, number, @date))
				when 2 then datediff(day, getdate(), dateadd(week, number, @date))
				when 3 then datediff(day, getdate(), dateadd(month, number, @date))
				when 4 then datediff(day, getdate(), dateadd(year, number, @date))
			end iDaysFromToday
			from InterestRateCurve
			where @iCurvePointID is null or curvePointID = @iCurvePointID
			order by iDaysFromToday */

Return(@@error)

GO

CREATE Procedure dbo.usp_InterestRateCurve_Save
	@iCurvePointID int = null output,	-- InterestRateCurve's identifier
	@iPeriodTypeID int = null,		-- type of periods identifier
	@iNumber int = null,			-- number
	@fRateAsk float = null,			-- ask
	@fRateBid float = null			-- bid
as
/*
	Created by Andrey Sinkin
	this procedure saves data to IRPoint (InterestRateCurve) table 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iCurveID int
		select @iCurveID=curveID from IRCurve

	if @iCurvePointID is null and @iPeriodTypeID is not null and @iNumber is not null
		select @iCurvePointID = IRPointID from IRPointView where periodTypeID = @iPeriodTypeID and num = @iNumber

	Begin Transaction
		if @iCurvePointID is null 
			begin
				insert into IRPoint (periodTypeID, num, longRate, ShortRate, HTBRate,CurveID) 
				values (@iPeriodTypeID, @iNumber, @fRateAsk, @fRateBid,@fRateAsk,@iCurveID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

				select @iCurvePointID = @@identity
			end
		else
			begin
				update IRPoint set
					periodTypeID = isnull(@iPeriodTypeID, periodTypeID),
					num = isnull(@iNumber, num),
					longRate = isnull(@fRateAsk, longRate),
					shortRate = isnull(@fRateBid, shortRate)
				where IRPointID = @iCurvePointID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end

/*	if @iCurvePointID is null and @iPeriodTypeID is not null and @iNumber is not null
		select @iCurvePointID = curvePointID from InterestRateCurve where periodTypeID = @iPeriodTypeID and number = @iNumber
	Begin Transaction
		if @iCurvePointID is null 
			begin
				insert into InterestRateCurve (periodTypeID, number, rateAsk, rateBid) 
				values (@iPeriodTypeID, @iNumber, @fRateAsk, @fRateBid)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				select @iCurvePointID = @@identity
			end
		else
			begin
				update InterestRateCurve set
					periodTypeID = isnull(@iPeriodTypeID, periodTypeID),
					number = isnull(@iNumber, number),
					rateAsk = isnull(@fRateAsk, rateAsk),
					rateBid = isnull(@fRateBid, rateBid)
				where @iCurvePointID = curvePointID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end */

		set @error = 0
	Commit Transaction
finish:

Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
