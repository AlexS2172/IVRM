/*
	this fix eliminates presence of removed records in EMM BO (Exchanges by Stock dialog)
*/



SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER      procedure usp_ContractInExchange_Get
	@iContractID int,
	@bFullInfo tinyint = 1 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractPrice=Exchange table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if @bFullInfo <> 0
		select E.exchangeID as iExchangeID,
				E.ExchangeName as vcExchangeName,
				isnull(convert(bit,left(CP.exchangeLinkType,1)),0) as iType1,
				isnull(convert(bit,right(CP.exchangeLinkType,1)),0) as iType2
			from ContractExchangeLink CP, ExchangeView E
			where CP.contractID = @iContractID and CP.exchangeID =* E.exchangeID
			order by E.ExchangeName
	else
		select cp.exchangeID as iExchangeID,
				isnull(convert(bit,left(CP.exchangeLinkType,1)),0) as iType1,
				isnull(convert(bit,right(CP.exchangeLinkType,1)),0) as iType2
			from ContractExchangeLink CP
					join ExchangeView on CP.exchangeID=ExchangeView.exchangeID
			where CP.contractID = @iContractID and CP.exchangeLinkType <> 0

	set @error = @@error

Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

