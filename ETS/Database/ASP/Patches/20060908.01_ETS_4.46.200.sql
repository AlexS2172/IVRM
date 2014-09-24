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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.199'  or (keyValue = '3.46.200') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.200 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.200' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

ALTER   PROC dbo.usp_AspEodUpdateMarketStructure_ProcessMissedFutures 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	-- delete expired futures
	truncate table EodContractForDelete
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForDelete
	select f.futureID
	from Future f (nolock)
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join Contract uc (nolock) on fr.underlyingID = uc.contractID and uc.actionID < 3
		join EodUnderlyingMap eod_um on eod_um.contractID = uc.contractID
	where datediff(d,uc.MSUpdateDate,  f.actionDate) < 0  --uc.actionDate
		and f.futureID not in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to get useless expired futures list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_DeleteFutures
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to delete expired futures', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- mark used expired options as invalid
	truncate table EodContractForMark
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForMark
	select f.futureID
	from Future f (nolock)
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join Contract uc (nolock) on fr.underlyingID = uc.contractID and uc.actionID < 3
		join EodUnderlyingMap eod_um on eod_um.contractID = uc.contractID
	where datediff(d, uc.MSUpdateDate, f.actionDate) < 0 or uc.MSUpdateDate is null
		and f.futureID in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to get used expired futures list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_MarkContracts
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to mark expired futures as invalid', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error


GO
GO

