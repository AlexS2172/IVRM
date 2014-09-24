if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[job_force_shrink_log]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[job_force_shrink_log]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
Create Procedure dbo.job_force_shrink_log
	@target_percent tinyint = 0,
	@target_size_MB int = 10,
	@max_iterations int = 1000,
	@backup_log_opt nvarchar(1000) = 'with truncate_only'
as
/*
	Purpose: Shrink transaction log of the current database in SQL Server 7.0.
	Author:  Andrew Zanevsky, AZ Databases, Inc., 12/25/1999, v5 - 08/18/2000
		 zanevsky@azdatabases.com
*/
	--------------------------------------------------
	set nocount on
	--------------------------------------------------

	declare @db sysname, @last_row int, @log_size decimal(15,2), @unused1 decimal(15,2), @unused decimal(15,2), 
		@shrinkable decimal(15,2), @iteration int, @file_max int, @file int, @fileid varchar(5) 

	select	@db = db_name(), @iteration = 0

	create table #loginfo (id int identity, FileId int, FileSize numeric(22,0), StartOffset numeric(22,0), FSeqNo int, 
				Status int, Parity smallint, CreateTime datetime)

	create unique clustered index loginfo_FSeqNo on #loginfo ( FSeqNo, StartOffset )

	create table #logfiles (id int identity(1,1), fileid varchar(5) not null)

	insert #logfiles ( fileid ) select convert( varchar, fileid ) from sysfiles where status & 0x40 = 0x40        
	select @file_max = @@rowcount

	if object_id( 'table_to_force_shrink_log' ) is null
		exec( 'create table table_to_force_shrink_log ( x nchar(3000) not null )' )

	insert  #loginfo ( FileId, FileSize, StartOffset, FSeqNo, Status, Parity, CreateTime ) exec ( 'dbcc loginfo' )
	select  @last_row = @@rowcount

	select  @log_size = sum( FileSize ) / 1048576.00,
        	@unused = sum( case when Status = 0 then FileSize else 0 end ) / 1048576.00,
	        @shrinkable = sum( case when id < @last_row - 1 and Status = 0 then FileSize else 0 end ) / 1048576.00
		from #loginfo

	select  @unused1 = @unused -- save for later

	select  'iteration'          = @iteration,
        	'log size, MB'       = @log_size,
	        'unused log, MB'     = @unused,
        	'shrinkable log, MB' = @shrinkable,
	        'shrinkable %'       = convert( decimal(6,2), @shrinkable * 100 / @log_size )

	while @shrinkable * 100 / @log_size > @target_percent 
		  and @shrinkable > @target_size_MB 
		  and @iteration < @max_iterations 
		begin
			select  @iteration = @iteration + 1 -- this is just a precaution

			exec( 'insert table_to_force_shrink_log select name from sysobjects
				delete table_to_force_shrink_log')

			select @file = 0

			while @file < @file_max 
				begin
					select @file = @file + 1
					select @fileid = fileid from #logfiles where id = @file
					exec( 'dbcc shrinkfile( ' + @fileid + ' )' )
				end

			exec( 'backup log [' + @db + '] ' + @backup_log_opt )

			truncate table #loginfo 

			insert  #loginfo ( FileId, FileSize, StartOffset, FSeqNo, Status, Parity, CreateTime ) exec ( 'dbcc loginfo' )
			select  @last_row = @@rowcount

			select  @log_size = sum( FileSize ) / 1048576.00,
				@unused = sum( case when Status = 0 then FileSize else 0 end ) / 1048576.00,
				@shrinkable = sum( case when id < @last_row - 1 and Status = 0 then FileSize else 0 end ) / 1048576.00
				from #loginfo

			select  'iteration'          = @iteration,
				'log size, MB'       = @log_size,
				'unused log, MB'     = @unused,
				'shrinkable log, MB' = @shrinkable,
				'shrinkable %'       = convert( decimal(6,2), @shrinkable * 100 / @log_size )
		end

	if @unused1 < @unused 
		select	'After ' + convert( varchar, @iteration ) + 
			' iterations the unused portion of the log has grown from ' +
			convert( varchar, @unused1 ) + ' MB to ' +
			convert( varchar, @unused ) + ' MB.'
		union all
		select	'Since the remaining unused portion is larger than 10 MB,' where @unused > 10
		union all
		select	'you may try running this procedure again with a higher number of iterations.' where @unused > 10
		union all
		select	'Sometimes the log would not shrink to a size smaller than several Megabytes.' where @unused <= 10



	else
		select  'It took ' + convert( varchar, @iteration ) + 
			' iterations to shrink the unused portion of the log from ' +
			convert( varchar, @unused1 ) + ' MB to ' +
			convert( varchar, @unused ) + ' MB'

		exec( 'drop table table_to_force_shrink_log' )
Return(0)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[job_ReorganizeMainTables]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[job_ReorganizeMainTables]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




Create Procedure dbo.job_ReorganizeMainTables
	@object_name varchar(128) = null
as
/*
	Created by Andrey Sinkin
	Description: Reorganization data pages in given/(all main) table(s)
*/
	------------------------------------------------------
	set nocount on
	------------------------------------------------------

	-- Declaring variables
	declare @object_id int

	if @object_name is not null	-- for selected object
		begin
			set @object_id = object_id(@object_name)
			dbcc showcontig (@object_id)
			dbcc dbreindex (@object_name)
		end
	else				-- for all objects
		begin
			declare @tbl_name varchar(128)

			declare main cursor for
			select name from sysobjects where xtype = 'u' and status > 1

			open main
			fetch main into @tbl_name

			while @@fetch_status = 0
				begin
					exec job_ReorganizeMainTables @object_name = @tbl_name

					fetch main into @tbl_name
				end

			close main
			deallocate main


			exec sp_updatestats
		end
Return(0)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveSurface_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE            Procedure dbo.usp_ActiveSurface_Get 
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
		select @iContractID = contractID from ContractView where symbol = @vcSymbolName and  contractTypeID < 3

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
	set @iVolaSurfaceDataID = 0
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveSurface_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_ActiveSurface_Save
	@vcSymbolName varchar(12), --the contract symbol
	@iVolaSurfaceDataID int -- the VolaSurfaceData row ID
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure return default surface identifier
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
--clear isDefaultSurface Status
	update VolaSurfaceData 
		set isDefaultSurface=0
		from VolaSurfaceData 
				join Contract on VolaSurfaceData.contractID=Contract.contractID
		where Contract.symbol = @vcSymbolName
	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
--set isDefaultSurface Status
	update VolaSurfaceData 
		set isDefaultSurface=1
		from VolaSurfaceData 
				join Contract on VolaSurfaceData.contractID=Contract.contractID
		where Contract.symbol = @vcSymbolName and volaSurfaceDataID=@iVolaSurfaceDataID
	
	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	set @error = @@error
	Commit Transaction
    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockEOD_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE         proc dbo.usp_allStockEOD_Get
	@iFormat int = 1	-- 1 - list only
						-- 2 - list with prices by exchange
  as
	---------------
	set nocount on
	---------------
--=================================================================
-- select stocks for which trades was
--=================================================================
	if object_ID('tempdb..#SelContracts') is null
		Create table #SelContracts(contractID int not null)

	insert into #SelContracts
		select contractID 
			from Contract
		where 
		contractID in (select distinct O.underlyingContractID from OptionView O join TradeView TV on TV.contractID=O.optionID)
	union 
		select distinct contractID 
			from contract 
		where contractID in (select contractID from TradeView)
	
--=================================================================
--base section 
--=================================================================
	if @iFormat = 1
		select	iContractID, 
				iContractTypeID, 
				vcSymbol,
				tiCalcOptionType,
				tiCalcModelType,
				tiExpCalendarID,
				tiIsBasket
			from vALLStockEOD A
		where iContractID in (select contractID from #SelContracts)
	else
		-- use NULL exchange only
		select iContractID, 
				iContractTypeID, 
				vcSymbol, 
				null as vcExchangeCode,  
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceClose as fPriceClose,
				A.bIsHTB as bIsHTB,
				tiCalcOptionType,
				tiCalcModelType,
				tiExpCalendarID,
				tiIsBasket  
			from vALLStockEOD A, ContractPrice CP (nolock)
			where A.iContractID *= CP.contractID and CP.exchangeID is null
			and A.iContractID in (select contractID from #SelContracts)

if object_ID('tempdb..#SelContracts') is not null
	Drop table #SelContracts

if (@@error <> 0)
	Return (-1)
else  
   Return(0)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockOptionEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockOptionEOD_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE       proc dbo.usp_allStockOptionEOD_Get
	@iStockID int = null
as
	if @iStockID is null 
		-- all options
	 	 select	C.contractID as iContractID,
				C.Symbol as vcSymbol,  
				null as vcExchangeCode, 
				null as iExchangeID, 
				O.isCall as bIsCall, 
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry
         	from ContractView C (nolock)
					join OptionView O (nolock) on O.optionID = C.contractID
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
	   		where EC.expiryID > (select top 1 expiryID from ExpiryInCalendar where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc)
	else
	-- get list of NULL exchanges

	 	select C.contractID as iContractID,
				C.Symbol as vcSymbol,  
				null as vcExchangeCode, 
				null as iExchangeID, 
				O.isCall as bIsCall, 
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry
            from Contract C (nolock)
					join OptionView O (nolock) on O.optionID = C.contractID
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
	   		where O.underlyingContractID = @iStockID
				and EC.expiryID > (select top 1 expiryID from ExpiryInCalendar where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc)
		
Return(@@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Book_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   proc usp_Book_Del
		@iBookID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		declare @iPriority int
			select @iPriority = priority from Book where bookID=@iBookID
		update Book set priority = priority-1 where priority > @iPriority
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	
		update Book set actionID=3 where bookID = @iBookID
--		delete Book where bookID = @iBookID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Book_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE     proc usp_Book_Get
	@iBookID int = Null,
	@bInTrades bit = 0,
	@bShortFormat bit = 0,
	@dtActionDate datetime = null 
as
/*
	Edited by Zaur Nuraliev
	Description: this procedure is getting information from Book table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if (@iBookID is Null)
		begin
			if (@bInTrades = 0 and @bShortFormat=0)
				select
					bookID as iBookID,
					bookName as vcBookName,
					[Description] as vcDescription,
					priority as iPriority,
					actionDate as dtActionDate,
					actionID as tiActionID
				from BookView
				where
					(actionDate>=@dtActionDate or @dtActionDate is null)
					and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
				order by priority
			else if (@bInTrades = 0 and @bShortFormat=1)
				select
					bookID as iBookID,
					bookName as vcBookName,
					actionDate as dtActionDate,
					actionID as tiActionID
				from BookView
				where
					(actionDate>=@dtActionDate or @dtActionDate is null)
					and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
				order by bookName
			else
				select
					bookID as iBookID,
					bookName as vcBookName,
					[Description] as vcDescription,
					priority as iPriority,
					actionDate as dtActionDate,
					actionID as tiActionID
				from BookView
				where exists (select tr.bookID from TradeView tr  where tr.bookID = BookView.bookID)
					and (actionDate>=@dtActionDate or @dtActionDate is null)
					and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
				order by priority
		end
	else
		begin
			select
				bookID as iBookID,
				bookName as vcBookName,
				[Description] as vcDescription,
				priority as iPriority,
				actionDate as dtActionDate,
				actionID as tiActionID
			from BookView
			where bookID = @iBookID
				and (actionDate>=@dtActionDate or @dtActionDate is null)
				and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
		end
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Book_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE     proc usp_Book_Save
		@iBookID int = null,
		@vcBookName varchar(12) = null,
		@vcDescription varchar(120) = null
as
	----------------------------------------------------
	set nocount on
	----------------------------------------------------
	if exists (select * from Book where BookName = @vcBookName and actionID < 3 and (@iBookID <> bookID or @iBookID is null))
		begin
			Raiserror ('Book with this name already exists!', 16, 1)
			Return (-1)
		end   
	Declare @iPriority int
		select @iPriority = max(priority) + 1 from Book
	if @iPriority is null
		set @iPriority = 1
   
	if (@iBookID is Null)  
		begin
			insert into Book
				(bookName, [Description],priority)
			values
				(@vcBookName, @vcDescription, @iPriority)
	
			if (@@error <> 0)
				return (-1)
			else	
				return @@identity
		end
	else 
		begin
			update Book
			set bookName = @vcBookName,
				[Description] = @vcDescription
			where bookID = @iBookID
	
			if (@@error <> 0)
				return (-1)
			else  
				return 0
		end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRule_GetAll]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRule_GetAll]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE procedure usp_BookRule_GetAll
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select B.BookName as PortfolioName,
			BRI.ItemName , 
			M.MaskName as Mask, 
			T.itemTypeName as TypeName
			from BookView b
				join BookRuleItem BRI on B.bookID = BRI.bookID
				join RuleItemType T on T.ruleItemTypeID = BRI.ruleItemTypeID
				join BookRuleItemMask M on BRI.bookRuleItemID = M.bookRuleItemID
			order by B.Priority
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItem_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_BookRuleItem_Del 
	@iBookRuleItemID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from BookRuleItem table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete BookRuleItem	where bookRuleItemID = @iBookRuleItemID 
		
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItem_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_BookRuleItem_Get 
	@iBookID int = Null  
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from BookRuleItem table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 
		bookRuleItemID as iBookRuleItemID,
		itemName as vcItemName,
		BookRuleItem.ruleItemTypeID as iRuleItemTypeID,
		bookID as iBookID,
		itemTypeName as vcItemTypeName 
		from BookRuleItem
				join RuleItemType on BookRuleItem.ruleItemTypeID=RuleItemType.ruleItemTypeID
		where (BookRuleItem.bookID=@iBookID)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItem_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_BookRuleItem_Save 
	@iBookRuleItemID int = Null Output, --the row identifier
	@vcItemName varchar(255) = Null, --the name of item
	@iRuleItemTypeID int = Null,  --the type ID
	@iBookID int -- the parent table row identifier
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save information into BookRuleItem table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		if not exists(select * From BookRuleItem where bookRuleItemID = @iBookRuleItemID) 
			begin
				insert into BookRuleItem (bookID,ruleItemTypeID,itemName) 
					values (@iBookID,@iRuleItemTypeID,@vcItemName)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iBookRuleItemID = @@identity
			end
		else
			begin
				update BookRuleItem set 
					bookID = isnull(@iBookID, bookID),
					itemName = isnull(@vcItemName, itemName),
					ruleItemTypeID = isnull(@iRuleItemTypeID, ruleItemTypeID)
				where bookRuleItemID = @iBookRuleItemID
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItemMask_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItemMask_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_BookRuleItemMask_Del 
	@iBookRuleItemMaskID int 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from BookRuleItemMask table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete BookRuleItemMask	where bookRuleItemMaskID = @iBookRuleItemMaskID 
		
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItemMask_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItemMask_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_BookRuleItemMask_Get 
	@iBookRuleItemID int = Null 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from BookRuleItemMask table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 
		bookRuleItemMaskID as iBookRuleItemMaskID,
		bookRuleItemID as iBookRuleItemID,
		maskName as vcMaskName
		from BookRuleItemMask	
		where (bookRuleItemID = @iBookRuleItemID or @iBookRuleItemID is null)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItemMask_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItemMask_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_BookRuleItemMask_Save 
	@iBookRuleItemMaskID int = Null Output, --the row identifier
	@iBookRuleItemID int = Null,  			--the book rule item ID
	@vcMaskName varchar(255) = Null 		--the mask name
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into BookRuleItemMask
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		if not exists(select * From BookRuleItemMask where bookRuleItemMaskID = @iBookRuleItemMaskID) 
			begin
				insert into BookRuleItemMask (bookRuleItemID,maskName) 
				values (@iBookRuleItemID,@vcMaskName)
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iBookRuleItemMaskID = @@identity
			end
		else
			begin
				update BookRuleItemMask set 
					bookRuleItemID = isnull(@iBookRuleItemID,bookRuleItemID),
					maskName = isnull(@vcMaskName, maskName)
				where bookRuleItemMaskID = @iBookRuleItemMaskID
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookSwap_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookSwap_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



Create  proc usp_BookSwap_Save
		@iBook1ID int, 
		@iBook2ID int
as
/*
	Created by Zaur Nuraliev
	Description: this procedure saves priorities
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		Declare @iPriority1 int,@iPriority2 int
		select @iPriority1 = priority from BookView where bookID=@iBook1ID
		select @iPriority2 = priority from BookView where bookID=@iBook2ID
		update Book set priority = @iPriority1 where bookID=@iBook2ID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update Book set priority = @iPriority2 where bookID=@iBook1ID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Broker_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE        proc usp_Broker_Del
		@iBrokerID int
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Broker table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
--	delete Commission where brokerID = @iBrokerID
	update Commission set actionID=3 where brokerID = @iBrokerID
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
--	delete Trade where brokerID = @iBrokerID
	update Trade set actionID=0 where brokerID = @iBrokerID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	update Broker set actionID=3 where brokerID = @iBrokerID
--	delete Broker where brokerID = @iBrokerID
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Broker_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



create    proc dbo.usp_Broker_Get
	@tiClearing tinyint = Null,
	@iBrokerID int = Null,
	@bInTrades bit = 0,
	@dtActionDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Broker table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 	brokerID as iBrokerID,
			brokerName as vcBrokerName,
			[Description] as vcDescription,
			isClearing as tiIsClearing,
			actionDate as dtActionDate,
			actionID as tiActionID
			from Broker
			where (isClearing = @tiClearing or @tiClearing is null)
				and (brokerID = @iBrokerID or @iBrokerID is Null)
				and (exists(select tr.clearingBrokerID from Trade tr where tr.clearingBrokerID=Broker.brokerID) or @bInTrades = 0)
				and (actionDate>=@dtActionDate or @dtActionDate is null)
				and (@dtActionDate is not null or @dtActionDate is null and actionID<3)				
			order by BrokerName
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Broker_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    proc usp_Broker_Save
	   @iBrokerID int,
	   @vcBrokerName varchar(12) = null,
	   @vcDescription varchar(120) = null ,
	   @tiClearing tinyint 
  as
	if exists (select * from Broker where BrokerName = @vcBrokerName and actionID < 3 and (@iBrokerID <> brokerID or @iBrokerID is null))
		begin
			Raiserror ('Broker with this name already exists!', 16, 1)
			Return (-1)
		end   
	if @iBrokerID is null 
	--if not exists(select * from Broker where brokerID=@iBrokerID)	  
		begin
		   insert into Broker (BrokerName, [Description], isClearing)
			  values (@vcBrokerName, @vcDescription, @tiClearing)
		   
		   if (@@error <> 0)   
			  return (-1)
			else
			  return @@identity
		end
	   else
		begin 
		   update Broker
			   set BrokerName = @vcBrokerName,
					 [Description] = @vcDescription,
					 isClearing = @tiClearing
			where brokerID = @iBrokerID
		  
		   if (@@error <> 0)   
			  return (-1)
			else
			  return 0
		end 		 



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BrokerRate_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BrokerRate_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE proc usp_BrokerRate_Get
	@iBrokerID int,
	@dtValueDate smalldatetime
as
	set @dtValueDate = cast(floor(cast(@dtValueDate as float)) as datetime)
	select top 1
		commissionValue as fRate
	from CommissionView 
	where brokerID = @iBrokerID and commissionDate <= @dtValueDate
	order by commissionDate desc
/*	select
		IDENTITY(int, 1,1) AS ID,
		c.commissionDate,
		c.commissionValue
	into #s 
	from Commission c
	where c.brokerID = @iBrokerID
	order by c.commissionDate
	select @fRate = Rates.commissionValue
	from
		(select s1.commissionValue, s1.commissionDate 'Dt1', s2.commissionDate 'Dt2'
		from #s s1, #s s2
		where s2.id =* s1.id + 1) as Rates
	where (Rates.Dt1 <= @dtValueDate) and ((@dtValueDate < Rates.Dt2) or (Rates.Dt2 is Null))
	
	drop table #s */
	return @@error



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ClearingHouseTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ClearingHouseTrade_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



Create procedure usp_ClearingHouseTrade_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @Now integer
		exec usp_CurrentDate_Get @Now output
	select * from ClearingHouseTrade where iDateOfTrade = @Now
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ClearingHouseTrade_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ClearingHouseTrade_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



Create procedure usp_ClearingHouseTrade_Save
				 	@iCacheID     int ,
					@ins_upd_del_flag tinyint,
					@put_call	 tinyint,
					@buy_sell tinyint,
					@origin   tinyint,
					@open_close tinyint,
					@trade_id   varchar(10),
					@user_id    varchar(10),
					@trading_sym varchar(7),
					@base_sym	varchar(7),
					@parent_account varchar(11),
					@account varchar(11),
					@q_account varchar(11),
					@security_type varchar(3),
					@security_subtype varchar(3),
					@clearing_firm varchar(5),
					@cusip varchar(10),
					@basket_id varchar(6),
					@giveup_firm varchar(5),	
					@contra_firm varchar(5),
					@exec_broker varchar(4),
					@contra_broker varchar(4),
					@mmcs_sub_acct varchar(3),
					@exercise_multiplier float(53),
					@extended_premium float(53),
					@premium float(53),
					@strike float(53),
					@price float(53),
					@quantity int,
					@tag int,
					@expiration_date int,
					@execution_date int,
					@last_upd_date int,
					@contract_date int,
					@source_system int ,
					@trade_id_num int ,
					@trade_group_id int ,
					@settlement_date int,
					@entry_firm varchar(5),
					@matched_flag tinyint,
					@spread_ind tinyint,
					@premium_multiplier float(53),
					@source_code varchar(3),
					@parent_acct_intrl_id varchar(14),
					@acct_intrl_id varchar(14),
					@q_acct_intrl_id varchar(14),
					@cap_interval float(53),
					@trade_exchange int ,
					@settled_unsettled_ind tinyint,
					@stock_origin tinyint,
					@mm_card_number varchar(8),
					@basket_tag int ,
					@basket_price float(53),
					@short_sale tinyint,
					@commission_code varchar(3),
					@commission float(53),
					@same_day_exer tinyint,
					@terminal_name varchar(11),
					@portfolio_name	varchar(255)
as
	Declare @Now integer
		exec usp_CurrentDate_Get @Now output
if exists (select * from ClearingHouseTrade where iDateOfTrade = @Now and iCacheID = @iCacheID)
	Return(0)
insert into ClearingHouseTrade values
(
	@Now,
	@iCacheID,
	@ins_upd_del_flag,
	@put_call	 ,
	@buy_sell ,
	@origin  , 
	@open_close ,
	@trade_id   ,
	@user_id    ,
	@trading_sym ,
	@base_sym	,
	@parent_account ,
	@account ,
	@q_account ,
	@security_type ,
	@security_subtype ,
	@clearing_firm ,
	@cusip ,
	@basket_id ,
	@giveup_firm ,	
	@contra_firm ,
	@exec_broker ,
	@contra_broker ,
	@mmcs_sub_acct ,
	@exercise_multiplier ,
	@extended_premium ,
	@premium ,
	@strike ,
	@price ,
	@quantity ,
	@tag,
	@expiration_date ,
	@execution_date ,
	@last_upd_date ,
	@contract_date ,
	@source_system ,
	@trade_id_num ,
	@trade_group_id ,
	@settlement_date ,
	@entry_firm ,
	@matched_flag ,
	@spread_ind ,
	@premium_multiplier ,
	@source_code ,
	@parent_acct_intrl_id ,
	@acct_intrl_id ,
	@q_acct_intrl_id ,
	@cap_interval ,
	@trade_exchange ,
	@settled_unsettled_ind ,
	@stock_origin ,
	@mm_card_number ,
	@basket_tag ,
	@basket_price ,
	@short_sale ,
	@commission_code ,
	@commission ,
	@same_day_exer ,
	@terminal_name,
	@portfolio_name
)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ClearingHouseTradeByCacheID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ClearingHouseTradeByCacheID_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



Create procedure usp_ClearingHouseTradeByCacheID_Get
 	@iCacheID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @Now int
		exec usp_CurrentDate_Get @Now output
	select * 
		from ClearingHouseTrade 
		where iDateOfTrade = @Now and iCacheID = @iCacheID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Commission_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   proc usp_Commission_Del
		@iCommissionID int,
		@iBrokerID int = Null
	  
  as
	   if (@iCommissionID is Null) and (@iBrokerID is Null)
		  return (-2)
	
	   if (@iCommissionID is Null) 
			update Commission set actionID=3 where brokerID = @iBrokerID
		  --delete Commission where brokerID = @iBrokerID
		else
			update Commission set actionID=3 where commissionID = @iCommissionID
		  --delete Commission where commissionID = @iCommissionID
   if (@@error <> 0)
	  return (-1)
	else
	  return 0				   



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Commission_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   proc usp_Commission_Get
	@iBrokerID int = null,
	@iCommissionID int = null,
	@dtActionDate datetime = null
  as
	   if (@iCommissionID is Null)
		  begin
			  if (@iBrokerID is Null)
				 select commissionID as iCommissionID,
					brokerID as iBrokerID,
					commissionDate as dtCommissionDate,
					commissionValue as fCommissionValue,
					actionDate as dtActionDate,
					actionID as tiActionID 
				from Commission
				where	(actionDate>=@dtActionDate or @dtActionDate is null and actionID < 3)
				order by brokerID, commissionDate
			   else
					select commissionID as iCommissionID,
						brokerID as iBrokerID,
						commissionDate as dtCommissionDate,
						commissionValue as fCommissionValue,
						actionDate as dtActionDate,
						actionID as tiActionID 
					from Commission
					where brokerID = @iBrokerID and 
						(actionDate>=@dtActionDate or @dtActionDate is null and actionID < 3)
					order by commissionDate 	 
		  end
		else 
			select commissionID as iCommissionID,
					brokerID as iBrokerID,
					commissionDate as dtCommissionDate,
					commissionValue as fCommissionValue,
					actionDate as dtActionDate,
					actionID as tiActionID 
				from Commission 
				where commissionID = @iCommissionID and 
					(actionDate>=@dtActionDate or @dtActionDate is null and actionID < 3)
  Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Commission_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_Commission_Save
	   @iCommissionID int,
	   @iBrokerID int,
	   @dtCommissionDate smalldatetime,
	   @fCommissionValue float
	  
  as
	if exists (select * from Commission where brokerID = @iBrokerID and commissionDate = @dtCommissionDate and 
				actionID < 3 and (@iCommissionID <> commissionID or @iCommissionID is null))
		begin
			Raiserror ('Record with this commissionDate and Broker already exists!', 16, 1)
			Return (-1)
		end  
	   if (@iCommissionID is Null)
		  begin
			  insert into Commission (brokerID, commissionDate, commissionValue)
				 values (@iBrokerID, @dtCommissionDate, @fCommissionValue)
			 
			  if (@@error <> 0)  
				 return (-1)
			   else
				 return @@identity	
		  end  
		else
		  begin 	   
			  update Commission
				   set brokerID = @iBrokerID,
						 commissionDate = @dtCommissionDate,
						 commissionValue = @fCommissionValue 
			   where commissionID = @iCommissionID 	 
			  if (@@error <> 0)  
				 return (-1)
			   else
				 return 0
		  end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_Contract_Del
	@iContractID int	-- contract identifier
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
-- PLEASE CHECK THE LOGIC OF THIS PROCEDURE CAREFULLY !
	Begin Transaction
		delete StockDataHistory where stockID=@iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		--delete Dividend where stockID=@iContractID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		update Stock set actionID=3 where stockID=@iContractID
		--delete Stock where stockID=@iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		exec @error=usp_Option_Del @iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		update Contract set actionID=0 where contractID = @iContractID
		--delete Contract where contractID = @iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
finish:
Return(@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO




CREATE       Procedure dbo.usp_Contract_Get
	@iContractID int = null,	-- contract's identifier
	@iContractTypeID int = null,	-- contract's type identifier
	@bAllWithoutOptions bit = 1,	-- flag for data output
	@dtActionDate datetime = null
as
/*
	Created by Andrey Sinkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @bAllWithoutOptions is null		set @bAllWithoutOptions = 0

	if @bAllWithoutOptions = 1
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.calcOptionType is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType,
				case 
					when Stock.IsHTB is not null then Stock.IsHTB 
					else IndexView.IsHTB end bIsHTB
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where contractTypeID < 3
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	else
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.calcOptionType is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType,
				case 
					when Stock.IsHTB is not null then Stock.IsHTB 
					else IndexView.IsHTB end bIsHTB
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where (@iContractTypeID is null or contractTypeID = @iContractTypeID)
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	
Return(@@Error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





CREATE     proc usp_Contract_Save
	@iContractID int,
	@vcSymbol varchar(8),
	@vcContractName varchar(255),
	@iLotSize int,
	@fPriceBid float,
	@fPriceAsk float,
	@iExchangeID int,
	@fPriceLast float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iOpenInterest int

  as

-------------------------------------------------
set nocount on
-------------------------------------------------
  begin tran
	update Contract
		 set symbol = @vcSymbol,
			   contractName = CASE WHEN contractTypeID in (1, 2) THEN @vcContractName ELSE contractName END
	  where contractID = @iContractID
   if (@@error <> 0) begin rollback tran return @@error end  

	if exists(select * from ContractPrice where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID)	
		begin
			update ContractPrice 
				set lotSize = @iLotSize,
					priceBid = @fPriceBid,
					priceAsk = @fPriceAsk,
					priceLast = @fPriceLast,
					priceOpen = @fPriceOpen,
					priceClose = @fPriceClose,
					priceLow = @fPriceLow,
					priceHigh = @fPriceHigh,
					volume = @iVolume
				where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID
			   if (@@error <> 0) begin rollback tran return @@error end  
		end
	else 
		begin
			insert into ContractPrice (contractID,
					exchangeID,
					lotSize,
					priceBid,
					priceAsk,
					priceLast,
					priceOpen,
					priceClose,
					priceLow,
					priceHigh,
					volume)
			values (@iContractID,
					@iExchangeID,
					@iLotSize,
					@fPriceBid,
					@fPriceAsk,
					@fPriceLast,
					@fPriceOpen,
					@fPriceClose,
					@fPriceLow,
					@fPriceHigh,
					@iVolume)
		   if (@@error <> 0) begin rollback tran return @@error end  
		end

   if (@iOpenInterest is not Null)
	  begin
		declare @ctID int
		select @ctID = contractTypeID from Contract where contractID = @iContractID
		if (@ctID = 3)
		  begin
			update ContractPrice
				set openInterest = @iOpenInterest
				where contractID = @iContractID and isnull(exchangeID,0) = isnull(@iExchangeID,0)
			   if (@@error <> 0) begin rollback tran return @@error end  
		  end	
	  end
  commit tran

return @@error





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractByGroupID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractByGroupID_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_ContractByGroupID_Get 
	@iGroupID int =Null --the group identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure return information about 
				contract rule and group
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iGroupID Is Not Null 
		select	Contract.contractID as iContractID,
				Contract.symbol as vcSymbolName,
				GRG.groupID as iGroupID,
				Contract.ruleID as iRuleID 
			from ContractView Contract 
					join GenerationRuleGroup GRG on GRG.groupID=Contract.groupID
			where Contract.contractTypeID < 3 
					and GRG.groupID = @iGroupID
			order by Contract.symbol asc
	else
		select	Contract.contractID as iContractID,
				Contract.symbol as vcSymbolName,
				GRG.groupID as iGroupID,
				Contract.ruleID as iRuleID 
			from ContractView Contract 
					left join GenerationRuleGroup GRG on GRG.groupID=Contract.groupID
			where Contract.contractTypeID < 3 
			order by Contract.symbol asc
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractByID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractByID_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE	proc usp_ContractByID_Get 
		@iContractID int,
		@vcExchangeIDs varchar(8000) = Null --the Exchange identifier
as
/*
	Changed by Zaur Nuraliev
	[Description]: 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @vcExchangeIDs Is Null or @vcExchangeIDs=''
		begin
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID
				from ContractView c 
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where c.contractID = @iContractID  
						and C.contractTypeID in (1, 2)
			union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID
				from ContractView c  
						join OptionView O on O.optionID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractID = @iContractID  
					and C.contractTypeID not in (1, 2)
		end
	else
		begin
		--===================================================================================
		-- decompose ID's from varchat string
		--===================================================================================
			declare @vcExchangeID varchar(100),
					@i int
			create table #Exchange(exchangeID int not Null)
			
			select @vcExchangeID='', @i=1
			while @i <= len(@vcExchangeIDs) 
				begin
					if substring(@vcExchangeIDs,@i,1)<>'|' 
						begin
							set @vcExchangeID=@vcExchangeID + substring(@vcExchangeIDs,@i,1)
							if len(@vcExchangeIDs)=@i
								insert into #Exchange values(@vcExchangeID)
						end
					else --if substring(@vcExchangeIDs,@i,1)='|' 
						begin
							insert into #Exchange values(@vcExchangeID)
							set @vcExchangeID=''
						end
					set @i=@i+1
				end
			if len(@vcExchangeIDs)=1 
				insert into #Exchange values(@vcExchangeID)
		
		--===================================================================================
		-- make output recordset
		--===================================================================================
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID
				from ContractView c  
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractID = @iContractID  
						and C.contractTypeID in (1, 2) and exchangeID in (select ExchangeID from #Exchange)
			union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID
				from ContractView c  
						join OptionView O on O.optionID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractID = @iContractID  
						and c.contractTypeID not in (1, 2)
						and exchangeID in (select ExchangeID from #Exchange)
		end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractBySymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractBySymbol_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE	proc dbo.usp_ContractBySymbol_Get
		@vcSymbol varchar(8)
as
begin
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.priceTheo as fPriceTheo,
					CP.priceDate as fPriceDate,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName
			from ContractView c 
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where C.symbol = @vcSymbol	
					and c.contractTypeID in (1, 2)
	union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.priceTheo as fPriceTheo,
					CP.priceDate as fPriceDate,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName
			from ContractView c 
					join OptionView O on O.optionID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID				
		   where c.symbol = @vcSymbol  
					and c.contractTypeID not in (1, 2)
end







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractBySymbol_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractBySymbol_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE proc usp_ContractBySymbol_Save
	  @iContractID int,
	  @vcSymbol varchar(8)
	  
  as
begin 
	update Contract
		 set symbol = @vcSymbol
	  where contractID = @iContractID
	return @@error
end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractExchangeLink_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractExchangeLink_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE proc usp_ContractExchangeLink_Get
	@iContractID int = Null
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select contractID as iContractID,
			exchangeID as iExchangeID,
			exchangeLinkType as vcExchangeLinkType,
			ContractExchangeLinkID as iContractExchangeLinkID
		from ContractExchangeLink
		where contractID=@iContractID or (@iContractID is null)
		
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractForTrade_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE          proc dbo.usp_ContractForTrade_Get
	@iContractID int = null,
	@iUnderlyingID int = null,
	@tiNewOnly tinyint = 0,
	@tiExtendedOrder tinyint = 0
as
	declare @dtToday datetime
	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)
	if isnull(@tiExtendedOrder, 0) = 0
		select
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				null as fPriceTheoClose,
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				null as dtExpiry,
				null as fStrike,
				null as bIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.symbol as vcUnderlyingSymbol,
				cp.priceBid as fUnderlyingPriceBid,
				cp.priceAsk as fUnderlyingPriceAsk,
				cp.priceClose as fUnderlyingPriceClose,
				cp.lotSize as iUnderlyingLotSize
			from ContractView c  
				left join StockView S on S.stockID = C.contractID
				left join IndexView I on I.indexID = C.contractID
				left join ContractPrice CP on C.contractID = CP.contractID
			where	
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and c.contractID = @iUnderlyingID)
				and c.contractTypeID in (1, 2)
				and CP.exchangeID is null
		union
		select	
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				cp.priceTheo as fPriceTheoClose,
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol,
				ucp.priceBid as fUnderlyingPriceBid,
				ucp.priceAsk as fUnderlyingPriceAsk,
				ucp.priceClose as fPriceClose,
				ucp.lotSize as iUnderlyingLotSize
		from ContractView c  
				left join OptionView O on O.optionID = C.contractID
				left join Contract UC on O.underlyingContractID = UC.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
				and (@tiNewOnly = 0 or @tiNewOnly = 1 and EC.expiryDate >= @dtToday)
				and c.contractTypeID = 3
				and CP.exchangeID is null
				and ucp.exchangeID is null
		order by C.symbol
	else
		select * from 
		(select
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				null as fPriceTheoClose,
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				null as dtExpiry,
				null as fStrike,
				null as bIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.symbol as vcUnderlyingSymbol,
				cp.priceBid as fUnderlyingPriceBid,
				cp.priceAsk as fUnderlyingPriceAsk,
				cp.priceClose as fUnderlyingPriceClose,
				cp.lotSize as iUnderlyingLotSize
			from ContractView c  
				left join StockView S on S.stockID = C.contractID
				left join IndexView I on I.indexID = C.contractID
				left join ContractPrice CP on C.contractID = CP.contractID
			where	
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and c.contractID = @iUnderlyingID)
				and c.contractTypeID in (1, 2)
				and CP.exchangeID is null
		union
		select	
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				cp.priceTheo as fPriceTheoClose,
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol,
				ucp.priceBid as fUnderlyingPriceBid,
				ucp.priceAsk as fUnderlyingPriceAsk,
				ucp.priceClose as fPriceClose,
				ucp.lotSize as iUnderlyingLotSize
		from ContractView c  
				left join OptionView O on O.optionID = C.contractID
				left join Contract uc on o.underlyingContractID = uc.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
				and (@tiNewOnly = 0 or @tiNewOnly = 1 and EC.expiryDate >= @dtToday)
				and c.contractTypeID = 3
				and CP.exchangeID is null
				and ucp.exchangeID is null
		) as c order by year(c.dtExpiry), month(c.dtExpiry), c.fStrike, c.bIsCall desc

	Return(0)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGenerationRule_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGenerationRule_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE             Procedure dbo.usp_ContractGenerationRule_Get 
	@iContractID int --the contract identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure return ContractGenerationRule information 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		select 
			GR.ruleID as iRuleID,
			GR.ruleType as iRuleType,
			GR.ruleName as vcRuleName, 
			GR.interpolationFactor as fInterpolationFactor, 
			GR.callShift as fCallShift , 
			GR.putShift as fPutShift,
			GR.requiredPoints as iRequiredPoints,
			GR.smileAccelerator as fSmileAccelerator , 
			GR.strikeIncrement as iStrikeIncrement, 
			GR.strikeMoneynessLo as fStrikeMoneynessLo,
			GR.strikeMoneynessHi as fStrikeMoneynessHi,
			GR.isDefaultRule as bIsDefaultRule,
			case when
					exists(select * from ContractView c  join GenerationRuleGroup GRG on C.groupID=GRG.groupID 
							where GRG.ruleID=GR.RuleID and C.ContractID=@iContractID) then 1
				else 0
			end as bIsGroupRule,
			case when exists(select * from Contract where Contract.ruleID=GR.RuleID and contractID=@iContractID) then 1
				else 0
			end as bIsContractRule
		from GenerationRule GR
		order by ruleName 
	if @@error <> 0 begin set @error = @@error goto finish end
/*
	if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[result_values]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
		drop table [dbo].[result_values]
	if @@error <> 0 begin set @error = @@error goto finish end
	Create table  dbo.result_values(iRuleID int not null,
									iRuleType int null, 	
									vcRuleName varchar(250) null,
									fInterpolationFactor float null,
									fCallShift float null,
 									fPutShift float null,
									iRequiredPoints int null, 
									fSmileAccelerator float null, 
									iStrikeIncrement int null, 
									fStrikeMoneynessLo float null,
									fStrikeMoneynessHi float null, 
									bIsDefaultRule bit not null, 
									bIsGroupRule bit not null, 
									bIsContractRule bit not null)
		if @@error <> 0 begin set @error = @@error goto finish end
	-- Declare and using a READ_ONLY cursor
	Declare GenerationRule_Cursor Cursor
		READ_ONLY
		For Select ruleID from GenerationRule order by ruleName 
	
	Declare @iRuleID int
		Open GenerationRule_Cursor
	
	Fetch Next From GenerationRule_Cursor Into @iRuleID
	While (@@fetch_status <> -1)
	begin
		if (@@fetch_status <> -2)
		begin
			insert into result_values
			select 
				GR.ruleID as iRuleID,
				GR.ruleType as iRuleType,
				GR.ruleName as vcRuleName, 
				GR.interpolationFactor as fInterpolationFactor, 
				GR.callShift as fCallShift , 
				GR.putShift as fPutShift,
				GR.requiredPoints as iRequiredPoints,
				GR.smileAccelerator as fSmileAccelerator , 
				GR.strikeIncrement as iStrikeIncrement, 
				GR.strikeMoneynessLo as fStrikeMoneynessLo,
				GR.strikeMoneynessHi as fStrikeMoneynessHi,
				GR.isDefaultRule as bIsDefaultRule,
				case when
						exists(select * from ContractView c  join GenerationRuleGroup GRG on C.groupID=GRG.groupID 
								where GRG.ruleID=@iRuleID and C.ContractID=@iContractID) then 1
					else 0
				end as bIsGroupRule,
				case when exists(select * from Contract where Contract.ruleID=@iRuleID and contractID=@iContractID) then 1
					else 0
				end as bIsContractRule
			from GenerationRule GR
			where GR.ruleID=@iRuleID
		end
		Fetch Next From GenerationRule_Cursor into @iRuleID
	end
	
	Close GenerationRule_Cursor
	Deallocate GenerationRule_Cursor
		if @@error <> 0 begin set @error = @@error goto finish end
	select * from result_values
		if @@error <> 0 begin set @error = @@error goto finish end
	if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[result_values]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
		drop table [dbo].[result_values]
	if @@error <> 0 begin set @error = @@error goto finish end
*/
/*
	select distinct G.ruleID as iRuleID,
			ruleType as iRuleType,
			ruleName as vcRuleName, 
			interpolationFactor as fInterpolationFactor, 
			callShift as fCallShift , 
			putShift as fPutShift,
			requiredPoints as iRequiredPoints,
			smileAccelerator as fSmileAccelerator , 
			strikeIncrement as iStrikeIncrement, 
			strikeMoneynessLo as fStrikeMoneynessLo,
			strikeMoneynessHi as fStrikeMoneynessHi,
			isDefaultRule as bIsDefaultRule,
			Case when (CC.groupID is not null) and (GRGC.ruleID=G.ruleID) and (CC.ContractID=@iContractID) then 1
				else 0
			end as bIsGroupRule,
			Case when ((C.ruleID is not null) and (C.ContractID=@iContractID)) then 1
				else 0
			end as bIsContractRule
		from  GenerationRule G 
				left join ContractView c  on G.ruleID=C.ruleID
				left join GenerationRuleGroup GRGC on G.ruleID=GRGC.ruleID
				left join ContractView c C on (GRGC.groupID=CC.groupID and CC.ContractID=@iContractID)
		order by ruleName 
*/
    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroup_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroup_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_ContractGroup_Del
	@iGroupID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	update ContractGroup set actionID=3 where groupID = @iGroupID
	--delete ContractGroup where groupID = @iGroupID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroup_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_ContractGroup_Get
	@iGroupID int = null,
    @bShortFormat bit = 0,
	@dtActionDate datetime = null 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if (@iGroupID is Null and @bShortFormat = 0)
		select groupID as iGroupID, 
			groupName as vcGroupName, 
			[Description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
			from ContractGroup
			where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
			order by groupName
	else if (@iGroupID is Null and @bShortFormat = 1)
		select groupID as iGroupID, 
			groupName as vcGroupName,
			actionDate as dtActionDate,
			actionID as tiActionID 
			from ContractGroup
			where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
			order by groupName
	else 		 
		select groupID as iGroupID, 
			groupName as vcGroupName, 
			[Description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
			from ContractGroupView
			where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
				and groupID = @iGroupID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroup_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroup_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_ContractGroup_Save
	@iGroupID int out,
	@vcGroupName varchar(30),
	@vcDescription varchar(120)
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from ContractGroup where groupName = @vcGroupName and actionID < 3 and (@iGroupID <> groupID or @iGroupID is null))
		begin
			Raiserror ('Contract group with this name already exists!', 16, 1)
			Return (-1)
		end   
	if @iGroupID is Null 
		begin
			insert into ContractGroup (
					groupName,
					[Description]) 
				values (
					@vcGroupName,
					@vcDescription)
			select @iGroupID = @@identity
		end
	else
		begin
			update ContractGroup set
				groupName = isNull(@vcGroupName, groupName),
				[Description] = isNull(@vcDescription, [Description])
			where @iGroupID = groupID
		end
	set @error = @@error
	if (@error = 0) set @error = @iGroupID
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroupItem_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroupItem_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE     Procedure dbo.usp_ContractGroupItem_Save
	@iContractID int,		--the stock identifier
	@iGroupID int=Null		--the group identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save group identifier
			      
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iContractID is not null
	--if exists(select * from Contract where contractID=@iContractID)
		begin
			update Contract set groupID = @iGroupID
			where contractID=@iContractID
		end 
/*	else
		begin
			insert into VolaManagementData(contractID,basePoint,skewDelta,interpolationFactor,groupID)
				values(@iContractID,0,0,0,@iGroupID)
		end
*/
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractIDForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractIDForTrade_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



create proc usp_ContractIDForTrade_Get
		@iContractID int,
		@iUnderlyingID int
  as
		
	  if (@iContractID is Null)
		  if (@iUnderlyingID is Null)
			  select c.contractID as iContractID
				 from ContractView c , IndexView i
			   where c.contractTypeID = 1
				   and i.indexID = c.contractID
				 union
			  select c.contractID as iContractID
				 from ContractView c , Stock s
			   where c.contractTypeID = 2
				   and s.stockID = c.contractID				
				 union
			  select c.contractID as iContractID
				 from ContractView c , OptionView o
			   where c.contractTypeID = 3
				   and o.optionID = c.contractID
		  else
			  select c.contractID as iContractID
				 from ContractView c , IndexView i
			   where c.contractID = @iUnderlyingID
				   and c.contractTypeID = 1
				   and i.indexID = c.contractID
				 union
			  select c.contractID as iContractID
				 from ContractView c , Stock s
			   where c.contractID = @iUnderlyingID
				  and c.contractTypeID = 2
				  and s.stockID = c.contractID			   
				 union
			  select c.contractID as iContractID
				 from ContractView c , OptionView o
			   where o.underlyingContractID = @iUnderlyingID
				  and o.optionID = c.contractID
				  and c.contractTypeID = 3
	  else
			  select c.contractID as iContractID
				 from ContractView c , IndexView i
			   where c.contractID = @iContractID
				   and c.contractTypeID = 1
				   and i.indexID = c.contractID
				 union
			  select c.contractID as iContractID
				 from ContractView c , Stock s
			   where c.contractID = @iContractID
				   and c.contractTypeID = 2
				   and s.stockID = c.contractID				
				 union
			  select c.contractID as iContractID
				 from ContractView c , OptionView o
			   where c.contractID = @iContractID
				   and c.contractTypeID = 3
				   and o.optionID = c.contractID
		
		   Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE     Procedure dbo.usp_ContractInExchange_Del 
	@iExchangeID int,        --the exchangeID
	@iContractID int   	 --the contractID
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure remove exchange link type data from ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		delete from ContractExchangeLink where  contractID=@iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
    set @error = @@Error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE      procedure usp_ContractInExchange_Get
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE     Procedure dbo.usp_ContractInExchange_Save 
	@iExchangeID int,        --the exchangeID
	@iContractID int,   	 --the contractID
	@itype1 bit=null, 	--the link type 1
	@itype2 bit=null 	--the link type 2
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @itype1 is null 
		select @itype1=left(exchangeLinkType,1) from ContractExchangeLink where contractID=@iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
	set @itype1=isnull(@itype1,0)
	if @itype2 is null 
		select @itype2=right(exchangeLinkType,1) from ContractExchangeLink where contractID=@iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
	set @itype2=isnull(@itype2,0)
	declare @vcExchangeLinkType varchar(2)
		set @vcExchangeLinkType = convert(varchar(1),@itype1) + convert(varchar(1),@itype2)
--select @vcExchangeLinkType
	Begin Transaction
		if not exists(select * from ContractExchangeLink where isnull(exchangeID,0)=isnull(@iExchangeID,0) and contractID=@iContractID) 
			begin
				insert into ContractExchangeLink (contractID,exchangeID,exchangeLinkType) 
					values (@iContractID,@iExchangeID,@vcExchangeLinkType)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update ContractExchangeLink set 
					exchangeLinkType = @vcExchangeLinkType
				where @iContractID = @iContractID and isnull(exchangeID,0) =isnull(@iExchangeID,0)
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroup_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO



CREATE   PROCEDURE dbo.usp_ContractInGroup_Get
	@iGroupID int = null,
	@iContractID int = null,
	@dtActionDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractInGroup table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 	groupID iGroupID, 
			contractID iContractID,
			actionDate as dtActionDate,
			actionID as tiActionID
		from ContractInGroupView
		where ((@iGroupID is null or @iGroupID is not null and groupID = @iGroupID)
			and (@iContractID is null or @iContractID is not null and contractID = @iContractID))
			and (actionDate>=@dtActionDate or @dtActionDate is null)
			and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroupByContract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroupByContract_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_ContractInGroupByContract_Del
		  @iContractID int,
		  @iGroupID int = Null
 as
begin
   if (@iContractID is Null) return (-2)
   if (@iGroupID is Null)	 
		update ContractInGroup set actionID=3 where contractID = @iContractID
	  --delete ContractInGroup where contractID = @iContractID
	else
		update ContractInGroup set actionID=3 where contractID = @iContractID and groupID = @iGroupID
	  --delete ContractInGroup where contractID = @iContractID and groupID = @iGroupID
 
   if (@@error <> 0)
	  return (-1)
   else
	  return 0	 
end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroupByContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroupByContract_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE proc usp_ContractInGroupByContract_Get
		  @iContractID int
as
  select g.groupID as iGroupID,
	g.groupName as vcGroupName,
	CASE WHEN cig.contractID is Null THEN 0 ELSE 1 END as iMark
	   from ContractGroupView g, ContractInGroupView cig
	 where cig.contractID = @iContractID
		and cig.groupID =* g.groupID
	  order by g.groupName
  Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroupByContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroupByContract_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   proc usp_ContractInGroupByContract_Save
		  @iContractID int,
		  @iGroupID int
 as
	if exists (select * from ContractInGroup  where contractID = @iContractID and groupID = @iGroupID and actionID < 3)
		begin
			--Raiserror ('Row with same identifiers already exists!', 16, 1)
			Return (-1)
		end   
   insert into ContractInGroup (contractID, groupID) values (@iContractID, @iGroupID)
   if (@@error <> 0)
	  return (-1)
   else
	  return 0	 



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractMarketData_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractMarketData_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_ContractMarketData_Del
	@iContractID int		-- Contract's identifier
as
/*
	Created by Andrey Sinkin
	It deletes data from ContractMarketData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	delete ContractMarketData where contractID = @iContractID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractMarketData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractMarketData_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_ContractMarketData_Get
	@iContractID int		-- Contract's identifier
as
/*
	Created by Andrey Sinkin
	It returns data from ContractMarketData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	select contractID icontractID, [date] dtDate, priceMid fPriceMid
		from ContractMarketData
		where contractID = @iContractID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractMarketData_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractMarketData_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_ContractMarketData_Save
	@iContractID int,		-- Contract's identifier
	@dtDate datetime,	-- date
	@fPriceMid float	-- middle price
as
/*
	Created by Andrey Sinkin
	It deletes data from ContractMarketData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @rowcount int
	if @dtDate is null		set @dtDate = getdate()
	Begin Transaction
		update ContractMarketData set priceMid = @fPriceMid, date = @dtDate where @iContractID = contractID
		select @error = @@error, @rowcount = @@rowcount
		if @error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		if @rowcount = 0
			begin
				insert into ContractMarketData (contractID, date, priceMid) values (@iContractID, @dtDate, @fPriceMid)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end			
			end
		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPositionForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPositionForRisk_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE proc usp_ContractPositionForRisk_Get
		@iContractID int,
		@iBookID int,
		@iStrategyID int,
		@iTraderID int,
		@iUnderlyingID int = Null,
		@dtTradeDate datetime = Null,
		@tiIsClose tinyint = Null
as
begin
  if (@iContractID is Null)
	 select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		SUM(t.quantity * t.lotSize) as iQuantity
	   from TradeView t, ContractView c 
	  where (@iBookID is Null or @iBookID is not Null and t.bookID = @iBookID)
		and (@iStrategyID is Null or @iStrategyID is not Null and t.strategyID = @iStrategyID)
		and (@iTraderID is Null or @iTraderID is not Null and t.traderID = @iTraderID)
		and (@iUnderlyingID is Null or @iUnderlyingID is not Null and t.contractID = @iUnderlyingID)
		and c.contractID = t.contractID
		and c.contractTypeID in (1, 2)
		and (@dtTradeDate is Null or @dtTradeDate is not Null and t.TradeDate = @dtTradeDate)
		and (@tiIsClose is Null or @tiIsClose is not Null and t.isClose = @tiIsClose)
	  group by c.contractID, c.contractTypeID, c.symbol 
	 union
	 select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		SUM(t.quantity * t.lotSize) as iQuantity
	  from TradeView t, OptionView o, ContractView c 
	  where (@iBookID is Null or @iBookID is not Null and t.bookID = @iBookID)
		and (@iStrategyID is Null or @iStrategyID is not Null and t.strategyID = @iStrategyID)
		and (@iTraderID is Null or @iTraderID is not Null and t.traderID = @iTraderID)
	        and o.optionID = t.contractID
		and (@iUnderlyingID is Null or @iUnderlyingID is not Null and o.underlyingContractID = @iUnderlyingID)
		and c.contractID = t.contractID	   
		and (@dtTradeDate is Null or @dtTradeDate is not Null and t.TradeDate = @dtTradeDate)
		and (@tiIsClose is Null or @tiIsClose is not Null and t.isClose = @tiIsClose)
	 group by c.contractID, c.contractTypeID, c.symbol 
	else
	 select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		SUM(t.quantity * t.lotSize) as iQuantity
	   from ContractView c , Trade t
	  where c.contractID = @iContractID
		and c.contractTypeID in (1, 2)
		and t.contractID = c.contractID
	        and (@iBookID is Null or @iBookID is not Null and t.bookID = @iBookID)
		and (@iStrategyID is Null or @iStrategyID is not Null and t.strategyID = @iStrategyID)
		and (@iTraderID is Null or @iTraderID is not Null and t.traderID = @iTraderID)
		and (@dtTradeDate is Null or @dtTradeDate is not Null and t.TradeDate = @dtTradeDate)
		and (@tiIsClose is Null or @tiIsClose is not Null and t.isClose = @tiIsClose)
	  group by c.contractID, c.contractTypeID, c.symbol
	 union
	 select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		SUM(t.quantity * t.lotSize) as iQuantity
	  from OptionView o, ContractView c , Trade t
	 where o.underlyingContractID = @iContractID
	   and c.contractID = o.optionID
	   and t.contractID = o.optionID	   
	        and (@iBookID is Null or @iBookID is not Null and t.bookID = @iBookID)
		and (@iStrategyID is Null or @iStrategyID is not Null and t.strategyID = @iStrategyID)
		and (@iTraderID is Null or @iTraderID is not Null and t.traderID = @iTraderID)
		and (@dtTradeDate is Null or @dtTradeDate is not Null and t.TradeDate = @dtTradeDate)
		and (@tiIsClose is Null or @tiIsClose is not Null and t.isClose = @tiIsClose)
	 group by c.contractID, c.contractTypeID, c.symbol
end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPrice_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_ContractPrice_Get 
	@iContractPriceID int = Null output,
	@iContractID int = Null,
	@iExchangeID int = Null 
AS
/*
	Created by Zaur Nuraliev
	[Description]:	this procedure save information into 
					ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	contractID as iContractID,
			exchangeID as iExchangeID, 
			priceBid as fPriceBid,
			priceAsk as fPriceAsk,
			lotSize as iLotSize,
			priceLast as fPriceLast,
			priceOpen as fPriceOpen,
			priceClose as fPriceClose,
			priceLow as fPriceLow,
			priceHigh as fPriceHigh,
			volume as iVolume,
			openInterest as iOpenInterest
			from ContractPrice	
			where	(@iContractPriceID is not Null and contractPriceID = @iContractPriceID) or 
					(@iContractID is not Null and contractID=@iContractID and exchangeID=@iExchangeID)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPrice_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPrice_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    proc usp_ContractPrice_Save
	@iContractPriceID int = Null output,
	@iContractID int = Null,
	@iExchangeID int = Null, 
	@fPriceBid float = Null,
	@fPriceAsk float = Null,
	@iLotSize int = Null,
	@fPriceLast float = Null,
	@fPriceOpen float = Null,
	@fPriceClose float = Null,
	@fPriceLow float = Null,
	@fPriceHigh float = Null,
	@iVolume int = Null,
	@iOpenInterest int = Null,
	@vcSymbol varchar(8) = Null,
	@iContractTypeID int = Null, --2-stock 3-option
	@vcExchangeCode varchar(10) = Null
as
/*
	Created by Zaur Nuraliev
	[Description]:	this procedure save information into 
					ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	set ANSI_NULLS OFF --(exchangeID may be null)
	-------------------------------------------------
	declare @error int		set @error = -1
	if @vcSymbol is not null and @iContractTypeID is not null 
		begin
			Select @iContractID = contractID from Contract where symbol=@vcSymbol and contractTypeID=@iContractTypeID
			if @iContractID is null goto finish 
		end
	if @vcExchangeCode is not null
		begin
			Select @iExchangeID=exchangeID from Exchange where exchangeCode=@vcExchangeCode
		end
	Begin Transaction
		if (not exists (select * from ContractPrice where contractPriceID = @iContractPriceID) and @iContractPriceID is not Null)
		or (@iContractID is not Null and not exists(select * from ContractPrice where contractID=@iContractID and isnull(exchangeID,0)=isnull(@iExchangeID,0)))
		 begin
			insert into ContractPrice (contractID,
						exchangeID, 
						priceBid,
						priceAsk,
						lotSize,
						priceLast,
						priceOpen,
						priceClose,
						priceLow,
						priceHigh,
						volume,
						openInterest)
				  values (@iContractID,
						@iExchangeID, 
						@fPriceBid,
						@fPriceAsk,
						@iLotSize,
						@fPriceLast,
						@fPriceOpen,
						@fPriceClose,
						@fPriceLow,
						@fPriceHigh,
						@iVolume,
						@iOpenInterest)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			select @iContractPriceID = @@identity
		 end
	   else
		 begin
			update ContractPrice
				set contractID = isNull(@iContractID,contractID),
					exchangeID = isNull(@iExchangeID,exchangeID),
					priceBid = isNull(@fPriceBid,priceBid),
					priceAsk = isNull(@fPriceAsk,priceAsk),
					lotSize = isNull(@iLotSize,lotSize),
					priceLast = isNull(@fPriceLast,priceLast),
					priceOpen = isNull(@fPriceOpen,priceOpen),
					priceClose = isNull(@fPriceClose,priceClose),
					priceLow = isNull(@fPriceLow,priceLow),
					priceHigh = isNull(@fPriceHigh,priceHigh),
					volume = isNull(@iVolume,volume),
					openInterest = isNull(@iOpenInterest,openInterest),
					priceDate = GetDate()
					where (@iContractPriceID is not Null and contractPriceID = @iContractPriceID) or 
						(@iContractID is not Null and contractID=@iContractID and exchangeID=@iExchangeID)
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
		--===========================================================================================
		-- create record with Null ExchangeID
		--===========================================================================================
		if not exists(select * from ContractPrice where ContractID=@iContractID and exchangeID is null)
			begin
				insert into ContractPrice (contractID,
							exchangeID, 
							priceBid,
							priceAsk,
							lotSize,
							priceLast,
							priceOpen,
							priceClose,
							priceLow,
							priceHigh,
							volume,
							openInterest)
					  values (@iContractID,
							Null, 
							@fPriceBid,
							@fPriceAsk,
							@iLotSize,
							@fPriceLast,
							@fPriceOpen,
							@fPriceClose,
							@fPriceLow,
							@fPriceHigh,
							@iVolume,
							@iOpenInterest)
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceEOD_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceEOD_Complete_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




Create procedure dbo.usp_ContractPriceEOD_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	Begin tran 
	   if object_id('tempdb..##ContractPriceEOD') is null
		   begin
		       raiserror ( 'Cannot complete EOD. Cannot find save data.', 16, 1)
		       return (-1)
		   end
	
		select	C.contractID, 
				E.exchangeID, 
				A.priceClose 
			into #temp
			from ##ContractPriceEOD A, Contract C (nolock), Exchange E (nolock)
			where A.symbol = C.symbol and A.exchangeCode *= E.exchangeCode and C.actionID < 3
	       
		update ContractPrice
			set contractID = S.contractID, 
				exchangeID = S.exchangeID, 
				priceClose = S.priceClose, 
				priceDate = getdate()
		from ContractPrice CP join #temp S on CP.contractID = S.contractID and isnull(CP.exchangeID, 0) = isnull(S.exchangeID, 0)
			--where Z.priceDate between convert(datetime, convert(varchar(15), GetDate(), 112), 112) and
			--               	convert(datetime, convert(varchar(15), GetDate()+1, 112), 112)
	
		insert into ContractPrice (contractID, exchangeID, priceClose, priceDate)
			select *, getdate()
				from #temp B
				where not exists(select * from ContractPrice C where C.contractID = B.contractID and isnull(C.exchangeID, 0) = isnull(B.exchangeID, 0))
	
	Commit tran

	if object_id('tempdb..##ContractPriceEOD') is not null
		drop table ##ContractPriceEOD

	set @error = @@error
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceEOD_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceEOD_Init_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



Create procedure dbo.usp_ContractPriceEOD_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if object_id('tempdb..##ContractPriceEOD') is not null
	begin
		raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
		return (-1) 
		-- drop table ##ContractPriceEOD
	end
	create table ##ContractPriceEOD(
		symbol varchar (8) null ,
		contractTypeID int not null,
		priceClose float not null,
		exchangeCode char(10) null) 
	set @error = @@error
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceEOD_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceEOD_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   procedure dbo.usp_ContractPriceEOD_Save
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceTheo_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceTheo_Complete_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   procedure dbo.usp_ContractPriceTheo_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	Begin tran 
		if object_id('tempdb..##ContractPriceTheo') is null
		   begin
		       raiserror ( 'Cannot complete EOD. Cannot find save data of theo price.', 16, 1)
		       return (-1)
		   end
	
		update ContractPrice
			set priceTheo = S.priceTheo,
				priceDate = getdate()
		from ContractPrice CP join ##ContractPriceTheo S on CP.contractID = S.contractID and CP.exchangeID is null
--		from ContractPrice CP join ##ContractPriceTheo S on CP.contractID = S.contractID and isnull(CP.exchangeID, 0) = isnull(S.exchangeID, 0)

/*		insert into ContractPrice (contractID, exchangeID, priceTheo, priceDate)
			select contractID, exchangeID, priceTheo, getdate()
				from ##ContractPriceTheo B
				where not exists(select * from ContractPrice C where C.contractID = B.contractID and isnull(C.exchangeID, 0) = isnull(B.exchangeID, 0))
*/

	Commit tran

	if object_id('tempdb..##ContractPriceTheo') is not null
		drop table ##ContractPriceTheo

	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceTheo_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceTheo_Init_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE  procedure dbo.usp_ContractPriceTheo_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if object_id('tempdb..##ContractPriceTheo') is not null
	begin
		raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
		return (-1) 
		-- drop table ##ContractPriceTheo
	end

	--create table ##ContractPriceTheo(contractID int, exchangeID int null, priceTheo float Null)
		create table ##ContractPriceTheo(contractID int, priceTheo float Null)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractStockAndIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractStockAndIndex_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE proc usp_ContractStockAndIndex_Get
	 @iGroupID int,
	 @bIsAll bit,
	 @iContractTypeID int	 
	   
  as
		 
   if (@iGroupID is Null)
	  if (@bIsAll is not Null)	and   (@bIsAll > 0)
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				S.defaultModelTypeID as iDefaultModelTypeID,
				S.defaultSurfaceTypeID as iDefaultSurfaceTypeID,
				S.divFreq as iDivFreq, 
				S.baseIndexID as iBaseIndexID,
				Null as fYield
					from ContractView c 
							join Stock S on S.stockID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 2) or (@iContractTypeID is Null))
							and C.contractTypeID = 2
			union
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				I.defaultModelTypeID as iDefaultModelTypeID,
				Null as iDefaultSurfaceTypeID,
				Null as iDivFreq, 
				Null as iBaseIndexID,
				i.yield as fYield
					from ContractView c 
							join IndexView I on I.indexID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 1) or (@iContractTypeID is Null))
							and c.contractTypeID = 1
					order by c.symbol	
	   else
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				S.defaultModelTypeID as iDefaultModelTypeID,
				S.defaultSurfaceTypeID as iDefaultSurfaceTypeID,
				S.beta as fBeta,
				S.divFreq as iDivFreq, 
				S.baseIndexID as iBaseIndexID,
				Null as fYield
					from ContractView c  
							join Stock S on S.stockID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 2) or (@iContractTypeID is Null))
						and c.contractTypeID = 2
						and not exists (select * from ContractsInGroup where contractID = c.contractID)
		  union
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				I.defaultModelTypeID as iDefaultModelTypeID,
				Null as iDefaultSurfaceTypeID,
				Null as fBeta,
				Null as iDivFreq, 
				Null as iBaseIndexID,
				i.yield as fYield
					from ContractView c  
							join IndexView I on I.indexID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 1) or (@iContractTypeID is Null))
							and c.contractTypeID = 1
							and not exists (select * from ContractsInGroup where contractID = c.contractID) 
					order by c.symbol	
	else
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				S.defaultModelTypeID as iDefaultModelTypeID,
				S.defaultSurfaceTypeID as iDefaultSurfaceTypeID,
				S.beta as fBeta,
				S.divFreq as iDivFreq, 
				S.baseIndexID as iBaseIndexID,
				Null as fYield
			 from ContractsInGroup CIG
					join ContractView c  on C.contractID = CIG.contractID
					join Stock S on S.stockID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
			 where CIG.groupID = @iGroupID
					   and c.contractTypeID = 2
	   union
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				I.defaultModelTypeID as iDefaultModelTypeID,
				Null as iDefaultSurfaceTypeID,
				Null as fBeta,
				Null as iDivFreq, 
				Null as iBaseIndexID,
				i.yield as fYield
				from ContractsInGroup CIG
						join ContractView c  on C.contractID = CIG.contractID
						join IndexView I on I.indexID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
				where CIG.groupID = @iGroupID
						and c.contractTypeID = 1
				order by c.symbol  
Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractSymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractSymbol_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc dbo.usp_ContractSymbol_Get
	  @iGroupID int,
	  @bIsAll bit,
	  @bStockOnly bit,   
	  @bIndexesOnly bit,
	  @bOptionsOnly bit 
	   
  as
	 if (@iGroupID is Null)
		if (@bIsAll is not Null)  and	(@bIsAll > 0)
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, 
					C.symbol as vcSymbol, S.calcOptionType as tiCalcOptionType
				from ContractView c  
						join Stock S on S.stockID = C.contractID
				where ((@bStockOnly is not Null) and (@bStockOnly > 0))
				   and c.contractTypeID = 2
		  union
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, 
					C.symbol as vcSymbol, I.calcOptionType as tiCalcOptionType
				 from ContractView c  
						join IndexView I on I.indexID = C.contractID
				 where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
						 and c.contractTypeID = 1
		  union 
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, 
					C.symbol as vcSymbol, null as tiCalcOptionType
				 from ContractView c 
						join OptionView O on O.optionID = C.contractID
				  where ((@bOptionsOnly is not Null) and (@bOptionsOnly > 0)) 
						  and c.contractTypeID in (3, 4)
			 order by c.symbol	 
	   else
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol, S.calcOptionType as tiCalcOptionType
					from ContractView c  
							join Stock S on s.stockID = c.contractID
					where ((@bStockOnly is not Null) and (@bStockOnly > 0)) 
						   and c.contractTypeID = 2
						   and not exists (select * from ContractInGroup where contractID = c.contractID)
		  union
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol, I.calcOptionType as tiCalcOptionType
					from ContractView c  
							join IndexView I on I.indexID = C.contractID
					where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
						 and C.contractTypeID = 1
						 and not exists (select * from ContractInGroup where contractID = c.contractID)
					 order by C.symbol 
	  else
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol, S.calcOptionType as tiCalcOptionType
				from ContractInGroup CIG
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
				where CIG.groupID = @iGroupID
					   and ((@bStockOnly is not Null) and (@bStockOnly > 0))	
					   and c.contractTypeID = 2
		 union
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol, I.calcOptionType as tiCalcOptionType
				 from ContractInGroup CIG
						join ContractView c  on C.contractID = CIG.contractID
						join IndexView I on I.indexID = C.contractID
				 where CIG.groupID = @iGroupID
						 and ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
						 and c.contractTypeID = 1
				 order by c.symbol			 
