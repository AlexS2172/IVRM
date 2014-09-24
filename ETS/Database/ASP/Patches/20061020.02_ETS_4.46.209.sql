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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.208'  or (keyValue = '3.46.209') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.209 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.209' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO


if exists (select * from dbo.sysobjects where name = 'usp_MmIndexUnderlyingBeta_Eod_Get' and id = object_id(N'[dbo].[usp_MmIndexUnderlyingBeta_Eod_Get]'))
DROP PROCEDURE dbo.usp_MmIndexUnderlyingBeta_Eod_Get
GO

CREATE  PROC dbo.usp_MmIndexUnderlyingBeta_Eod_Get
	@iContractID int,
	@iHedgeSymbolID int
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select 	eod_ib.beta as fBeta
	from ETS_ASP_EOD.dbo.IndexBeta eod_ib
		inner join EodUnderlyingMap eod_i on eod_i.eodContractID = eod_ib.indexID
		inner join EodUnderlyingMap eod_u on eod_u.eodContractID = eod_ib.contractID
	where eod_ib.actionID <3 and eod_i.contractID = @iHedgeSymbolID 
				 and eod_u.contractID = @iContractID

	return @@error
GO

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_Exchanges_Execute]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodUpdateMarketStructure_Exchanges_Execute]
GO


CREATE   PROC dbo.usp_AspEodUpdateMarketStructure_Exchanges_Execute
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
declare @error int set @error = 0
declare @vcTranName varchar(32)
 if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
 if @iWithTran is not null begin tran @vcTranName
	 --Delete all Option Only exchange Links

	delete from ContractExchangeLink where isOption = 1 and (isUnderlying is null or isUnderlying = 0)

	if @@error != 0
	 begin
		  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market exchanges: Fail to delete option Exchanges', @iErrorCode = @@error
		  set @error = -1
		  if @iWithTran is not null rollback tran @vcTranName
		  goto finish
	 end
	delete from ContractExchangeLink where ActionID =3

	if @@error != 0
	 begin
		  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market exchanges: Fail to remove option Exchanges', @iErrorCode = @@error
		  set @error = -1
		  if @iWithTran is not null rollback tran @vcTranName
		  goto finish
	 end

	-- Clear IsOption flaf for the rest of them
	update ContractExchangeLink set isOption = 0 where isOption = 1
	if @@error != 0
	 begin
		  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market exchanges: Fail to update stock Exchanges', @iErrorCode = @@error
		  set @error = -1
		  if @iWithTran is not null rollback tran @vcTranName
		  goto finish
	 end

	update ContractExchangeLink 
	 set 	isOption = 1
	  from ContractExchangeLink el
	   inner join Exchange Exch on el.ExchangeID = Exch.ExchangeID 
	   inner join EodUnderlyingMap map on el.ContractID =map.ContractID
	   inner join ETS_ASP_EOD.dbo.Exchange EodExch on Exch.ExchangeName = EodExch.ExchangeName 
	   inner join ETS_ASP_EOD.dbo.ContractExchangeLinkView exchLink on EodExch.ExchangeID = exchLink.ExchangeID and  map.eodContractID = exchLink.ContractID and exchLink.ISOption = 1
	
 	 if @@error != 0
	 begin
		  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market exchanges: Fail to update existing Exchange Linkss', @iErrorCode = @@error
		  set @error = -1
		  if @iWithTran is not null rollback tran @vcTranName
		  goto finish
	 end

	insert into ContractExchangeLink (exchangeID, contractID, isOption, isUnderlying)
	( select 
		Exch.exchangeId as exchangeID,
		map.ContractID as contractID,
		exchLink.isOption as isOption,
		0 as isUnderlying
	from ETS_ASP_EOD.dbo.ContractExchangeLinkView exchLink
		 inner join ETS_ASP_EOD.dbo.Exchange eodExch on eodExch.ExchangeID = exchLink.ExchangeID and eodExch.ActionID <3
	 	 inner join EodUnderlyingMap map on exchLink.contractID =map.eodContractID
	 	 inner join Exchange exch on  exch.exchangeName = eodExch.exchangeName and exch.isOption = 1 and exch.actionID <3
		 left join  ContractExchangeLinkView link on map.ContractID = link.ContractID and exch.ExchangeID = link.ExchangeID
	where link.contractExchangeLinkID is null
 
       )

	if @@error != 0
	 begin
		  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market exchanges: Fail to insert option Exchange Linkss', @iErrorCode = @@error
		  set @error = -1
		  if @iWithTran is not null rollback tran @vcTranName
		  goto finish
	 end


	

if @iWithTran is not null commit tran @vcTranName

finish:
return @error
GO


GO


ALTER    PROC dbo.usp_AspEodUpdateMarketStructure_Execute 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	exec usp_AspEodUpdateMarketStructure_Main_Execute 	@iWithTran
	exec usp_AspEodUpdateMarketStructure_DefaultIVs_Execute @iWithTran
	exec usp_AspEodUpdateMarketStructure_Exchanges_Execute  @iWithTran

GO


GO
/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.209')
	begin
		update DataInfo set keyValue = '3.46.209' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.209' '3.46.209 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO