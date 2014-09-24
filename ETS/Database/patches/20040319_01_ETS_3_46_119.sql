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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.118' or (keyValue = '3.46.119')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.119 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.119' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- usp_BoIndexWithBasketInfo_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_BoIndexWithBasketInfo_Get') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_BoIndexWithBasketInfo_Get
GO

CREATE PROC dbo.usp_BoIndexWithBasketInfo_Get
AS
	set nocount on

	select 
		c.contractID as iIndexID,
		c.symbol as vcSymbol,
		i.isBasket as tiIsBasket,
		case when isnull(def.indexID, 0) <> 0 then 1 else 0 end as tiHasComponents
	from IndexView i
		inner join ContractView c on i.indexID = c.contractID
		left join
		(
			select indexID
			from IndexDefinitionView
			where isnull(weight, 0) <> 0
			group by indexID
		) def on def.indexID = i.indexID

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoIndexBasketWeight_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_BoIndexBasketWeight_Save') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_BoIndexBasketWeight_Save
GO

CREATE PROC dbo.usp_BoIndexBasketWeight_Save
	@iIndexID int = null out,
	@vcIndexSymbol varchar(20) = null,
	@iStockID int = null out,
	@vcStockSymbol varchar(20) = null,
	@fWeight float = null,
	@tiAddNewIndex tinyint = 0,
	@tiAddNewStock tinyint = 0,
	@tiRaiseError tinyint = 0,
	@vcMessage varchar(255) = null out
AS
	set nocount on
	declare @error int set @error = 0
	declare @iContractTypeID int
	declare @iMsHint int set @iMsHint = 0

	set @tiRaiseError = isnull(@tiRaiseError, 0)
	set @tiAddNewIndex = isnull(@tiAddNewIndex, 0)
	set @tiAddNewStock = isnull(@tiAddNewStock, 0)

	if @iIndexID is null and @iStockID is null and @vcIndexSymbol is null and @vcStockSymbol is null 
	begin
		set @vcMessage = 'Impossible define identifiers of the Stock and Index!'
		if @tiRaiseError != 0 raiserror (@vcMessage, 16, 1)
		return -1
	end   

	if @vcIndexSymbol is not null and @iIndexID is null
	begin
		select
			@iIndexID = contractID,
			@iContractTypeID = contractTypeID
		from ContractView where
			symbol = @vcIndexSymbol
			and contractTypeID in (1, 2)

		if @iIndexID is null 
		begin
			if @tiAddNewIndex != 0
			begin
				exec @iIndexID = usp_Index_Save @iIndexID = null, @vcSymbol = @vcIndexSymbol, @tiIsBasket = 1, @tiIsActive = 1, @tiCheckExistance = 0
				if @@error != null
				begin
					set @vcMessage = 'Fail to save new Index ' + @vcIndexSymbol + '!'
					if @tiRaiseError != 0 raiserror (@vcMessage, 16, 1)
					return -2
				end

				if isnull(@iIndexID, 0) <= 0
				begin
					set @vcMessage = 'Fail to save new Index: Index or stock with ticker ' + @vcIndexSymbol + ' already exists!'
					if @tiRaiseError != 0 raiserror (@vcMessage, 16, 1)
					return -3
				end

				set @iMsHint = 1
			end
			else
			begin
				set @vcMessage = 'Impossible define identifier of the Index ' +  @vcIndexSymbol + '!'
				if @tiRaiseError != 0 raiserror (@vcMessage, 16, 1)
				return -4
			end
		end
		else if @iContractTypeID != 1
		begin
			set @vcMessage = 'Fail to save new Index: Stock with ticker ' + @vcIndexSymbol + ' already exists!'
			if @tiRaiseError != 0 raiserror (@vcMessage, 16, 1)
			return -3
		end
	end

	if @vcStockSymbol is not null and @iStockID is null
	begin
		select
			@iStockID = contractID,
			@iContractTypeID = contractTypeID
		from ContractView where
			symbol = @vcStockSymbol
			and contractTypeID in (1, 2)

		if @iStockID is null 
		begin
			if @tiAddNewStock != 0
			begin
				exec @iStockID = usp_Stock_Save @iStockID = null, @vcSymbol = @vcStockSymbol, @tiIsActive = 1, @tiCheckExistance = 0
				if @@error != null
				begin
					set @vcMessage = 'Fail to save new Stock ' + @vcStockSymbol + '!'
					if @tiRaiseError != 0 raiserror (@vcMessage, 16, 1)
					return -5
				end

				if isnull(@iStockID, 0) <= 0
				begin
					set @vcMessage = 'Fail to save new Stock: Index or stock with ticker ' + @vcStockSymbol + 'already exists!'
					if @tiRaiseError != 0 raiserror (@vcMessage, 16, 1)
					return -6
				end

				set @iMsHint = @iMsHint + 2
			end
			else
			begin
				set @vcMessage = 'Impossible define identifier of the Stock ' + @vcStockSymbol + '!'
				if @tiRaiseError != 0 raiserror (@vcMessage, 16, 1)
				return -7
			end
		end
		else if @iContractTypeID != 2
		begin
			set @vcMessage = 'Fail to save new Stock: Index with ticker ' + @vcStockSymbol + ' already exists!'
			if @tiRaiseError != 0 raiserror (@vcMessage, 16, 1)
			return -6
		end
	end

	Begin Transaction

	update [Index]
	set isBasket = 1
	where indexID = @iIndexID and actionID < 3
	if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end

	if not exists(select * from IndexDefinitionView where indexID = @iIndexID and stockID = @iStockID) 
	begin
		insert into IndexDefinition (indexID, stockID, weight) 
		values (@iIndexID, @iStockID, @fWeight)

		if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end
	end
	else
	begin
		update IndexDefinition
		set weight = @fWeight
		where indexID = @iIndexID and stockID = @iStockID

		if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end
	end
	Commit Transaction

finish:
	if @error = 0
		return @iMsHint
	else
		return @error
GO

/* RESULTS ANALYZING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.119')
	begin
		update DataInfo set keyValue = '3.46.119' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.119' '3.46.119 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