Return (0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractSymbolForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractSymbolForRisk_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE proc usp_ContractSymbolForRisk_Get
		@iBookID int,
		@iStrategyID int,
		@iTraderID int,
		@iUnderlyingID int 
as
begin
  select c.contractID as iContractID,
	c.symbol as vcSymbol
	 from Trade t, ContractView c 
	where ((@iBookID is Null) or ((@iBookID is not Null) and (t.bookID = @iBookID)))
	  and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (t.strategyID = @iStrategyID)))
	  and ((@iTraderID is Null) or ((@iTraderID is not Null) and (t.traderID = @iTraderID)))
	  and ((@iUnderlyingID is Null) or (@iUnderlyingID is not Null) and (t.contractID = @iUnderlyingID))
	  and c.contractID = t.contractID
	  and c.contractTypeID in (1, 2)
   union
  select c.contractID as iContractID,
	  c.symbol as vcSymbol
	 from Trade t, OptionView o, ContractView c 
	where ((@iBookID is Null) or ((@iBookID is not Null) and (t.bookID = @iBookID)))
	  and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (t.strategyID = @iStrategyID)))
	  and ((@iTraderID is Null) or ((@iTraderID is not Null) and (t.traderID = @iTraderID)))
	  and o.optionID = t.contractID
	  and ((@iUnderlyingID is Null) or (@iUnderlyingID is not Null) and (o.underlyingContractID  = @iUnderlyingID))
	  and c.contractID = o.underlyingContractID 
end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractVM_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO



CREATE Procedure dbo.usp_ContractVM_Save 
	@iContractID int = null output,		-- contract's identifier
	@iContractTypeID int = null,		-- contract's type identifier
	@vcSymbolName varchar(12) = null,	-- index's ticker
	@iExchangeID int = null,		-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,			-- lot size (not used)
					-- Stock's data:
	@iDivFrequency int = null,		-- dividend's frequency
	@fAmount float = null, 			-- amount
	@dDivDate datetime = null,		-- dividend's date
					-- Index's data:
	@fYield float = null,			-- yield
	@bIsAutoUpdateWeight bit = null,	-- flag for enable/disable index's weights updating
					-- ETFs data:
	@iIndexBaseID int = null,		-- index's base identifier
	@rDenominator real = null,		-- denominator
	@bIsActive bit = null,			-- flag for enable/disable
					-- Option's data:
	@iUnderlyingID int = null,		-- underlying identifier
	@iIsCall bit = null,			-- flag for call/put
	@fStrike float = null,			-- strike
	@dtExpDate datetime = null,		-- expiration's date
	@iGroupID int = null,
	@iRuleID int = null
as
/*
*/
	-----------------------------------------------------------
	set nocount on
	-----------------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int, @iStockID int, @iOptionID int
	-- checking for wrong parameters
	if @iContractTypeID is null or @iContractTypeID not in (1,2,3)		goto finish

	if @iContractTypeID = 1	-- index
		exec @error = usp_IndexVM_Save
				@iIndexID = @iContractID output,		-- index's identifier
				@iContractTypeID = @iContractTypeID,		-- contract's type identifier
				@iSymbolName = @vcSymbolName,			-- index's ticker
				@iExchangeID = @iExchangeID,			-- exchange's identifier
				@vcDescription = @vcDescription,		-- description
				@iLotSize = 0,					-- lot size (not used)
			
				@fYield = @fYield,				-- yield
				@bIsAutoUpdateWeight = @bIsAutoUpdateWeight,	-- flag for enable/disable index's weights updating
				@iGroupID = @iGroupID,
				@iRuleID = @iRuleID
