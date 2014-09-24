/*
	this script fix MLine00000202 bug in EMM FW BO
*/


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER     proc usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceAsk float,
	@fPriceBid float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iExchangeID int = null,  --the ExchangeID special for save data into ContractPrice table
	@iLotSize int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

		update Contract
		    set ContractName = @vcContractName
	 	where contractID = @iContractID
	
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		update ContractPrice 
			set priceAsk = @fPriceAsk,
			priceBid = @fPriceBid,
			PriceOpen = @fPriceOpen,
			PriceClose = @fPriceClose,
			PriceLow = @fPriceLow,
			PriceHigh = @fPriceHigh,
			Volume = @iVolume,
			lotSize = isnull(@iLotSize,lotSize)
		where contractID = @iContractID and exchangeID=@iExchangeID

		if (@@error <> 0)
			begin
				Raiserror ('Error. Can''t update Contract Price!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID,lotSize)
					values(@iContractID,null,@iLotSize)
				if (@@error <> 0) begin rollback tran return(-1) end           
			end 

		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from stock where stockID=@iContractID))
			begin 
				Declare @df smallint

				update Stock 
					set divAmt=@fDivAmt,divDate=@dtDivDate 
				where stockID=@iContractID

				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
	
				if not exists(select * from Dividend where stockID=@iContractID and divDate=@dtDivDate)
					insert into Dividend (stockID, DivAmt, DivDate)
						values (@iContractID, @fDivAmt, @dtDivDate)
					if (@@error <> 0)
						begin
							Raiserror ('Error. Can''t insert row into Dividend table!', 16, 1)
							set @error = -1 
							if @@TranCount = 1 Rollback Transaction goto finish 
						end
				else
					update Dividend
						set DivAmt = isnull(@fDivAmt,DivAmt)
						where stockID=@iContractID and DivDate=@dtDivDate
					if (@@error <> 0)
						begin
							Raiserror ('Error. Can''t update row in Dividend table!', 16, 1)
							set @error = -1 
							if @@TranCount = 1 Rollback Transaction goto finish 
						end
			end

	Commit Transaction

    set @error = 0

finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

