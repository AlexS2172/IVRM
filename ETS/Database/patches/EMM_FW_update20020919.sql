/*
	patch for MLine00000657
	DB version: 3.37.026
*/
set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchangeEOD_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchangeEOD_Init_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchangeEOD_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchangeEOD_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchangeEOD_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchangeEOD_Complete_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   proc dbo.usp_ContractInExchangeEOD_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if object_id('tempdb..##ContractExchangeLinkEOD') is not null
		begin
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			return (-1) 
		end

	create table ##ContractExchangeLinkEOD(
			exchangeID int NULL ,
			contractID int NOT NULL ,
			isUnderlying tinyint NULL,
			isOption tinyint NULL) 

	set @error = @@error
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

CREATE  proc dbo.usp_ContractInExchangeEOD_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	Begin tran 
		if object_id('tempdb..##ContractExchangeLinkEOD') is null
		   begin
		       raiserror ( 'Cannot complete EOD. Cannot find save data of IV bands.', 16, 1)
		       return (-1)
		   end
 		
		update ContractExchangeLinkView
			set exchangeID = EOD.exchangeID, 
				contractID = EOD.contractID, 
				isUnderlying = EOD.isUnderlying, 
				isOption = EOD.isOption
			from ContractExchangeLinkView CE 
				inner join ##ContractExchangeLinkEOD EOD
					on isnull(CE.exchangeID,0) = isnull(EOD.exchangeID,0) and
						CE.contractID = EOD.contractID

 		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
 	
		insert into ContractExchangeLinkView (	exchangeID,contractID,isUnderlying,isOption )
			select exchangeID,contractID,isUnderlying,isOption
				from ##ContractExchangeLinkEOD EOD
					where not exists
						(select * from ContractExchangeLinkView CE where isnull(CE.exchangeID,0) = isnull(EOD.exchangeID,0) and CE.contractID = EOD.contractID)

 		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit tran
    set @error = 0
finish:
	if object_id('tempdb..##ContractExchangeLinkEOD') is not null
		drop table ##ContractExchangeLinkEOD
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

CREATE   proc dbo.usp_ContractInExchangeEOD_Save
	@iExchangeID int,       --the exchangeID
	@iContractID int,   	--the contractID
	@itype1 tinyint = null, --the link type 1
	@itype2 tinyint = null  --the link type 2
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1
	if (select object_id('tempdb..##ContractExchangeLinkEOD')) is not null
		begin
			insert into ##ContractExchangeLinkEOD (exchangeID,contractID,isUnderlying,isOption)
				values (@iExchangeID, @iContractID, @itype1, @itype2)
		end
    set @error = @@error
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

CREATE        Procedure dbo.usp_IndexDefinition_Save 
	@iIndexID int = null, --the index identifier
	@iStockID int = null, --the stock identifier
	@fWeight float = null,  --the weight
	@fBeta float = null,
	@vcIndexSymbol varchar(8) = null,
	@vcStockSymbol varchar(8) = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into IndexDefinition table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iIndexID is null and @iStockID is null and @vcIndexSymbol is null and @vcStockSymbol is null 
		begin
			Raiserror ('Impossible define identifiers of the Stock and Index!', 16, 1)
			Return (-1)
		end   

	if @vcIndexSymbol is not null and @iIndexID is null
		select	@iIndexID = contractID from Contract where symbol = @vcIndexSymbol and contractTypeID = 1 and actionID < 3
		if @iIndexID is null 
			begin
				Raiserror ('Impossible define identifier of the Index!', 16, 1)
				Return (-1)
			end   

	if @vcStockSymbol is not null and @iStockID is null 
		select	@iStockID = contractID from Contract where symbol = @vcStockSymbol and contractTypeID = 2 and actionID < 3
		if @iStockID is null 
			begin
				Raiserror ('Impossible define identifier of the Stock!', 16, 1)
				Return (-1)
			end   

	Begin Transaction
		if not exists(select * From IndexDefinitionView where indexID = @iIndexID and stockID = @iStockID) 
			begin
				insert into IndexDefinition (indexID,stockID,weight,beta) 
				values (@iIndexID,@iStockID,@fWeight,@fBeta)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexDefinition
					set weight = isnull(@fWeight, weight),
						beta=isnull(@fBeta,beta)
					where indexID = @iIndexID and stockID = @iStockID
	
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




--===================================================================================
--change DB version
--===================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.37.026')
	begin
		update DataInfo set keyValue = '3.37.026' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.37.026'
	end