--				@iIndexBaseID = null,				-- index's base identifier
--				@rDenominator = null,				-- denominator
--				@bIsActive = null				-- flag for enable/disable
	else
	if @iContractTypeID = 2	-- stock
		exec @error = usp_StockVM_Save
				@iStockID = @iContractID output,		-- stock's identifier
			
				@iContractTypeID = @iContractTypeID,		-- contract's type identifier (not used)
				@vcSymbolName = @vcSymbolName,			-- stock's ticker
				@iExchangeID = @iExchangeID,			-- exchange's identifier
				@vcDescription = @vcDescription,		-- description
				@iLotSize = 0,					-- lot size (not used)
			
				@iDivFrequency = @iDivFrequency,		-- dividend's frequency
				@fAmount = @fAmount, 				-- amount
				@dDivDate = @dDivDate,				-- dividend's date
				@iGroupID = @iGroupID,
				@iRuleID = @iRuleID
/*
	else
	if @iContractTypeID = 3	-- option
		exec @error = usp_OptionVM_Save
				@iOptionID = @iContractID output,	-- option's identifier
			
				@iContractTypeID = @iContractTypeID,-- contract's type identifier
				@iSymbolName = @vcSymbolName,		-- option's ticker
				@iExchangeID = @iExchangeID,		-- exchange's identifier
				@vcDescription = @vcDescription,	-- description
				@iLotSize = @iLotSize,				-- lot's size
			
				@iUnderlyingID = @iUnderlyingID,	-- underlying identifier
				@iIsCall = @iIsCall,				-- flag for call/put
				@fStrike = @fStrike,				-- strike
				@dtExpDate = @dtExpDate,			-- expiration's date
				@iGroupID = @iGroupID,
				@iRuleID = @iRuleID
*/
--====================================================================
-- update ruleID, groupID fields
--====================================================================
	if @iContractID is not null
	--if exists(select * from Contract where contractID=@iContractID)
		begin
			if (@iGroupID is not null) or (@iRuleID is not null)
				update Contract 
					set groupID = isnull(@iGroupID,groupID), ruleID = isnull(@iRuleID,ruleID)
				where contractID = @iContractID
		end 
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractWithSpot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractWithSpot_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE proc usp_ContractWithSpot_Get
as
begin
  
	select Contract.contractID as iContractID, symbol as vcSymbol, contractName as vcContractName, CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk
		from ContractView Contract
				left join ContractPrice CP on Contract.contractID=CP.contractID
		where contractTypeID in (1, 2)
		order by symbol 
end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CreateDefaultVolaSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CreateDefaultVolaSurface_Save]
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
		

/*			select Top 1 @fPrice = 
				case 
					when (priceBid is not null and priceAsk is null) then priceBid
					when (priceBid is null and priceAsk is not null) then priceAsk
					when (priceBid is not null and priceAsk is not null) then (priceAsk + priceBid)/2
					when (priceBid is null and priceAsk is null) then priceLast
					when (priceBid is null and priceAsk is null and priceLast is null) then priceClose
				end
					from ContractPriceHistory 
					where contractID = @iContractID
					order by PriceDate desc */
			
			select @fPrice = 
				case 
					when (priceBid is not null and priceAsk is null) then priceBid
					when (priceBid is null and priceAsk is not null) then priceAsk
					when (priceBid is not null and priceAsk is not null) then (priceAsk + priceBid)/2
					when (priceBid is null and priceAsk is null) then priceLast
					when (priceBid is null and priceAsk is null and priceLast is null) then priceClose
				end
					from ContractPrice
					where contractID = @iContractID and exchangeID is null 

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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CurrentDate_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CurrentDate_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  procedure usp_CurrentDate_Get
	@DayOffset int output
as 
	declare @Year int,
			@DayOfYear int, 
			@Now smalldatetime
	set @Now = getdate()
	set @Year = datename(year,@Now)
	set @DayOfYear = DATENAME (dayofyear, @Now)
	set @DayOffset = @Year * 366 + @DayOfYear



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewData_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewData_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_CustomStrikeSkewData_Del
	@iCustomStrikeSkewID int,
	@iCustomStrikeSkewPointID int
as
/*
	this procedure deletes data from CustomStrikeSkewData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		delete CustomStrikeSkewData where customStrikeSkewID = @iCustomStrikeSkewID and customStrikeSkewPointID = @iCustomStrikeSkewPointID
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewData_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE     Procedure dbo.usp_CustomStrikeSkewData_Get
	@iVolaSurfaceDataID int,
	@dtExpDate datetime = null,
	@iSetupMode int,
	@iCustomStrikeSkewID int = null,
	@iCustomStrikeSkewPointID int = null,
	@bType bit = 1
as
/*
	this procedure gets data from CustomStrikeSkewData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	-- truncate time information
	--set @dtExpDate = convert(varchar(8), @dtExpDate, 112)
	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	if @bType is null	set @bType = 1

	if @iExpiryID is not null
--	if @dtExpDate is not null
		begin
			if not exists(select * 
							from CustomStrikeSkewPointView C
									join VolaSurfaceData  on C.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
							where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and C.expiryID = @iExpiryID and type = @bType)
--			set @dtExpDate = null
			set @iExpiryID = null		

			select distinct CustomStrikeSkewPoint.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPoint.volaSurfaceDataID iVolaSurfaceDataID, 
				setupMode iSetupMode, 
				strike fStrike, 
				case when expiryID = @iExpiryID then isnull(volatility, 0) else 0 end fVolatility, 
				case when expiryID = @iExpiryID then CustomStrikeSkew.customStrikeSkewID else null end iCustomStrikeSkewID, 
				case when expiryID = @iExpiryID then E.expiryDate else null end dtExpDate,
				type bType
		
				from CustomStrikeSkewPointView
						join VolaSurfaceData on CustomStrikeSkewPoint.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
						join Contract C on  VolaSurfaceData.contractID = C.contractID
						join ExpiryInCalendar E on E.expCalendarID = C.expCalendarID
						left join CustomStrikeSkewData on CustomStrikeSkewPoint.customStrikeSkewPointID = CustomStrikeSkewData.customStrikeSkewPointID
						left join CustomStrikeSkew on CustomStrikeSkewData.customStrikeSkewID = CustomStrikeSkew.customStrikeSkewID
				where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and setupMode = @iSetupMode and type = @bType
					and (@dtExpDate is null or expiry = @dtExpDate)
					and (@iCustomStrikeSkewID is null or CustomStrikeSkew.customStrikeSkewID = @iCustomStrikeSkewID)
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPoint.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
/*
			select distinct CustomStrikeSkewPoint.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPoint.volaSurfaceDataID iVolaSurfaceDataID, 
				setupMode iSetupMode, 
				strike fStrike, 
				case when expiry=@dtExpDate then isnull(volatility, 0) else 0 end fVolatility, 
				case when expiry=@dtExpDate then CustomStrikeSkew.customStrikeSkewID else null end iCustomStrikeSkewID, 
				case when expiry=@dtExpDate then expiry else null end dtExpDate,
				type bType
		
				from CustomStrikeSkewPointView
						inner join VolaSurfaceData on CustomStrikeSkewPoint.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
						left join CustomStrikeSkewData on CustomStrikeSkewPoint.customStrikeSkewPointID = CustomStrikeSkewData.customStrikeSkewPointID
						left join CustomStrikeSkew on CustomStrikeSkewData.customStrikeSkewID = CustomStrikeSkew.customStrikeSkewID
				where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and setupMode = @iSetupMode and type = @bType
					and (@dtExpDate is null or expiry = @dtExpDate)
					and (@iCustomStrikeSkewID is null or CustomStrikeSkew.customStrikeSkewID = @iCustomStrikeSkewID)
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPoint.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
*/
		end
	else
		select  CustomStrikeSkewPointView.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPointView.volaSurfaceDataID as ivolaSurfaceDataID, 
				0 iSetupMode, 
				strike fStrike, 
				volatility as fVolatility, 
				0 as iCustomStrikeSkewID, 
				E.expiryDate as dtExpDate,
				type as bType
			from CustomStrikeSkewPointView
				join VolaSurfaceData on CustomStrikeSkewPointView.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
				join Contract C on  VolaSurfaceData.contractID = C.contractID
				join ExpiryInCalendar E on E.expCalendarID = C.expCalendarID
			where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and type = @bType
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPointView.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
	set @error = @@error
Return(@error)








GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewData_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewData_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_CustomStrikeSkewData_Save
	@iCustomStrikeSkewID int = null,
	@iCustomStrikeSkewPointID int,
	@fVolatility float,
	@iVolaSurfaceDataID int = null,
	@vcContractTicker varchar(12) = null,
	@dtExpDate datetime = null
as
/*
	this procedure saves data to CustomStrikeSkewData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	--declare @iVolaSurfaceDataID int 
	-- wrong parameters
	--if @iCustomStrikeSkewID is null and (@iCustomStrikeSkewPointID is null and @vcContractTicker is null and @iContractID is null or @dtExpDate is null)	goto finish
	--if @iContractID is null and @vcContractTicker is not null
	--	select @iContractID = contractID from Contract where symbolName = @vcContractTicker and contractTypeID = 2
	--if @iContractID is not null
	--	select @iVolaSurfaceDataID = VolaSurfaceDataID from VolaSurfaceData where contractID = @iContractID
	--if @iContractID is null and @iCustomStrikeSkewPointID is not null
	--	select @iVolaSurfaceDataID = VolaSurfaceDataID from CustomStrikeSkewPoint where customStrikeSkewPointID = @iCustomStrikeSkewPointID
	if @iCustomStrikeSkewID is null and @iVolaSurfaceDataID is not null and @dtExpDate is not null
		select @iCustomStrikeSkewID = customStrikeSkewID from CustomStrikeSkew where volaSurfaceDataID = @iVolaSurfaceDataID and expiry = @dtExpDate
	Begin Transaction
		--if @iCustomStrikeSkewID is null
		--	begin
		--		exec usp_CustomStrikeSkew_Save @iCustomStrikeSkewID = @iCustomStrikeSkewID output, @iVolaSurfaceDataID = @iVolaSurfaceDataID, @dtExpDate = @dtExpDate
		--		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		--	end
		if not exists (select * from CustomStrikeSkewData where customStrikeSkewID = @iCustomStrikeSkewID and customStrikeSkewPointID = @iCustomStrikeSkewPointID)
			begin
				insert into CustomStrikeSkewData (customStrikeSkewID, customStrikeSkewPointID, volatility) values (@iCustomStrikeSkewID, @iCustomStrikeSkewPointID, @fVolatility)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update CustomStrikeSkewData set
					customStrikeSkewPointID = isnull(@iCustomStrikeSkewPointID, customStrikeSkewPointID),
					volatility = isnull(@fVolatility, volatility)
				where customStrikeSkewID = @iCustomStrikeSkewID and customStrikeSkewPointID = @iCustomStrikeSkewPointID
				if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewDefaultPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewDefaultPoint_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE   procedure dbo.usp_CustomStrikeSkewDefaultPoint_Save
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

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

--select @dtExpiry as '@dtExpiry', @iVolaSurfaceDataID as '@iVolaSurfaceDataID'
	if @iVolaSurfaceDataID is not null
		begin
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID,strike,type,volatility,isBasePoint,expiryID)
				select distinct @iVolaSurfaceDataID, strike as fStrike, @iType,@fVolatility,-1,@iExpiryID --convert(varchar(6),expiry,112) + '01' as expiry
					from OptionView 
					where OptionView.expiryID = @iExpiryID and underlyingContractID = @iContractID
						and not exists(select * 
											from CustomStrikeSkewPointView 
											where VolaSurfaceDataID = @iVolaSurfaceDataID and 
													strike = OptionView.strike 
													and CustomStrikeSkewPointView.expiryID = OptionView.expiryID)
					--and convert(varchar(8),expDate,112) = convert(varchar(6),expiry,112) + '01')
		end




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Copy]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Copy]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






CREATE   Procedure dbo.usp_CustomStrikeSkewPoint_Copy 
	@iVolaSurfaceDataIDSrc int,
	@iVolaSurfaceDataIDDest int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is move information inside CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryID,
				isBasePoint
			into #SourcePoints
			from CustomStrikeSkewPoint
			where volaSurfaceDataID=@iVolaSurfaceDataIDSrc
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
		delete CustomStrikeSkewPoint where volaSurfaceDataID=@iVolaSurfaceDataIDDest
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	
		insert into CustomStrikeSkewPoint (volaSurfaceDataID,strike,type,volatility,expiryID,isBasePoint)
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryID,
				isBasePoint
				from #SourcePoints
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
	drop table #SourcePoints
Return(@error)







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_CustomStrikeSkewPoint_Del
	@iCustomStrikeSkewPointID int = null
as
/*
	this procedure deletes data from CustomStrikeSkewPoint table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	update CustomStrikeSkewPoint set actionID=3
--	delete CustomStrikeSkewPoint
		where customStrikeSkewPointID = @iCustomStrikeSkewPointID
	set @error = 0
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE Procedure dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null,
	@dtActionDate datetime = null 
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	select 
		CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
		CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
		CSSP.strike as fStrike, 
		CSSP.type as iType,
		CSSP.volatility as fVolatility,
		convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
		CSSP.isBasePoint as iIsBasePoint,
		CSSP.actionDate as dtActionDate,
		CSSP.actionID as tiActionID
		from CustomStrikeSkewPoint CSSP
			join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			join Contract C on  VolaSurfaceData.contractID = C.contractID
			join ExpiryInCalendar E on E.expCalendarID = C.expCalendarID and CSSP.expiryID = E.expiryID
		where 
			(@iCustomStrikeSkewPointID is null or customStrikeSkewPointID=@iCustomStrikeSkewPointID)
		and (@iVolaSurfaceDataID is null or CSSP.volaSurfaceDataID=@iVolaSurfaceDataID) 
		and (@iType is null or type=@iType) 
		and (@dtExpDate is null or E.expiryID = @iExpiryID)
		and ((CSSP.actionDate>=@dtActionDate or @dtActionDate is null) and (@dtActionDate is not null or @dtActionDate is null and CSSP.actionID<3))

	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE          Procedure dbo.usp_CustomStrikeSkewPoint_Save 
	@iCustomStrikeSkewPointID int = Null output,
	@iVolaSurfaceDataID int = Null,
	@iType int = Null,
	@fStrike float = Null,
	@fVolatility float = Null,
	@dtExpDate datetime = Null,
	@iIsBasePoint int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into CustomStrikeSkewPoint table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)


	if exists (select * from CustomStrikeSkewPoint where 
						volaSurfaceDataID = @iVolaSurfaceDataID and 
						strike = @fStrike and 
						type = @iType and 
						expiryID = @iExpiryID
				and actionID < 3 and (@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null))
		begin
			if @iCustomStrikeSkewPointID is null
				select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
					from CustomStrikeSkewPoint 
					where volaSurfaceDataID = @iVolaSurfaceDataID and strike = @fStrike and type = @iType and expiryID = @iExpiryID and actionID < 3
			else if @iCustomStrikeSkewPointID is not null 
				begin
					Raiserror ('Row in CustomStrikeSkewPoint table  with same volaSurfaceDataID, strike, type and expDate already exists!', 16, 1)
					Return (-1)
				end
		end 
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					expiryID,
					isBasePoint) 
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						@iExpiryID,
						@iIsBasePoint)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iCustomStrikeSkewPointID=@@IDENTITY
			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID),
					isBasePoint = isnull(@iIsBasePoint,isBasePoint)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomVolatilityPointIsExist_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomVolatilityPointIsExist_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE     Procedure dbo.usp_CustomVolatilityPointIsExist_Get
	@vcSymbolName varchar(8) = null
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
			EC.expiryDate as dtExpiry,
			case when CSSP.volaSurfaceDataID is null then 0  
			else 1 end 'CustomVolatilityPointsCount'
		from OptionView OV
			join ContractView CV on OV.underlyingContractID = CV.contractID
			join ExpiryInCalendar EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
			left join VolaSurfaceDataView VSD on OV.underlyingContractID = VSD.contractID and VSD.isDefaultSurface = 1
			left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
and OV.expiryID = CSSP.expiryID

--and Convert(varchar(6),OV.expiry,112) = Convert(varchar(6),CSSP.expDate,112)


		where 
			CSSP.volaSurfaceDataID is null and 
			(CV.symbol = @vcSymbolName or @vcSymbolName is null) 
		order by CV.contractID,EC.expiryDate


Return(0)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DatabaseInitialization]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DatabaseInitialization]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE  procedure dbo.usp_DatabaseInitialization
as
--Description: this procedure fill up key references tables
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

Begin Transaction

	if not exists(Select * From DataInfo where dataInfoID=1)
		begin
			insert into DataInfo (dataInfoID, keyName,keyValue) values(1,'version','3.23')
		end

	if not exists(Select * From DataInfo where dataInfoID=4)
		begin
			insert into DataInfo (dataInfoID, keyName,keyValue) values(4,'wtd vega maturity','60')
		end

	if not exists(Select * From DataInfo where dataInfoID=5)
		begin
			insert into DataInfo (dataInfoID, keyName,keyValue) values(5,'lastTradeTime','1011646800')
		end

	if not exists(select * from TraderRole)
		DBCC CHECKIDENT ('TraderRole', RESEED,0)

	set identity_insert TraderRole on

	if not exists(Select * From TraderRole where traderRoleID=1)
		begin
			insert into TraderRole (traderRoleID,roleName) values(1,'Administrator')
		end

	if not exists(Select * From TraderRole where traderRoleID=2)
		begin
			insert into TraderRole (traderRoleID,roleName) values(2,'Head RM')
		end

	if not exists(Select * From TraderRole where traderRoleID=3)
		begin
			insert into TraderRole (traderRoleID,roleName) values(3,'RM')
		end

	if not exists(Select * From TraderRole where traderRoleID=4)
		begin
			insert into TraderRole (traderRoleID,roleName) values(4,'Trader')
		end

	set identity_insert TraderRole off


	if not exists(select * from Tenor)
		DBCC CHECKIDENT ('Tenor', RESEED,0)

	set identity_insert Tenor on

	if not exists(Select * From Tenor where tenorID=1)
		begin
			insert into Tenor (tenorID, value,valueIV) values(1,10,30)
		end

	if not exists(Select * From Tenor where tenorID=2)
		begin
			insert into Tenor (tenorID, value,valueIV) values(2,20,30)
		end

	if not exists(Select * From Tenor where tenorID=3)
		begin
			insert into Tenor (tenorID, value,valueIV) values(3,30,30)
		end

	if not exists(Select * From Tenor where tenorID=4)
		begin
			insert into Tenor (tenorID, value,valueIV) values(4,60,60)
		end

	if not exists(Select * From Tenor where tenorID=5)
		begin
			insert into Tenor (tenorID, value,valueIV) values(5,90,90)
		end

	if not exists(Select * From Tenor where tenorID=6)
		begin
			insert into Tenor (tenorID, value,valueIV) values(6,120,120)
		end

	if not exists(Select * From Tenor where tenorID=7)
		begin
			insert into Tenor (tenorID, value,valueIV) values(7,150,150)
		end

	if not exists(Select * From Tenor where tenorID=8)
		begin
			insert into Tenor (tenorID, value,valueIV) values(8,180,180)
		end

	set identity_insert Tenor off

	set identity_insert FilterField on

	if not exists(Select * From FilterField where filterFieldID = 1)
		begin
			insert into FilterField (filterFieldID,fieldName) values(1,'traderID')
		end

	if not exists(Select * From FilterField where filterFieldID = 2)
		begin
			insert into FilterField (filterFieldID,fieldName) values(2,'bookID')
		end

	if not exists(Select * From FilterField where filterFieldID = 3)
		begin
			insert into FilterField (filterFieldID,fieldName) values(3,'strategyID')
		end

	if not exists(Select * From FilterField where filterFieldID = 4)
		begin
			insert into FilterField (filterFieldID,fieldName) values(4,'groupID')
		end

	set identity_insert FilterField off

	set identity_insert RuleItemType on

	if not exists(Select * From RuleItemType where ruleItemTypeID = 1)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(1,'int')
		end

	if not exists(Select * From RuleItemType where ruleItemTypeID = 2)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(2,'double')
		end

	if not exists(Select * From RuleItemType where ruleItemTypeID = 3)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(3,'char')
		end

	if not exists(Select * From RuleItemType where ruleItemTypeID = 4)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(4,'string')
		end

	if not exists(Select * From RuleItemType where ruleItemTypeID = 5)
		begin
			insert into RuleItemType (ruleItemTypeID,itemTypeName) values(5,'time')
		end

	set identity_insert RuleItemType off

	set identity_insert GenerationRule on

	if not exists(Select * From GenerationRule where ruleID = 1)
		begin
			insert into GenerationRule (ruleID,ruleName,interpolationFactor,callShift,putShift,smileAccelerator,strikeIncrement, strikeMoneynessLo,strikeMoneynessHi,isDefaultRule,requiredPoints,ruleType) values(1,'<Default>',0.52,500.0,500.0,0,2,10,150.0,1,9,0)
		end

	set identity_insert GenerationRule off


--============================================================================================================
--fill up Expiry table
--============================================================================================================
if not exists (select * from Expiry)
	begin
		set nocount on
		insert into Expiry(expiryID,expiryPeriod) values (1,'JAN2000')
		insert into Expiry(expiryID,expiryPeriod) values (2,'FEB2000')
		insert into Expiry(expiryID,expiryPeriod) values (3,'MAR2000')
		insert into Expiry(expiryID,expiryPeriod) values (4,'APR2000')
		insert into Expiry(expiryID,expiryPeriod) values (5,'MAY2000')
		insert into Expiry(expiryID,expiryPeriod) values (6,'JUN2000')
		insert into Expiry(expiryID,expiryPeriod) values (7,'JUL2000')
		insert into Expiry(expiryID,expiryPeriod) values (8,'AUG2000')
		insert into Expiry(expiryID,expiryPeriod) values (9,'SEP2000')
		insert into Expiry(expiryID,expiryPeriod) values (10,'OCT2000')
		insert into Expiry(expiryID,expiryPeriod) values (11,'NOV2000')
		insert into Expiry(expiryID,expiryPeriod) values (12,'DEC2000')
		insert into Expiry(expiryID,expiryPeriod) values (13,'JAN2001')
		insert into Expiry(expiryID,expiryPeriod) values (14,'FEB2001')
		insert into Expiry(expiryID,expiryPeriod) values (15,'MAR2001')
		insert into Expiry(expiryID,expiryPeriod) values (16,'APR2001')
		insert into Expiry(expiryID,expiryPeriod) values (17,'MAY2001')
		insert into Expiry(expiryID,expiryPeriod) values (18,'JUN2001')
		insert into Expiry(expiryID,expiryPeriod) values (19,'JUL2001')
		insert into Expiry(expiryID,expiryPeriod) values (20,'AUG2001')
		insert into Expiry(expiryID,expiryPeriod) values (21,'SEP2001')
		insert into Expiry(expiryID,expiryPeriod) values (22,'OCT2001')
		insert into Expiry(expiryID,expiryPeriod) values (23,'NOV2001')
		insert into Expiry(expiryID,expiryPeriod) values (24,'DEC2001')
		insert into Expiry(expiryID,expiryPeriod) values (25,'JAN2002')
		insert into Expiry(expiryID,expiryPeriod) values (26,'FEB2002')
		insert into Expiry(expiryID,expiryPeriod) values (27,'MAR2002')
		insert into Expiry(expiryID,expiryPeriod) values (28,'APR2002')
		insert into Expiry(expiryID,expiryPeriod) values (29,'MAY2002')
		insert into Expiry(expiryID,expiryPeriod) values (30,'JUN2002')
		insert into Expiry(expiryID,expiryPeriod) values (31,'JUL2002')
		insert into Expiry(expiryID,expiryPeriod) values (32,'AUG2002')
		insert into Expiry(expiryID,expiryPeriod) values (33,'SEP2002')
		insert into Expiry(expiryID,expiryPeriod) values (34,'OCT2002')
		insert into Expiry(expiryID,expiryPeriod) values (35,'NOV2002')
		insert into Expiry(expiryID,expiryPeriod) values (36,'DEC2002')
		insert into Expiry(expiryID,expiryPeriod) values (37,'JAN2003')
		insert into Expiry(expiryID,expiryPeriod) values (38,'FEB2003')
		insert into Expiry(expiryID,expiryPeriod) values (39,'MAR2003')
		insert into Expiry(expiryID,expiryPeriod) values (40,'APR2003')
		insert into Expiry(expiryID,expiryPeriod) values (41,'MAY2003')
		insert into Expiry(expiryID,expiryPeriod) values (42,'JUN2003')
		insert into Expiry(expiryID,expiryPeriod) values (43,'JUL2003')
		insert into Expiry(expiryID,expiryPeriod) values (44,'AUG2003')
		insert into Expiry(expiryID,expiryPeriod) values (45,'SEP2003')
		insert into Expiry(expiryID,expiryPeriod) values (46,'OCT2003')
		insert into Expiry(expiryID,expiryPeriod) values (47,'NOV2003')
		insert into Expiry(expiryID,expiryPeriod) values (48,'DEC2003')
		insert into Expiry(expiryID,expiryPeriod) values (49,'JAN2004')
		insert into Expiry(expiryID,expiryPeriod) values (50,'FEB2004')
		insert into Expiry(expiryID,expiryPeriod) values (51,'MAR2004')
		insert into Expiry(expiryID,expiryPeriod) values (52,'APR2004')
		insert into Expiry(expiryID,expiryPeriod) values (53,'MAY2004')
		insert into Expiry(expiryID,expiryPeriod) values (54,'JUN2004')
		insert into Expiry(expiryID,expiryPeriod) values (55,'JUL2004')
		insert into Expiry(expiryID,expiryPeriod) values (56,'AUG2004')
		insert into Expiry(expiryID,expiryPeriod) values (57,'SEP2004')
		insert into Expiry(expiryID,expiryPeriod) values (58,'OCT2004')
		insert into Expiry(expiryID,expiryPeriod) values (59,'NOV2004')
		insert into Expiry(expiryID,expiryPeriod) values (60,'DEC2004')
		insert into Expiry(expiryID,expiryPeriod) values (61,'JAN2005')
		insert into Expiry(expiryID,expiryPeriod) values (62,'FEB2005')
		insert into Expiry(expiryID,expiryPeriod) values (63,'MAR2005')
		insert into Expiry(expiryID,expiryPeriod) values (64,'APR2005')
		insert into Expiry(expiryID,expiryPeriod) values (65,'MAY2005')
		insert into Expiry(expiryID,expiryPeriod) values (66,'JUN2005')
		insert into Expiry(expiryID,expiryPeriod) values (67,'JUL2005')
		insert into Expiry(expiryID,expiryPeriod) values (68,'AUG2005')
		insert into Expiry(expiryID,expiryPeriod) values (69,'SEP2005')
		insert into Expiry(expiryID,expiryPeriod) values (70,'OCT2005')
		insert into Expiry(expiryID,expiryPeriod) values (71,'NOV2005')
		insert into Expiry(expiryID,expiryPeriod) values (72,'DEC2005')
		insert into Expiry(expiryID,expiryPeriod) values (73,'JAN2006')
		insert into Expiry(expiryID,expiryPeriod) values (74,'FEB2006')
		insert into Expiry(expiryID,expiryPeriod) values (75,'MAR2006')
		insert into Expiry(expiryID,expiryPeriod) values (76,'APR2006')
		insert into Expiry(expiryID,expiryPeriod) values (77,'MAY2006')
		insert into Expiry(expiryID,expiryPeriod) values (78,'JUN2006')
		insert into Expiry(expiryID,expiryPeriod) values (79,'JUL2006')
		insert into Expiry(expiryID,expiryPeriod) values (80,'AUG2006')
		insert into Expiry(expiryID,expiryPeriod) values (81,'SEP2006')
		insert into Expiry(expiryID,expiryPeriod) values (82,'OCT2006')
		insert into Expiry(expiryID,expiryPeriod) values (83,'NOV2006')
		insert into Expiry(expiryID,expiryPeriod) values (84,'DEC2006')
		insert into Expiry(expiryID,expiryPeriod) values (85,'JAN2007')
		insert into Expiry(expiryID,expiryPeriod) values (86,'FEB2007')
		insert into Expiry(expiryID,expiryPeriod) values (87,'MAR2007')
		insert into Expiry(expiryID,expiryPeriod) values (88,'APR2007')
		insert into Expiry(expiryID,expiryPeriod) values (89,'MAY2007')
		insert into Expiry(expiryID,expiryPeriod) values (90,'JUN2007')
		insert into Expiry(expiryID,expiryPeriod) values (91,'JUL2007')
		insert into Expiry(expiryID,expiryPeriod) values (92,'AUG2007')
		insert into Expiry(expiryID,expiryPeriod) values (93,'SEP2007')
		insert into Expiry(expiryID,expiryPeriod) values (94,'OCT2007')
		insert into Expiry(expiryID,expiryPeriod) values (95,'NOV2007')
		insert into Expiry(expiryID,expiryPeriod) values (96,'DEC2007')
		insert into Expiry(expiryID,expiryPeriod) values (97,'JAN2008')
		insert into Expiry(expiryID,expiryPeriod) values (98,'FEB2008')
		insert into Expiry(expiryID,expiryPeriod) values (99,'MAR2008')
		insert into Expiry(expiryID,expiryPeriod) values (100,'APR2008')
		insert into Expiry(expiryID,expiryPeriod) values (101,'MAY2008')
		insert into Expiry(expiryID,expiryPeriod) values (102,'JUN2008')
		insert into Expiry(expiryID,expiryPeriod) values (103,'JUL2008')
		insert into Expiry(expiryID,expiryPeriod) values (104,'AUG2008')
		insert into Expiry(expiryID,expiryPeriod) values (105,'SEP2008')
		insert into Expiry(expiryID,expiryPeriod) values (106,'OCT2008')
		insert into Expiry(expiryID,expiryPeriod) values (107,'NOV2008')
		insert into Expiry(expiryID,expiryPeriod) values (108,'DEC2008')
		insert into Expiry(expiryID,expiryPeriod) values (109,'JAN2009')
		insert into Expiry(expiryID,expiryPeriod) values (110,'FEB2009')
		insert into Expiry(expiryID,expiryPeriod) values (111,'MAR2009')
		insert into Expiry(expiryID,expiryPeriod) values (112,'APR2009')
		insert into Expiry(expiryID,expiryPeriod) values (113,'MAY2009')
		insert into Expiry(expiryID,expiryPeriod) values (114,'JUN2009')
		insert into Expiry(expiryID,expiryPeriod) values (115,'JUL2009')
		insert into Expiry(expiryID,expiryPeriod) values (116,'AUG2009')
		insert into Expiry(expiryID,expiryPeriod) values (117,'SEP2009')
		insert into Expiry(expiryID,expiryPeriod) values (118,'OCT2009')
		insert into Expiry(expiryID,expiryPeriod) values (119,'NOV2009')
		insert into Expiry(expiryID,expiryPeriod) values (120,'DEC2009')
		insert into Expiry(expiryID,expiryPeriod) values (121,'JAN2010')
		insert into Expiry(expiryID,expiryPeriod) values (122,'FEB2010')
		insert into Expiry(expiryID,expiryPeriod) values (123,'MAR2010')
		insert into Expiry(expiryID,expiryPeriod) values (124,'APR2010')
		insert into Expiry(expiryID,expiryPeriod) values (125,'MAY2010')
		insert into Expiry(expiryID,expiryPeriod) values (126,'JUN2010')
		insert into Expiry(expiryID,expiryPeriod) values (127,'JUL2010')
		insert into Expiry(expiryID,expiryPeriod) values (128,'AUG2010')
		insert into Expiry(expiryID,expiryPeriod) values (129,'SEP2010')
		insert into Expiry(expiryID,expiryPeriod) values (130,'OCT2010')
		insert into Expiry(expiryID,expiryPeriod) values (131,'NOV2010')
		insert into Expiry(expiryID,expiryPeriod) values (132,'DEC2010')
		insert into Expiry(expiryID,expiryPeriod) values (133,'JAN2011')
		insert into Expiry(expiryID,expiryPeriod) values (134,'FEB2011')
		insert into Expiry(expiryID,expiryPeriod) values (135,'MAR2011')
		insert into Expiry(expiryID,expiryPeriod) values (136,'APR2011')
		insert into Expiry(expiryID,expiryPeriod) values (137,'MAY2011')
		insert into Expiry(expiryID,expiryPeriod) values (138,'JUN2011')
		insert into Expiry(expiryID,expiryPeriod) values (139,'JUL2011')
		insert into Expiry(expiryID,expiryPeriod) values (140,'AUG2011')
		insert into Expiry(expiryID,expiryPeriod) values (141,'SEP2011')
		insert into Expiry(expiryID,expiryPeriod) values (142,'OCT2011')
		insert into Expiry(expiryID,expiryPeriod) values (143,'NOV2011')
		insert into Expiry(expiryID,expiryPeriod) values (144,'DEC2011')
		insert into Expiry(expiryID,expiryPeriod) values (145,'JAN2012')
		insert into Expiry(expiryID,expiryPeriod) values (146,'FEB2012')
		insert into Expiry(expiryID,expiryPeriod) values (147,'MAR2012')
		insert into Expiry(expiryID,expiryPeriod) values (148,'APR2012')
		insert into Expiry(expiryID,expiryPeriod) values (149,'MAY2012')
		insert into Expiry(expiryID,expiryPeriod) values (150,'JUN2012')
		insert into Expiry(expiryID,expiryPeriod) values (151,'JUL2012')
		insert into Expiry(expiryID,expiryPeriod) values (152,'AUG2012')
		insert into Expiry(expiryID,expiryPeriod) values (153,'SEP2012')
		insert into Expiry(expiryID,expiryPeriod) values (154,'OCT2012')
		insert into Expiry(expiryID,expiryPeriod) values (155,'NOV2012')
		insert into Expiry(expiryID,expiryPeriod) values (156,'DEC2012')
		insert into Expiry(expiryID,expiryPeriod) values (157,'JAN2013')
		insert into Expiry(expiryID,expiryPeriod) values (158,'FEB2013')
		insert into Expiry(expiryID,expiryPeriod) values (159,'MAR2013')
		insert into Expiry(expiryID,expiryPeriod) values (160,'APR2013')
		insert into Expiry(expiryID,expiryPeriod) values (161,'MAY2013')
		insert into Expiry(expiryID,expiryPeriod) values (162,'JUN2013')
		insert into Expiry(expiryID,expiryPeriod) values (163,'JUL2013')
		insert into Expiry(expiryID,expiryPeriod) values (164,'AUG2013')
		insert into Expiry(expiryID,expiryPeriod) values (165,'SEP2013')
		insert into Expiry(expiryID,expiryPeriod) values (166,'OCT2013')
		insert into Expiry(expiryID,expiryPeriod) values (167,'NOV2013')
		insert into Expiry(expiryID,expiryPeriod) values (168,'DEC2013')
		insert into Expiry(expiryID,expiryPeriod) values (169,'JAN2014')
		insert into Expiry(expiryID,expiryPeriod) values (170,'FEB2014')
		insert into Expiry(expiryID,expiryPeriod) values (171,'MAR2014')
		insert into Expiry(expiryID,expiryPeriod) values (172,'APR2014')
		insert into Expiry(expiryID,expiryPeriod) values (173,'MAY2014')
		insert into Expiry(expiryID,expiryPeriod) values (174,'JUN2014')
		insert into Expiry(expiryID,expiryPeriod) values (175,'JUL2014')
		insert into Expiry(expiryID,expiryPeriod) values (176,'AUG2014')
		insert into Expiry(expiryID,expiryPeriod) values (177,'SEP2014')
		insert into Expiry(expiryID,expiryPeriod) values (178,'OCT2014')
		insert into Expiry(expiryID,expiryPeriod) values (179,'NOV2014')
		insert into Expiry(expiryID,expiryPeriod) values (180,'DEC2014')
		insert into Expiry(expiryID,expiryPeriod) values (181,'JAN2015')
		insert into Expiry(expiryID,expiryPeriod) values (182,'FEB2015')
		insert into Expiry(expiryID,expiryPeriod) values (183,'MAR2015')
		insert into Expiry(expiryID,expiryPeriod) values (184,'APR2015')
		insert into Expiry(expiryID,expiryPeriod) values (185,'MAY2015')
		insert into Expiry(expiryID,expiryPeriod) values (186,'JUN2015')
		insert into Expiry(expiryID,expiryPeriod) values (187,'JUL2015')
		insert into Expiry(expiryID,expiryPeriod) values (188,'AUG2015')
		insert into Expiry(expiryID,expiryPeriod) values (189,'SEP2015')
		insert into Expiry(expiryID,expiryPeriod) values (190,'OCT2015')
		insert into Expiry(expiryID,expiryPeriod) values (191,'NOV2015')
		insert into Expiry(expiryID,expiryPeriod) values (192,'DEC2015')
		insert into Expiry(expiryID,expiryPeriod) values (193,'JAN2016')
		insert into Expiry(expiryID,expiryPeriod) values (194,'FEB2016')
		insert into Expiry(expiryID,expiryPeriod) values (195,'MAR2016')
		insert into Expiry(expiryID,expiryPeriod) values (196,'APR2016')
		insert into Expiry(expiryID,expiryPeriod) values (197,'MAY2016')
		insert into Expiry(expiryID,expiryPeriod) values (198,'JUN2016')
		insert into Expiry(expiryID,expiryPeriod) values (199,'JUL2016')
		insert into Expiry(expiryID,expiryPeriod) values (200,'AUG2016')
		insert into Expiry(expiryID,expiryPeriod) values (201,'SEP2016')
		insert into Expiry(expiryID,expiryPeriod) values (202,'OCT2016')
		insert into Expiry(expiryID,expiryPeriod) values (203,'NOV2016')
		insert into Expiry(expiryID,expiryPeriod) values (204,'DEC2016')
		insert into Expiry(expiryID,expiryPeriod) values (205,'JAN2017')
		insert into Expiry(expiryID,expiryPeriod) values (206,'FEB2017')
		insert into Expiry(expiryID,expiryPeriod) values (207,'MAR2017')
		insert into Expiry(expiryID,expiryPeriod) values (208,'APR2017')
		insert into Expiry(expiryID,expiryPeriod) values (209,'MAY2017')
		insert into Expiry(expiryID,expiryPeriod) values (210,'JUN2017')
		insert into Expiry(expiryID,expiryPeriod) values (211,'JUL2017')
		insert into Expiry(expiryID,expiryPeriod) values (212,'AUG2017')
		insert into Expiry(expiryID,expiryPeriod) values (213,'SEP2017')
		insert into Expiry(expiryID,expiryPeriod) values (214,'OCT2017')
		insert into Expiry(expiryID,expiryPeriod) values (215,'NOV2017')
		insert into Expiry(expiryID,expiryPeriod) values (216,'DEC2017')
		insert into Expiry(expiryID,expiryPeriod) values (217,'JAN2018')
		insert into Expiry(expiryID,expiryPeriod) values (218,'FEB2018')
		insert into Expiry(expiryID,expiryPeriod) values (219,'MAR2018')
		insert into Expiry(expiryID,expiryPeriod) values (220,'APR2018')
		insert into Expiry(expiryID,expiryPeriod) values (221,'MAY2018')
		insert into Expiry(expiryID,expiryPeriod) values (222,'JUN2018')
		insert into Expiry(expiryID,expiryPeriod) values (223,'JUL2018')
		insert into Expiry(expiryID,expiryPeriod) values (224,'AUG2018')
		insert into Expiry(expiryID,expiryPeriod) values (225,'SEP2018')
		insert into Expiry(expiryID,expiryPeriod) values (226,'OCT2018')
		insert into Expiry(expiryID,expiryPeriod) values (227,'NOV2018')
		insert into Expiry(expiryID,expiryPeriod) values (228,'DEC2018')
		insert into Expiry(expiryID,expiryPeriod) values (229,'JAN2019')
		insert into Expiry(expiryID,expiryPeriod) values (230,'FEB2019')
		insert into Expiry(expiryID,expiryPeriod) values (231,'MAR2019')
		insert into Expiry(expiryID,expiryPeriod) values (232,'APR2019')
		insert into Expiry(expiryID,expiryPeriod) values (233,'MAY2019')
		insert into Expiry(expiryID,expiryPeriod) values (234,'JUN2019')
		insert into Expiry(expiryID,expiryPeriod) values (235,'JUL2019')
		insert into Expiry(expiryID,expiryPeriod) values (236,'AUG2019')
		insert into Expiry(expiryID,expiryPeriod) values (237,'SEP2019')
		insert into Expiry(expiryID,expiryPeriod) values (238,'OCT2019')
		insert into Expiry(expiryID,expiryPeriod) values (239,'NOV2019')
		insert into Expiry(expiryID,expiryPeriod) values (240,'DEC2019')
		insert into Expiry(expiryID,expiryPeriod) values (241,'JAN2020')
		insert into Expiry(expiryID,expiryPeriod) values (242,'FEB2020')
		insert into Expiry(expiryID,expiryPeriod) values (243,'MAR2020')
		insert into Expiry(expiryID,expiryPeriod) values (244,'APR2020')
		insert into Expiry(expiryID,expiryPeriod) values (245,'MAY2020')
		insert into Expiry(expiryID,expiryPeriod) values (246,'JUN2020')
		insert into Expiry(expiryID,expiryPeriod) values (247,'JUL2020')
		insert into Expiry(expiryID,expiryPeriod) values (248,'AUG2020')
		insert into Expiry(expiryID,expiryPeriod) values (249,'SEP2020')
		insert into Expiry(expiryID,expiryPeriod) values (250,'OCT2020')
		insert into Expiry(expiryID,expiryPeriod) values (251,'NOV2020')
		insert into Expiry(expiryID,expiryPeriod) values (252,'DEC2020')
	
	end

--============================================================================================================
--fill up ExpiryInCalendar table
--============================================================================================================
if not exists (select * from ExpiryInCalendar) 
	begin
		set nocount on
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,1,'2000-01-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,2,'2000-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,3,'2000-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,4,'2000-04-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,5,'2000-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,6,'2000-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,7,'2000-07-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,8,'2000-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,9,'2000-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,10,'2000-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,11,'2000-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,12,'2000-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,13,'2001-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,14,'2001-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,15,'2001-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,16,'2001-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,17,'2001-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,18,'2001-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,19,'2001-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,20,'2001-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,21,'2001-09-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,22,'2001-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,23,'2001-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,24,'2001-12-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,25,'2002-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,26,'2002-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,27,'2002-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,28,'2002-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,29,'2002-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,30,'2002-06-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,31,'2002-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,32,'2002-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,33,'2002-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,34,'2002-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,35,'2002-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,36,'2002-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,37,'2003-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,38,'2003-02-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,39,'2003-03-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,40,'2003-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,41,'2003-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,42,'2003-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,43,'2003-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,44,'2003-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,45,'2003-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,46,'2003-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,47,'2003-11-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,48,'2003-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,49,'2004-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,50,'2004-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,51,'2004-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,52,'2004-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,53,'2004-05-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,54,'2004-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,55,'2004-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,56,'2004-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,57,'2004-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,58,'2004-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,59,'2004-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,60,'2004-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,61,'2005-01-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,62,'2005-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,63,'2005-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,64,'2005-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,65,'2005-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,66,'2005-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,67,'2005-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,68,'2005-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,69,'2005-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,70,'2005-10-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,71,'2005-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,72,'2005-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,73,'2006-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,74,'2006-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,75,'2006-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,76,'2006-04-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,77,'2006-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,78,'2006-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,79,'2006-07-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,80,'2006-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,81,'2006-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,82,'2006-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,83,'2006-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,84,'2006-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,85,'2007-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,86,'2007-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,87,'2007-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,88,'2007-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,89,'2007-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,90,'2007-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,91,'2007-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,92,'2007-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,93,'2007-09-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,94,'2007-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,95,'2007-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,96,'2007-12-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,97,'2008-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,98,'2008-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,99,'2008-03-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,100,'2008-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,101,'2008-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,102,'2008-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,103,'2008-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,104,'2008-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,105,'2008-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,106,'2008-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,107,'2008-11-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,108,'2008-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,109,'2009-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,110,'2009-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,111,'2009-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,112,'2009-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,113,'2009-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,114,'2009-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,115,'2009-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,116,'2009-08-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,117,'2009-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,118,'2009-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,119,'2009-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,120,'2009-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,121,'2010-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,122,'2010-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,123,'2010-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,124,'2010-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,125,'2010-05-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,126,'2010-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,127,'2010-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,128,'2010-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,129,'2010-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,130,'2010-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,131,'2010-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,132,'2010-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,133,'2011-01-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,134,'2011-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,135,'2011-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,136,'2011-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,137,'2011-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,138,'2011-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,139,'2011-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,140,'2011-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,141,'2011-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,142,'2011-10-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,143,'2011-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,144,'2011-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,145,'2012-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,146,'2012-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,147,'2012-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,148,'2012-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,149,'2012-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,150,'2012-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,151,'2012-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,152,'2012-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,153,'2012-09-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,154,'2012-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,155,'2012-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,156,'2012-12-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,157,'2013-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,158,'2013-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,159,'2013-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,160,'2013-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,161,'2013-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,162,'2013-06-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,163,'2013-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,164,'2013-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,165,'2013-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,166,'2013-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,167,'2013-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,168,'2013-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,169,'2014-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,170,'2014-02-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,171,'2014-03-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,172,'2014-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,173,'2014-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,174,'2014-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,175,'2014-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,176,'2014-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,177,'2014-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,178,'2014-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,179,'2014-11-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,180,'2014-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,181,'2015-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,182,'2015-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,183,'2015-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,184,'2015-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,185,'2015-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,186,'2015-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,187,'2015-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,188,'2015-08-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,189,'2015-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,190,'2015-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,191,'2015-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,192,'2015-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,193,'2016-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,194,'2016-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,195,'2016-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,196,'2016-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,197,'2016-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,198,'2016-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,199,'2016-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,200,'2016-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,201,'2016-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,202,'2016-10-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,203,'2016-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,204,'2016-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,205,'2017-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,206,'2017-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,207,'2017-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,208,'2017-04-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,209,'2017-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,210,'2017-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,211,'2017-07-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,212,'2017-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,213,'2017-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,214,'2017-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,215,'2017-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,216,'2017-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,217,'2018-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,218,'2018-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,219,'2018-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,220,'2018-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,221,'2018-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,222,'2018-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,223,'2018-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,224,'2018-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,225,'2018-09-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,226,'2018-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,227,'2018-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,228,'2018-12-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,229,'2019-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,230,'2019-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,231,'2019-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,232,'2019-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,233,'2019-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,234,'2019-06-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,235,'2019-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,236,'2019-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,237,'2019-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,238,'2019-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,239,'2019-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,240,'2019-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,241,'2020-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,242,'2020-02-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,243,'2020-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,244,'2020-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,245,'2020-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,246,'2020-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,247,'2020-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,248,'2020-08-22')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,249,'2020-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,250,'2020-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,251,'2020-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (1,252,'2020-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,1,'2000-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,2,'2000-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,3,'2000-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,4,'2000-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,5,'2000-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,6,'2000-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,7,'2000-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,8,'2000-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,9,'2000-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,10,'2000-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,11,'2000-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,12,'2000-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,13,'2001-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,14,'2001-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,15,'2001-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,16,'2001-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,17,'2001-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,18,'2001-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,19,'2001-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,20,'2001-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,21,'2001-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,22,'2001-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,23,'2001-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,24,'2001-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,25,'2002-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,26,'2002-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,27,'2002-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,28,'2002-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,29,'2002-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,30,'2002-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,31,'2002-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,32,'2002-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,33,'2002-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,34,'2002-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,35,'2002-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,36,'2002-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,37,'2003-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,38,'2003-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,39,'2003-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,40,'2003-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,41,'2003-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,42,'2003-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,43,'2003-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,44,'2003-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,45,'2003-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,46,'2003-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,47,'2003-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,48,'2003-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,49,'2004-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,50,'2004-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,51,'2004-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,52,'2004-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,53,'2004-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,54,'2004-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,55,'2004-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,56,'2004-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,57,'2004-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,58,'2004-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,59,'2004-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,60,'2004-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,61,'2005-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,62,'2005-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,63,'2005-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,64,'2005-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,65,'2005-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,66,'2005-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,67,'2005-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,68,'2005-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,69,'2005-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,70,'2005-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,71,'2005-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,72,'2005-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,73,'2006-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,74,'2006-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,75,'2006-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,76,'2006-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,77,'2006-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,78,'2006-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,79,'2006-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,80,'2006-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,81,'2006-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,82,'2006-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,83,'2006-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,84,'2006-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,85,'2007-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,86,'2007-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,87,'2007-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,88,'2007-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,89,'2007-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,90,'2007-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,91,'2007-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,92,'2007-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,93,'2007-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,94,'2007-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,95,'2007-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,96,'2007-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,97,'2008-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,98,'2008-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,99,'2008-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,100,'2008-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,101,'2008-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,102,'2008-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,103,'2008-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,104,'2008-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,105,'2008-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,106,'2008-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,107,'2008-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,108,'2008-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,109,'2009-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,110,'2009-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,111,'2009-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,112,'2009-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,113,'2009-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,114,'2009-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,115,'2009-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,116,'2009-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,117,'2009-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,118,'2009-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,119,'2009-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,120,'2009-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,121,'2010-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,122,'2010-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,123,'2010-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,124,'2010-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,125,'2010-05-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,126,'2010-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,127,'2010-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,128,'2010-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,129,'2010-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,130,'2010-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,131,'2010-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,132,'2010-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,133,'2011-01-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,134,'2011-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,135,'2011-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,136,'2011-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,137,'2011-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,138,'2011-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,139,'2011-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,140,'2011-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,141,'2011-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,142,'2011-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,143,'2011-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,144,'2011-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,145,'2012-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,146,'2012-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,147,'2012-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,148,'2012-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,149,'2012-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,150,'2012-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,151,'2012-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,152,'2012-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,153,'2012-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,154,'2012-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,155,'2012-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,156,'2012-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,157,'2013-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,158,'2013-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,159,'2013-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,160,'2013-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,161,'2013-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,162,'2013-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,163,'2013-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,164,'2013-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,165,'2013-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,166,'2013-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,167,'2013-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,168,'2013-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,169,'2014-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,170,'2014-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,171,'2014-03-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,172,'2014-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,173,'2014-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,174,'2014-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,175,'2014-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,176,'2014-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,177,'2014-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,178,'2014-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,179,'2014-11-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,180,'2014-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,181,'2015-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,182,'2015-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,183,'2015-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,184,'2015-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,185,'2015-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,186,'2015-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,187,'2015-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,188,'2015-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,189,'2015-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,190,'2015-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,191,'2015-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,192,'2015-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,193,'2016-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,194,'2016-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,195,'2016-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,196,'2016-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,197,'2016-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,198,'2016-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,199,'2016-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,200,'2016-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,201,'2016-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,202,'2016-10-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,203,'2016-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,204,'2016-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,205,'2017-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,206,'2017-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,207,'2017-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,208,'2017-04-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,209,'2017-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,210,'2017-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,211,'2017-07-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,212,'2017-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,213,'2017-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,214,'2017-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,215,'2017-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,216,'2017-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,217,'2018-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,218,'2018-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,219,'2018-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,220,'2018-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,221,'2018-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,222,'2018-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,223,'2018-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,224,'2018-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,225,'2018-09-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,226,'2018-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,227,'2018-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,228,'2018-12-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,229,'2019-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,230,'2019-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,231,'2019-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,232,'2019-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,233,'2019-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,234,'2019-06-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,235,'2019-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,236,'2019-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,237,'2019-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,238,'2019-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,239,'2019-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,240,'2019-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,241,'2020-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,242,'2020-02-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,243,'2020-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,244,'2020-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,245,'2020-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,246,'2020-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,247,'2020-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,248,'2020-08-21')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,249,'2020-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,250,'2020-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,251,'2020-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (2,252,'2020-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,1,'2000-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,2,'2000-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,3,'2000-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,4,'2000-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,5,'2000-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,6,'2000-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,7,'2000-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,8,'2000-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,9,'2000-09-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,10,'2000-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,11,'2000-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,12,'2000-12-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,13,'2001-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,14,'2001-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,15,'2001-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,16,'2001-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,17,'2001-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,18,'2001-06-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,19,'2001-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,20,'2001-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,21,'2001-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,22,'2001-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,23,'2001-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,24,'2001-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,25,'2002-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,26,'2002-02-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,27,'2002-03-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,28,'2002-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,29,'2002-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,30,'2002-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,31,'2002-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,32,'2002-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,33,'2002-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,34,'2002-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,35,'2002-11-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,36,'2002-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,37,'2003-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,38,'2003-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,39,'2003-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,40,'2003-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,41,'2003-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,42,'2003-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,43,'2003-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,44,'2003-08-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,45,'2003-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,46,'2003-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,47,'2003-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,48,'2003-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,49,'2004-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,50,'2004-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,51,'2004-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,52,'2004-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,53,'2004-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,54,'2004-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,55,'2004-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,56,'2004-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,57,'2004-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,58,'2004-10-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,59,'2004-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,60,'2004-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,61,'2005-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,62,'2005-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,63,'2005-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,64,'2005-04-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,65,'2005-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,66,'2005-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,67,'2005-07-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,68,'2005-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,69,'2005-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,70,'2005-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,71,'2005-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,72,'2005-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,73,'2006-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,74,'2006-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,75,'2006-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,76,'2006-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,77,'2006-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,78,'2006-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,79,'2006-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,80,'2006-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,81,'2006-09-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,82,'2006-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,83,'2006-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,84,'2006-12-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,85,'2007-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,86,'2007-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,87,'2007-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,88,'2007-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,89,'2007-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,90,'2007-06-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,91,'2007-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,92,'2007-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,93,'2007-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,94,'2007-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,95,'2007-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,96,'2007-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,97,'2008-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,98,'2008-02-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,99,'2008-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,100,'2008-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,101,'2008-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,102,'2008-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,103,'2008-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,104,'2008-08-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,105,'2008-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,106,'2008-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,107,'2008-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,108,'2008-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,109,'2009-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,110,'2009-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,111,'2009-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,112,'2009-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,113,'2009-05-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,114,'2009-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,115,'2009-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,116,'2009-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,117,'2009-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,118,'2009-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,119,'2009-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,120,'2009-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,121,'2010-01-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,122,'2010-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,123,'2010-03-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,124,'2010-04-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,125,'2010-05-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,126,'2010-06-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,127,'2010-07-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,128,'2010-08-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,129,'2010-09-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,130,'2010-10-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,131,'2010-11-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,132,'2010-12-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,133,'2011-01-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,134,'2011-02-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,135,'2011-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,136,'2011-04-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,137,'2011-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,138,'2011-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,139,'2011-07-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,140,'2011-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,141,'2011-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,142,'2011-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,143,'2011-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,144,'2011-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,145,'2012-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,146,'2012-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,147,'2012-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,148,'2012-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,149,'2012-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,150,'2012-06-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,151,'2012-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,152,'2012-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,153,'2012-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,154,'2012-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,155,'2012-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,156,'2012-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,157,'2013-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,158,'2013-02-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,159,'2013-03-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,160,'2013-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,161,'2013-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,162,'2013-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,163,'2013-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,164,'2013-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,165,'2013-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,166,'2013-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,167,'2013-11-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,168,'2013-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,169,'2014-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,170,'2014-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,171,'2014-03-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,172,'2014-04-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,173,'2014-05-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,174,'2014-06-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,175,'2014-07-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,176,'2014-08-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,177,'2014-09-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,178,'2014-10-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,179,'2014-11-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,180,'2014-12-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,181,'2015-01-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,182,'2015-02-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,183,'2015-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,184,'2015-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,185,'2015-05-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,186,'2015-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,187,'2015-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,188,'2015-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,189,'2015-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,190,'2015-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,191,'2015-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,192,'2015-12-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,193,'2016-01-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,194,'2016-02-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,195,'2016-03-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,196,'2016-04-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,197,'2016-05-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,198,'2016-06-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,199,'2016-07-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,200,'2016-08-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,201,'2016-09-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,202,'2016-10-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,203,'2016-11-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,204,'2016-12-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,205,'2017-01-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,206,'2017-02-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,207,'2017-03-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,208,'2017-04-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,209,'2017-05-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,210,'2017-06-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,211,'2017-07-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,212,'2017-08-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,213,'2017-09-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,214,'2017-10-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,215,'2017-11-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,216,'2017-12-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,217,'2018-01-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,218,'2018-02-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,219,'2018-03-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,220,'2018-04-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,221,'2018-05-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,222,'2018-06-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,223,'2018-07-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,224,'2018-08-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,225,'2018-09-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,226,'2018-10-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,227,'2018-11-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,228,'2018-12-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,229,'2019-01-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,230,'2019-02-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,231,'2019-03-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,232,'2019-04-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,233,'2019-05-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,234,'2019-06-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,235,'2019-07-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,236,'2019-08-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,237,'2019-09-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,238,'2019-10-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,239,'2019-11-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,240,'2019-12-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,241,'2020-01-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,242,'2020-02-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,243,'2020-03-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,244,'2020-04-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,245,'2020-05-14')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,246,'2020-06-18')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,247,'2020-07-16')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,248,'2020-08-20')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,249,'2020-09-17')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,250,'2020-10-15')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,251,'2020-11-19')
		insert into ExpiryInCalendar(expCalendarID,expiryID,expiryDate) values (3,252,'2020-12-17')
	end


Commit Transaction


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DataInfo_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DataInfo_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_DataInfo_Get
	@iDataInfoID int = null,		-- DataInfo identifier
	@vcKeyName varchar(50) = null,		-- key's name
	@dtDateMin datetime = null,		-- the minimum date for seek
	@dtDateMax datetime = null,		-- the maximum date for seek
	@iTenorValue int = null,		-- tenor's value
	@vcSID varchar(32) = '',		-- session's identifier
	@iStatus int = null out,		-- -1 - unexpected error
						--  0 - success
						--  1 - user is not logged on. Access denied
	@bOnlyMaxDate bit = 0			-- flag for max date returning
as
/*
	Created by Andrey Sinkin
	Description:	this procedure returns information about databases parameters
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
/*	----------------------------------------------------------------------------------------
--	declare @iStatus int	set @iStatus = -1
	exec xsp_Permission_Check
		@vcSID = @vcSID,		-- session's identifier
		@iStatus = @iStatus output	-- -1 - unexpected error
						--  0 - success
						--  1 - user is not logged on. Access denied
	if @iStatus <> 0	begin set @error = @iStatus goto finish end
	----------------------------------------------------------------------------------------
*/
	-- Declaring variables
	declare @iTenorID int
	if @iDataInfoID is null and @vcKeyName is not null
		select @iDataInfoID = @iDataInfoID from DataInfo where keyName = @vcKeyName
	if @iDataInfoID in (2, 3) and @iTenorValue is not null
		begin
			select @iTenorID = tenorID from Tenor where value = @iTenorValue
		end
