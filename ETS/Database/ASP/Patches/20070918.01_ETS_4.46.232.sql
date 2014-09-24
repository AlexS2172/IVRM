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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.231'  or (keyValue = '3.46.232') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.232 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.232' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/

--DATA SAMPLE
-- declare @vXMLData nchar(1000)
-- set  @vXMLData = 
-- -- '<root>
-- -- 	<asset headid="908118" elemid="1452042" coeff="77.6000000001" usehead="0" />
-- -- 	<asset headid="1063695" elemid="121212" coeff="734.6000000001" usehead="0" />
-- -- 	<asset headid="123559" elemid="121213" coeff="774.6000000001" usehead="0" />
-- -- 	<asset headid="123559" elemid="121213" coeff="774.6000000001" usehead="0" />
-- -- 	<asset headid="123559" elemid="123559" coeff="774.6000000001" usehead="0" />
-- -- </root>'
-- '<root>	
--	<asset headid=" 908119" elemid=" 1452274" coeff=" .1" usehead="1" />
--	<asset headid=" 908119" elemid=" 1452393" coeff=" .1" usehead="1" />
--	<asset headid=" 908119" elemid=" 1452421" coeff=" .1" usehead="1" />
--	<asset headid=" 908119" elemid=" 1452422" coeff=" .1" usehead="1" />
-- </root>'
-- 
-- exec usp_AssetGroup_Save @vXMLData

---------------------------------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[AssetGroup]') 
	   AND 	  OBJECTPROPERTY(id, N'IsUserTable') = 1)
    DROP TABLE [AssetGroup]
GO

CREATE TABLE [AssetGroup] (
	[iHeadID] [int] ,
	[iElementID] [int] NOT NULL ,
	[bUseHeadForPrice] [int] NULL ,
	[fCoeff] [float] NULL )
GO
---------------------------------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_AllGroupAssets_Get]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_AllGroupAssets_Get
GO
create	procedure dbo.usp_AllGroupAssets_Get
As
	select
		iHeadID 	 as iHeadID,
		iElementID 	 as iElementID,
		bUseHeadForPrice as bPriceByHead,
		fCoeff		 as fCoeff
	from 
		AssetGroup
GO
---------------------------------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_UnderlyingsForAssetGroup_Get]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_UnderlyingsForAssetGroup_Get
GO
create  procedure dbo.usp_UnderlyingsForAssetGroup_Get
As
	
	select
		cv.ContractID	as iContractID,
		cv.Symbol 	as vcSymbol,
		case when agc.iContractID is null then  0 else  1 end as bIsUsed,
		cv.ContractTypeID as iContractType,
		case when agc.IncludeType is null then 2 else agc.IncludeType end as iInclusionType,
		case when agw.fCoeff is null then 0.0 else agw.fCoeff end as fCoeff,
		case when agw.bUseHeadForPrice is null then 0 else agw.bUseHeadForPrice end as bPriceByHead
	from ContractView cv
	left join (	select 
				distinct iElementId as iContractID, 0 as IncludeType
			from
				AssetGroup
			union
			select 
				distinct iHeadId as iContractID, 1 as IncludeType
			from
				AssetGroup
		    ) agc on cv.contractid = agc.iContractID
	left join AssetGroup agw on cv.contractid = agw.iHeadID
	where
		cv.contractTypeId in (1,2)
	order by 
		cv.ContractTypeID desc

GO
---------------------------------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_AssetGroupElement_Save]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_AssetGroupElement_Save
GO

create  procedure dbo.usp_AssetGroupElement_Save
  @iHeadId int,
  @iElementID int,
  @bUseHead int = NULL,
  @fCoeff float = NULL
As

	if exists(select 1 from AssetGroup where iHeadId = @iHeadId and iElementId = @iElementId)
	begin
		update
			AssetGroup
		set
			bUseHeadForPrice = isnull(@bUseHead, bUseHeadForPrice),
			fCoeff = isnull(@fCoeff, fCoeff)
		where
			iHeadId = @iHeadId and iElementId = @iElementId
	end
	else
	begin
		insert
		into
			AssetGroup(iHeadId, iElementId, bUseHeadForPrice, fCoeff)
		values
			(@iHeadId, @iElementId, @bUseHead, @fCoeff)
	end
GO

---------------------------------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_GroupAsset_Get]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_GroupAsset_Get
GO
create  procedure dbo.usp_GroupAsset_Get
  @iHeadID int = NULL
As
	select
		AG.iHeadId as iHeadID,
		AG.iElementId as iElementID,
		AG.bUseHeadForPrice as bUseHead,
		AG.fCoeff as fCoefficient,
		CVh.symbol as vcHeadSymbol,
		CVe.symbol as vcElementSymbol
	from
		AssetGroup AG
		left join ContractView CVh on CVh.ContractID = AG.iHeadId
		left join ContractView CVe on CVe.ContractID = AG.iElementId
	where
		iHeadId = @iHeadID
GO

---------------------------------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_GroupAsset_Del]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_GroupAsset_Del
GO
create  procedure dbo.usp_GroupAsset_Del
  @iHeadID int
As
	delete
	from
		AssetGroup
	where
		iHeadId = @iHeadID
GO

---------------------------------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_AssetGroupElement_Del]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_AssetGroupElement_Del
GO
create	procedure dbo.usp_AssetGroupElement_Del
  @iHeadID int,
  @iElementID int
As
	delete
	from
		AssetGroup
	where
		iHeadId = @iHeadId and iElementId = @iElementID
GO

---------------------------------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_AssetGroup_Save]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_AssetGroup_Save
GO

create      procedure dbo.usp_AssetGroup_Save
  @vXMLData ntext  
As  

	DECLARE @idoc int  
	EXEC sp_xml_preparedocument @idoc OUTPUT, @vXMLData  
	
	begin Tran
	-- dlete asset
	delete
	from 
		AssetGroup
	where
		iHeadId in 	(	
					select distinct headid
					from openxml (@idoc, '/root/asset', 2) 
					with (headid int '@headid')
				)
	-- update asset
	insert 
		AssetGroup(iHeadId, iElementId, bUseHeadForPrice, fCoeff)
	select 
			vXML.headid, 
			vXML.elemid,
			vXML.usehead,
			vXML.coeff
		from openxml (@idoc, '/root/asset', 2) with
	 	(
			headid int '@headid',
			elemid int '@elemid',
			coeff float '@coeff',
			usehead int '@usehead'
		)vXML

	

	if @@Error <> 0
	begin
		rollback
		Return -1
	end

	commit
	EXEC sp_xml_removedocument @idoc   
	Return 0

GO

GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.232')
	begin
		update DataInfo set keyValue = '3.46.232' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.232' '3.46.232 update message:' 
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




