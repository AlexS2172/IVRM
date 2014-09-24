/*
	bug fix for database (version 3.01) for ETSMM Bask Office 
	MLine00000197:"Error message on EOD procedure and Load Market Structure"
*/

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


ALTER   procedure dbo.usp_ContractPriceEOD_Save
	@vcSymbol varchar(8),
	@fPriceClose float,
	@iContractTypeID int, --2-stock 3-option
	@vcExchangeCode varchar(10) = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	if (select object_id('tempdb..##ContractPriceEOD')) is not null
		begin
			insert into ##ContractPriceEOD (symbol, contractTypeID, priceClose, exchangeCode)
				values (@vcSymbol, @iContractTypeID, @fPriceClose, @vcExchangeCode)
		end

    set @error = @@error

Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