/*	if @iDataInfoID = 2
		begin
			if @bOnlyMaxDate <> 1
				begin
					if @dtDateMin is null		set @dtDateMin = getdate()-getdate()
					if @dtDateMax is null		set @dtDateMax = getdate()
					select @dtDateMin = convert(varchar(8), @dtDateMin, 112), @dtDateMax = convert(varchar(8), @dtDateMax, 112)
					select [date] dtDate 
						from IndexDataHistory 
						where tenorID = @iTenorID and [date] between @dtDateMin and @dtDateMax
						group by [date] 
						order by [date]
				end
			else
				select max([date]) dtDate from IndexDataHistory where tenorID = @iTenorID
		end */
	else
	if @iDataInfoID = 3
		begin
			if @bOnlyMaxDate <> 1
				begin
					if @dtDateMin is null		set @dtDateMin = getdate()-getdate()
					if @dtDateMax is null		set @dtDateMax = getdate()
					select @dtDateMin = convert(varchar(8), @dtDateMin, 112), @dtDateMax = convert(varchar(8), @dtDateMax, 112)
					select [date] dtDate 
						from StockDataHistory 
						where tenorID = @iTenorID and [date] between @dtDateMin and @dtDateMax
						group by [date] 
						order by [date]
				end
			else
				select max(date) dtDate from StockDataHistory where tenorID = @iTenorID
		end
	else
		select dataInfoID iDataInfoID, keyName vcKeyName, keyValue vcKeyValue
			from DataInfo
			where (@iDataInfoID is null or dataInfoID = @iDataInfoID)
	set @error = @@error
finish:
	if @iStatus is null 
		set @iStatus=0
Return(@iStatus)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_Exchange_Del
		@iExchangeID int
  as
	update Exchange set actionID=3 where exchangeID = @iExchangeID
	--delete Exchange where exchangeID = @iExchangeID
	  if (@@error <> 0)
		 return (-1)
	   else  
		 return 0



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE     proc usp_Exchange_Get
	@iExchangeID int = null,
	@dtActionDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Exchange table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
  
	select
		exchangeID as iExchangeID,
		exchangeName as vcExchangeName,
		[Description] as vcDescription,
		ExchangeCode as vcExchangeCode,
		actionDate as dtActionDate,
		actionID as tiActionID
	from Exchange
	where (exchangeID=@iExchangeID or @iExchangeID is null)
		and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   proc usp_Exchange_Save
		@iExchangeID int = null output,
		@vcExchangeName varchar(30) = null,
		@vcDescription varchar(120) = null,
		@vcExchangeCode varchar(10) = null
  as
	if exists (select * from Exchange where ExchangeName = @vcExchangeName and actionID < 3 and (@iExchangeID <> ExchangeID or @iExchangeID is null))
		begin
			Raiserror ('Exchange with this name already exists!', 16, 1)
			Return (-1)
		end   
	if exists (select * from Exchange where ExchangeCode = @vcExchangeCode and actionID < 3 and (@iExchangeID <> ExchangeID or @iExchangeID is null))
		begin
			Raiserror ('Exchange with this code already exists!', 16, 1)
			Return (-1)
		end   
   
	if @iExchangeID is null
	--if not exists(select * from Exchange where exchangeID = @iExchangeID)  
	 begin
		insert into Exchange (exchangeName, [Description],exchangeCode)
				values (@vcExchangeName, @vcDescription,@vcExchangeCode)
		if (@@error <> 0) return (-1)
		set @iExchangeID = @@identity
	 end
   else 
	 begin
		update Exchange
			 set  exchangeName = isnull(@vcExchangeName,exchangeName),
				[Description] = isnull(@vcDescription,[Description]),
				exchangeCode = isnull(@vcExchangeCode,exchangeCode)
		  where exchangeID = @iExchangeID
		if (@@error <> 0)
		   return (-1)
		 else  
		   return (0)
	 end				



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHoliday_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExchangeHoliday_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_ExchangeHoliday_Del
	  @iExchangeID int,
	  @iHolidayID int = Null
as
	  if ((@iExchangeID is Null) and (@iHolidayID is Null)) return (-2)
   
	  if (@iHolidayID is Null)
		update ExchangeHoliday set actionID=3 where exchangeID = @iExchangeID
		 --delete ExchangeHoliday where exchangeID = @iExchangeID
	   else
		update ExchangeHoliday set actionID=3 where exchangeID = @iExchangeID and holidayID = @iHolidayID
		 --delete ExchangeHoliday where exchangeID = @iExchangeID and holidayID = @iHolidayID
	
	  if (@@error <> 0)
		 return (-1)
	   else
		 return 0



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHoliday_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExchangeHoliday_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_ExchangeHoliday_Get
	@iExchangeID int = null,		-- exchange's identifier
	@dtActionDate datetime = null
as
/*
	Created by Andrey Sinkin
	this procedure gets data from ExchangeHoliday table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	exchangeID iExchangeID, 
			holidayID iHolidayID,
			actionDate as dtActionDate,
			actionID as tiActionID
		from ExchangeHoliday
		where (@iExchangeID is null or exchangeID = @iExchangeID)
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHoliday_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExchangeHoliday_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_ExchangeHoliday_Save
	  @iExchangeID int,
	  @iHolidayID int
as
	if exists (select * from ExchangeHoliday 
				where exchangeID = @iExchangeID and holidayID = @iHolidayID and actionID < 3)
		begin
			Raiserror ('Exchange holiday with this exchangeID and holidayID already exists!', 16, 1)
			Return (-1)
		end
	  insert into ExchangeHoliday (exchangeID, holidayID)
		   values (@iExchangeID, @iHolidayID)
	  if (@@error <> 0)
		 return (-1)
	   else
		 return (0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE    Procedure dbo.usp_Expiry_Del
	@iExpiryID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int
	set @error = -1

	Raiserror ('Can''t delete expiration information ',16,1)
/*
		begin tran
			update expiry set actionID=3 where expiryID = @iExpiryID
			--delete expiry where expiryID = @iExpiryID
	
		  set @error = @@error
		  if (@error <> 0)
			begin
			  rollback tran
			  goto finish
			end 
		commit tran 
*/
  finish:	 
		Return(@error)







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE       Procedure dbo.usp_Expiry_Get
	@iExpiryID int = null,
	@vcExpiryPeriod varchar(30) = null,
	@dtActionDate datetime = null,
	@iExpCalendarID int = 1
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select Expiry.expiryID as iExpiryID, 
		expiryPeriod as vcExpiryPeriod, 
		EC.expiryDate as dtExpiryDate,
		EC.vegaWeight as fVegaWeight,
		actionDate as dtActionDate,
		actionID as tiActionID
		from Expiry
				join ExpiryInCalendar EC on Expiry.expiryID = EC.expiryID and EC.expCalendarID = @iExpCalendarID

		where ((expiryPeriod = @vcExpiryPeriod or @vcExpiryPeriod is null) and 
				(Expiry.expiryID = @iExpiryID or @iExpiryID is null))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)

	order by expiryDate

	set @error = @@error
Return(@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_Expiry_Save
	@iExpiryID int,
	@vcExpiryPeriod varchar(30),
	@dtExpiryDate smalldatetime = Null,
	@fVegaWeight float = Null,
	@iExpCalendarID int = 1 
as
/*
	Changed by Zaur Nuraliev 20011204
	[Description]:	this procedure save expiry parametrs for GU interface
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	update Expiry
	   	set expiryPeriod = isNull(@vcExpiryPeriod,expiryPeriod)
		where expiryID = @iExpiryID
	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

	if @dtExpiryDate is not null or @fVegaWeight is not null
		begin
			if exists(select * from ExpiryInCalendar where expiryID = @iExpiryID  and expCalendarID = @iExpCalendarID)
				begin
					update ExpiryInCalendar
					   	set vegaWeight = isNull(@fVegaWeight,vegaWeight),
							expiryDate = isNull(@dtExpiryDate,expiryDate)
						where expiryID = @iExpiryID and expCalendarID = @iExpCalendarID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else	
				begin
					insert into ExpiryInCalendar (expiryID,expCalendarID,vegaWeight,expiryDate)
						values(@iExpiryID,@iExpCalendarID,@fVegaWeight,@dtExpiryDate)

					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				end
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE PROCEDURE dbo.usp_ExpiryCalendar_Del 
	@iExpCalendarID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iExpCalendarID <= 3 
	begin
		Raiserror('You can''t delete system defined calendar!',16,1)
		Return(-1)
	end

	begin tran
		delete from ExpiryInCalendar where expCalendarID  = @iExpCalendarID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
	
		delete from ExpiryCalendar where expCalendarID  = @iExpCalendarID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
	commit tran


	set @error = @@error

finish:
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE PROCEDURE dbo.usp_ExpiryCalendar_Get 
	@iExpCalendarID int = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select 
		expCalendarID as iExpCalendarID,
		expCalendarName  as vcExpCalendarName,
		expCalendarDesc  as vcExpCalendarDesc
		from ExpiryCalendar 
		where (expCalendarID = @iExpCalendarID or @iExpCalendarID is null)
		
	set @error = @@error
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE  PROCEDURE dbo.usp_ExpiryCalendar_Save 
	@iExpCalendarID int = null output, 
	@vcExpCalendarName varchar(50) = null,
	@vcExpCalendarDesc varchar(250) = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction
		if not exists(select * From ExpiryCalendar where expCalendarID = @iExpCalendarID) 
			begin
				insert into ExpiryCalendar 
					(expCalendarName, expCalendarDesc) 
					values (@vcExpCalendarName,@vcExpCalendarDesc)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iExpCalendarID = @@identity
			end
		else
			begin
				update ExpiryCalendar set 
					expCalendarName = isnull(@vcExpCalendarName, expCalendarName),
					expCalendarDesc = isnull(@vcExpCalendarDesc, expCalendarDesc)
				where expCalendarID = @iExpCalendarID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction
    set @error = 0
finish:
	if @error=0 
	    set @error = @iExpCalendarID

Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Filter_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Filter_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_Filter_Del 
	@iFilterID int
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Filter table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete FilterContent where filterID = @iFilterID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		delete Filter where filterID = @iFilterID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
finish:
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Filter_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Filter_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_Filter_Get 
	@iFilterID int = Null 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Filter table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if @iFilterID is not null
		select	Filter.filterID as iFilterID,
				Filter.filterName as vcFilterName,
				FF.fieldName as vcFieldName,
				FC.fieldValue as vcFieldValue
			from Filter	
				join FilterContent FC on Filter.filterID =FC.filterID
				join FilterField FF on FC.filterFieldID = FF.filterFieldID
			where Filter.filterID=@iFilterID
	else 
		select	filterID as iFilterID,
				filterName as vcFilterName
			from Filter	
			order by filterID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Filter_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Filter_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE     Procedure dbo.usp_Filter_Save 
	@iFilterID int = Null Output,		--the row identifier
	@vcFilterName varchar(80) = Null, 	--the filter name
	@iTraderID int = Null,  		
	@iBookID int = Null,  		
	@iStrategyID int = Null,  	
	@iContractGroupID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into Filter structure
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from Filter where filterName = @vcFilterName and (@iFilterID <> filterID or @iFilterID is null))
		begin
			Raiserror ('Filter with this name already exists!', 16, 1)
			Return (-1)
		end   
	Begin Transaction
		if not exists(select * from Filter where filterID = @iFilterID) 
			begin
				insert into Filter (filterName) 
					values (@vcFilterName)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iFilterID = @@identity 
			end
		else
			begin
				update Filter set 
					filterName = isnull(@vcFilterName, filterName)
					where filterID = @iFilterID
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	--==========================================================================
	--update filter content 
	--==========================================================================
--	if @iTraderID is not null
--		begin
			if exists(select * from FilterContent where filterID = @iFilterID and filterFieldID = 1)
				begin
					update FilterContent 
						set fieldValue = @iTraderID
						where filterID = @iFilterID and filterFieldID = 1
		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				begin
					insert into FilterContent 
						(filterID,filterFieldID,fieldValue)
						values (@iFilterID,1,@iTraderID) 
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				end
--		end
--	if @iBookID is not null
--		begin
			if exists(select * from FilterContent where filterID = @iFilterID and filterFieldID = 2)
				begin
					update FilterContent 
						set fieldValue = @iBookID
						where filterID = @iFilterID and filterFieldID = 2
		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				begin
					insert into FilterContent 
						(filterID,filterFieldID,fieldValue)
						values (@iFilterID,2,@iBookID) 
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				end
--		end
--	if @iStrategyID is not null
--		begin
			if exists(select * from FilterContent where filterID = @iFilterID and filterFieldID = 3)
				begin
					update FilterContent 
						set fieldValue = @iStrategyID
						where filterID = @iFilterID and filterFieldID = 3
		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				begin
					insert into FilterContent 
						(filterID,filterFieldID,fieldValue)
						values (@iFilterID,3,@iStrategyID) 
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				end
--		end
--	if @iContractGroupID is not null
--		begin
			if exists(select * from FilterContent where filterID = @iFilterID and filterFieldID = 4)
				begin
					update FilterContent 
						set fieldValue = @iContractGroupID
						where filterID = @iFilterID and filterFieldID = 4
		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				begin
					insert into FilterContent 
						(filterID,filterFieldID,fieldValue)
						values (@iFilterID,4,@iContractGroupID) 
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				end
--		end
Commit Transaction
    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FilterData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FilterData_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_FilterData_Get
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
	set @vcSelect1 = 'select distinct contractID = case isnull(O.underlyingContractID, 0)'
			 +' when isnull(null, 0) then T.contractID'
			 +' else O.underlyingContractID end'
			 +' from TradeView T left join OptionView O on T.contractID = O.optionID'
	set @vcSelect2 =''
	set @vcSelect3 = ' where 1=1'  
	declare @vcFiledValue varchar(50)
	-- TraderID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 1 )	
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
			set @vcSelect2 = @vcSelect2 + ' join ContractInGroup C1 on (O.underlyingContractID=C1.contractID or T.contractID = C1.contractID) and C1.groupID=' + @vcFiledValue 
		end
	--select @vcSelect1+@vcSelect2+@vcSelect3
	execute (@vcSelect1 + @vcSelect2 + @vcSelect3)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FilterList_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FilterList_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_FilterList_Get 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Filter table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select	Filter.filterID as iFilterID,
			Filter.filterName as vcFilterName
		from Filter	
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FilterTradeData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FilterTradeData_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE     Procedure dbo.usp_FilterTradeData_Get
	@iTraderID int = null,
	@iBookID int = null,
	@iStrategyID int = null
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
	set @vcSelect1 = 'Select distinct tradeID from TradeView where '
	set @vcSelect2 =''
	-- TraderID
	if @iTraderID is not null
		begin
			set @vcSelect2 = 'traderID=' + Convert(varchar(15),@iTraderID)
		end
	else set @vcSelect2 = ''
	-- BookID
	if @iBookID is not null
		begin
			if @vcSelect2 = ''
				set @vcSelect2 = ' bookID=' + Convert(varchar(15),@iBookID)
			else
				set @vcSelect2 = @vcSelect2 + ' and bookID=' + Convert(varchar(15),@iBookID)
		end	
	-- StrategyID
	if @iStrategyID is not null
		begin
			if @vcSelect2 = ''
				set @vcSelect2 = ' strategyID=' + Convert(varchar(15),@iStrategyID)
			else
				set @vcSelect2 = @vcSelect2 + ' and strategyID=' + Convert(varchar(15),@iStrategyID)
		end
	--else set @vcSelect2 = ''
	if @vcSelect2 =''
		set @vcSelect2=' 1=1'
	
/*
	if @vcSelect2 =''
		set @vcSelect2=' traderID is not null'
	else 
		set @vcSelect2= @vcSelect2 + ' and traderID is not null'
*/
	--====================================================================
	-- execute created script
	--====================================================================
	--select @vcSelect1 + @vcSelect2
	
	execute (@vcSelect1 + @vcSelect2)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FixMarketStructure]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FixMarketStructure]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE proc dbo.usp_FixMarketStructure
