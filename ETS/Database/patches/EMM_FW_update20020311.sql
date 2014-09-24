/*
	this script make the ability to generate default volatility in Vola Manager
*/
set nocount on 
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.03')
	begin
		update DataInfo set keyValue = '3.03' where DataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.03'
	end
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveSurface_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CreateDefaultVolaSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CreateDefaultVolaSurface_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewDefaultPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewDefaultPoint_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomVolatilityPointIsExist_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomVolatilityPointIsExist_Get]
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE procedure usp_CreateDefaultVolaSurface_Save
	@iContractID int
as
/*
	Created by Zaur Nuraliev
	Description: this procedure create default surface by ContractID
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iVolaSurfaceDataID int

	if not exists(select * from VolaSurfaceDataView 
					where contractID=@iContractID and isDefaultSurface = 1) and @iContractID is not null
		begin

			declare @fPrice float
		
			select Top 1 @fPrice = 
				case 
					when (priceBid is not null and priceAsk is null) then priceBid
					when (priceBid is null and priceAsk is not null) then priceAsk
					when (priceBid is not null and priceAsk is not null) then (priceAsk + priceBid)/2
					when (priceBid is null and priceAsk is null) then priceLast
					when (priceBid is null and priceAsk is null and priceLast is null) then priceClose
				end
					from ContractPriceHistory 
					where contractID = @iContractID
					order by PriceDate desc

			insert into VolaSurfaceData(contractID,isDefaultSurface,baseUnderlinePrice)
				values (@iContractID,1,isnull(@fPrice,0))

			select @iVolaSurfaceDataID = @@identity
		end
	else 
		select @iVolaSurfaceDataID = VolaSurfaceDataID 
			from VolaSurfaceDataView 
			where contractID=@iContractID and isDefaultSurface = 1 and @iContractID is not null

Return(@iVolaSurfaceDataID)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE           Procedure dbo.usp_ActiveSurface_Get 
	@vcSymbolName varchar(12) --the contract symbol
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure return default surface identifier
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractID int
		select @iContractID = contractID from Contract where symbol = @vcSymbolName 

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0
		exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select	@iVolaSurfaceDataID as iVolaSurfaceDataID

/*
	select	volaSurfaceDataID as iVolaSurfaceDataID
		from VolaSurfaceDataView V
				join ContractView C on V.contractID=C.contractID
		where C.symbol = @vcSymbolName 
				and V.isDefaultSurface=1
*/

	set @error = @@error
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  procedure usp_CustomStrikeSkewDefaultPoint_Save
	@iContractID int,
	@dtExpiry smalldatetime,
	@iType int,
	@fVolatility float
as
/*
	Created by Zaur Nuraliev
	Description: this procedure ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0
		exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

--select @dtExpiry as '@dtExpiry', @iVolaSurfaceDataID as '@iVolaSurfaceDataID'

	insert into CustomStrikeSkewPoint(VolaSurfaceDataID,strike,type,volatility,isBasePoint,expDate)
		select distinct @iVolaSurfaceDataID, strike as fStrike, @iType,@fVolatility,-1,convert(varchar(6),expiry,112) + '01' as expiry
			from OptionView 
			where expiry = @dtExpiry and underlyingContractID = @iContractID
				and not exists(select * 
									from CustomStrikeSkewPointView 
									where VolaSurfaceDataID = @iVolaSurfaceDataID and 
											strike = OptionView.strike and convert(varchar(8),expDate,112) = convert(varchar(6),expiry,112) + '01')


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  Procedure dbo.usp_CustomVolatilityPointIsExist_Get

as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------

	select distinct 
			CV.contractID as iContractID,
			CV.symbol as vcSymbolName,
			OV.expiry as dtExpiry,
			case when CSSP.volaSurfaceDataID is null then 0  
			else 1 end 'CustomVolatilityPointsCount'
		from OptionView OV
			join ContractView CV on OV.underlyingContractID = CV.contractID
			left join VolaSurfaceDataView VSD on OV.underlyingContractID = VSD.contractID and VSD.isDefaultSurface = 1
			left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
			and Convert(varchar(6),OV.expiry,112) = Convert(varchar(6),CSSP.expDate,112)
		where 
			--CV.ContractID=4111		
			CSSP.volaSurfaceDataID is null
		order by CV.contractID,OV.expiry
/*
	select distinct 
			CV.contractID as iContractID,
			CV.symbol as vcSymbolName,
			OV.expiry as dtExpiry,
			case when CSSP.volaSurfaceDataID is null then 0  
			else 1 end 'CustomVolatilityPointsCount'
		from OptionView OV
			join ContractView CV on OV.underlyingContractID = CV.contractID
			left join VolaSurfaceDataView VSD on OV.underlyingContractID = VSD.contractID
			left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
			and Convert(varchar(6),OV.expiry,112) = Convert(varchar(6),CSSP.expDate,112)
		order by CV.contractID,OV.expiry
*/

Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

