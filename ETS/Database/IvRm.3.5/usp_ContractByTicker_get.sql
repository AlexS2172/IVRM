SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractByTicker_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractByTicker_Get]
GO

--select top 10* from ContractView where contracttypeid = 3
--exec usp_ContractByTicker_Get 'NDX', 4

create  proc dbo.usp_ContractByTicker_Get
		@vcSymbol varchar(20),
		@iContractType int 
as

-------------------------------------------------
set nocount on
-------------------------------------------------
	 
	select
		contractID as iContractID
	from
		ContractView
	where
 		RTRIM(LTRIM(Symbol)) = RTRIM(LTRIM(@vcSymbol)) AND
 		contractTypeID 	=	case @iContractType
	 					when 0 then 2 --stock
						when 1 then 4 --future
						when 5 then 3 --option
						when 4 then 1 --index
						when 8 then 5 --future option
					end
					
return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