@iNumberOfHours int = null,
@iContractID int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure mark us deleted unusefull options
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iNumberOfHours is null 
		set @iNumberOfHours = 2

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select O.OptionID--,A.MSUpdateDate,O.actionDate
			from [Option] O
				join (select contractID,MSUpdateDate from Contract where MSUpdateDate is not null and actionID<3) A 
					on O.underlyingContractID = A.contractID 
					and O.actionDate < A.MSUpdateDate 
			where O.actionID < 3 and O.isManualyEntered=0 and 
					datediff(hour,O.actionDate, A.MSUpdateDate) > @iNumberOfHours and 
					(A.contractID = @iContractID or @iContractID is null)

	exec usp_Option_Del
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationGroupItemsClear_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationGroupItemsClear_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_GenerationGroupItemsClear_Save
	@iGroupID int --the  group identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure break relation between StockGroup and Stock table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	update Contract set groupID=Null where groupID = @iGroupID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRule_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRule_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_GenerationRule_Del
	@iRuleID int  --the rule identifier
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure remove inf. 
					from GenerationRule table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
	update Contract set ruleID=Null where ruleID=@iRuleID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end	
	delete GenerationRule Where ruleID = @iRuleID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRule_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRule_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE       Procedure dbo.usp_GenerationRule_Get
	@iRuleID int=Null --the rule identifier
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure get inf. 
					from GenerationRule table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	ruleID as iRuleID,
			ruleType as iRuleType,
			ruleName as vcRuleName, 
			interpolationFactor as fInterpolationFactor, 
			callShift as fCallShift , 
			putShift as fPutShift,
			smileAccelerator as fSmileAccelerator , 
			strikeIncrement as iStrikeIncrement, 
			strikeMoneynessLo as fStrikeMoneynessLo,
			strikeMoneynessHi as fStrikeMoneynessHi,
			isDefaultRule as bIsDefaultRule,
			requiredPoints as iRequiredPoints
		from GenerationRule
		where (ruleID = @iRuleID and @iRuleID Is Not Null) or (@iRuleID Is Null)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRule_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRule_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE         Procedure dbo.usp_GenerationRule_Save
	@iRuleID int=Null,		 	--the row identifier
	@iRuleType int=Null,			--the rule type	
	@vcRuleName varchar(250)=Null, 	--the rule name
	@fInterpolationFactor float=Null,--the Factor of Interpolation
	@fCallShift float = Null,			--the CallRange
	@fPutShift float = Null,			--the PutRange
	@fSmileAccelerator float=Null,		--the Smile Accelerator
	@iStrikeIncrement int=Null,		--the Strike Increment
	@fStrikeMoneynessHi float=Null,		--the Amount Of Points Required
	@fStrikeMoneynessLo float=Null,		--the Amount Of Points low value
	@bIsDefaultRule bit=Null,		--the IsDefault atribute
	@iRequiredPoints int=Null		--the number of Points Required
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if not exists (select * from GenerationRule where ruleID = @iRuleID) 
		begin
			insert into GenerationRule (
					ruleType,
					ruleName,
					interpolationFactor,
					callShift,
					putShift,
					smileAccelerator,
					strikeIncrement,
					strikeMoneynessHi,
					strikeMoneynessLo,
					isDefaultRule,
					requiredPoints
					) 
				values (
					@iRuleType,
					@vcRuleName,
					@fInterpolationFactor,
					@fCallShift,
					@fPutShift,
					@fSmileAccelerator,
					@iStrikeIncrement,
					@fStrikeMoneynessHi,
					@fStrikeMoneynessLo,
					@bIsDefaultRule,
					@iRequiredPoints)
	
			set @iRuleID=@@IDENTITY	
		end
	else
		begin
			update GenerationRule set
				ruleName = isnull(@vcRuleName, ruleName),
				ruleType = isnull(	@iRuleType, ruleType),
				interpolationFactor = isnull(@fInterpolationFactor, interpolationFactor),
				callShift = isnull(@fCallShift, callShift),
				putShift = isnull(@fPutShift, putShift),
				smileAccelerator = isnull(@fSmileAccelerator, smileAccelerator),
				strikeIncrement = isnull(@iStrikeIncrement, strikeIncrement),
				strikeMoneynessHi = isnull(@fstrikeMoneynessHi, strikeMoneynessHi),
				strikeMoneynessLo = isnull(@fStrikeMoneynessLo, strikeMoneynessLo),
				isDefaultRule=isnull(@bIsDefaultRule,IsDefaultRule),
				requiredPoints = isnull(@iRequiredPoints,requiredPoints)
			where @iRuleID = ruleID
		end
	set @error = @@error
if @error<>0 
	Return(@error)
else 
	Return(@iRuleID)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRuleGroup_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRuleGroup_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_GenerationRuleGroup_Del
	@iGroupID int --the  row identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure remove information 
			 from GenerationRuleGroup table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
    Begin Transaction
		update Contract set groupID=Null where groupID = @iGroupID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		delete GenerationRuleGroup where groupID = @iGroupID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
    Commit Transaction
    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRuleGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRuleGroup_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_GenerationRuleGroup_Get
	@iGroupID int =Null --the group identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure return information 
			     from GenerationRuleGroup table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	groupID as iGroupID,
			groupName as vcGroupName,
			ruleID as iRuleID, 
			isActive as bIsActive
		from GenerationRuleGroup
		where (groupID = @iGroupID and @iGroupID Is Not Null) or (@iGroupID Is Null)
		order by groupName
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRuleGroup_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRuleGroup_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_GenerationRuleGroup_Save
	@iGroupID int=Null, 			--the group identifier
	@vcGroupName varchar(250)=Null, --the group name
	@iRuleID int=Null, 				--the rule identifier
	@bIsActive bit=Null 			--the row 'isActive' atribute 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save information 
			 into GenerationRuleGroup table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
--	declare @error int		set @error = -1
	if not exists(select * from GenerationRuleGroup where groupID=@iGroupID)
		begin
			insert into GenerationRuleGroup(
					groupName,
					ruleID,
					isActive) 
				values (
					@vcGroupName,
					@iRuleID,
					@bIsActive)
			set @iGroupID = @@identity
		end
	else
		begin
			update GenerationRuleGroup set
				groupName = isnull(@vcGroupName, groupName),
				ruleID = isnull(@iRuleID, ruleID),
				isActive = isnull(@bIsActive, isActive)
			where @iGroupID = groupID
		end
--	set @error = @@error
Return(@iGroupID)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_HistRangesCache_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_HistRangesCache_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure usp_HistRangesCache_Del
	@iContractID int	-- contract's identifier
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure deletes data from HistRangesCache table
			using unic key fields combination 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	Delete HistRangesCache 
	Where @iContractID = contractID
