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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.258' or (keyValue = '4.46.259') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.259 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.259' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

---------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/
---------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QueryToCVSFile_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QueryToCVSFile_Save]
GO


CREATE    proc [dbo].[usp_QueryToCVSFile_Save]
 	@query varchar(6000),
	@file_name varchar(254) = 'Query',
	@timestamp datetime = null,
	@dir varchar(1000) = 'C:\Temp\',
	@delimeter char(1) = ','
as
/*	Created by Evgeny Pinchuk      */

     -- Delete char(13) and char(10) from string (need for bcp)
	declare @cmd varchar(8000)
	declare @curpos int
	declare @curchar char(1)
        
	set @curpos = 1
	while( @curpos <= len(@query))
	begin
	
	  set @curchar = substring(@query,@curpos,1)    
	  if ( ASCII(@curchar) = 13 or ASCII(@curchar) = 10 ) 
	    set @query = substring(@query,1,@curpos-1) + ' '+substring(@query,@curpos+1,6000)
	
	  set @curpos = @curpos + 1
	end
            
      -- File name = @filename + @timestamp + '.csv'
	if right(@dir,1)!='\' set @dir=@dir+'\'
	set @file_name = @file_name +
	IsNull('_' + replace(replace(replace(replace(Substring(convert(varchar(50),@timestamp,121),1,16),'-',''),':',''),' ',''),'.',''),'')
	+ '.csv'
       
      -- Delete If exists
	set @cmd = 'del /F /S /Q ' + @dir + @file_name
	exec master..xp_cmdshell @cmd--, no_output

	set @cmd = 'bcp "' + @query + '" queryout "'+ @dir + @file_name + '" -t'+@delimeter+' -c -C RAW -S ' + @@SERVERNAME + ' -T'
        print @cmd
	exec master..xp_cmdshell @cmd--, no_output

GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

-------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExportData_Trades]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExportData_Trades]
GO

CREATE    proc [dbo].[usp_ExportData_Trades]
as
/*	Created by Eugene Karnygin      */

select 
	case t.IsPosition 
		when 0 then 'Trade' 
		else 'Position'
	end								as Type,
	t.TradeDate,
	tr.acronym						as Trader,
	c.symbol						as SecuritySymbol,
	case t.IsBuy 
		when 0 then -1*t.quantity 
		when 1 then t.quantity		
	end								as Quantity,
	t.price							as Price,
	o.expiryDate					as ExpirationDate,
	o.strike						as Strike,
	case o.IsCall 
		when 0 then 'Put'			
		else 'Call'			
	end								as CallPut
from 
	TradeView t
	left join TraderView tr on (t.traderID = tr.traderId)
	left join ContractView c on (c.contractid = t.contractid)
	left join OptionView o on (c.contractid = o.optionid)
order by	
	tr.acronym, 
	t.TradeDate,
	c.symbol

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

---------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.259')
	begin
		update DataInfo set keyValue = '4.46.259' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 4.46.259' '4.46.259 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


