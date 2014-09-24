/*
Fix for defects  MLine00001211,MLine00001200
*/

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.41.065' or (keyValue = '3.41.066')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.41.066 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.41.066' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FilterData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FilterData_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingAverageVolumes_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingAverageVolumes_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE  Procedure dbo.usp_FilterData_Get
	@iFilterID int, --the row identifier
	@iTraderID int = null output,
	@iBookID int = null output,
	@iStrategyID int = null output
as
/*
	Created by Zaur Nuraliev
	Description: this procedure return data from Filter structure
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @vcSelect1 varchar(8000)
	declare @vcSelect2 varchar(8000)
	declare @vcSelect3 varchar(8000)

	set @vcSelect1 = 'select distinct case isnull(ORt.underlyingID,0) when 0 then T.contractID else ORt.underlyingID end ''contractID'''
			+ ' from TradeView T left join OptionView O on T.contractID=O.optionID'
			+ ' left join OptionRoot ORt on ORt.optionRootID=O.optionRootID'

	set @vcSelect2 =''
	set @vcSelect3 = ' where 1=1'  
	declare @vcFiledValue varchar(50)
	-- TraderID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 1)	
		begin
			select @vcFiledValue=fieldValue from FilterContent where filterID=@iFilterID and filterFieldID=1
			set @vcSelect3 = ' and T.traderID=' + @vcFiledValue
			select @iTraderID = convert(int,@vcFiledValue)
		end
	-- BookID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 2)	
		begin
			select @vcFiledValue=fieldValue from FilterContent where filterID=@iFilterID and filterFieldID=2
			set @iBookID = convert(int,@vcFiledValue)
			set @vcSelect3 = @vcSelect3 + ' and T.bookID=' + @vcFiledValue
		end	
	-- StrategyID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 3)	
		begin
			select @vcFiledValue=fieldValue from FilterContent where filterID=@iFilterID and filterFieldID=3
			set @iStrategyID = convert(int,@vcFiledValue)
 			set @vcSelect3 = @vcSelect3 + ' and T.strategyID=' + @vcFiledValue
		end
	
	-- ContractGroupID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 4)	
		begin
			select @vcFiledValue=fieldValue from FilterContent where filterID=@iFilterID and filterFieldID=4
			--set @vcSelect3 = @vcSelect3 + ' and T.contractID in (select contractID from ContractInGroupView where groupID=' + @vcFiledValue + ')'  
			set @vcSelect3 = @vcSelect3 + ' and case isnull(ORt.underlyingID,0) when 0 then T.contractID else ORt.underlyingID end 
				in (select contractID from ContractInGroupView where groupID=' + @vcFiledValue + ')'  
		end
	--select @vcSelect1+@vcSelect2+@vcSelect3
	execute (@vcSelect1 + @vcSelect2 + @vcSelect3)


	set @error = @@error
Return(@error)
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE   PROCEDURE dbo.usp_UnderlyingAverageVolumes_Save 
	@vcSymbol varchar(20), 
	@iVolume10Day int = null,
	@iVolume5Expiration int = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractID int

	select @iContractID = contractID
		from ContractView 
		where contractTypeID < 3 and upper(symbol) = upper(@vcSymbol)

	Begin Transaction
		if not exists(select * from ContractPrice where contractID=@iContractID and isnull(exchangeID,0)=0)
			begin
				insert into ContractPrice (contractID,
						exchangeID, 
						volume10Day,
						volume5Expiration)
				  values (@iContractID,
						null,
						@iVolume10Day,
						@iVolume5Expiration)

				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin

				update ContractPrice set 
					volume10Day = isnull(@iVolume10Day, volume10Day),
					volume5Expiration = isnull(@iVolume5Expiration, volume5Expiration)
				where contractID = @iContractID and isnull(exchangeID,0)=0

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
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.41.066')
	begin
		update DataInfo set keyValue = '3.41.066' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.41.066' '3.41.066 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