ALTER     PROCEDURE usp_IC_Save
      @iICID int,
      @vcSymbol varchar(20) = null,
      @vcName varchar(255) = null,
      @iICType int,   
      @tiGreeksCalcType tinyint = null,
      @tiIsActive tinyint = null,
      @iFutureUnderlyingTypeID int,
      @tiCheckExistance tinyint = null,
      @iUndPriceProfileID int = null output,
      @iOptPriceProfileID int = null output
  
     as
    ---------------
    set nocount on
    ---------------
  --  if ((@iICType <> 6) And (@iICType <> 7 ))
  --    begin
  --      Raiserror ('Wrong Interest/FutureUnderlying type value', 16, 1)
  --      Return (-1)
  --    end
  
    if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID = @iICType and actionID < 3 and (@iICID <> contractID or @iICID is null))
      begin
        if isnull(@tiCheckExistance, 1) <> 0
          Raiserror ('Interest or FutureUnderlying with this ticker already exists!', 16, 1)
        Return (-1)
      end   
  
     if (@iICID is Null)  
     begin
       
       begin tran
        if @iUndPriceProfileID is null
        begin
          select
            @iUndPriceProfileID = priceProfileID
          from
            PriceProfileView
          where
            defaultID = 3
            and isOptionProfile = 0
        end
  
        if @iOptPriceProfileID is null
        begin
          select
            @iOptPriceProfileID = priceProfileID
          from
            PriceProfileView
          where
            defaultID = 1
            and isOptionProfile = 1
        end

      if @iICType = 6
      begin		
          --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')
          --set @iExpCalendarID = scope_identity()

               insert into Contract (contractTypeID, Symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
                    values ( @iICType, @vcSymbol, @vcName/*, null*/, @iUndPriceProfileID, @iOptPriceProfileID)
      end
      else
      begin
          --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')
          --set @iExpCalendarID = scope_identity()  

               insert into Contract (contractTypeID, Symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
                    values ( @iICType, @vcSymbol, @vcName/*, null*/, @iUndPriceProfileID, @iOptPriceProfileID)	
      end
  
         select @iICID = @@identity      
           if (@@error <> 0)
              begin
              RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
              ROLLBACK TRAN
              RETURN (-1)
              end
         insert into [FutureUnderlying] (
                iFutureUnderlyingID, 
                tiActive,
                tiGreeksCalculationType,
                iFutureUnderlyingTypeID)
             values (
                @iICID, 
                @tiIsActive,
                @tiGreeksCalcType,
                @iFutureUnderlyingTypeID)
           if (@@error <> 0)
            begin
               RAISERROR ('Error. Can''t insert into [FutureUnderlying]!', 16, 1)
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
            contractName = isNull(@vcName,contractName),
            --expCalendarID = null,--isNull(@iExpCalendarID, expCalendarID),
            undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
            optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
          where contractID = @iICID
        if (@@error <> 0)
           begin
            RAISERROR ('Error. Can''t update Contract!', 16, 1)
            ROLLBACK TRAN
            RETURN (-1)
           end
        update [FutureUnderlying]
           set  tiGreeksCalculationType = isnull(@tiGreeksCalcType, tiGreeksCalculationType),
            tiActive = isnull(@tiIsActive, tiActive),
            iFutureUnderlyingTypeID = isnull(@iFutureUnderlyingTypeID, iFutureUnderlyingTypeID)
           where iFutureUnderlyingID = @iICID
         if (@@error <> 0)
           begin
            RAISERROR ('Error. Can''t update [FutureUnderlying]!', 16, 1)
            ROLLBACK TRAN
            RETURN (-1)
           end       
       commit tran
     end
  
    --========================================================================
    -- make record with null exchange for this index
    --========================================================================
  --  if not exists(select * from ContractPrice where exchangeID is null and contractID = @iICID)
  --    begin
  --      insert into ContractPrice(contractID,exchangeID)
  --        values(@iICID,null)
  --      if (@@error <> 0) begin rollback tran return(-1) end              
  --    end   
  
  if (@@error <> 0)
    return (-1)
  else  
    return @iICID


GO

ALTER	PROC usp_ICFutures_Get
		@iICID INT = NULL
AS
	-- retrieve future
	SET NOCOUNT ON
	IF @iICID IS NOT NULL
		SELECT 
			F.maturityDate  AS MaturityDate,
			Cm.iFutureUnderlyingID  AS CommodityID, 
			Fr.futureRootName  AS FutureRootName, Fr.futureLotSize  AS FutureLotSize, 
			Fr.futureRootSymbol  AS FutureRootSymbol, CF.symbol   AS vcSymbol, Fr.optionLotSize  AS OptioLotSize, 
			Fr.exportSymbol  AS FRExportSymbol, CF.contractName  AS FutureName, F.futureRootID   AS FutureRootID,
			CP.priceClose  AS priceClose
		FROM 
			Future F
				INNER JOIN FutureRoot FR ON FR.futureRootID = F.futureRootID And Fr.underlyingID =  @iICID
				INNER JOIN FutureUnderlying  CM ON Cm.iFutureUnderlyingID  =  @iICID 
				INNER JOIN Contract   CF  ON CF.ContractId    =  F.futureID
				LEFT JOIN ContractPrice CP ON CP.ContractID = CF.ContractId
		ORDER BY FutureName
	ELSE
		SELECT 
			F.maturityDate AS MaturityDate, Cm.iFutureUnderlyingID  AS CommodityID, 
			Fr.futureRootName  AS FutureRootName, Fr.futureLotSize  AS FutureLotSize, 
			Fr.futureRootSymbol  AS FutureRootSymbol, CF.symbol   AS vcSymbol, Fr.optionLotSize  AS OptioLotSize, 
			Fr.exportSymbol  AS FRExportSymbol, CF.contractName  AS FutureName, F.futureRootID   AS FutureRootID,
			CP.priceClose  AS priceClose
		FROM
			Future F
				INNER JOIN FutureRoot FR ON FR.futureRootID = F.futureRootID 
				INNER JOIN FutureUnderlying  CM ON  CM.iFutureUnderlyingID  IS NOT NULL 
				INNER JOIN Contract   CF  ON CF.ContractId    =  F.futureID
				LEFT JOIN ContractPrice CP ON CP.ContractID = CF.ContractId
		ORDER BY 
			FutureName


GO

GO
/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.200')
	begin
		update DataInfo set keyValue = '3.46.200' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.200' '3.46.200 update message:' 
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