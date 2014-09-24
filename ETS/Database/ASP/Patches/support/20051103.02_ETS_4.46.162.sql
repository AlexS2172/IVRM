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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.161'  or (keyValue = '3.46.162') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.162 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.162' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO


GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroup_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractInGroup_Import]
GO


CREATE    procedure dbo.usp_ContractInGroup_Import  
-- Created by Artem Shylaev Egar T  
-- 2005 - 08 - 24  
-- Modified by Anar Alimov
-- 2005 - 11 - 03
  @vXMLData ntext  
As  
  DECLARE @idoc int  
  EXEC sp_xml_preparedocument @idoc OUTPUT, @vXMLData  
  
  select V_XML.Symbol As SymbolsNotExist  
         from ( SELECT * FROM OPENXML(@idoc, '/Groups/Group',1)   
			 WITH (GroupId [varchar] (255) , Symbol[varchar](255) )      
               ) V_XML  
	 where V_XML.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS not in (
		select symbol from contract where ContractTypeID in (1, 2) and actionID < 3
	 )  
    
  Begin Tran  
  
    update ContractInGroup set actionid = 1 where actionid = 2  
    update ContractGroup set actionid = 1 where actionid = 2  
    delete ContractInGroup where groupid in (
	select groupid from ContractGroup where actionid = 3)  
    delete ContractGroup where actionid = 3  
    delete ContractInGroup where actionid = 3  
    delete ContractInGroup where groupid in ( 
	select CG.groupid from ContractGroup  CG  
		join ( SELECT distinct GroupId FROM OPENXML (@idoc, '/Groups/Group',1)   
						    WITH (GroupId [varchar] (255)
		     )      
                ) V_XML on  V_XML.GroupID =  CG.GroupName COLLATE SQL_Latin1_General_CP1_CI_AS
	)   
  
  
insert into ContractGroup   ( groupName , [description] )  
    select V_XML.GroupId As groupName , '' As [description]  
         from
		( SELECT distinct GroupId FROM OPENXML (@idoc, '/Groups/Group',1)   
					  WITH (GroupId [varchar] (255) )      
		) V_XML  
		where V_XML.GroupId COLLATE SQL_Latin1_General_CP1_CI_AS not in ( 
			select groupName from ContractGroup 
			)   
    if @@Error <> 0  
	Begin
	   rollback  
	   return 1 
	End   
  
 insert into ContractInGroup   ( contractID , groupID )  
      select Cn.ContractID, CG.groupID   
           from 
		( SELECT * FROM OPENXML(@idoc, '/Groups/Group',1)   
			   WITH (GroupId [varchar] (255), Symbol [varchar](255) )      
                ) V_XML  
    	   join ContractGroup CG on CG.groupName = V_XML.GroupId COLLATE SQL_Latin1_General_CP1_CI_AS  
	   join Contract Cn 	 on Cn.symbol = V_XML.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS  
	   where Cn.ContractID not in (
			 select distinct contractID from  ContractInGroup   
			 where groupID = CG.groupID
                         )  
		 and Cn.contractTypeID in (1,2,4)
		 and Cn.actionID<3  

  Commit  
    
  
  EXEC sp_xml_removedocument @idoc   
  return 0


GO


--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.162')
	begin
		update DataInfo set keyValue = '3.46.162' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.162' '3.46.162 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