Return(@@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_HistRangesCache_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_HistRangesCache_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    PROCEDURE usp_HistRangesCache_Get 
	@iContractID int 	-- the contract identifier
AS
/*
	Created by Zaur Nuraliev
	Description:	this procedure return rows from  HistRangesCache
					table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Select 
		contractID as iContractID,
		optionExpDate as dtOptionExpDate,
		strike as fStrike,
		HVLow as fHVLow,
		HVHigh as fHVHigh,
		isATM as bIsATM
	From HistRangesCache
	Where contractID = @iContractID
	Order by iContractID,dtOptionExpDate,fStrike asc
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_HistRangesCache_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_HistRangesCache_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  PROCEDURE usp_HistRangesCache_Save 
	@iContractID int, 			--the ContractID value	
	@dtOptionExpDate datetime,	--the OptionExpDate value
	@fStrike float = Null, 		--the Strike value
	@fHVLow float  = Null, 		--the HVLow value
	@fHVHigh float  = Null, 	--the HVHigh value
	@bIsATM bit  = Null   	--the IsATM value
AS
/*
	Created by Zaur Nuraliev
	Description:	this procedure save information to 
					HistRangesCache table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
    Begin Transaction
		if not exists(select * From HistRangesCache where @iContractID = contractID and @dtOptionExpDate = optionExpDate and @fStrike = strike) 
			begin
				insert into HistRangesCache( 
					ContractID,
					OptionExpDate,
					Strike,
					HVLow,
					HVHigh,
					isATM) 
				values (
						@iContractID,
						@dtOptionExpDate,
						@fStrike,
						@fHVLow,
						@fHVHigh,
						@bIsATM)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update HistRangesCache
					set HVLow = isnull(@fHVLow,HVLow),
						HVHigh = isnull(@fHVHigh,HVHigh),
						isATM = isnull(@bIsATM,isATM)
				where @iContractID = contractID and @dtOptionExpDate = optionExpDate and @fStrike = strike
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Holiday_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Holiday_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_Holiday_Del
	  @iHolidayID int
 as
	
	update Holiday set actionID=3 where holidayID = @iHolidayID 
	--delete Holiday where holidayID = @iHolidayID 
   
	if (@@error <> 0) 
	   return (-1)
	 else 
	   return 0



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Holiday_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Holiday_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   proc usp_Holiday_Get
		@iExchangeID int = null,
		@iHolidayID int = null,
		@dtActionDate datetime = null
 as
	 if (@iHolidayID is Null)
	   begin
		   if (@iExchangeID is Null)
			 select holidayID as iHolidayID, 
					holidayName as vcHolidayName, 
					holidayDate as dtHolidayDate,
					actionDate as dtActionDate,
					actionID as tiActionID
				from Holiday
				where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
				order by holidayDate 
			else
			 select h.holidayID as iHolidayID, 
					h.holidayName as vcHolidayName, 
					h.holidayDate as dtHolidayDate,
					h.actionDate as dtActionDate,
					h.actionID as tiActionID
				from ExchangeHolidayView eh join Holiday h on h.holidayID = eh.holidayID
				where eh.exchangeID = @iExchangeID
					and (h.actionDate>=@dtActionDate or @dtActionDate is null and h.actionID<3)
			  order by h.holidayDate
	   end
	 else
		 select holidayID as iHolidayID, 
				holidayName as vcHolidayName, 
				holidayDate as dtHolidayDate,
				actionDate as dtActionDate,
				actionID as tiActionID
		   from Holiday
		 where holidayID = @iHolidayID 
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
Return(0)	  



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Holiday_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Holiday_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_Holiday_Save
		@iHolidayID int,
		@dtHolidayDate smalldatetime,
		@vcHolidayName varchar(30)
 as
  
	if exists (select * from Holiday where holidayDate = @dtHolidayDate and actionID < 3 and (@iHolidayID <> holidayID or @iHolidayID is null))
		begin
			Raiserror ('Row in Holiday table with same holiday date already exists!', 16, 1)
			Return (-1)
		end   
	if exists (select * from Holiday where holidayName = @vcHolidayName and actionID < 3 and (@iHolidayID <> holidayID or @iHolidayID is null))
		begin
			Raiserror ('Row in Holiday table with same holiday name already exists!', 16, 1)
			Return (-1)
		end  
   if (@iHolidayID is Null)  
	 begin
		 insert into Holiday (holidayName, holidayDate)
				values (@vcHolidayName, @dtHolidayDate)
		 if (@@error <> 0)
			return (-1)
		  else	
			return @@identity
	 end
   else 
	 begin
		update Holiday 
			 set holidayName = @vcHolidayName,
				  holidayDate = @dtHolidayDate		  
		  where holidayID = @iHolidayID
		if (@@error <> 0)
		   return (-1)
		 else  
		   return 0
	 end				



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE     proc usp_Index_Del
		@iIndexID int
  as
/*
	Created by ...
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	 if exists (select tradeID from Trade
					 where Trade.contractID = @iIndexID)
	 begin
	   RAISERROR ('Error. Can''t delete [Index] because of the existing Trade', 16, 1)
	   RETURN (-1)
	 end

	Begin Transaction

		delete ModelParameter where contractID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update ContractInGroup set actionID=3 where contractID = @iIndexID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update [Index] set actionID=3 where indexID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update [Option] set actionID=3 where underlyingContractID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract set actionID=3 
		where contractID in (select OptionID from [Option] where underlyingContractID = @iIndexID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract set actionID=3 
		where contractID in (select underlyingContractID from [Option] where optionID= @iIndexID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract set actionID=3 where contractID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		
		set @error = @@error
	Commit Transaction
finish:
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  proc dbo.usp_Index_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@dtActionDate datetime = Null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
	if isnull(@tiWithPosition, 0) = 0

	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractView C 	
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractView C 	
					inner join [Index] I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join [Index] I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractView c 	
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, I.isBasket
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractView c 	
					inner join [Index] I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, I.isBasket
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID,I.isBasket
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join [Index] I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID,I.isBasket
				order by C.symbol
		end
	end
if (@@error <> 0)
	Return (-1)
else  
	Return (0)











GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE   proc dbo.usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(8) = Null,
		@vcContractName varchar(255) = Null,
		@iDefaultModelTypeID int = Null,
		@fYield float = Null,
		@bIsHTB bit = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@tiExpCalendarID tinyint = null,
		@tiIsBasket tinyint = null
   as
	---------------
	set nocount on
	---------------

	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=1 and actionID < 3 and (@iIndexID <> contractID or @iIndexID is null))
		begin
			Raiserror ('Index with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iIndexID is Null)  
	 begin
		 
		 begin tran
			 insert into Contract (contractTypeID, symbol, contractName,expCalendarID)
				  values (1, @vcSymbol, @vcContractName,@tiExpCalendarID)
			 select @iIndexID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Index] (indexID, defaultModelTypeID, yield,IsHTB,calcOptionType,calcModelType,isBasket)
					 values (@iIndexID, @iDefaultModelTypeID, @fYield,isNull(@bIsHTB,0),@tiCalcOptionType,@tiCalcModelType,isnull(@tiIsBasket,0))
				 if (@@error <> 0)
					begin
					   RAISERROR ('Error. Can''t insert into [Index]!', 16, 1)
					   ROLLBACK TRAN
					   RETURN (-1)
					end 			
		 commit tran
	 end
   else
	 begin
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@tiExpCalendarID, expCalendarID)
				where contractID = @iIndexID
			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end
		  update [Index]
			   set	defaultModelTypeID = isNull(@iDefaultModelTypeID,defaultModelTypeID),
					yield = isNull(@fYield,yield),
					isHTB = isNull(@bIsHTB,isHTB),
					calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
					calcModelType = isNull(@tiCalcModelType, calcModelType),
					isBasket = isNull(@tiIsBasket,isBasket)
			   where indexID = @iIndexID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update [Index]!', 16, 1)
				  ROLLBACK TRAN
				  RETURN (-1)
			   end		   
		 commit tran
	 end

	--========================================================================
	-- make record with null exchange for this index
	--========================================================================
	if not exists(select * from ContractPrice where exchangeID is null and contractID = @iIndexID)
		begin
			insert into ContractPrice(contractID,exchangeID)
				values(@iIndexID,null)
			if (@@error <> 0) begin rollback tran return(-1) end           		
		end		


if (@@error <> 0)
	return (-1)
else	
	return @iIndexID




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexCorrelationDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexCorrelationDH_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_IndexCorrelationDH_Get
	@vcSymbol1 varchar(8), 
	@vcSymbol2 varchar(8),
	@fCorrelationPrice float = Null Output,
	@fCorrelationVola  float = Null Output
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from IndexCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID1 int, 
			@iIndexID2 int,
			@iIndexID3 int
	select @iIndexID1=contractID from Contract where symbol=@vcSymbol1 and contractTypeID=1
	select @iIndexID2=contractID from Contract where symbol=@vcSymbol2 and contractTypeID=1
	if 	@iIndexID1 > @iIndexID2
		select @iIndexID3=@iIndexID1,@iIndexID1=@iIndexID2,@iIndexID2=@iIndexID3
	select	@fCorrelationPrice=correlationPrice,
			@fCorrelationVola=correlationVola
		from IndexCorrelation	
		where indexID1 = @iIndexID1 and indexID2 = @iIndexID2
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexCorrelationDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexCorrelationDH_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_IndexCorrelationDH_Save
	@vcSymbol1 varchar(8), 
	@vcSymbol2 varchar(8),
	@fCorrelationPrice float = Null,
	@fCorrelationVola  float = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from IndexCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID1 int, 
			@iIndexID2 int,
			@iIndexID3 int  
	select @iIndexID1=contractID from ContractView where symbol=@vcSymbol1 and contractTypeID=1
	select @iIndexID2=contractID from ContractView where symbol=@vcSymbol2 and contractTypeID=1
	if @iIndexID1>@iIndexID2
		select @iIndexID3=@iIndexID1,@iIndexID1=@iIndexID2,@iIndexID2=@iIndexID3
	Begin Transaction
		if not exists(select * from IndexCorrelation where IndexID1=@iIndexID1 and IndexID2=@iIndexID2) 
			begin
				insert into IndexCorrelation(IndexID1,IndexID2,correlationPrice,correlationVola)
					values(@iIndexID1,@iIndexID2,@fCorrelationPrice,@fCorrelationVola)
			end 
		else
			begin
				update IndexCorrelation
					set IndexID1=@iIndexID1,
						IndexID2=@iIndexID2,
						correlationPrice=isnull(@fCorrelationPrice,CorrelationPrice),
						correlationVola=isnull(@fCorrelationVola,CorrelationVola)
				where IndexID1=@iIndexID1 and IndexID2=@iIndexID2	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDataHistory_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDataHistory_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_IndexDataHistory_Save
	@vcIndexTicker varchar(12) = null,	-- index ticker
	@iIndexID int = null,			-- index identifier
	@dtDate datetime,			-- date
	@iTenorID int = null,			-- tenor identifier
	@iTenorValue int = null,		-- tenor value
	@fDelta1 float = null,			-- delta 1 index's parameter
	@fDelta3 float = null,			-- delta 3 index's parameter 
	@fImpliedCorrelation float = null,	-- implied correlation index's parameter
	@fCurrentIV float = null,		-- current implied volatility index's parameter
	@fIVMean float = null,			-- implied volatility mean index's parameter
	@fIVCall float = null,			-- implied volatility call index's parameter
	@fIVPut float = null,			-- implied volatility put index's parameter
	@fCurrentHV float = null,		-- current historical volatility index's parameter
	@fPriceClose float = null,		-- index's close price
	@fCurrentVega float = null,
	@fCurrentVolOfVol float = null
as
/*
	Created by Andrey Sinkin
	this procedure saves historical data for index(s)
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iMarketDataID int
	-- if wrong parameters -> exit
	if @vcIndexTicker is null and @iIndexID is null or @iTenorID is null and @iTenorValue is null	goto finish
	if @iIndexID is null	select @iIndexID = contractID from ContractView where symbol = @vcIndexTicker and contractTypeID = 1
	if @iTenorID is null	select @iTenorID = tenorID from Tenor where value = @iTenorValue
	Begin Transaction
		if not exists(select * from IndexDataHistory where indexID = @iIndexID and date = @dtDate and tenorID = @iTenorID)
			begin
				insert into IndexDataHistory (indexID, date, tenorID, delta1, delta3, impliedCorrelation, currentIV, IVMean, IVCall, IVPut, currentHV,currentVega,currentVolOfVol) 
					values (@iIndexID, @dtDate, @iTenorID, @fDelta1, @fDelta3, @fImpliedCorrelation, @fCurrentIV, @fIVMean, @fIVCall, @fIVPut, @fCurrentHV,@fCurrentVega,@fCurrentVolOfVol)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexDataHistory set 
					delta1 = isnull(@fDelta1, delta1),
					delta3 = isnull(@fDelta3, delta3),
					impliedCorrelation = isnull(@fImpliedCorrelation, impliedCorrelation),
					currentIV = isnull(@fCurrentIV, currentIV),
					IVMean = isnull(@fIVMean, IVMean),
					IVCall = isnull(@fIVCall, IVCall),
					IVPut = isnull(@fIVPut, IVPut),
					currentHV = isnull(@fCurrentHV, currentHV),
					currentVolOfVol = isnull(@fCurrentVolOfVol,currentVolOfVol),
					currentVega = isnull(@fCurrentVega,currentVega)
				where indexID = @iIndexID and date = @dtDate and tenorID = @iTenorID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
/*
		if @fPriceClose is not null
			begin
				exec usp_MarketData_Save 
					@iMarketDataID = @iMarketDataID output,	-- market data identifier
					@iContractID = @iIndexID,		-- contract's identifier
					@dDate = @dtDate,			-- date
					@fPriceClose = @fPriceClose		-- close price
				if @error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
			end
*/
		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDataHistoryDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDataHistoryDH_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_IndexDataHistoryDH_Get
	@vcSymbol varchar(8),	-- index ticker
	@fCurrentIV float = null Output,
	@fCurrentHV float = null Output,
	@fCurrentVolOfVol float = null Output,
	@fCurrentVega float = null Output
as
/*
	Created by Zaur Nuraliev
	this procedure returns historical data for index
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int
	select @iIndexID=contractID from ContractView where symbol=@vcSymbol and contractTypeID = 1
	select 	@fCurrentIV=currentIV, 
			@fCurrentHV=currentHV,
			@fCurrentVolOfVol=currentVolOfVol,
			@fCurrentVega=currentVega
		from IndexDataHistory
		where tenorID=8 
			and [date]=(select max([date]) from IndexDataHistory where indexID=@iIndexID and tenorID=8)
			and indexID=@iIndexID
	set @error = @@error
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_IndexDefinition_Del 
	@iIndexID int = null, 
	@iStockID int = null 
AS
/*
	Created by ...
	Description: this procedure is delete information from IndexDefinition table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		if @iIndexID is null and @iStockID is null
			begin
				update IndexDefinition set actionID=3
				--delete from IndexDefinition
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else if @iIndexID is not null and @iStockID is null
			begin
				update IndexDefinition set actionID=3 where indexID = @iIndexID 
				--delete from IndexDefinition where indexID = @iIndexID 
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else if @iIndexID is null and @iStockID is not null
			begin
				update IndexDefinition set actionID=3 where stockID = @iStockID
				--delete from IndexDefinition where stockID = @iStockID
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else if @iIndexID is not null and @iStockID is not null
			begin
				update IndexDefinition set actionID=3 where stockID = @iStockID and indexID = @iIndexID 
				--delete from IndexDefinition where stockID = @iStockID and indexID = @iIndexID 
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		set @error = @@error
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_IndexDefinition_Get 
	@iIndexID int = Null, 
	@iStockID int = Null,
	@dtActionDate datetime = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from IndexDefinition table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select indexID as iIndexID,
			stockID as iStockID,
			weight as fWeight,
			beta as fBeta,
			actionDate as dtActionDate,
			actionID as tiActionID
		from IndexDefinition
		where ((@iIndexID is null or indexID = @iIndexID) and (@iStockID is null or stockID = @iStockID))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Save]
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
				values (@iIndexID,@iStockID,isnull(@fWeight,0),@fBeta)
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexID_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  procedure usp_IndexID_Get 
	   @vcSymbol varchar(8) 
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting index identifiers
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select C.contractID as iIndexID
		from ContractView c 	
				join IndexView I on I.indexID = C.contractID
		where C.contractTypeID = 1 and C.symbol = @vcSymbol
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexStockCorrelation_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexStockCorrelation_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



Create Procedure dbo.usp_IndexStockCorrelation_Get 
	@iIndexID int,
	@iStockID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting  correlation information from IndexDefinition table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 
		correlationPrice as fCorrelationPrice,
		correlationVola as fCorrelationVola
		from IndexDefinitionView
		where indexID = @iIndexID and stockID = @iStockID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexStockCorrelationDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexStockCorrelationDH_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_IndexStockCorrelationDH_Get
	@vcIndexSymbol varchar(8), 
	@vcStockSymbol varchar(8),
	@fCorrelationPrice float = Null Output,
	@fCorrelationVola  float = Null Output
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from IndexStockCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int, 
			@iStockID int
	select @iIndexID=contractID from ContractView where symbol=@vcIndexSymbol and contractTypeID=1
	select @iStockID=contractID from ContractView where symbol=@vcStockSymbol and contractTypeID=2
	select	@fCorrelationPrice=correlationPrice,
			@fCorrelationVola=correlationVola
		from IndexDefinitionView
		where indexID = @iIndexID and stockID = @iStockID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexStockCorrelationDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexStockCorrelationDH_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_IndexStockCorrelationDH_Save
	@vcIndexSymbol varchar(8), 
	@vcStockSymbol varchar(8),
	@fCorrelationPrice float = Null,
	@fCorrelationVola  float = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save Index-Stock correlation information
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int, 
			@iStockID int
	select @iIndexID=contractID from ContractView where symbol=@vcIndexSymbol and contractTypeID=1
	select @iStockID=contractID from ContractView where symbol=@vcStockSymbol and contractTypeID=2
	if (@iIndexID is null or @iStockID is null)
		goto finish
	Begin Transaction
	if not exists(select * from IndexDefinitionView where indexID=@iIndexID and stockID=@iStockID)
		begin
			insert into IndexDefinition(indexID,stockID,correlationPrice,correlationVola,weight) 
				values(@iIndexID,@iStockID,@fCorrelationPrice,@fCorrelationVola,0)
   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		end
	else
		begin
			update IndexDefinition
				set indexID=@iIndexID,
					stockID=@iStockID,
					correlationPrice=isnull(@fCorrelationPrice,correlationPrice),
					correlationVola=isnull(@fCorrelationVola,correlationVola)
			where indexID=@iIndexID and stockID=@iStockID
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexVM_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_IndexVM_Save
	@iIndexID int = null output,		-- index's identifier
	@iContractTypeID int = null,		-- contract's type identifier
	@iSymbolName varchar(12) = null,	-- index's ticker
	@iExchangeID int = null,			-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,				-- lot size (not used)
	@fYield float = null,				-- yield
	@bIsAutoUpdateWeight bit = null,	-- flag for enable/disable index's weights updating
	@iTenorID int = null,				-- tenor's identifier
	@dtDate datetime = null,			-- date
	@fDelta1 float = null,				-- delta 1
	@fDelta3 float = null,				-- delta 3
	@fImpliedCorrelation float = null,	-- implied correlation
	@fCurrentIV float = null,			-- current implied volatility
	@fIVMean float = null,				-- implied volatility mean
	@fCurrentHV float = null,			-- current historical volatility
	@iTenorValue int = null,			-- tenor's value
	@fIVCall float = null,				-- iv call value
	@fIVPut float = null,				-- iv put value
	@iIndexBaseID int = null,			-- index's base identifier
	@rDenominator real = null,			-- denominator
	@bIsActive bit = null,				-- flag for enable/disable
	@fPriceClose float = null,			-- index's close price
	@iGroupID int = null,
	@iRuleID int = null
as
/*
	Created by Andrey Sinkin
	This procedure saves index data
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	if @iIndexID is null and @iSymbolName is null 		goto finish
	set @iContractTypeID = 1	-- index
	set @iLotSize = 0
	if @iIndexID is null and @iSymbolName is not null
		select @iIndexID = contractID from ContractView where symbol = @iSymbolName and contractTypeID = 1
	if @iTenorID is null	select @iTenorID = tenorID from Tenor where value = @iTenorValue
	Begin Transaction
		if @iIndexID is null
			begin
				insert into Contract (contractTypeID, symbol, contractName, groupID,ruleID)
					values (@iContractTypeID, @iSymbolName, @vcDescription,@iGroupID,@iRuleID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iIndexID = @@identity
--				insert into [Index] (indexID, yield, isAutoUpdateWeight) values (@iIndexID, @fYield, @bIsAutoUpdateWeight)
				insert into [Index] (indexID, yield) values (@iIndexID, @fYield)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				if @iContractTypeID is not null or @iSymbolName is not null or @iExchangeID is not null or @vcDescription is not null or @iLotSize is not null
					begin
						update Contract set 
							contractTypeID = isnull(@iContractTypeID, contractTypeID),
							symbol = isnull(@iSymbolName, symbol),
							contractName = isnull(@vcDescription, contractName),
							groupID = isnull(@iGroupID, groupID),
							ruleID = isnull(@iRuleID, ruleID)
						where contractID = @iIndexID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
					end
				
				--if @fYield is not null or @bIsAutoUpdateWeight is not null
				if @fYield is not null 
					begin
						update [Index] set 
							yield = isnull(@fYield, yield)
							--isAutoUpdateWeight = isnull(@bIsAutoUpdateWeight, isAutoUpdateWeight)
						where indexID = @iIndexID
						if @@error <> 0 begin set @error = 4 if @@TranCount = 1 Rollback Transaction goto finish end
					end
			end
		--save lotSize value
		if @iLotSize is not null
			begin
				if exists(select * from ContractPrice where contractID = @iIndexID and (exchangeID = @iExchangeID or @iExchangeID is null))
					update ContractPrice 
						set lotSize = isnull(@iLotSize,lotSize),
							exchangeID = @iExchangeID,
							contractID = isnull(@iIndexID,contractID)
						where contractID = @iIndexID and (exchangeID = @iExchangeID or @iExchangeID is null)
					if @@error <> 0 begin set @error = 4 if @@TranCount = 1 Rollback Transaction goto finish end
				else
					insert into ContractPrice (contractID,exchangeID,lotSize)
						values(@iIndexID,@iExchangeID,@iLotSize)
					if @@error <> 0 begin set @error = 4 if @@TranCount = 1 Rollback Transaction goto finish end
			end
--=================================================================
-- related data update
--=================================================================
		if @dtDate is not null or @fDelta1 is not null or @fDelta3 is not null or @fImpliedCorrelation is not null or @fCurrentIV is not null or @fIVMean is not null or @fPriceClose is not null
			begin
			   RAISERROR ('Error. Can''t save index data history (attempt of a call usp_IndexDataHistory_Save)! Please send this error message to DB developer !', 16, 1)
/*				exec @error = usp_IndexDataHistory_Save 
					@iIndexID = @iIndexID, 
					@dtDate = @dtDate, 
					@iTenorID = @iTenorID, 
					@fDelta1 = @fDelta1, 
					@fDelta3 = @fDelta3, 
					@fImpliedCorrelation = @fImpliedCorrelation, 
					@fCurrentIV = @fCurrentIV, 
					@fIVMean = @fIVMean, 
					@fCurrentHV = @fCurrentHV, 
					@fIVCall = @fIVCall, 
					@fIVPut = @fIVPut,
					@fPriceClose = @fPriceClose
				if @error <> 0 begin set @error = 5 if @@TranCount = 1 Rollback Transaction goto finish end */
			end
		if @fCurrentHV is not null
			begin
			   RAISERROR ('Error. Can''t save index data snapshot (attempt of a call usp_IndexDataSnapshot_Save)! Please send this error message to DB developer !', 16, 1)
/*				exec @error = usp_IndexDataSnapshot_Save 
					@iIndexID = @iIndexID, 
					@iTenorID = @iTenorID, 
					@fCurrentHV = @fCurrentHV, 
					@dtDateUpdate = @dtDate
				if @error <> 0 begin set @error = 6 if @@TranCount = 1 Rollback Transaction goto finish end */
			end
		if @iIndexBaseID is not null
			begin
			   RAISERROR ('Error. Can''t save ETFLink data (attempt of a call usp_ETFLink_Save)! Please send this error message to DB developer !', 16, 1)
/*				exec @error = usp_ETFLink_Save 
					@iIndexID = @iIndexID, 
					@iIndexBaseID = @iIndexBaseID, 
					@rDenominator = @rDenominator, 
					@bIsActive = @bIsActive
				if @error <> 0 begin set @error = 7 if @@TranCount = 1 Rollback Transaction goto finish end */
			end
/*		if @fPriceClose is not null
			begin
				exec usp_MarketData_Save 
					@iMarketDataID = @iMarketDataID output,	-- market data identifier
					@iContractID = @iIndexID,		-- contract's identifier
					@dDate = @dtDate,			-- date
					@fPriceClose = @fPriceClose		-- close price
				if @error <> 0 begin set @error = 8 if @@TranCount = 1 Rollback Transaction goto finish end
			end
*/
		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexWithPrice_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE	  proc usp_IndexWithPrice_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@bIsBestPriceOnly bit = 0
  as
	if (@iGroupID is Null)
		if @bIsBestPriceOnly=0
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				I.defaultModelTypeID as iDefaultModelTypeID,
				I.yield as fYield,
				CP.priceLast as fPriceLast,
				CP.priceClose as fPriceClose,
				CP.priceOpen as fPriceOpen,
				CP.priceLow as fPriceLow,
				CP.priceHigh as fPriceHigh,
				CP.volume as iVolume,
				E.exchangeID as iExchangeID,
				E.exchangeCode as vcExchangeCode
				from ContractView c 	
						join IndexView I on I.indexID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractTypeID=1
					and (C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null)
				order by C.symbol,E.exchangeCode
		else 
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				I.defaultModelTypeID as iDefaultModelTypeID,
				I.yield as fYield,
				CP.priceLast as fPriceLast,
				CP.priceClose as fPriceClose,
				CP.priceOpen as fPriceOpen,
				CP.priceLow as fPriceLow,
				CP.priceHigh as fPriceHigh,
				CP.volume as iVolume,
				E.exchangeID as iExchangeID,
				E.exchangeName as vcExchangeCode
				from ContractView c 	
						join IndexView I on I.indexID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractTypeID=1
					and (C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null)
					and CP.exchangeID is null
				order by C.symbol,E.exchangeCode
	else
		select C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.lotSize as iLotSize,
			I.defaultModelTypeID as iDefaultModelTypeID,
			I.yield as fYield,
			CP.priceLast as fPriceLast,
			CP.priceClose as fPriceClose,
			CP.priceOpen as fPriceOpen,
			CP.priceLow as fPriceLow,
			CP.priceHigh as fPriceHigh,
			CP.volume as iVolume,
			E.exchangeID as iExchangeID,
			E.exchangeCode as vcExchangeCode
			from ContractViewInGroup CIG
					join ContractView c  on C.contractID = CIG.contractID
					join IndexView I on I.indexID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where CIG.groupID = @iGroupID
					 and C.contractTypeID=1
			order by C.symbol,E.exchangeCode
if (@@error <> 0)
	Return (-1)
else  
	Return (0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexYield_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexYield_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_IndexYield_Save 
	@iIndexID int,
	@fYield float =null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		if @iIndexID is null
		--if not exists(select * From [Index] where indexID=@iIndexID) 
			begin
				insert into [Index] (indexID,yield,isHTB) 
					values (@iIndexID,@fYield,0)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update [Index] set 
					yield = isnull(@fYield, yield)
				where indexID = @iIndexID
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_InterestRateCurve_Del
	@iCurvePointID int	-- InterestRateCurve's identifier
as
/*
	Created by Andrey Sinkin
	this procedure deletes data from InterestRateCurve table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	-- declare @error int	set @error = -1
	--delete InterestRateCurve where curvePointID = @iCurvePointID
	exec usp_IRPoint_Del @iIRpointID = @iCurvePointID
	Return(@@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE  Procedure dbo.usp_InterestRateCurve_Get
	@iCurvePointID int = null,	-- InterestRateCurve identifier
	@bAdvancedMode bit = 0		-- flag for advanced mode
as
/*
	Created by Andrey Sinkin
	this procedure returns data from InterestRateCurve table (now from IRPoint table)
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @date datetime		set @date = getdate()
	if @bAdvancedMode = 0
		select 	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				longRate as fRateAsk, 
				shortRate as fRateBid,
				HTBRate as fHTBRate
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iPeriodTypeID, iNumber
	else
		select	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				longRate as fRateAsk, 
				shortRate as fRateBid,
			case periodTypeID
				when 1 then datediff(day, getdate(), dateadd(day, num, @date))
				when 2 then datediff(day, getdate(), dateadd(week, num, @date))
				when 3 then datediff(day, getdate(), dateadd(month, num, @date))
				when 4 then datediff(day, getdate(), dateadd(year, num, @date))
			end iDaysFromToday,
			HTBRate as fHTBRate
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iDaysFromToday

Return(@@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_InterestRateCurve_Save
	@iCurvePointID int = null output,	-- InterestRateCurve's identifier
	@iPeriodTypeID int = null,		-- type of periods identifier
	@iNumber int = null,			-- number
	@fRateAsk float = null,			-- ask
	@fRateBid float = null			-- bid
as
/*
	Created by Andrey Sinkin
	this procedure saves data to IRPoint (InterestRateCurve) table 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iCurveID int
		select @iCurveID=curveID from IRCurve
	if @iCurvePointID is null and @iPeriodTypeID is not null and @iNumber is not null
		select @iCurvePointID = IRPointID from IRPointView where periodTypeID = 
@iPeriodTypeID and num = @iNumber
	Begin Transaction
		if @iCurvePointID is null 
			begin
				insert into IRPoint (periodTypeID, num, longRate, ShortRate, 
HTBRate,CurveID) 
				values (@iPeriodTypeID, @iNumber, @fRateAsk, 
@fRateBid,@fRateAsk,@iCurveID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 
Rollback Transaction goto finish end
				select @iCurvePointID = @@identity
			end
		else
			begin
				update IRPoint set
					periodTypeID = isnull(@iPeriodTypeID, periodTypeID),
					num = isnull(@iNumber, num),
					longRate = isnull(@fRateAsk, longRate),
					shortRate = isnull(@fRateBid, shortRate)
				where IRPointID = @iCurvePointID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 
Rollback Transaction goto finish end
			end
/*	if @iCurvePointID is null and @iPeriodTypeID is not null and @iNumber is not null
		select @iCurvePointID = curvePointID from InterestRateCurve where 
periodTypeID = @iPeriodTypeID and number = @iNumber
	Begin Transaction
		if @iCurvePointID is null 
			begin
				insert into InterestRateCurve (periodTypeID, number, 
rateAsk, rateBid) 
				values (@iPeriodTypeID, @iNumber, @fRateAsk, @fRateBid)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 
Rollback Transaction goto finish end
				select @iCurvePointID = @@identity
			end
		else
			begin
				update InterestRateCurve set
					periodTypeID = isnull(@iPeriodTypeID, periodTypeID),
					number = isnull(@iNumber, number),
					rateAsk = isnull(@fRateAsk, rateAsk),
					rateBid = isnull(@fRateBid, rateBid)
				where @iCurvePointID = curvePointID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 
Rollback Transaction goto finish end
			end */
		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRCurve_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRCurve_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_IRCurve_Del
	@iCurveID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	begin tran
		update IRPoint set actionID=3 where curveID = @iCurveID
	--delete IRPoint Where curveID = @iCurveID
		update IRCurve set actionID=3 where curveID = @iCurveID
	--delete IRCurve Where curveID = @iCurveID
	if @@error = 0
		commit tran
	else
		rollback tran
	
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRCurve_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRCurve_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_IRCurve_Get
	@iCurveID int = null,
	@dtActionDate datetime = null 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		select curveID as iCurveID,
			curveName as vcCurveName, 
			curveTypeID as iCurveTypeID, 
			ruleTypeID as iRuleTypeID, 
			positionThreshold as mPositionThreshold,
			actionDate as dtActionDate,
			actionID as tiActionID
	from IRCurve
	where
		@iCurveID is Null or @iCurveID is not Null and curveID = @iCurveID and
		(actionDate>=@dtActionDate or @dtActionDate is null)
		and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRCurve_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRCurve_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_IRCurve_Save
	@iCurveID int = Null,
	@vcCurveName varchar(30) = Null,
	@iRuleTypeID int = Null,
	@iCurveTypeID int = Null,
	@mPositionThreshold money = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iCurveID is Null 
		begin
			insert into IRCurve (
					curveName,
					ruleTypeID,
					curveTypeID,
					positionThreshold
					) 
				values (
					@vcCurveName,
					@iRuleTypeID,
					@iCurveTypeID,
					@mPositionThreshold
										)
			select @iCurveID = @@identity
		end
	else
		begin
			update IRCurve set
				curveName = isNull(@vcCurveName, curveName),
				ruleTypeID = isNull(@iRuleTypeID, ruleTypeID),
				curveTypeID = isNull(@iCurveTypeID, curveTypeID),
				positionThreshold = isNull(@mPositionThreshold, positionThreshold)
			where curveID = @iCurveID
		end
	set @error = @@error
	if (@error = 0) set @error = @iCurveID
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoint_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO



CREATE Procedure dbo.usp_IRPoint_Del
	@iIRpointID int = Null,
	@iCurveID int = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	if @iIRpointID is not Null or @iCurveID is not Null
	begin
		begin tran
	
		update IRPoint set actionID=3 
		where (@iCurveID is Null or @iCurveID is not Null and curveID = @iCurveID)
			and (@iIRpointID is Null or @iIRpointID is not Null and IRpointID = @iIRpointID)
/*
		delete IRPoint
		where (@iCurveID is Null or @iCurveID is not Null and curveID = @iCurveID)
		and (@iIRpointID is Null or @iIRpointID is not Null and IRpointID = @iIRpointID)
*/
		if @@error = 0
			commit tran
		else
			rollback tran
	end
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoint_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO



CREATE  Procedure dbo.usp_IRPoint_Get
	@iCurveID int,
	@dtActionDate datetime = null 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select
		IRpointID as iIRpointID, 
		curveID as iCurveID, 
		periodTypeID as iPeriodTypeID, 
		num as iNum, 
		shortRate as fShortRate, 
		HTBRate as fHTBRate, 
		longRate as fLongRate,
		actionDate as dtActionDate,
		actionID as tiActionID
	from
		IRPoint
	where
		(@iCurveID is Null or not @iCurveID is Null and curveID = @iCurveID) and 
			(actionDate>=@dtActionDate or @dtActionDate is null)
			and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
	order by periodTypeID, num 
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoint_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO



CREATE  Procedure dbo.usp_IRPoint_Save
	@iIRpointID int = Null,
	@iCurveID int = Null,
	@iPeriodTypeID int = Null,
	@iNum int = Null,
	@fShortRate float = Null,
	@fHTBRate float = Null,
	@fLongRate float = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from IRPoint where Num = @iNum and  PeriodTypeID=@iPeriodTypeID and actionID < 3 and (@iIRpointID <> IRpointID or @iIRpointID is null))
		begin
			Raiserror ('IRPoint with this period already exists!', 16, 1)
			Return (-1)
		end   
	if @iCurveID is not Null and @iIRpointID is Null
		begin
			insert into IRPoint
					(
					curveID,
					periodTypeID,
					num,
					shortRate,
					HTBRate,
					longRate
					) 
				values (
					@iCurveID,
					@iPeriodTypeID,
					@iNum,
					@fShortRate,
					@fHTBRate,
					@fLongRate
										)
			select @iIRpointID = @@identity
		end
	else if @iIRpointID is not Null
		begin
			update IRPoint set
				curveID = isNull(@iCurveID, curveID),
				periodTypeID = isNull(@iPeriodTypeID, periodTypeID),
				num = isNull(@iNum, num),
				shortRate = isNull(@fShortRate, shortRate),
				HTBRate = isNull(@fHTBRate, HTBRate),
				longRate = isNull(@fLongRate, longRate)
			where IRpointID = @iIRpointID
		end
	set @error = @@error
	if (@error = 0) set @error = @iCurveID
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IsSymbolExists_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IsSymbolExists_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_IsSymbolExists_Get 
	@vcSymbol varchar(8)
AS
/*
	Created by ...
	Description: this procedure is getting information from Contract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select contractID as iContractID
		from ContractView	
		where contractTypeID < 3 and symbol=@vcSymbol
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_LastCacheID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_LastCacheID_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



Create procedure usp_LastCacheID_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @Now integer
		exec usp_CurrentDate_Get @Now output
	select count(*) from ClearingHouseTrade where iDateOfTrade = @Now
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_LastTradeSeqNum_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_LastTradeSeqNum_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO



CREATE PROC usp_LastTradeSeqNum_Get
AS
	select max(seqNum) as iSeqNum
	from Trade 



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_LastTradeTime_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_LastTradeTime_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    procedure usp_LastTradeTime_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select convert(int,keyValue) as iLastTradeTime from DataInfo
		where keyName='lastTradeTime'
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_LastTradeTime_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_LastTradeTime_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



Create  procedure usp_LastTradeTime_Save
	@iLastTradeTime int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	update DataInfo set keyValue = @iLastTradeTime
		where keyName='lastTradeTime'
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MLState_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MLState_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



/********************************************************
	name: usp_MLState_Del
	Author: SP Generator V1.0.6
	Date: 26.03.2001
*********************************************************/
CREATE Procedure dbo.usp_MLState_Del
	@iMlStateID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	delete MLState Where mlStateID = @iMlStateID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MLState_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MLState_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



/********************************************************
	name: usp_MLState_Get
	Author: SP Generator V1.0.6
	Date: 26.03.2001
*********************************************************/
CREATE Procedure dbo.usp_MLState_Get
	@iMlStateID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		if (@iMlStateID is Null) 
		   select mlStateID as iMlStateID, mlType as iMlType, name as vcName, columns as vcColumns, groups as vcGroups, data as vcData
		from MLState
		 else
		   select mlStateID as iMlStateID, mlType as iMlType, name as vcName, columns as vcColumns, groups as vcGroups, data as vcData
		from MLState
		where mlStateID = @iMlStateID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MLState_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MLState_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



/********************************************************
	name: usp_MLState_Save
	Author: SP Generator V1.0.6
	Date: 26.03.2001
*********************************************************/
CREATE Procedure dbo.usp_MLState_Save
	@iMlStateID int out,
	@iMLType int,
	@vcName varchar,
	@vcColumns varchar,
	@vcGroups varchar,
	@vcData varchar
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iMlStateID is Null 
		begin
			insert into MLState (
					mlType,
					name,
					columns,
					groups,
					data) 
				values (
					@iMLType,
					@vcName,
					@vcColumns,
					@vcGroups,
					@vcData)
			select @iMlStateID = @@identity
		end
	else
		begin
			update MLState set
				mlType = isNull(@iMLType, mlType),
				name = isNull(@vcName, name),
				columns = isNull(@vcColumns, columns),
				groups = isNull(@vcGroups, groups),
				data = isNull(@vcData, data)
			where @iMlStateID = mlStateID
		end
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



/********************************************************
	name: usp_ModelParameter_Del
	Author: SP Generator V1.0.6
	Date: 25.01.2001
*********************************************************/
CREATE Procedure dbo.usp_ModelParameter_Del
	@iParameterID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	delete ModelParameter Where parameterID = @iParameterID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_ModelParameter_Get
		@iContractID int,
		@bDefaultOnly bit,
		@iParameterID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		if (@iParameterID is Null) 
		   if (@iContractID is Null)			
			  select parameterID as iParameterID, contractID as iContractID, modelTypeID as iModelTypeID, param1 as fParam1, param2 as fParam2, param3 as fParam3
				 from ModelParameter
				order by contractID, modelTypeID
			 else
			  if ((@bDefaultOnly is NOT Null) and (@bDefaultOnly = 1))		  
				  select mp.parameterID as iParameterID, mp.contractID as iContractID, mp.modelTypeID as iModelTypeID, mp.param1 as fParam1, mp.param2 as fParam2, mp.param3 as fParam3
					from StockView s, ModelParameter mp
				   where s.stockID = @iContractID
					 and mp.contractID = s.stockID
					 and mp.modelTypeID = s.defaultModelTypeID 
				  union 
				  select mp.parameterID as iParameterID, mp.contractID as iContractID, mp.modelTypeID as iModelTypeID, mp.param1 as fParam1, mp.param2 as fParam2, mp.param3 as fParam3
					from IndexView i, ModelParameter mp
				   where i.indexID = @iContractID
					 and mp.contractID = i.indexID
					 and mp.modelTypeID = i.defaultModelTypeID
				else
			  select parameterID as iParameterID, contractID as iContractID, modelTypeID as iModelTypeID, param1 as fParam1, param2 as fParam2, param3 as fParam3
					from ModelParameter
				   where contractID = @iContractID
				  order by modelTypeID
		 else
			  select parameterID as iParameterID, contractID as iContractID, modelTypeID as iModelTypeID, param1 as fParam1, param2 as fParam2, param3 as fParam3
		from ModelParameter
		where parameterID = @iParameterID
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



/********************************************************
	name: usp_ModelParameter_Save
	Author: SP Generator V1.0.6
	Date: 25.01.2001
*********************************************************/
CREATE Procedure dbo.usp_ModelParameter_Save
	@iParameterID int out,
	@iContractID int,
	@iModelTypeID int,
	@fParam1 float,
	@fParam2 float,
	@fParam3 float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iParameterID is Null 
		begin
			insert into ModelParameter (
					contractID,
					modelTypeID,
					param1,
					param2,
					param3) 
				values (
					@iContractID,
					@iModelTypeID,
					@fParam1,
					@fParam2,
					@fParam3)
			select @iParameterID = @@identity
		end
	else
		begin
			update ModelParameter set
				contractID = isNull(@iContractID, contractID),
				modelTypeID = isNull(@iModelTypeID, modelTypeID),
				param1 = isNull(@fParam1, param1),
				param2 = isNull(@fParam2, param2),
				param3 = isNull(@fParam3, param3)
			where @iParameterID = parameterID
		end
	set @error = @@error
if (@error = 0) set @error = @iParameterID
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_NextDaySurface_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_NextDaySurface_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_NextDaySurface_Del 
	@iNextDaySurfaceID int = null, 	-- the row identifier
	@iContractID int = null, 		-- the contract identifier
	@iVolasurfaceDataID int = null  -- the VolasurfaceData table identifier
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from NextDaySurface table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete NextDaySurface	
			where (nextDaySurfaceID = @iNextDaySurfaceID 
					and @iNextDaySurfaceID is not null and @iContractID is null and @iVolasurfaceDataID is null)
				or
				(@iNextDaySurfaceID is null and @iContractID is not null and @iVolasurfaceDataID is not null and ContractID=@iContractID and volasurfaceDataID=@iVolasurfaceDataID)
		
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_NextDaySurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_NextDaySurface_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_NextDaySurface_Get 
	@iNextDaySurfaceID int = Null 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from NextDaySurface table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select
		nextDaySurfaceID as iNextDaySurfaceID,
		Contract.contractID as iContractID,
		VolaSurfaceData.volaSurfaceDataID as iVolaSurfaceDataID,
		dueDate as dtDueDate,
		Contract.symbol as vcSymbolName,
		VolaSurfaceData.surfaceName as SurfaceName
		from NextDaySurface	
				join ContractView Contract on NextDaySurface.contractID=Contract.contractID
				join VolaSurfaceData on NextDaySurface.volaSurfaceDataID=VolaSurfaceData.volaSurfaceDataID
		where (@iNextDaySurfaceID is null or nextDaySurfaceID=@iNextDaySurfaceID)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_NextDaySurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_NextDaySurface_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_NextDaySurface_Save 
	@iNextDaySurfaceID int = Null output,	--the row identifier
	@iContractID int = Null, 				--the Contract identifier
	@iVolaSurfaceDataID int = Null,			--the surface data ID
	@dtDueDate datetime = Null				--the due date value
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into NextDaySurface table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if 	@iNextDaySurfaceID is null and @iVolaSurfaceDataID is null and @iContractID is null
		goto finish	
	if @dtDueDate is null
		set @dtDueDate=GetDate()+1
	Begin Transaction
		if not exists(select * From NextDaySurface where nextDaySurfaceID=@iNextDaySurfaceID and @iNextDaySurfaceID is not null) 
			and not exists (select * From NextDaySurface where contractID=@iContractID and volaSurfaceDataID=@iVolaSurfaceDataID and @iNextDaySurfaceID is null)
			begin
				insert into NextDaySurface (contractID,volaSurfaceDataID,dueDate) 
					values (@iContractID,@iVolaSurfaceDataID,@dtDueDate)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			
				set @iNextDaySurfaceID=@@identity
			end
		else
			begin
				update NextDaySurface 
					set 
						--contractID=isnull(@iContractID, contractID),
						--volaSurfaceDataID=isnull(@iVolaSurfaceDataID, volaSurfaceDataID),
						dueDate=isnull(@dtDueDate, dueDate)
				where (nextDaySurfaceID=@iNextDaySurfaceID and @iNextDaySurfaceID is not null) or (contractID=@iContractID and volaSurfaceDataID=@iVolaSurfaceDataID and @iVolaSurfaceDataID is not null and @iContractID is not null)
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE          Procedure dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete ContractPriceHistory where ContractID in (select OptionID from #Options_for_delete)
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractInGroup set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		if @bisRemovePermanent = 1
			begin
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete Contract where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
			end

	Commit Transaction DelOpt

finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO







CREATE        proc dbo.usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
/*
	Created by Zaur Nuraliev
	[Description]: this procedure return inf. from OptionView table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if isnull(@tiWithPosition, 0) = 0
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
			from ContractView C  
				inner join [Option] O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
			order by o.underlyingContractID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
			from ContractView c  
				inner join [Option] O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
	end
	else
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
			from ContractView C  
				inner join [Option] O on O.optionID = C.contractID
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0 and (tr.traderID = @iTraderID or @iTraderID is Null)
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,CP.lotSize
			order by o.underlyingContractID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
			from ContractView c  
				inner join [Option] O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null						
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0 and (tr.traderID = @iTraderID or @iTraderID is Null)
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,CP.lotSize
	end


	set @error = @@error
Return(@error)






GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Import]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE         Procedure dbo.usp_Option_Import
	@iUnderlyingContractID int = null,  --underlying contractID
	@vcSymbol varchar(8),  -- option symbol
	@fPriceAsk float, 
	@fPriceBid float, 
	@iVolume int,
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iOpenInterest int,
	@iExchangeID int = Null, -- the exchange identifier (special for ContractPrice update)
	@vcUnderlyingSymbol varchar(8) = null,  -- stock symbol
	@iLotSize int = null
as
/*
	Created by ... Redesigned by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendar where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendar where expiryID = @iExpiryID and expCalendarID = 1


	--===================================================================			
	-- check the consistensy of input parameters (for stock)
	--===================================================================			
	declare @iUnderlyingFirstContractID int
		set @iUnderlyingFirstContractID = @iUnderlyingContractID
	--select underlying ID
	select @iUnderlyingContractID = contractID  
		from Contract 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3
	if @iUnderlyingFirstContractID <> @iUnderlyingContractID
		begin
			Raiserror ('Wrong underlying contract ID in usp_Option_Import!', 16, 1)
			Return (-1)
		end  
	--select underlying option symbol
	if @vcUnderlyingSymbol is null
		select @vcUnderlyingSymbol=Contract.Symbol 
			from Contract (nolock) 	
			where Contract.contractID = @iUnderlyingContractID and Contract.contractTypeID < 3

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if (@OptionID is Null) 
		select @OptionID = optionID
			from [Option] (nolock)
			where underlyingContractID = @iUnderlyingContractID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- try to find Option with same ticker in DB
	--===================================================================			
	declare @iIncorrectOptionID int

	select @iIncorrectOptionID = contractID 
		from Contract (nolock)
				join [Option] (nolock) on Contract.contractID = [Option].optionID
		where symbol = @vcSymbol and contractTypeID = 3 and 
			[Option].actionID < 3 and (@OptionID <> contractID) -- or @OptionID is null

	if @iIncorrectOptionID is not null
		begin
			-- mark as delete
			update [Option] set [Option].actionID = 3 
				where optionID = @iIncorrectOptionID and expiryID < @iLastExpiryID

			if @@rowcount = 0 
				begin
					--Raiserror ('Option with this symbol already exists!', 16, 1)
					--Return (-1)
					update [Option] set [Option].actionID = 3 
						where optionID = @iIncorrectOptionID
					update Contract set actionID = 3 
						where ContractID = @iIncorrectOptionID
				end   
			else
				update Contract set actionID = 3 
					where ContractID = @iIncorrectOptionID
		end

	--===================================================================			
	-- make the contract name
	--===================================================================			
--  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
--					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
--						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)

  if exists (select contractID from Contract(nolock) where contractID = @iUnderlyingContractID and contractTypeID > 2)
	 begin
	   RAISERROR ('Error. Can''t insert values to [Option]. The Underlying Contract is not stock or index!', 16, 1)
	   RETURN (-1)
	 end 

  if (@OptionID is Null) 
	begin
		Begin Transaction
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into ContractPrice (contractID,exchangeID, priceBid, priceAsk, volume,openInterest,lotSize)
				 values (@OptionID,@iExchangeID, @fPriceBid, @fPriceAsk, @iVolume,@iOpenInterest,@iLotSize)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID,lotSize)
						values(@OptionID,null,@iLotSize)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
		Commit Transaction
	end 
   else
	begin
		  update Contract
			   set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 
		  update ContractPrice
				set	priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					volume = @iVolume,
					openInterest=@iOpenInterest,
					lotSize = isnull(@iLotSize,lotSize)
				where contractID = @OptionID and isnull(exchangeID,0)=isnull(@iExchangeID,0)
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract price!', 16, 1)
				  RETURN (-1)
			   end 

			update [Option]
				set 
				IsCall = isnull(@iIsCall,IsCall),
				strike = isnull(@fStrike,Strike),
				expiryID = isnull(@iExpiryID,expiryID)
			where optionID = @OptionID

		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
				  RETURN (-1)
			   end 

	end 
	 
-- return @OptionID 
    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





CREATE      proc dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(8) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@tiIsManualyEntered tinyint
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

   if exists (select contractID from ContractView where contractID = @iUnderlyingContractID and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 

	declare	@OptionID int, 
			@ContractName varchar(255),
			@iExpiryID int

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select @dtExpiry = expiryDate from ExpiryInCalendar where expiryID = @iExpiryID and expCalendarID = 1


   if exists (select optionID from OptionView where underlyingContractID = @iUnderlyingContractID and 
					isCall = @iIsCall and 
					strike = @fStrike and 
					expiryID = @iExpiryID and
					(@iOptionID is Null or optionID <> @iOptionID))
		begin
		  RAISERROR ('Error. Can''t insert or update Option values. There are ambiguous Option in DB!', 16, 1)
		  RETURN (-1)
		end  

   set @ContractName = @vcSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
						 ' ' + cast(round(@fStrike, 2) as varchar)
 
	Begin Transaction
	   if (@iOptionID is Null)	
		 begin
			insert into Contract (contractTypeID, symbol, contractName)
				values (3, @vcSymbol, @ContractName)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  

			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				values (@iOptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@ContractName,contractName)
				where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]
				set underlyingContractID = @iUnderlyingContractID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end		

		if @tiIsManualyEntered in (0,1) 
			begin
--select @iOptionID as '@iOptionID*******************'
				update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

	Commit Transaction

    set @error = 0
finish:
if (@error <> 0) return (@error) else return @iOptionID







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionDataCache_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionDataCache_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_OptionDataCache_Del
	@iOptionID int = Null,	-- option's identifier
	@iContractID int = Null	-- contract's identifier
as
/*
	Create by Andrey Sinkin
	It deletes data from OptionDataCache table
	--Changed by Zaur Nuraliev at 19 oct. 2001 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	if @iOptionID Is Not Null and @iContractID Is Null
		Begin
			Delete OptionDataCache where optionID = @iOptionID
			set @error = @@error
		End
	Else if @iOptionID Is Null and @iContractID Is Not Null
		Begin
			Delete OptionDataCache 
				Where optionId in 
					(Select optionId 
					From OptionView O join ContractView c  on C.contractID = O.underlyingContractID 
					Where C.contractID =@iContractID)
			set @error = @@error
		End
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionDataCache_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionDataCache_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_OptionDataCache_Get
	@iContractID int,			-- Contract's identifier
	@vcOptionTicker varchar(12),	-- option's ticker
	@bIsCall bit = 1		-- flag for call/put option status
as
/*
	Create by Andrey Sinkin
	It gets data from OptionDataCache table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iOptionID int
	if @iContractID is not null
		begin
			if @bIsCall is not null
				select Contract.symbol vcOptionTicker, date dtDate, IV fIV,priceMid fPriceMid, isCall bIsCall
					from OptionDataCache
					inner join OptionView on OptionDataCache.optionID = OptionView.optionID
					inner join ContractView Contract on OptionDataCache.optionID = Contract.contractID
					where underlyingContractID = @iContractID and isCall = @bIsCall
			else
				select Contract.symbol vcOptionTicker, date dtDate, IV fIV, priceMid fPriceMid, isCall bIsCall
					from OptionDataCache
					inner join OptionView on OptionDataCache.optionID = OptionView.optionID
					inner join ContractView Contract on OptionDataCache.optionID = Contract.contractID
					where underlyingContractID = @iContractID
		end
	else
		begin
			if @bIsCall is not null
				select Contract.symbol vcOptionTicker, date dtDate, IV fIV, priceMid fPriceMid, isCall bIsCall
					from OptionDataCache
					inner join OptionView on OptionDataCache.optionID = OptionView.optionID
					inner join ContractView Contract on OptionDataCache.optionID = Contract.contractID
					where symbol = @vcOptionTicker and contractTYpeID = 3 and isCall = @bIsCall
			else
				select Contract.symbol vcOptionTicker, date dtDate, IV fIV, priceMid fPriceMid, isCall bIsCall
					from OptionDataCache
					inner join OptionView on OptionDataCache.optionID = OptionView.optionID
					inner join ContractView Contract on OptionDataCache.optionID = Contract.contractID
					where symbol = @vcOptionTicker and contractTYpeID = 3	-- option
		end
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionDataCache_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionDataCache_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_OptionDataCache_Save
	@vcOptionTicker varchar(12),	-- option's ticker
	@dtDate datetime,				-- date
	@fIV float,						-- implied volatility
	@fPriceMid float				-- middle price
as
/*
	Create by Andrey Sinkin
	It deletes data from OptionDataCache table
	-- changed by Zaur Nuraliev 
	-- 19 oct. 2001
	-- removed  @fHVLow & @fHVHigh parametrs 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @rowcount int
	declare @iOptionID int		-- contract's identifier
	select @iOptionID = contractID from ContractView where symbol = @vcOptionTicker and contractTypeID = 3	-- option
		if @iOptionID is null 
			goto finish
	if @dtDate is null	set @dtDate = getdate()
	Begin Transaction
		update OptionDataCache set
			IV = isnull(@fIV, IV),
			priceMid = isnull(@fPriceMid, priceMid),
			date = @dtDate
		where optionID = @iOptionID
		select @error = @@error, @rowcount = @@rowcount
		if @error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		if @rowcount = 0
			begin
				insert into OptionDataCache (optionID, date, IV, priceMid) values (@iOptionID, @dtDate, @fIV, @fPriceMid)
				if @error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end			
			end
		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionPair_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionPair_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   procedure dbo.usp_OptionPair_Del
	@iOptionIDCall int,
	@iOptionIDPut int
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Option table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Create table #Options_for_delete(OptionID int)
	if  @iOptionIDCall is not null
		begin
			insert into #Options_for_delete values(@iOptionIDCall)
				if @@error > 0 begin set @error = 1 goto finish end
		end
	if  @iOptionIDPut is not null
		begin
			insert into #Options_for_delete values(@iOptionIDPut)
			if @@error > 0 begin set @error = 1 goto finish end
		end
	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Error. Can''t delete Option because of the existing Trade', 16, 1)
			Return (-1)
		end 
	if object_ID('tempdb..#Options_for_delete') is not null
		begin
			exec usp_Option_Del
			if @@error <> 0 set @error = 1 goto finish
		end
	set @error = @@error
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionPermanent_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionPermanent_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_OptionPermanent_Del
	@iOptionID int = null 	-- contract identifier
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete ContractPriceHistory where ContractID in (select OptionID from #Options_for_delete)
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete [Option] where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete Contract where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	Commit Transaction DelOpt

finish:
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionUnderlying_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



create proc usp_OptionUnderlying_Get
		@iOptionID int
  as
	 select C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk
	   from OptionView O
				join ContractView c  on C.contractID = O.underlyingContractID
				left join ContractPrice CP on C.contractID=CP.contractID
	   where O.optionID = @iOptionID
		 
	 Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionVM_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionVM_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_OptionVM_Get
	@iOptionID int = null,			-- option's identifier
	@iUnderlyingID int,				-- underlying's identifier
	@dtExpDateMin datetime = null,	-- expiration's date
	@bIsCall bit = null,
	@iSortOrder int = 1	--			-- 1, 2 or 3. see comments below
as
/*
	Created by Andrey Sinkin
	This procedure returns option's data
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iSortOrder = 1	-- sort by symbol
		select	O.OptionID as iOptionID, 
				O.underlyingContractID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as  dtExpDate,
				C.symbol as vcSymbolName, 
				null as iExchangeID,
				C.contractName as vcDescription,
				CP.lotSize as iLotSize
				from OptionView O
						join ContractView C on O.OptionID = C.contractID
						join ContractPrice CP on CP.contractID = C.contractID and exchangeID is null
						join Contract C1 on O.underlyingContractID = C1.contractID
						join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where O.underlyingContractID = @iUnderlyingID
						and (@iOptionID is null or O.OptionID = @iOptionID)
						and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
						and (@bIsCall is null or O.isCall = @bIsCall)
				order by C.symbol
	else
	if @iSortOrder = 2	-- sort by isCall+year(expiry)+month(expiry)+strike
		select	O.OptionID as iOptionID, 
				O.underlyingContractID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
				C.symbol as vcSymbolName, 
				null iExchangeID,
				C.contractName as vcDescription, 
				ContractPrice.lotSize as iLotSize
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where O.underlyingContractID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by isCall, year(EC.expiryDate),  month(EC.expiryDate), strike
	else
	if @iSortOrder = 3	-- sort by expiry+strike+symbol(without two last symbols)+isCall
		select 	O.OptionID iOptionID,
				O.underlyingContractID iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
				C.symbol vcSymbolName, 
				null iExchangeID,
				C.contractName vcDescription,	
				ContractPrice.lotSize iLotSize
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where O.underlyingContractID = @iUnderlyingID
					and (@iOptionID is null or EC.expiryDate = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by EC.expiryDate, strike, left(C.symbol, len(C.symbol) - 2), isCall desc
	set @error = @@error
Return(@error)







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionWithPrice_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE	proc dbo.usp_OptionWithPrice_Get
	  @iUnderlyingID int = Null,		 
	  @dtMinExpiry datetime = Null,
	  @iOptionID int = Null,
	  @bIsBestPriceOnly bit = 0
  as
   select	C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			O.underlyingContractID as iUnderlyingContractID,
			C2.contractTypeID as iUnderlyingContractTypeID,
			O.strike as fStrike, 
			EC.expiryDate as dtExpiry,
			O.isCall as bIsCall,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.lotSize as iLotSize, 
			CP.priceLast as fPriceLast,
			CP.priceClose as fPriceClose,
			CP.priceOpen as fPriceOpen,
			CP.priceLow as fPriceLow, 
			CP.priceHigh as fPriceHigh,
			CP.volume as iVolume,
			CP.openInterest as iOpenInterest, 
			E.exchangeID as iExchangeID,
			E.exchangeCode as vcExchangeCode,
			CP.priceTheo as fPriceTheo 
			  from ContractView c  
					join OptionView O on O.optionID = C.contractID
					join ContractView c2 on C2.contractID = O.underlyingContractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and c2.expCalendarID = EC.expCalendarID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where c.contractTypeID=3
					and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
					and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
					and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
					and (@bIsBestPriceOnly = 0 or @bIsBestPriceOnly = 1 and CP.exchangeID is null)
			order by o.underlyingContractID, year(EC.expiryDate), month(EC.expiryDate), o.strike,E.exchangeCode, o.isCall desc
 
  if (@@error <> 0)
	   return (-1)
	 else  
	   Return(0)








GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Position_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Position_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO



CREATE    PROCEDURE dbo.usp_Position_Get
	@iUnderlyingID int = Null,
	@iTraderID int = Null,
	@iBookID int = Null,
	@iStrategyID int = Null,
	@dtMaxDate datetime = null
AS

	declare @dtToday datetime
		set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
	
		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		null as dtExpiry,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		null as fPosDailyTheoPrevDay,
		null as fPriceTheoClose
	
	from TradeView tr 
		join ContractView c  on tr.contractID = c.contractID
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
	where c.contractTypeID in (1, 2)
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield
	
	union

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,

		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		EC.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		sum((case when tr.tradeDate < @dtToday then isnull(cp.priceTheo, 0) else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyTheoPrevDay,
		cp.priceTheo as fPriceTheoClose

	from TradeView tr 
		join ContractView C on tr.contractID = C.contractID
		join ContractPrice CP on C.contractID = cp.contractID and exchangeID is null
		left join (OptionView o join Contract UC on o.underlyingContractID = UC.contractID) on c.contractID = o.optionID
		join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
		left join StockView s on uc.contractID = s.stockID
		left join IndexView i on uc.contractID = i.indexID

	where c.contractTypeID = 3
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)

	group by C.contractID, C.contractTypeID, C.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield,
		EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, cp.priceTheo

	order by vcSymbol





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PriceCloseDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PriceCloseDH_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  Procedure dbo.usp_PriceCloseDH_Get
	@vcSymbol varchar(8),	--ticker of stock  
	@fPriceClose float = Null Output
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractPrice table for DeltaHedger
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select @fPriceClose = priceClose
		from ContractPrice	
			join ContractView Contract on Contract.contractID=ContractPrice.contractID
		where contractTypeID in (1,2) 
			and exchangeID is null
			and Contract.symbol=@vcSymbol
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PriceCloseDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PriceCloseDH_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_PriceCloseDH_Save 
	@vcSymbol varchar(8),	--ticker of stock(index)  
	@fPriceClose float
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractPrice table for DeltaHedger
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iSymbolID int
	select @iSymbolID=contractID from Contract where symbol=@vcSymbol and ContractTypeID < 3
	Begin Transaction
		if not exists(select * From ContractPrice where contractID=@iSymbolID and exchangeID is null) 
			begin
				insert into ContractPrice (contractID,exchangeID,priceClose) 
				values (@iSymbolID,null,@fPriceClose)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update ContractPrice set 
					priceClose = @fPriceClose
				where contractID=@iSymbolID and exchangeID is null
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportMarketStructure_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportMarketStructure_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE   Procedure dbo.usp_ReportMarketStructure_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare	@iLastExpiryID int
		select top 1 @iLastExpiryID = expiryID from ExpiryInCalendar where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc

	create table #FilteredStocks(stockID int)

	if @iStockID is null 
		begin
			insert into #FilteredStocks exec usp_FilterData_Get @iFilterID
		end

if @iStockID is not null and @iFilterID is null
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	O.expiryID > @iLastExpiryID and 
				underlyingContractID=@iStockID

else if @iStockID is not null and @iFilterID is not null
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	O.expiryID > @iLastExpiryID and
				underlyingContractID = @iStockID and 
				O.underlyingContractID in (select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is not null
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where O.expiryID > @iLastExpiryID and
			underlyingContractID in (Select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is null
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where O.expiryID > @iLastExpiryID
/*
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where O.expiryID > @iLastExpiryID and
	(
		(@iStockID is not null and @iFilterID is null and underlyingContractID=@iStockID) or 
		(@iStockID is null and @iFilterID is not null and underlyingContractID in (Select stockID from #FilteredStocks)) or 
		(@iStockID is not null  and @iFilterID is not null and (underlyingContractID=@iStockID and O.underlyingContractID in (Select stockID from #FilteredStocks))) or 
		(@iStockID is null and @iFilterID is null)
	)

*/
	if object_id('tempdb..#FilteredStocks') is not null
		drop table #FilteredStocks

	set @error = @@error
Return(@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportStock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportStock_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE        Procedure dbo.usp_ReportStock_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	--=====================================================
	-- work with filter
	--=====================================================
	create table #FilteredStocks(stockID int)
              insert into #FilteredStocks 
                      exec usp_FilterData_Get @iFilterID
	--=====================================================
	-- data selection
	--=====================================================
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			case when isDivCustom = 1 then divFreqCustom else Stock.divfreq end iDivFreq,
			case when isDivCustom = 1 then divAmtCustom else Stock.divAmt end fDivAmt,
			case when isDivCustom = 1 then divDateCustom else Stock.DivDate end dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		Stock.isHTB as bIsHTB,
			Stock.isDivCustom as tiIsDivCustom,
			Stock.CalcOptionType as tiCalcOptionType,
			Stock.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID,
			null as tiIsBasket
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join StockView Stock on Contract.contractID = Stock.StockID
					left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory join
									(select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
								on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
						on S2.stockID=Stock.stockID
	
			where (
				(@iStockID is null and (Stock.stockID in (select stockID from #FilteredStocks)))
				or 
				(@iStockID is not null and Contract.ContractID=@iStockID 
					and (Stock.stockID in (select stockID from #FilteredStocks) or @iFilterID is null))
				)
				and Contract.contractTypeID = 2
	union
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			0 as iDivFreq,
			I.yield as fDivAmt,
	  		null as dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		I.isHTB as bIsHTB,
			null as tiIsDivCustom,
			I.CalcOptionType as tiCalcOptionType,
			I.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID,
			I.IsBasket as tiIsBasket
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join IndexView I on Contract.contractID = I.indexID
					left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
									(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
								on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
						on S2.indexID=I.indexID
			where (
				(@iStockID is null and (I.indexID in (select stockID from #FilteredStocks))) or 
				(@iStockID is not null and @iFilterID is not null and (Contract.ContractID=@iStockID and (I.indexID in (select stockID from #FilteredStocks)))) or 
				(@iStockID is not null and I.indexID = @iStockID and @iFilterID is null)
				)
				and Contract.contractTypeID = 1
		order by Contract.symbol


	set @error = @@error
             drop table #FilteredStocks
Return(@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportStockTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportStockTrade_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE                       Procedure dbo.usp_ReportStockTrade_Get
	@iStockID int = Null, 
	@iFilterID int = Null,
	@dtBeginDate datetime = Null,
	@dtEndDate datetime = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from OptionView table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	--=======================================================
	--prepare operations for work with filter
	--=======================================================
	create table #FilteredStocks(stockID int)
	-- for filtering by trades
	create table #T(tradeID int)
	if @iFilterID is not null
		begin
			declare @iTraderID int,
					@iBookID int,
					@iStrategyID int,
					@bTradeRowCount bit
			set @bTradeRowCount=0
			insert into #FilteredStocks
				exec usp_FilterData_Get @iFilterID, @iTraderID output, @iBookID output, @iStrategyID output
			if (@iTraderID<>0 or @iBookID<>0 or @iStrategyID<>0)
				begin
					set @bTradeRowCount = 1
					insert into #T
						exec usp_FilterTradeData_Get @iTraderID,@iBookID,@iStrategyID
				end
		end
	--cut the time part of date
	select	@dtBeginDate = convert(datetime,convert(varchar(10),@dtBeginDate,112)),
			@dtEndDate = convert(datetime,convert(varchar(10),@dtEndDate,112))
	--=====================================================
	-- data selection
	--=====================================================
	select 
		Contract.contractID as iContractID,
		T.lotSize * (case T.isBuy when 0 then T.quantity * -1 else T.quantity end) as iQuantity,
		T.tradeDate as dtTradeDate,
		T.price as fPriceTrade,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		T.traderID as iTraderID,
		T.bookID as iBookID,
		T.strategyID as iStrategyID,
		CP.priceClose as fPriceClose
		from ContractView Contract
				join TradeView T on Contract.ContractID = T.contractID
				join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
		where 
			(
			(@iStockID is not null and @iFilterID is null and Contract.ContractID=@iStockID)
				or 
			(@iStockID is null and @iFilterID is null) 
				or
			(@iStockID is null and @iFilterID is not null and (Contract.ContractID in (select stockID from #FilteredStocks)) and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0))  
				or 
			(@iStockID is not null and @iFilterID is not null 
				and Contract.ContractID=@iStockID 
				and (Contract.ContractID in (select stockID from #FilteredStocks)) 
				and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0)) 
			)
			and 
			(
			(@dtBeginDate is not null and @dtEndDate is not null and (convert(datetime,convert(varchar(10),T.tradeDate,112)) between @dtBeginDate and @dtEndDate) )	or 
			(@dtBeginDate is not null and @dtEndDate is null and convert(datetime,convert(varchar(10),T.tradeDate,112)) >= @dtBeginDate) or 
			(@dtBeginDate is null and @dtEndDate is not null and convert(datetime,convert(varchar(10),T.tradeDate,112)) <= @dtEndDate) or
			(@dtBeginDate is null and @dtEndDate is null)
			)
			and Contract.ContractTypeID < 3
	set @error = @@error
	if @iFilterID is not null
		begin
			drop table #FilteredStocks
		end
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportTrade_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE         Procedure dbo.usp_ReportTrade_Get
	@iStockID int = Null, 
	@iFilterID int = Null,
	@dtBeginDate datetime = Null,
	@dtEndDate datetime = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from OptionView table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	create table #FilteredStocks(stockID int)
	
	-- for filtering by trades
	create table #T(tradeID int)
	if @iFilterID is not null
		begin
			declare @iTraderID int,
					@iBookID int,
					@iStrategyID int,
					@bTradeRowCount bit
			set @bTradeRowCount=0
			insert into #FilteredStocks
				exec usp_FilterData_Get @iFilterID, @iTraderID output, @iBookID output, @iStrategyID output
			if (@iTraderID<>0 or @iBookID<>0 or @iStrategyID<>0)
				begin
					set @bTradeRowCount = 1
					insert into #T
						exec usp_FilterTradeData_Get @iTraderID,@iBookID,@iStrategyID
				end
		end
--	select * from #T	
	select	@dtBeginDate = convert(datetime,convert(varchar(10),@dtBeginDate,112)),
			@dtEndDate = convert(datetime,convert(varchar(10),@dtEndDate,112))
	select 
		O.underlyingContractID as iUnderlyingContractID,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		case T.isBuy when 0 then T.quantity * -1 else T.quantity end as iQuantity,
		T.tradeDate as dtTradeDate,
		T.price as fPriceTrade,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceLast as fPriceLast,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceTheo as fPriceTheo,
		T.traderID as iTraderID,
		T.bookID as iBookID,
		T.strategyID as iStrategyID,
		CP.priceClose as fPriceClose,
		Contract.symbol as vcSymbol,
		T.lotSize as iLotSize,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join TradeView T on O.optionID = T.contractID 
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
		where EC.expiryDate > GetDate() and
			((
				(@iStockID is not null and @iFilterID is null and underlyingContractID=@iStockID) or 
				(@iStockID is null and @iFilterID is null) or
				(@iStockID is null and @iFilterID is not null and (underlyingContractID in (select stockID from #FilteredStocks)) and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0)  or 
				(@iStockID is not null and @iFilterID is not null 
					and underlyingContractID=@iStockID 
					and (underlyingContractID in (select stockID from #FilteredStocks)) 
					and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0))
			))
				and
				(
				(@dtBeginDate is  not null and @dtEndDate is not null and (convert(datetime,convert(varchar(10),T.tradeDate,112)) between @dtBeginDate and @dtEndDate)) or 
				(@dtBeginDate is not null and @dtEndDate is null and convert(datetime,convert(varchar(10),T.tradeDate,112)) >= @dtBeginDate) or
				(@dtBeginDate is null and @dtEndDate is not null and convert(datetime,convert(varchar(10),T.tradeDate,112)) <= @dtEndDate) or
				(@dtBeginDate is null and @dtEndDate is null)
				)
			)
	set @error = @@error
	if @iFilterID is not null
		begin
			drop table #FilteredStocks
		end
Return(@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenario_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenario_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE   Procedure dbo.usp_RMScenario_Del
	@iRMScenarioID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	update RMScenario set actionID=3 where RMScenarioID = @iRMScenarioID
	--delete RMScenario Where RMScenarioID = @iRMScenarioID

	set @error = @@error
        Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenario_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenario_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE     Procedure dbo.usp_RMScenario_Get
	@iRMScenarioID int = null,
	@dtActionDate datetime = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
    select rs.RMScenarioID as iRMScenarioID, 
			rs.ScenarioName as vcScenarioName, 
			rs.[Description] as vcDescription,
            points1 as iPoints1,
			step1 as fStep1,
			isAbs1 as tiIsAbs1,
			type1 as tiType1,
			points2 as iPoints2,
			step2 as fStep2,
			isAbs2 as tiIsAbs2,
			type2 as tiType2,
			actionDate as dtActionDate,
			actionID as tiActionID
		from RMScenario rs
		where ((@iRMScenarioID is Null) 
			or (@iRMScenarioID is Not Null and rs.RMScenarioID = @iRMScenarioID))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
set @error = @@error
    Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenario_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenario_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE      Procedure dbo.usp_RMScenario_Save
	@iRMScenarioID int = Null output,
	@vcScenarioName varchar(30) = Null,
	@vcDescription varchar(120) = Null,
    @iPoints1 int = Null,
	@fStep1 int = Null,
	@tiIsAbs1 tinyint = Null,
	@tiType1 tinyint = Null,
	@iPoints2 int = Null,
	@fStep2 float= Null,
	@tiIsAbs2 tinyint = Null,
	@tiType2 tinyint = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
/*
	if exists (select * from RMScenario where scenarioName = @vcScenarioName and actionID < 3 and (@iRMScenarioID <> RMScenarioID or @iRMScenarioID is null))
		begin
			Raiserror ('Row in RMScenario table with same scenario name already exists!', 16, 1)
			Return (-1)
		end
*/   
	if @iRMScenarioID is null
	--if not exists(select * from RMScenario where RMScenarioID = @iRMScenarioID)
		begin
			insert into RMScenario(
					ScenarioName,
					[Description],
					points1,
					step1,
					isAbs1,
					type1,
					points2,
					step2,
					isAbs2,
					type2) 
				values (
					@vcScenarioName,
					@vcDescription,
				    @iPoints1,
					@fStep1,
					@tiIsAbs1,
					@tiType1,
					@iPoints2,
					@fStep2,
					@tiIsAbs2,
					@tiType2)
			select @iRMScenarioID = @@identity
		end
	else
		begin
			update RMScenario set
				ScenarioName = isNull(@vcScenarioName, ScenarioName),
				[Description] = isNull(@vcDescription, [Description]),
				points1 = isNull(@iPoints1, points1),
				step1 = isNull(@fStep1, step1),
				isAbs1 = isNull(@tiIsAbs1, isAbs1),
				type1 = isNull(@tiType1, type1),
				points2 = isNull(@iPoints2, points2),
				step2 = isNull(@fStep2, step2),
				isAbs2 = isNull(@tiIsAbs2, isAbs2),
				type2 = isNull(@tiType2, type2)
				where RMScenarioID = @iRMScenarioID
		end
	set @error = @@error
Return(@error)






GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RuleItemType_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RuleItemType_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_RuleItemType_Del 
	@iRuleItemTypeID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from RuleItemType table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete RuleItemType	where ruleItemTypeID = @iRuleItemTypeID 
		
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RuleItemType_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RuleItemType_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_RuleItemType_Get 
	@iRuleItemTypeID int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from RuleItemType table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select ruleItemTypeID as iRuleItemTypeID,
			itemTypeName as vcItemTypeName
		from RuleItemType	
		where (ruleItemTypeID=@iRuleItemTypeID or @iRuleItemTypeID is null)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RuleItemType_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RuleItemType_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_RuleItemType_Save 
	@iRuleItemTypeID int = Null, --the row ID
	@vcItemTypeName varchar(255)
AS
/*
	Created by ...
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		if not exists(select * From RuleItemType where ruleItemTypeID=@iRuleItemTypeID) 
			begin
				insert into RuleItemType (itemTypeName) values (@vcItemTypeName)
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iRuleItemTypeID=@@identity
			end
		else
			begin
				update RuleItemType set 
					itemTypeName = isnull(@vcItemTypeName, itemTypeName)
				where ruleItemTypeID = @iRuleItemTypeID
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE        proc dbo.usp_Stock_Del
        @iStockID int,
        @bisRemovePermanent bit = 0
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Stock table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	if exists (select * from TradeView(nolock) where contractID = @iStockID)
	begin
		Raiserror ('Error. Can''t delete Stock because of the existing Trade', 16, 1)
		Return (-1)
	end 
	if exists(select * from [Option](nolock) where underlyingContractID=@iStockID)
		begin
			Create table #Options_for_delete(OptionID int)
		
			insert into #Options_for_delete
				select optionID from [Option](nolock) where underlyingContractID=@iStockID
		
			if exists (select tradeID from TradeView where contractID in (select optionID from #Options_for_delete))
				begin
					Raiserror ('Error. Can''t delete Stock because of the existing Trade by Stock Options', 16, 1)
					Return (-1)
				end 
		end
	Begin Transaction
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = @bisRemovePermanent
			end

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID = @iStockID
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	    delete ModelParameter where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
        --delete ContractInGroup where contractID = @iStockID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        --delete ContractPriceHistory where contractID = @iStockID
		--	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete ContractPrice where contractID = @iStockID

			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		if (@bisRemovePermanent = 1) 
			begin
	        	delete IndexDefinition where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				delete TraderContract where contractID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		       	delete Stock where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	        	delete Contract where contractID = @iStockID
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
  
		set @error = @@error
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO






CREATE       proc dbo.usp_Stock_Get
	@iGroupID int = null,
	@iStockID int = null,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Stock,Contract tables
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if isnull(@tiWithPosition, 0) = 0
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
				from ContractView C  
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
				from ContractView C  
					inner join Stock S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join Stock S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				order by c.Symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
			    from ContractView c  
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			    where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
					S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 

					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
			    from ContractView c  
					inner join Stock S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			    where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
					S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
					S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join Stock S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
					S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID
				order by c.Symbol
		end
	end
	
	set @error = @@error
Return(@error)







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Import]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE proc dbo.usp_Stock_Import
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
		    set ContractName = @vcContractName,
				MSupdateDate = getDate()
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
		where contractID = @iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
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
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate!', 16, 1)
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE proc dbo.usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
        @iDefaultModelTypeID int = Null,
        @iDefaultSurfaceTypeID int = Null,
        @iBaseIndexID int = Null,
        @vcSymbol varchar(8) = Null,
        @vcContractName varchar(255) = Null,
		@bIsHTB  bit = Null,
        @iDivFreq int = Null,
		@fDivAmt float = Null,
		@dtDivDate smalldatetime = Null,
		@tiIsDivCustom tinyint = 0,
		@fDivAmtCustom float = Null,
		@iDivFreqCustom int = Null,
		@dtDivDateCustom smalldatetime = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@tiExpCalendarID tinyint = null
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=2 and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			Raiserror ('Stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStockID is Null)  
     begin
         begin tran
            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID)
                  values (2, @vcSymbol, @vcContractName, @tiExpCalendarID)
			if (@@error <> 0) begin rollback tran return(-1) end        

            set @iStockID = @@identity         
 
            insert into Stock (
				stockID, 
				defaultModelTypeID, 
				defaultSurfaceTypeID,
                divFreq, 
				baseIndexID, 
				isHTB,
				primaryExchangeID,
				divAmt,
				divDate,
				isDivCustom,
				divAmtCustom,
				divFreqCustom,
				divDateCustom,
				calcOptionType,
				calcModelType)
            values (
				@iStockID, 
				@idefaultModelTypeID, 
				@idefaultSurfaceTypeID,
                @iDivFreq, 
				@ibaseIndexID,
				@bIsHTB,
				@iExchangeID,
				@fDivAmt,
				@dtDivDate,
				@tiIsDivCustom,
				@fDivAmtCustom,
				@iDivFreqCustom,
				@dtDivDateCustom,
				@tiCalcOptionType,
				@tiCalcModelType)
			if (@@error <> 0) begin rollback tran return(-1) end           
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iStockID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iStockID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
         commit tran
     end
   else
     begin

        declare @df int

        select @df = DivFreq from Stock where stockID = @iStockID        
        begin tran
           update Contract
                set symbol = isNull(@vcSymbol,symbol),
                    contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@tiExpCalendarID,expCalendarID)
	            where contractID = @iStockID
			if (@@error <> 0) begin rollback tran return(-1) end         

			update Stock
	               set  defaultModelTypeID = isNull(@idefaultModelTypeID,defaultModelTypeID), 
	                    defaultSurfaceTypeID = isNull(@idefaultSurfaceTypeID,defaultSurfaceTypeID),
	                    divFreq = isNull(@iDivFreq,divFreq), 
	                    baseIndexID = isNull(@ibaseIndexID,baseIndexID),
						isHTB = isNull(@bIsHTB, isHTB),
						primaryExchangeID = isNull(@iExchangeID,primaryExchangeID),
						DivAmt = isNull(@fDivAmt, DivAmt),
						DivDate = isNull(@dtDivDate, DivDate),
						isDivCustom = isNull(@tiIsDivCustom, isDivCustom),
						divAmtCustom = isNull(@fDivAmtCustom, divAmtCustom),
						divFreqCustom = isNull(@iDivFreqCustom, divFreqCustom),
						divDateCustom = isNull(@dtDivDateCustom, divDateCustom),
						calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
						calcModelType = isNull(@tiCalcModelType, calcModelType)
		          where stockID = @iStockID
    	       if (@@error <> 0) begin rollback tran return(-1) end         

			if @iDivFreq = 0 
				begin
					update Stock set divAmt = null, divDate = null where stockID = @iStockID
				end

			if @iDivFreqCustom = 0 
				begin
					update Stock set divAmtCustom = null, divDateCustom = null where stockID = @iStockID
				end

         commit tran
     end        

 if (@@error <> 0)
    return (-1)
  else  
    return @iStockID








GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockCorrelationDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockCorrelationDH_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_StockCorrelationDH_Get 
	@vcSymbol1 varchar(8), 
	@vcSymbol2 varchar(8),
	@fCorrelationPrice float = Null Output,
	@fCorrelationVola  float = Null Output
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from StockCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iStockID1 int, 
			@iStockID2 int,
			@iStockID3 int
	select @iStockID1=contractID from ContractView where symbol=@vcSymbol1 and contractTypeID=2
	select @iStockID2=contractID from ContractView where symbol=@vcSymbol2 and contractTypeID=2
	if 	@iStockID1 > @iStockID2
		select @iStockID3=@iStockID1,@iStockID1=@iStockID2,@iStockID2=@iStockID3
	select	@fCorrelationPrice=correlationPrice,
			@fCorrelationVola=correlationVola
		from StockCorrelation	
		where stockID1 = @iStockID1
			and stockID2 = @iStockID2
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockCorrelationDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockCorrelationDH_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_StockCorrelationDH_Save
	@vcSymbol1 varchar(8), 
	@vcSymbol2 varchar(8),
	@fCorrelationPrice float = Null,
	@fCorrelationVola  float = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from StockCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iStockID1 int, 
			@iStockID2 int,
			@iStockID3 int
	select @iStockID1=contractID from Contract where symbol=@vcSymbol1 and contractTypeID=2
	select @iStockID2=contractID from Contract where symbol=@vcSymbol2 and contractTypeID=2
	if @iStockID1>@iStockID2
		select @iStockID3=@iStockID1,@iStockID1=@iStockID2,@iStockID2=@iStockID3
	Begin Transaction
		if not exists(select * from StockCorrelation where StockID1=@iStockID1 and StockID2=@iStockID2) 
			begin
				insert into StockCorrelation(stockID1,stockID2,correlationPrice,correlationVola)
					values(@iStockID1,@iStockID2,@fCorrelationPrice,@fCorrelationVola)
			end 
		else
			begin
				update StockCorrelation
					set stockID1=@iStockID1,
						stockID2=@iStockID2,
						correlationPrice=isnull(@fCorrelationPrice,CorrelationPrice),
						correlationVola=isnull(@fCorrelationVola,CorrelationVola)
				where StockID1=@iStockID1 and StockID2=@iStockID2	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockDataHistory_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockDataHistory_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_StockDataHistory_Save
	@iStockID int = null,
	@dtDate datetime = null,
	@iTenorID int = null,
	@fIVMean float = null,
	@vcStockTicker varchar(12) = null,
	@iTenorValue int = null,
	@fIVCall float = null,
	@fIVPut float = null,
	@fCurrentHV float = null,
	@fCurrentVolOfVol float = null
as
/*
	Created by Andrey Sinkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	-- if wrong parameters -> exit
	if @vcStockTicker is null and @iStockID is null or @iTenorID is null and @iTenorValue is null	goto finish
	if @iStockID is null	
		select @iStockID = contractID from Contract where symbol = @vcStockTicker and contractTypeID = 2
	if @iTenorID is null	
		select @iTenorID = tenorID from Tenor where value = @iTenorValue
	if @dtDate is null	
		set @dtDate = GetDate()
	Begin Transaction
		if not exists(select * from StockDataHistory where StockID = @iStockID and date = @dtDate and tenorID = @iTenorID)
			begin
				insert into StockDataHistory (StockID, [date], tenorID, IVMean, IVCall, IVPut, currentHV,currentVolOfVol) values (@iStockID, @dtDate, @iTenorID, @fIVMean, @fIVCall, @fIVPut, @fCurrentHV,@fCurrentVolOfVol)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update StockDataHistory set 
					IVMean = isnull(@fIVMean, IVMean),
					IVCall = isnull(@fIVCall, IVCall),
					IVPut = isnull(@fIVPut, IVPut),
					currentHV = isnull(@fCurrentHV, currentHV),
					currentVolOfVol = isnull(@fCurrentVolOfVol,currentVolOfVol)
				where StockID = @iStockID and [date] = @dtDate and tenorID = @iTenorID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockDataHistoryDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockDataHistoryDH_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_StockDataHistoryDH_Get
	@vcSymbol varchar(8),	--ticker of stock 
	@fCurrentIV float = null Output, 
	@fCurrentHV float = null Output, 
	@fCurrentVolOfVol float = null Output 
as
/*
	Created by Zaur Nuraliev
	this procedure returns historical data for stock by ticker
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iStockID int 
	select @iStockID=contractID from ContractView where symbol=@vcSymbol and contractTypeID = 2
	select 	@fCurrentIV = IVMean, 
			@fCurrentHV = currentHV,
			@fCurrentVolOfVol = currentVolOfVol
		from StockDataHistory
		where tenorID=8 
			and [date]=(select max([date]) from StockDataHistory where stockID=@iStockID and tenorID=8)
			and stockID=@iStockID
	set @error = @@error
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockID_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE procedure usp_StockID_Get
	   @vcSymbol varchar(8) 
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting stock identifiers
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select C.contractID as iStockID
		from ContractView c 	
				join StockView S on S.stockID = C.contractID
		where C.contractTypeID = 2 and C.symbol = @vcSymbol
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockReportDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockReportDH_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE     Procedure usp_StockReportDH_Get
	@vcSymbolList varchar(8000) = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from <table_name, sysname,table_name> table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Declare @iSPX_ID int,
			@iNDX_ID int,
			@iOEX_ID int,
			@iDJX_ID int
	select @iSPX_ID=ContractID from Contract where symbol='SPX' and contractTypeID=1
	select @iNDX_ID=ContractID from Contract where symbol='NDX' and contractTypeID=1
	select @iOEX_ID=ContractID from Contract where symbol='OEX' and contractTypeID=1
	select @iDJX_ID=ContractID from Contract where symbol='DJX' and contractTypeID=1
	declare @SDH_Dt datetime
		select @SDH_Dt=max([date]) from StockDataHistory where tenorID=8
	-- check if data not exists in StockDataHistory
	select  @SDH_Dt= isnull(@SDH_Dt, 0)
	select @iSPX_ID = isnull(@iSPX_ID, 0)
	select @iNDX_ID = isnull(@iNDX_ID, 0)
	select @iOEX_ID = isnull(@iOEX_ID, 0)
	select @iDJX_ID = isnull(@iDJX_ID, 0)
if @vcSymbolList is null 
begin
	select
		Contract.Symbol,
		CP.priceClose,
		SDH.currentHV as HV,
		SDH.IVMean as IV,
		sum(case when IndexD.indexID=@iSPX_ID then correlationPrice else null end) as CorrSPX,
		sum(case when IndexD.indexID=@iSPX_ID then correlationVola else null end) as VCorrSPX,
		sum(case when IndexD.indexID=@iNDX_ID then correlationPrice else null end) as CorrNDX,
		sum(case when IndexD.indexID=@iNDX_ID then correlationVola else null end) as VCorrNDX,
		sum(case when IndexD.indexID=@iOEX_ID then correlationPrice else null end) as CorrOEX,
		sum(case when IndexD.indexID=@iOEX_ID then correlationVola else null end) as VCorrOEX,
		sum(case when IndexD.indexID=@iDJX_ID then correlationPrice else null end) as CorrDJX,
		sum(case when IndexD.indexID=@iDJX_ID then correlationVola else null end) as VCorrDJX
		from ContractView Contract
			left join StockDataHistory SDH on Contract.contractID=SDH.stockID and SDH.[date]=@SDH_Dt and SDH.tenorID=8
			left join IndexDefinition IndexD on Contract.contractID=IndexD.stockID
			left join ContractPrice CP on contract.contractID=CP.contractID and CP.exchangeID is null 
		where contract.contractTypeID=2
		group by Contract.Symbol,CP.priceClose,SDH.currentHV,SDH.IVMean
end 
else 
begin
	declare @ExecString varchar(8000)
	select @ExecString='select
				Contract.Symbol,
				CP.priceClose,
				SDH.currentHV as HV,
				SDH.IVMean as IV,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iSPX_ID) + ' then correlationPrice else null end) as CorrSPX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iSPX_ID) + ' then correlationVola else null end) as VCorrSPX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iNDX_ID) + ' then correlationPrice else null end) as CorrNDX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iNDX_ID) + ' then correlationVola else null end) as VCorrNDX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iOEX_ID) + ' then correlationPrice else null end) as CorrOEX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iOEX_ID) + ' then correlationVola else null end) as VCorrOEX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iDJX_ID) + ' then correlationPrice else null end) as CorrDJX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iDJX_ID) + ' then correlationVola else null end) as VCorrDJX '
				+'from ContractView Contract
							left join StockDataHistory SDH on Contract.contractID=SDH.stockID and convert(varchar(15),SDH.[date],112)=' + convert(varchar(15),@SDH_Dt,112) + ' and SDH.tenorID=8 
							left join IndexDefinition IndexD on Contract.contractID=IndexD.stockID 
							left join ContractPrice CP on contract.contractID=CP.contractID and CP.exchangeID is null  
							where Symbol in ('	+ @vcSymbolList + ') and contract.contractTypeID=2' 
				+ ' group by Contract.Symbol,CP.priceClose,SDH.currentHV,SDH.IVMean'
	exec (@ExecString)
end
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockVM_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE      Procedure dbo.usp_StockVM_Save
	@iStockID int = null output,		-- stock's identifier
	@iContractTypeID int = null,		-- contract's type identifier (not used)
	@vcSymbolName varchar(12) = null,	-- stock's ticker
	@iExchangeID int = null,		-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,			-- lot size (not used)
	@iDivFrequency int = null,		-- dividend's frequency
	@dtDate datetime = null,		-- date
	@fIVMean float = null,			-- iv mean
	@fCurrentHV float = null,		-- current hv
	@fCurrentIV float = null,		-- current iv
	@fAmount float = null, 			-- amount
	@dDivDate datetime = null,		-- dividend's date
	@iTenorID int = null,			-- tenor's identifier
	@iTenorValue int = null,		-- tenor's value
	@fIVCall float = null,			-- iv call
	@fIVPut float = null,			-- iv put
	@fPriceClose float = null,		-- stock's close price
	@iGroupID int = null,
	@iRuleID int = null
as
/*
	Created by Andrey Sinkin
	Can to save data to Contract, Stock, StockDataHistory, StockDatasnapshot tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iDividendID int, @iMarketDataID int
	set @iContractTypeID = 2	-- stock
	set @iLotSize = 0
	-- if wrong parameters -> exit
	if @vcSymbolName is null and @iStockID is null	goto finish
	if @iTenorID is null and @iTenorValue is not null
		select @iTenorID = tenorID from Tenor where value = @iTenorValue
	if @iStockID is null and @vcSymbolName is not null
		select @iStockID = contractID from ContractView where symbol = @vcSymbolName and contractTypeID = 2	-- stock
	Begin Transaction
		if @iStockID is null
			begin
				insert into Contract (contractTypeID, symbol,contractName,groupID,ruleID) 
					values (@iContractTypeID, @vcSymbolName,@vcDescription, @iGroupID,@iRuleID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iStockID = @@identity
				insert into Stock (StockID, divFreq)
					values (@iStockID, @iDivFrequency)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				if @iContractTypeID is not null or @vcSymbolName is not null or @iExchangeID is not null or @vcDescription is not null or @iLotSize is not null
					begin
						update Contract set 
							contractTypeID = isnull(@iContractTypeID, contractTypeID),
							symbol = isnull(@vcSymbolName, symbol),
							contractName = isnull(@vcDescription, contractName),
							groupID = isnull(@iGroupID, groupID),
							ruleID = isnull(@iRuleID, ruleID)
						where contractID = @iStockID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
					end
			end
		--============================================================================================================
		if exists(select * from ContractPrice where contractID = @iStockID and (exchangeID = @iExchangeID or @iExchangeID is null))
			update ContractPrice 
				set exchangeID = @iExchangeID,
					lotSize = isnull(@iLotSize, lotSize),
					contractID = isnull(@iStockID, contractID)
				where contractID = @iStockID and (exchangeID = @iExchangeID or @iExchangeID is null)
		else
			insert into ContractPrice
				(exchangeID,contractID,lotSize)
			values (@iExchangeID,@iLotSize,@iStockID)
		--============================================================================================================
		if @dtDate is not null and @fIVMean is not null and @iTenorID is not null
			begin
				exec @error = usp_StockDataHistory_Save @iStockID = @iStockID, @dtDate = @dtDate, @fIVMean = @fIVMean, @iTenorID = @iTenorID, @fIVCall = @fIVCall, @fIVPut = @fIVPut, @fCurrentHV = @fCurrentHV
				if @error <> 0 begin set @error = 5 if @@TranCount = 1 Rollback Transaction goto finish end
			end

		if @fAmount is not null or @dDivDate is not null or @iDivFrequency is not null
			begin
				update Stock set 
					divFreq = isnull(@iDivFrequency, divFreq),
					divAmt = isnull(@fAmount, divAmt),
					divDate = isnull(@dDivDate, divDate)
					where StockID = @iStockID

				if @error <> 0 begin set @error = 7 if @@TranCount = 1 Rollback Transaction goto finish end
			end

		if (@fCurrentHV is not null or @fCurrentIV is not null) and @iTenorID is not null
			begin
				if @@TranCount = 1 Rollback Transaction
				Raiserror ('Error. Can''t save CurrentIV for Stock (usp_StockDataSnapshot_Save stored procedure), please send this error message to DB developer !', 16, 1)
			end

		if @fPriceClose is not null
			begin
				if @@TranCount = 1 Rollback Transaction
				Raiserror ('Error. Can''t save close price for Stock (usp_StockVM_Save stored procedure), please send this error message to DB developer !', 16, 1)
			end

		set @error = 0
	Commit Transaction
finish:
Return(@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockWithPrice_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE    proc usp_StockWithPrice_Get
	@iGroupID int = Null,
	@iStockID int = Null,
	@bIsBestPriceOnly bit = 0
  as
	if (@iGroupID is Null)
		if @bIsBestPriceOnly=0
			select	C.contractID as iContractID , C.contractTypeID as iContractTypeID , C.Symbol as vcSymbol, C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, S.DivFreq as iDivFreq,S.BaseIndexID as iBaseIndexID,
					S.DivDate as dtDivDate,S.DivAmt as fDivAmt, S.isHTB as bIsHTB,
				    CP.LotSize as iLotSize,CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk,
				    CP.priceLast as fPriceLast, CP.priceClose as fPriceClose, CP.priceOpen as fPriceOpen, CP.priceLow as fPriceLow, CP.priceHigh as fPriceHigh, CP.Volume as iVolume,
					E.exchangeID as iExchangeID,E.ExchangeCode as vcExchangeCode
				    from ContractView c  
						join StockView S on S.StockID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				   order by c.Symbol,E.ExchangeCode
		else
				select	C.contractID as iContractID , C.contractTypeID as iContractTypeID , C.Symbol as vcSymbol, C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, S.DivFreq as iDivFreq,S.BaseIndexID as iBaseIndexID,
					S.DivDate as dtDivDate,S.DivAmt as fDivAmt, S.isHTB as bIsHTB,
					CP.LotSize as iLotSize,CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk,
				    CP.priceLast as fPriceLast, CP.priceClose as fPriceClose, CP.priceOpen as fPriceOpen, CP.priceLow as fPriceLow, CP.priceHigh as fPriceHigh, CP.Volume as iVolume,
					E.exchangeID as iExchangeID,E.ExchangeCode as vcExchangeCode
				    from ContractView c  
						join StockView S on S.StockID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and CP.exchangeID is null
				   order by c.Symbol,E.ExchangeCode
	else
		select	C.contractID as iContractID , C.contractTypeID as iContractTypeID , C.Symbol as vcSymbol, C.ContractName as vcContractName, 
				S.defaultModelTypeID as iDefaultModelTypeID, S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, S.DivFreq as iDivFreq,S.BaseIndexID as iBaseIndexID,
				S.DivDate as dtDivDate,S.DivAmt as fDivAmt, S.isHTB as bIsHTB,
			    CP.LotSize as iLotSize,CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk,
			    CP.priceLast as fPriceLast, CP.priceClose as fPriceClose, CP.priceOpen as fPriceOpen, CP.priceLow as fPriceLow, CP.priceHigh as fPriceHigh, CP.Volume as iVolume,
				E.exchangeID as iExchangeID,E.ExchangeCode as vcExchangeCode
			    from ContractInGroupView cig
					join ContractView c  on C.contractID = CIG.contractID
					join StockView S on S.StockID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
		        where CIG.groupID = @iGroupID
		          and c.contractTypeID = 2
			   order by c.Symbol,E.ExchangeCode
if (@@error <> 0)
	Return (-1)
else  
   Return(0)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Strategy_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_Strategy_Del
        @iStrategyID int
  as
	update Strategy set actionID=3 where strategyID = @iStrategyID
--    delete Strategy where strategyID = @iStrategyID
        if (@@error <> 0)
           return (-1)
         else  
           return 0



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Strategy_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE      proc usp_Strategy_Get
	 @iStrategyID int = Null,
	 @bInTrade bit = 0,
	 @bShortFormat bit = 0,
	@dtActionDate datetime = null
  as
/*
	Edited by Zaur Nuraliev
	Description: this procedure is getting information from Strategy table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
     if (@iStrategyID is Null)
        if (@bInTrade = 0 and @bShortFormat = 0) 
           select 	strategyID as iStrategyID, 
					strategyName as vcStrategyName, 
					[description] as vcDescription,
					actionDate as dtActionDate,
					actionID as tiActionID
				from Strategy
				where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
				order by StrategyName
		else if (@bInTrade = 0 and @bShortFormat = 1) 
           select 	strategyID as iStrategyID, 
					strategyName as vcStrategyName,
					actionDate as dtActionDate,
					actionID as tiActionID
				from Strategy 
				where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
				order by StrategyName
        else
           select strategyID as iStrategyID, 
					strategyName as vcStrategyName, 
					[description] as vcDescription,
					actionDate as dtActionDate,
					actionID as tiActionID
				from Strategy
	            where exists (select tr.strategyID from TradeView tr  where tr.strategyID = Strategy.strategyID)
					and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
    	        order by StrategyName
	else 
        select	strategyID as iStrategyID, 
				strategyName as vcStrategyName, 
				[description] as vcDescription,
				actionDate as dtActionDate,
				actionID as tiActionID
			from Strategy 
			where strategyID = @iStrategyID
				and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Strategy_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    proc usp_Strategy_Save
        @iStrategyID int,
        @vcStrategyName varchar(12),
        @vcDescription varchar(120)
  as
   
	if exists (select * from Strategy where @vcStrategyName = strategyName and actionID < 3 and (@iStrategyID <> strategyID or @iStrategyID is null))
		begin
			Raiserror ('Row in Strategy table with same strategy name already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStrategyID is Null)  
     begin
         insert into Strategy (StrategyName, [Description])
                values (@vcStrategyName, @vcDescription)
         if (@@error <> 0)
            return (-1)
          else  
            return @@identity
     end
   else 
     begin
        update Strategy 
             set  StrategyName = @vcStrategyName,
                    [Description] = @vcDescription
          where strategyID = @iStrategyID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
     end                



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Symbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Symbol_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    proc usp_Symbol_Get
      @iGroupID int,
      @bIsAll bit,
      @bStockOnly bit,   
      @bIndexesOnly bit,
      @bOptionOnly bit 
  as
     if (@iGroupID is Null)
        if (@bIsAll is not Null)  and   (@bIsAll > 0)
           select	C.contractTypeID, C.Symbol
	            from ContractView C 
						join StockView S on S.StockID = C.contractID
	            where ((@bStockOnly is not Null) and (@bStockOnly > 0))
	               and c.contractTypeID = 2
          union
    	     select C.contractTypeID, C.Symbol
        	     from ContractView C 
						join Indexes I on I.Index_ID = C.contractID
	        	 where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
		    	         and c.contractTypeID = 1
          union 
	         select C.contractTypeID, C.Symbol
    	         from ContractView C
						join OptionView O on O.optionID = C.contractID
        		  where ((@bOptionOnly is not Null) and (@bOptionOnly > 0)) 
				          and c.contractTypeID in (3, 4)
	         order by c.Symbol   
       else
			select	C.contractTypeID, C.Symbol
           		from ContractView C 
						join StockView S on s.StockID = c.contractID
	            where ((@bStockOnly is not Null) and (@bStockOnly > 0)) 
		               and c.contractTypeID = 2
        		       and not exists (select * from ContractViewInGroups where contractID = c.contractID)
          union
	         select C.contractTypeID, C.Symbol
	            from ContractView C 
							join Indexes I on I.Index_ID = C.contractID
	    			where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
			             and C.contractTypeID = 1
	            		 and not exists (select * from ContractViewInGroups where contractID = c.contractID)
			         order by C.Symbol 
      else
         select C.contractTypeID, C.Symbol
			from ContractViewInGroups CIG
					join ContractView C on C.contractID = CIG.contractID
					join StockView S on S.StockID = C.contractID
	            where CIG.Group_ID = @iGroupID
            		   and ((@bStockOnly is not Null) and (@bStockOnly > 0))  
		               and c.contractTypeID = 2
         union
	         select C.contractTypeID, C.Symbol
	             from ContractViewInGroups CIG
						join ContractView C on C.contractID = CIG.contractID
						join Indexes I on I.Index_ID = C.contractID
		         where CIG.Group_ID = @iGroupID
			             and ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
			             and c.contractTypeID = 1
		         order by c.Symbol           
Return (0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TP]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TP]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE proc dbo.usp_TP
	@iContractID int,
	@fPriceTheo float = Null
--	,@iExchangeID int = Null, 
as
/*
	Created by Zaur Nuraliev
	[Description]:	this procedure save information into 
					ContractPriceHistory table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

--	if object_id('tempdb..##ContractPriceTheo') is not null
--		begin
--			insert into ##ContractPriceTheo (contractID, exchangeID, priceTheo)
--				values (@iContractID, @iExchangeID, @fPriceTheo)
--		end
	insert into ##ContractPriceTheo (contractID, priceTheo)
		values (@iContractID, @fPriceTheo)

    set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Clear]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO



CREATE PROCEDURE usp_Trade_Clear
AS
	begin tran
	
	delete from Trade
	
	if @@error = 0
		commit tran
	else
		rollback tran
	
	return @@error



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE   proc usp_Trade_Del
        @iTradeID int
as
	if @iTradeID is not null
	begin
		begin tran
		declare @nCount int,
			@iNewSeqNum int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iTradeID
		
		if @nCount = 1
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated)
			select tradeID,
				0,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated
			from Trade
			where tradeID = @iTradeID
			if @@error = 0
			begin
				set @iNewSeqNum = @@identity
				commit tran
				return @iNewSeqNum
			end
			else
			begin
				rollback tran
				return 0
			end
		end
		else
		begin
			rollback tran
			return 0
		end
	end
	else
		return 0
/*
        delete Trade where tradeID = @iTradeID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
*/



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE        proc dbo.usp_Trade_Get
	@iTradeID int = null,
	@iContractID int = null,
	@tiExtendedInfo tinyint = 0,
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iBookID int = null,
	@iStrategyID int = null,
	@iContractGroupID int = null,
	@dtMinTradeDate datetime = null
as

 if @tiExtendedInfo is null or @tiExtendedInfo = 0

	select
		tradeID as iTradeID,
		contractID as iContractID,
		isClose as tiIsClose,
		tradeDate as dtTradeDate,
		quantity as iQuantity,
		price as fPrice,
		isBuy as tiIsBuy,
		bookID as iBookID,     
		strategyID as iStrategyID, 
		traderID as iTraderID,
		brokerID as iBrokerID,
		clearingBrokerID as iClearingBrokerID,
		brokerCommissionID as iBrokerCommissionID,
		clearingBrokerCommissionID as iClearingBrokerCommissionID,
		tradedIV as fTradedIV,
		spotReference as fSpotReference,
		isPosition as tiIsPosition,
		lotSize as iLotSize,
		isSimulated as tiIsSimulated
		from Trade 
			where 
			(@iTradeID is null or @iTradeID is not null and tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and contractID = @iContractID)
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tradeDate >= @dtMinTradeDate)
		group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition,lotSize,isSimulated
		having min(actionID) = 1
		order by TradeDate

 else if @iContractGroupID is null
		select
			tr.tradeID as iTradeID,
			
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			null as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			cp.lotSize as iUnderlyingLotSize,
			
			tr.isSimulated as tiIsSimulated
		from Trade tr
			inner join Contract c on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID in (1, 2)
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,cp.priceClose,cp.lotSize
		having min(tr.actionID) = 1

   union

		select
			tr.tradeID as iTradeID,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			ucp.lotSize as iUnderlyingLotSize,
	
			tr.isSimulated as tiIsSimulated
		from Trade tr inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract UC on o.underlyingContractID = UC.contractID
			join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,
			cp.lotSize,cp.priceClose,ucp.lotSize,ucp.priceClose
		having min(tr.actionID) = 1
		order by TradeDate
  else
		select
			tr.tradeID as iTradeID,
			
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			null as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			cp.lotSize as iUnderlyingLotSize,
			
			tr.isSimulated as tiIsSimulated
		from Trade tr
			inner join Contract c on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractInGroup cig on c.contractID = cig.contractID
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID in (1, 2)
			and cig.groupID = @iContractGroupID
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,cp.priceClose,cp.lotSize
		having min(tr.actionID) = 1
   union
		select
			tr.tradeID as iTradeID,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			ucp.lotSize as iUnderlyingLotSize,
			
			tr.isSimulated as tiIsSimulated
		from Trade tr 
			inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract uc on O.underlyingContractID = UC.contractID
			join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractInGroup cig on o.underlyingContractID = cig.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and cig.groupID = @iContractGroupID
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,
			cp.lotSize,cp.priceClose,ucp.lotSize, ucp.priceClose
		having min(tr.actionID) = 1
		order by TradeDate

     Return(@@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_New]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE   proc usp_Trade_New
        @iContractID int = Null,
        @tiIsClose tinyint = Null,
        @iQuantity int = Null,
        @fPrice float = Null,
        @tiIsBuy tinyint = Null,  
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @iBrokerID int = Null,
        @iClearingBrokerID int = Null,
        @iBrokerCommissionID int =Null,
        @iClearingBrokerCommissionID int = Null,
        @fTradedIV  float = Null,
		@fSpotReference  float = Null,
		@tiIsPosition tinyint = 0,
		@iNewTradeID int out,
		@dtNewTradeDate datetime = Null out,
		@iLotSize int = Null,
		@tiIsSimulated tinyint = Null
  as
	begin tran
	declare @iNewSeqNum int
	select @iNewTradeID = max(tradeID) + 1 from Trade
	if @iNewTradeID is null set @iNewTradeID = 1
	if @dtNewTradeDate is null set @dtNewTradeDate = getdate()
	insert into Trade (
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		bookID, 
		strategyID, 
		traderID, 
		brokerID, 
		clearingBrokerID, 
		isBuy, 
		brokerCommissionID, 
		clearingBrokerCommissionID,
		tradedIV,
		spotReference,
		isPosition,
		lotSize,
		isSimulated)
	values (
		@iNewTradeID,
		1,
		@iContractID, 
		@tiIsClose, 
		@dtNewTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iBookID, 
		@iStrategyID, 
		@iTraderID, 
		@iBrokerID,
		@iClearingBrokerID,
		@tiIsBuy, 
		@iBrokerCommissionID,
		@iClearingBrokerCommissionID,
		@fTradedIV,
		@fSpotReference,
		@tiIsPosition,
		@iLotSize,
		@tiIsSimulated)
	if @@error = 0
	begin
		set @iNewSeqNum = @@identity 
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end
/*
   if not exists(select * from Trade where tradeID = @iTradeID)  
     begin
         insert into Trade (
				contractID,
				seqNum,
				publisherID, 
				isClose, 
				tradeDate, 
				quantity, 
				price, 
				bookID, 
				strategyID, 
				traderID, 
				brokerID, 
				clearingBrokerID, 
				isBuy, 
				brokerCommission, 
				clearingBrokerCommission,
				tradedIV,
				spotReference)
                values (
				@iContractID, 
				@iSeqNum,
				@vcPublisherID,
				@iIsClose, 
				@dtTradeDate, 
				@iquantity, 
				@fPrice, 
				@iBookID, 
				@iStrategyID, 
				@iTraderID, 
				@iBrokerID,
				@iClearingBrokerID,
				@iIsBuy, 
				@fBrokerCommission,
				@fClearingBrokerCommission,
				@fTradedIV,
				@fSpotReference)
         if (@@error <> 0)
            return (-1)
          else  
            return @@identity
     end
   else 
     begin
        update Trade 
             set contractID = isnull(@iContractID,contractID),
				seqNum = isnull(@iSeqNum,seqNum),
				publisherID = isnull(@vcPublisherID,publisherID),
				isClose = isnull(@iIsClose,isClose),
				tradeDate = isnull(@dtTradeDate,tradeDate),
				quantity = isnull(@iquantity,quantity),
				price = isnull(@fPrice,price),
				bookID = isnull(@iBookID,bookID),
				strategyID = isnull(@iStrategyID,strategyID),
				traderID = isnull(@iTraderID,traderID),
				brokerID = isnull(@iBrokerID,brokerID),
				clearingBrokerID = isnull(@iClearingBrokerID,clearingBrokerID),
				isBuy = isnull(@iIsBuy,isBuy),
				brokerCommission = isnull(@fBrokerCommission,brokerCommission),
				ClearingBrokerCommission = isnull(@fClearingBrokerCommission,ClearingBrokerCommission),
				tradedIV = isnull(@fTradedIV,tradedIV),
				spotReference = isnull(@fSpotReference,spotReference)
          where tradeID = @iTradeID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
     end
*/



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Split]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Split]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_Trade_Split
        @iContractID int,
        @fK float,
        @fF float,
        @iC int,
        @iW int,
		@iExchangeID int = Null -- the exchangeID special for update ContractPrice table
  as
begin   
  if (@fK = 0) or (@fF = 0)
     return(-2)
  begin tran
	update ContractPrice 
		set priceBid = priceBid / @fK,
           priceAsk = priceAsk / @fK,
           priceLast = priceLast / @fK,
           priceOpen = priceOpen / @fK,
           priceClose = priceClose / @fK,
           priceLow = priceLow / @fK,
           priceHigh = priceHigh / @fK
		where contractID = @iContractID and exchangeID=@iExchangeID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update ContractPrice for Underlying!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end 
   update Trade
      set Price = Price / @fk,
          quantity = round(quantity * @fK, 0)
    where contractID = @iContractID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update Trade for Underlying!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end 
   update Options
      set Strike = Strike / @fF
    where UnderlyingcontractID = @iContractID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update Options for Underlying!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end
   update ContractPrice
      set priceBid = priceBid / @fF,
          priceAsk = priceAsk / @fF,
          priceLast = priceLast / @fF,
          priceOpen = priceOpen / @fF,
          priceClose = priceClose / @fF,
          priceLow = priceLow / @fF,
          priceHigh = priceHigh / @fF,
          LotSize = @iW
     from Options
    where Options.UnderlyingcontractID = @iContractID
      and ContractPrice.contractID = Options.Option_ID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update ContractPrice for Options!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end
   update Trade
      set Price = Price / @fF,
          quantity = quantity * @iC
     from Options
    where Options.UnderlyingcontractID = @iContractID
      and Trade.contractID = Options.Option_ID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update Trade for Options!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end
  
  commit tran
  return 0
  
end  



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Update]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO


CREATE   proc usp_Trade_Update
		@iOldTradeID int,
        @iContractID int = Null,
        @tiIsClose tinyint = Null,
        @iQuantity int = Null,
        @fPrice float = Null,
        @tiIsBuy tinyint = Null,  
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @iBrokerID int = Null,
        @iClearingBrokerID int = Null,
        @iBrokerCommissionID float =Null,
        @iClearingBrokerCommissionID float = Null,
        @fTradedIV  float = Null,
		@fSpotReference  float = Null,
		@tiIsPosition tinyint = 0,
		@iNewTradeID int out,
        @dtNewTradeDate datetime out,
		@iLotSize int = Null,
		@tiIsSimulated tinyint = null
  as
	begin tran
	declare @bSucceed bit,
		@iNewSeqNum int
	set @bSucceed = 0
	if @iOldTradeID is not null
	begin
		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated)
			select tradeID,
				0,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated
			from Trade
			where tradeID = @iOldTradeID
		
			if @@error = 0 set @bSucceed = 1
		end
	end
	if @bSucceed = 1
	begin
		select @iNewTradeID = max(tradeID) + 1 from Trade
		if @iNewTradeID is null set @iNewTradeID = 1
		set @dtNewTradeDate = getdate()		
	
		insert into Trade (
			tradeID,
			actionID, 
			contractID,
			isClose, 
			tradeDate, 
			quantity, 
			price, 
			bookID, 
			strategyID, 
			traderID, 
			brokerID, 
			clearingBrokerID, 
			isBuy, 
			brokerCommissionID, 
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			lotSize,
			isSimulated)
		values (
			@iNewTradeID,
			1,
			@iContractID, 
			@tiIsClose, 
			@dtNewTradeDate, 
			@iQuantity, 
			@fPrice, 
			@iBookID, 
			@iStrategyID, 
			@iTraderID, 
			@iBrokerID,
			@iClearingBrokerID,
			@tiIsBuy, 
			@iBrokerCommissionID,
			@iClearingBrokerCommissionID,
			@fTradedIV,
			@fSpotReference,
			@tiIsPosition,
			@iLotSize,
			@tiIsSimulated)
		if @@error = 0
			set @bSucceed = 1
		else
			set @bSucceed = 0
	end
	if @bSucceed = 1
	begin
		set @iNewSeqNum = @@identity
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trader_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    proc usp_Trader_Del
        @iTraderID int
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
        update Trader set actionID=3 where traderRefID = @iTraderID		
        --delete Trader where traderRefID = @iTraderID		
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Trader set actionID=3 where traderID = @iTraderID
        --delete Trader where traderID = @iTraderID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trader_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE          proc usp_Trader_Get
	@iTraderID int = Null,
	@bInTrade bit = 0,
	@bShortFormat bit = 0,
	@dtActionDate datetime = null
  as
/*
	Edited by Zaur Nuraliev
	Description: this procedure is getting information from TraderView table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
     if (@iTraderID is Null)
        if (@bInTrade = 0 and @bShortFormat = 0) 
			select Trader.traderID as iTraderID, 
					Trader.acronym as vcAcronym, 
					Trader.traderName as vcTraderName, 
					Trader.clearingTraderCode as vcClearingTraderCode,
					Trader.traderRoleID as iTraderRoleID,
					Trader.traderRefID as iTraderRefID,
					TraderRole.roleName as vcRoleName,
					T.traderName as vcTraderRefName,
					Trader.actionDate as dtActionDate,
					Trader.actionID as tiActionID 
				from Trader
					left join TraderRoleView TraderRole on Trader.traderRoleID=TraderRole.traderRoleID
					left join TraderView t on Trader.traderRefID=T.traderID
				where	(Trader.actionDate>=@dtActionDate or @dtActionDate is null) and 
						(@dtActionDate is not null or @dtActionDate is null and Trader.actionID<3)
				order by Trader.TraderName
		else if (@bInTrade = 0 and @bShortFormat = 1) 
				select 	traderID as iTraderID, 
						traderName as vcTraderName 
				from Trader
				where	(Trader.actionDate >= @dtActionDate or (@dtActionDate is null and Trader.actionID < 3))
				order by TraderName
        else
			select Trader.traderID as iTraderID, 
					Trader.acronym as vcAcronym, 
					Trader.traderName as vcTraderName, 
					Trader.clearingTraderCode as vcClearingTraderCode,
					Trader.traderRoleID as iTraderRoleID,
					Trader.traderRefID as iTraderRefID,
					TraderRole.roleName as vcRoleName,
					T.traderName as vcTraderRefName,
					Trader.actionDate as dtActionDate,
					Trader.actionID as tiActionID 
				from Trader 
					left join TraderRoleView TraderRole on Trader.traderRoleID=TraderRole.traderRoleID
					left join TraderView t on Trader.traderRefID=T.traderID
				where exists (select tr.traderID from TradeView tr  where tr.traderID = Trader.traderID)
					and	(Trader.actionDate >= @dtActionDate or (@dtActionDate is null and Trader.actionID < 3))
	            order by Trader.TraderName
      else 
		select Trader.traderID as iTraderID, 
				Trader.acronym as vcAcronym, 
				Trader.traderName as vcTraderName, 
				Trader.clearingTraderCode as vcClearingTraderCode,
				Trader.traderRoleID as iTraderRoleID,
				Trader.traderRefID as iTraderRefID,
				TraderRole.roleName as vcRoleName,
				T.traderName as vcTraderRefName,
				Trader.actionDate as dtActionDate,
				Trader.actionID as tiActionID  
			from Trader 
				left join TraderRoleView TraderRole on Trader.traderRoleID=TraderRole.traderRoleID
				left join TraderView T on Trader.traderRefID=T.traderID
			where Trader.traderID = @iTraderID
					and	(Trader.actionDate >= @dtActionDate or (@dtActionDate is null and Trader.actionID < 3))
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trader_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE        proc usp_Trader_Save
        @iTraderID int = null,
        @vcAcronym varchar(12) = null,
        @vcTraderName varchar(120) = null,
        @vcClearingTraderCode varchar(11) = null,
		@iTraderRoleID int = null,
		@iTraderRefID int = null
  as
	if exists (select * from Trader where TraderName = @vcTraderName and actionID < 3 and (@iTraderID <> traderID or @iTraderID is null))
		begin
			Raiserror ('Trader with this name already exists!', 16, 1)
			Return (-1)
		end   
	if exists (select * from Trader where Acronym = @vcAcronym and actionID < 3 and (@vcAcronym <> Acronym or @iTraderID is null))
		begin
			Raiserror ('Trader with this acronym already exists!', 16, 1)
			Return (-1)
		end   
   if @iTraderID is null
   --if not exists(select * from Trader where traderID=@iTraderID)  
     begin
         insert into Trader (Acronym, TraderName,clearingTraderCode,traderRoleID,traderRefID)
                values (@vcAcronym, @vcTraderName,@vcClearingTraderCode,@iTraderRoleID,@iTraderRefID)
         if (@@error <> 0) 
			return (-1)
          else  
            return @@identity
     end
   else 
     begin
		if @iTraderRoleID<>3
			update Trader set traderRefID=Null where traderRefID=@iTraderID
		if @iTraderRoleID is null
			select @iTraderRoleID = traderRoleID from Trader where traderID=@iTraderID
		else if @iTraderRoleID=0 
			set @iTraderRoleID = null
	
		if @iTraderRefID is null
			select @iTraderRefID = traderRefID from Trader where traderID=@iTraderID
		else if @iTraderRefID=0 
			set @iTraderRefID = null
		if exists (select * from Trader where acronym = @vcAcronym and actionID < 3 and (@iTraderID <> traderID or @iTraderID is null))
			begin
				Raiserror ('Row in Trader table with same acronym already exists!', 16, 1)
				Return (-1)
			end   
		if exists (select * from Trader where clearingTraderCode = @vcClearingTraderCode and actionID < 3 and (@iTraderID <> traderID or @iTraderID is null))
			begin
				Raiserror ('Row in Trader table with same clearing trader code already exists!', 16, 1)
				Return (-1)
			end   
        update Trader 
             set acronym = isnull(@vcAcronym,acronym),
                traderName = isnull(@vcTraderName,traderName),
				clearingTraderCode = isnull(@vcClearingTraderCode,clearingTraderCode),
				traderRoleID = @iTraderRoleID, 	--isnull(@iTraderRoleID,traderRoleID),
				traderRefID = @iTraderRefID 	--isnull(@iTraderRefID,traderRefID)
          where traderID = @iTraderID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
     end                



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContract_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



Create   Procedure dbo.usp_TraderContract_Del 
	@iContractID int = null,
	@iTraderID int = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from TraderContract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
if 	@iTraderID is not null and @iContractID is not null
	update TraderContract set actionID=3
	--delete TraderContract 
		where traderID = @iTraderID  and ContractID=@iContractID
else if @iTraderID is not null and @iContractID is null
	update TraderContract set actionID=3
	--delete TraderContract 
		where traderID = @iTraderID
else if @iTraderID is null and @iContractID is not null
	update TraderContract set actionID=3
	--delete TraderContract 
		where ContractID=@iContractID
else if @iTraderID is null and @iContractID is null
	update TraderContract set actionID=3
	--delete TraderContract 
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContract_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE      Procedure dbo.usp_TraderContract_Get
	@iContractID int = null,
	@iTraderID int = null,
	@dtActionDate datetime = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is get information from TraderContract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	select traderID as iTraderID,contractID as iContractID
		from TraderContract
		where 
			(traderID = @iTraderID or @iTraderID is null) and 
			(contractID = @iContractID or @iContractID is null) and 
			(actionDate >= @dtActionDate or @dtActionDate is null and actionID < 3)

	set @error = @@error
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContract_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE       Procedure dbo.usp_TraderContract_Save
	@iContractID int,
	@iTraderID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is get information from TraderRole table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

/*	if exists (select * from TraderContract where 
				contractID=@iContractID and 
				traderID=@iTraderID and 
				actionID < 3)
		begin
			Raiserror ('Row in TraderContract table with same atributes already exists!', 16, 1)
			Return (-1)
		end   */

	--if @iContractID is null and @iTraderID is null 
	if not exists(select * from TraderContract where traderID=@iTraderID and contractID=@iContractID and actionID < 3)
		insert into TraderContract(traderID,contractID) 
			values(@iTraderID,@iContractID)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRM_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderRM_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE proc usp_TraderRM_Get
     @iTraderRefID int = Null
  as
     if (@iTraderRefID is Null)
		select 	traderID as iTraderID, 
				traderName as vcTraderName 
			from TraderView 
			where traderRefID is null and traderRoleID = 4
			order by TraderName
     else
		select 	traderID as iTraderID, 
				traderName as vcTraderName 
			from TraderView
			where traderRefID = @iTraderRefID  and traderRoleID = 4
            order by TraderName
     if (@@error <> 0) 
			Return (-1)
         else 
			Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderRM_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_TraderRM_Save
     @iTraderID int,
     @iTraderRefID int = Null
  as
	update Trader set traderRefID=@iTraderRefID where traderID = @iTraderID
     if (@@error <> 0) 
			Return (-1)
         else 
			Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRole_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderRole_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_TraderRole_Get 
	@iTraderRoleID int = null, 
	@vcRoleName varchar(30) = null,
	@dtActionDate datetime = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from TraderRole table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 	traderRoleID as iTraderRoleID,
			RoleName as vcRoleName,
			actionDate as dtActionDate,
			actionID as tiActionID
		from TraderRole
		where ((traderRoleID=@iTraderRoleID) or (@iTraderRoleID is null))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRole_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderRole_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   Procedure dbo.usp_TraderRole_Save 
	@iTraderRoleID int = Null Output,
	@vcRoleName varchar(30) = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save Roles
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from TraderRole where roleName = @vcRoleName and actionID < 3 and (@iTraderRoleID <> traderRoleID or @iTraderRoleID is null))
		begin
			Raiserror ('TraderRole with same name already exists!', 16, 1)
			Return (-1)
		end   
	Begin Transaction
		if @iTraderRoleID is null
		--if not exists(select * From TraderRole where traderRoleID=@iTraderRoleID) 
			begin
				insert into TraderRole (roleName) values (@vcRoleName)
				set @iTraderRoleID=@@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update TraderRole set 
					roleName = isnull(@vcRoleName, roleName)
				where traderRoleID = @iTraderRoleID
	
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Underlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Underlying_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE        proc dbo.usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
				
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
				
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize
				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 2
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
				
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and (@iContractID is null or c.contractID = @iContractID)
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
				
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
				
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
				
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				order by c.symbol	 
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
				
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
				
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where cig.groupID = @iGroupID
					and c.contractTypeID = 2
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
				
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where cig.groupID = @iGroupID
					and c.contractTypeID = 1
					and (@iContractID is null or c.contractID = @iContractID)
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
				
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
				
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where cig.groupID = @iGroupID
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
				
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where cig.groupID = @iGroupID
					and c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				order by c.symbol	 
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
				
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
				
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where
					c.contractTypeID = 2
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
				
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where
					c.contractTypeID = 1
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, cp.lotSize
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
				
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
				
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where 
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
				
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, cp.lotSize
				order by c.symbol	 
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
				
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
				
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID
					and c.contractTypeID = 2
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
				
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID
					and c.contractTypeID = 1
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, cp.lotSize
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
				
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
				
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
				
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
				
				    cp.priceClose as fPriceClose,
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID
					and c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, cp.lotSize
				order by c.symbol	 
		end
	end

	Return (0)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingOptions_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE proc dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
	set nocount on
	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end

	Return (0)







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VarIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VarIndex_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO



CREATE  PROCEDURE dbo.usp_VarIndex_Get
	@vcIndexSymbol as varchar(8)
AS
	select top 1
		ix.IndexID as iIndexID,
		null as fBeta,
		null as fVolatility,
		null as fCorrelation
	from
		IndexView ix,
		ContractView cr
	where
		ix.indexID = cr.contractID
		and cr.Symbol = @vcIndexSymbol



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VarIndex_Set]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VarIndex_Set]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO



CREATE PROCEDURE dbo.usp_VarIndex_Set
	@vcIndexName as varchar(8),
	@fBeta as float,
	@fVolatility as float,
	@fCorrelation as float
AS
	declare @iIndexID as integer
	set nocount on
	set @iIndexID = 0
	select top 1
		@iIndexID = contractID
	from
		ContractView
	where
		Symbol = @vcIndexName
		and contractTypeID = 1
	if @iIndexID <> 0
	begin
		update
			Indexes
		set
			fBeta = @fBeta,
			fVolatility = @fVolatility,
			fCorrelation = @fCorrelation
		where
			Index_ID = @iIndexID
	end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VarStockIndex_Set]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VarStockIndex_Set]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO



CREATE PROCEDURE dbo.usp_VarStockIndex_Set
	@vcStockName as varchar(8),
	@vcIndexName as varchar(8)
AS
	declare @iIndexID as integer
	declare @iStockID as integer
	set nocount on
	set @iIndexID = 0
	set @iStockID = 0
	select top 1
		@iStockID = contractID
	from
		ContractView
	where
		Symbol = @vcStockName
		and contractTypeID = 2
	select top 1
		@iIndexID = contractID
	from
		ContractView
	where
		Symbol = @vcIndexName
		and contractTypeID = 1
	if @iStockID <> 0 and @iIndexID <> 0
	begin
		update
			Stock
		set
			BaseIndexID = @iIndexID
		where
			StockID = @iStockID
	end



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaSurfaceData_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaSurfaceData_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE        Procedure dbo.usp_VolaSurfaceData_Del
	@iVolaSurfaceDataID int
as
/*
	this procedure deletes data from VolaManagementData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		update CustomStrikeSkewPoint set actionID=3
--		delete from CustomStrikeSkewPoint 
			where volaSurfaceDataID = @iVolaSurfaceDataID
			if @@error <> 0 begin set @error = 1 Rollback Transaction goto finish end
		delete from NextDaySurface 
			where volaSurfaceDataID = @iVolaSurfaceDataID
			if @@error <> 0 begin set @error = 1 Rollback Transaction goto finish end
		update VolaSurfaceData set actionID=3
--		delete VolaSurfaceData 
			where volaSurfaceDataID = @iVolaSurfaceDataID
			if @@error <> 0 begin set @error = 1 Rollback Transaction goto finish end
		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaSurfaceData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaSurfaceData_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE         Procedure dbo.usp_VolaSurfaceData_Get
	@iContractID int = null,
	@iVolaSurfaceDataID int = null,
	@dtActionDate datetime = null 
as
/*
	this procedure gets data from VolaManagementData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	VolaSurfaceData.volaSurfaceDataID as iVolaSurfaceDataID, 
			VolaSurfaceData.contractID as icontractID, 
			baseUnderlinePrice as fBaseUnderlinePrice, 
			smileAccelerator as fSmileAccelerator,
			interpolationFactor as fInterpolationFactor,
			strikeMoneynessHi as fStrikeMoneynessHi,
			strikeMoneynessLo as fStrikeMoneynessLo,
			surfaceName as vcSurfaceName,
			isPriceOverride as bIsPriceOverride,
			isDiscreteAcceleration as bIsDiscreteAcceleration,
			isDefaultSurface as bIsDefaultSurface,
			ATMVolatility as fATMVolatility,
			Contract.symbol as vcSymbolName,
			case when NextDaySurface.volaSurfaceDataID is not null then 1
				else 0 end bIsNextDaySurface,
			contract.contractTypeID as iContractTypeID,
			VolaSurfaceData.actionDate as dtActionDate,
			VolaSurfaceData.actionID as tiActionID
			from VolaSurfaceData
					join ContractView Contract on VolaSurfaceData.contractID=Contract.contractID
					left join NextDaySurface on VolaSurfaceData.volaSurfaceDataID=NextDaySurface.volaSurfaceDataID
			where	((@iContractID is Null or VolaSurfaceData.contractID = @iContractID) and 
					(@ivolaSurfaceDataID Is Null or VolaSurfaceData.volaSurfaceDataID = @ivolaSurfaceDataID))
				and 
					(VolaSurfaceData.actionDate>=@dtActionDate or @dtActionDate is null)
						and (@dtActionDate is not null or @dtActionDate is null and VolaSurfaceData.actionID<3)
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaSurfaceData_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaSurfaceData_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE       Procedure dbo.usp_VolaSurfaceData_Save
	@iVolaSurfaceDataID int = null output,
	@iContractID int = null,
	@fBaseUnderlinePrice float,
	@fSmileAccelerator float,
	@fInterpolationFactor float = null,
	@fStrikeMoneynessHi float = null,
	@fStrikeMoneynessLo float = null,
	@vcSurfaceName varchar(250) = null,
	@bIsPriceOverride bit = null,
	@bIsDiscreteAcceleration bit = null,
	@bIsDefaultSurface bit = null,
	@fATMVolatility float = null
as
/*
	this procedure saves data to VolaSurfaceData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @identity int
--	if @iVolaSurfaceDataID is null and @iContractID is not null
--		select @iVolaSurfaceDataID = volaSurfaceDataID from VolaSurfaceData where contractID = @iContractID
		if @iVolaSurfaceDataID is null
--		if not exists(select * from VolaSurfaceData where  VolaSurfaceDataID=@iVolaSurfaceDataID)
			begin
				insert into VolaSurfaceData (
							contractID, 
							baseUnderlinePrice, 
							smileAccelerator,
							interpolationFactor,
							strikeMoneynessHi,
							strikeMoneynessLo,
							surfaceName,
							isPriceOverride,
							isDiscreteAcceleration,
							isDefaultSurface,
							ATMVolatility) 
				values (@iContractID, 
						@fBaseUnderlinePrice,
						@fSmileAccelerator,
						@fInterpolationFactor,
						@fStrikeMoneynessHi,
						@fStrikeMoneynessLo,
						@vcSurfaceName,
						@bIsPriceOverride,
						@bIsDiscreteAcceleration,
						@bIsDefaultSurface,
						@fATMVolatility)
				if @@error <> 0 begin set @error = 1 Rollback Transaction goto finish end
				select @iVolaSurfaceDataID = @@identity
			end
		else
			begin
				update VolaSurfaceData set
					contractID = isnull(@iContractID, contractID),
					baseUnderlinePrice = isnull(@fBaseUnderlinePrice, baseUnderlinePrice),
					smileAccelerator = isnull(@fSmileAccelerator, smileAccelerator),
					interpolationFactor = isnull(@fInterpolationFactor, interpolationFactor),
					strikeMoneynessHi = IsNull(@fStrikeMoneynessHi,strikeMoneynessHi),
					strikeMoneynessLo = IsNull(@fStrikeMoneynessLo,strikeMoneynessLo),
					surfaceName = IsNull(@vcSurfaceName,surfaceName),
					isPriceOverride = IsNull(@bIsPriceOverride,isPriceOverride),
					isDiscreteAcceleration = IsNull(@bIsDiscreteAcceleration,isDiscreteAcceleration),
					isDefaultSurface = IsNull(@bIsDefaultSurface,isDefaultSurface),
					ATMVolatility = IsNull(@fATMVolatility,ATMVolatility)
				where volaSurfaceDataID = @iVolaSurfaceDataID
				if @@error <> 0 begin set @error = 2 Rollback Transaction goto finish end
			end
		set @error = 0
--==================================================================
-- set 'IsDefaultSurface'
--==================================================================
	if (select count(*) 
			from VolaSurfaceDataView 
			where contractID=(select contractID from VolaSurfaceData where VolaSurfaceDataID=@iVolaSurfaceDataID))=1
		begin
			update VolaSurfaceData set isDefaultSurface=1 
			where volaSurfaceDataID = @iVolaSurfaceDataID
		end
	
	else
		begin
			if @bIsDefaultSurface<>0
				begin
					update VolaSurfaceData set isDefaultSurface=0
					where contractID=(select contractID from VolaSurfaceDataView where VolaSurfaceDataID=@iVolaSurfaceDataID)
	
					update VolaSurfaceData set isDefaultSurface=@bIsDefaultSurface 
					where volaSurfaceDataID = @iVolaSurfaceDataID
				end
		end
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_WtdVegaMaturity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_WtdVegaMaturity_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_WtdVegaMaturity_Get 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from DataInfo table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select Convert(int,KeyValue) as iWtdVegaMaturity
		from DataInfo	
		where dataInfoID = 4
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_WtdVegaMaturity_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_WtdVegaMaturity_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_WtdVegaMaturity_Save 
	@iWtdVegaMaturity int		--the stored value of WtdVega
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure update WtdVega value
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		update DataInfo set keyValue = @iWtdVegaMaturity
			where dataInfoID = 4
	
    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


