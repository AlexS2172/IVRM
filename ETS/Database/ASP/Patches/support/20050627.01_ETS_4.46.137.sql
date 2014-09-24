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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.136'  or (keyValue = '3.46.137') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.137 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.137' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

if not exists (select * from dbo.sysindexes where name = N'CustomStrikeSkewPoint_9' and id = object_id(N'[dbo].[CustomStrikeSkewPoint]'))
begin
	CREATE  INDEX [CustomStrikeSkewPoint_9] ON [dbo].[CustomStrikeSkewPoint]([volaSurfaceDataID], [strike], [customStrikeSkewPointID], [type], [volatility], [expiryID], [isBasePoint]) ON [PRIMARY]
	CREATE STATISTICS [hind_104387441_2A_1A] ON [dbo].[volasurfacedata] ([contractid], [volasurfacedataid])
	CREATE STATISTICS [hind_618485282_2A_8A] ON [dbo].[option] ([iscall], [optionrootid])
	CREATE STATISTICS [hind_618485282_2A_3A_4A_5A_8A] ON [dbo].[option] ([iscall], [strike], [expiryid], [actionid], [optionrootid])
	CREATE STATISTICS [hind_618485282_2A_3A_5A_8A_4A] ON [dbo].[option] ([iscall], [strike], [actionid], [optionrootid], [expiryid])
	CREATE STATISTICS [hind_778485852_2A_1A_3A_4A_5A_6A_7A] ON [dbo].[customstrikeskewpoint] ([volasurfacedataid], [customstrikeskewpointid], [strike], [type], [volatility], [expiryid], [isbasepoint])
	CREATE STATISTICS [hind_1856061698_2A_1A] ON [dbo].[optionroot] ([underlyingid], [optionrootid])
end

GO

ALTER   PROC dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null
as
/*
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare	@iExpiryID int

	-- Sharky: faster request for special parameters' set
	if @iExpiryID is null and @iCustomStrikeSkewPointID is null
		and @iVolaSurfaceDataID is not null and @iType is not null
	begin
		select 
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint 
		from [option]
		inner join OptionRoot orr on [option].OptionRootId = orr.OptionRootID
		inner join Contract C on  C.ContractID = orr.UnderlyingID
		inner join VolaSurfaceData vsd on C.ContractID = vsd.ContractID
		inner join CustomStrikeSkewPoint CSSP on 
				CSSP.volaSurfaceDataID = vsd.volaSurfaceDataID and
				CSSP.ExpiryID = [option].ExpiryID  and 
				[option].Strike =  CSSP.strike
		where [option].ActionID <3 and 
		      CSSP.volaSurfaceDataID = @iVolaSurfaceDataID and
		      CSSP.type = @iType and
		      CSSP.actionID < 3
		group by
			CSSP.customStrikeSkewPointID,
			CSSP.volaSurfaceDataID,
			CSSP.strike ,
			CSSP.type,
			CSSP.volatility,
			CSSP.expiryID,
			CSSP.isBasePoint
			
	end
	else
	begin
		-- Sharky: old variant
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

		select 
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			--convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
		from CustomStrikeSkewPointView CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock)
			--join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			--join Contract C on  VolaSurfaceData.contractID = C.contractID
			--join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID and CSSP.expiryID = E.expiryID
		where 
			customStrikeSkewPointID = isnull(@iCustomStrikeSkewPointID, customStrikeSkewPointID)
			and CSSP.volaSurfaceDataID = isnull(@iVolaSurfaceDataID, CSSP.volaSurfaceDataID) 
			and type = isnull(@iType, type) 
			and CSSP.expiryID = isnull(@iExpiryID, CSSP.expiryID)
			--and E.expiryID = isnull(@iExpiryID, E.expiryID)
	end

	set @error = @@error
return(@error)
GO

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.137')
	begin
		update DataInfo set keyValue = '3.46.137' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.137' '3.46.137 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

