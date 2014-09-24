SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractType_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractType_Get]
GO

create  procedure dbo.usp_ContractType_Get
   @iContractID int
As
	select 
		ContractTypeID as iContractType
	from 
		ContractView
	where 
		ContractID = @iContractID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

