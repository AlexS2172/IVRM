




ALTER TABLE [Book] ADD 
	CONSTRAINT [PK_Books] PRIMARY KEY  CLUSTERED 
	(
		[bookID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Book] ADD 
	CONSTRAINT [CK_Book] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [Book] ADD 
	CONSTRAINT [DF_Book_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_Book_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_Book ON dbo.Book
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Book set actionDate=GetDate(),actionID=2
		from Book
				join inserted on Book.BookID=inserted.BookID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update Book set actionID=3, actionDate=deleted.actionDate
		from Book
				join deleted on Book.BookID=deleted.BookID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_BookName] ON [Book]([bookName]) ON [PRIMARY]
GO


ALTER TABLE [BookRuleItem] ADD 
	CONSTRAINT [PK_BookRuleItem] PRIMARY KEY  CLUSTERED 
	(
		[bookRuleItemID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [BookRuleItemMask] ADD 
	CONSTRAINT [PK_RiskItemMask] PRIMARY KEY  CLUSTERED 
	(
		[bookRuleItemMaskID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [Broker] ADD 
	CONSTRAINT [PK_Brokers] PRIMARY KEY  CLUSTERED 
	(
		[brokerID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Broker] ADD 
	CONSTRAINT [CHK_IsClearing] CHECK ([isClearing] = 1 or [isClearing] = 0),
	CONSTRAINT [CK_actionID] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [Broker] ADD 
	CONSTRAINT [DF__Broker__MDate__34213060] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [DF__Broker__MType__360978D2] DEFAULT (1) FOR [actionID]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE        TRIGGER tU_Broker ON dbo.Broker
    FOR Update
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------

	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Broker set actionDate=GetDate(),actionID=2
		from Broker
				join inserted on Broker.brokerID=inserted.brokerID
		where Broker.actionID < 3 --for eliminate set of 'delete' flag operation

	update Broker set actionID=3,actionDate=deleted.actionDate
		from Broker
				join deleted on Broker.brokerID=deleted.brokerID
		where deleted.actionID = 3 --for eliminate set 3-->1


	--======================================================================
	-- check programm logic
	--======================================================================
  if Update (IsClearing)
     begin
      if exists (select t.tradeID 
                        from inserted i, Trade t
                     where i.IsClearing = 0
                       and t.ClearingbrokerID = i.brokerID and t.actionID<3)
         begin 
             RAISERROR ('Error Can''t update IsClearing field in Brokers. Some brokers are clearing brokers in Trade table!', 16, 1)
             ROLLBACK TRANSACTION
             RETURN
         end
      if exists (select t.tradeID 
                        from inserted i, Trade t
                     where i.IsClearing <> 0
                       and t.brokerID = i.brokerID and t.actionID<3)
         begin 
             RAISERROR ('Error Can''t update IsClearing field in Brokers. Some brokers are nonclearing brokers in Trade table!', 16, 1)
             ROLLBACK TRANSACTION
             RETURN
         end 
    end







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_BrokerName] ON [Broker]([brokerName]) ON [PRIMARY]
GO

 CREATE  INDEX [I_IsClearing] ON [Broker]([isClearing]) ON [PRIMARY]
GO






 CREATE  CLUSTERED  INDEX [clearingHouseTradeID_ind] ON [ClearingHouseTrade]([ClearingHouseTradeID]) ON [PRIMARY]
GO

 CREATE  UNIQUE  INDEX [iDateOfTrade_iCacheID_ind] ON [ClearingHouseTrade]([iDateOfTrade], [iCacheID]) ON [PRIMARY]
GO


ALTER TABLE [Commission] ADD 
	CONSTRAINT [PK_Commissions] PRIMARY KEY  CLUSTERED 
	(
		[commissionID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Commission] ADD 
	CONSTRAINT [CK_Commission_action] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [Commission] ADD 
	CONSTRAINT [DF_Commission_MDate] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [DF_Commission_MType] DEFAULT (1) FOR [actionID]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create  Trigger tU_Commission ON dbo.Commission
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Commission set actionDate=GetDate(),actionID=2
		from Commission
				join inserted on Commission.commissionID=inserted.commissionID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update Commission set actionID=3,actionDate=deleted.actionDate
		from Commission
				join deleted on Commission.commissionID=deleted.commissionID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_CommissionBrokerDate] ON [Commission]([brokerID], [commissionDate]) ON [PRIMARY]
GO


ALTER TABLE [Contract] ADD 
	CONSTRAINT [PK_Contracts] PRIMARY KEY  CLUSTERED 
	(
		[contractID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Contract] ADD 
	CONSTRAINT [CK_Contract] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [Contract] ADD 
	CONSTRAINT [DF_Contract_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_Contract_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_Contract ON dbo.Contract
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Contract set actionDate=GetDate(),actionID=2
		from Contract
				join inserted on Contract.ContractID=inserted.ContractID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update Contract set actionID=3, actionDate=deleted.actionDate
		from Contract
				join deleted on Contract.ContractID=deleted.ContractID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_Contracts_Symbol] ON [Contract]([symbol]) ON [PRIMARY]
GO

 CREATE  INDEX [ContractTypeID_ind] ON [Contract]([contractTypeID]) ON [PRIMARY]
GO

 CREATE  INDEX [ContractID_ContractTypeID_ind] ON [Contract]([contractID], [contractTypeID]) ON [PRIMARY]
GO

 CREATE  INDEX [contractID_symbol_ind] ON [Contract]([contractID], [symbol]) ON [PRIMARY]
GO


ALTER TABLE [ContractExchangeLink] ADD 
	CONSTRAINT [PK_ContractExchangeLink] PRIMARY KEY  CLUSTERED 
	(
		[contractExchangeLinkID]
	)  ON [PRIMARY] 
GO





 CREATE  UNIQUE  INDEX [IX_ContractExchangeLink] ON [ContractExchangeLink]([exchangeID], [contractID]) ON [PRIMARY]
GO


ALTER TABLE [ContractGroup] ADD 
	CONSTRAINT [PK_Groups] PRIMARY KEY  CLUSTERED 
	(
		[groupID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [ContractGroup] ADD 
	CONSTRAINT [CK_ContractGroup] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [ContractGroup] ADD 
	CONSTRAINT [DF_ContractGroup_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_ContractGroup_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE Trigger tU_ContractGroup ON dbo.ContractGroup
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update ContractGroup set actionDate=GetDate(),actionID=2
		from ContractGroup
				join inserted on ContractGroup.groupID=inserted.groupID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update ContractGroup set actionID=3, actionDate=deleted.actionDate
		from ContractGroup
				join deleted on ContractGroup.groupID=deleted.groupID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_GroupName] ON [ContractGroup]([groupName]) ON [PRIMARY]
GO


ALTER TABLE [ContractInGroup] ADD 
	CONSTRAINT [PK_ContractsInGroups] PRIMARY KEY  NONCLUSTERED 
	(
		[contractInGroupID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [ContractInGroup] ADD 
	CONSTRAINT [CK_ContractInGroup] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [ContractInGroup] ADD 
	CONSTRAINT [DF_ContractInGroup_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_ContractInGroup_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_ContractInGroup ON dbo.ContractInGroup
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update ContractInGroup set actionDate=GetDate(),actionID=2
		from ContractInGroup
				join inserted on ContractInGroup.ContractID=inserted.ContractID and ContractInGroup.GroupID=inserted.GroupID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update ContractInGroup set actionID=3, actionDate=deleted.actionDate
		from ContractInGroup
				join deleted on ContractInGroup.ContractID=deleted.ContractID and ContractInGroup.GroupID=deleted.GroupID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_ContractsInGroups_GroupID] ON [ContractInGroup]([groupID]) ON [PRIMARY]
GO


ALTER TABLE [ContractMarketData] ADD 
	CONSTRAINT [PK_ContractMarketData] PRIMARY KEY  NONCLUSTERED 
	(
		[contractID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [ContractPrice] ADD 
	CONSTRAINT [PK_ContractPrice] PRIMARY KEY  CLUSTERED 
	(
		[contractPriceID]
	)  ON [PRIMARY] 
GO



ALTER TABLE [ContractPrice] ADD 
	CONSTRAINT [DF_ContractPrice_priceDate] DEFAULT (getdate()) FOR [priceDate]
GO



 CREATE  INDEX [exchangeID_ind] ON [ContractPrice]([exchangeID]) ON [PRIMARY]
GO

 CREATE  INDEX [contractID_ind] ON [ContractPrice]([contractID]) ON [PRIMARY]
GO

 CREATE  INDEX [contractID_priceClose_ind] ON [ContractPrice]([contractID], [priceClose]) ON [PRIMARY]
GO

 CREATE  INDEX [priceClose_ind] ON [ContractPrice]([priceClose]) ON [PRIMARY]
GO


ALTER TABLE [CustomStrikeSkewPoint] ADD 
	CONSTRAINT [PK_CustomStrikeSkewPoint] PRIMARY KEY  NONCLUSTERED 
	(
		[customStrikeSkewPointID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [CustomStrikeSkewPoint] ADD 
	CONSTRAINT [CK_CustomStrikeSkewPoint] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [CustomStrikeSkewPoint] ADD 
	CONSTRAINT [DF_CustomStrikeSkewPoint_isBasePoint] DEFAULT (0) FOR [isBasePoint],
	CONSTRAINT [DF_CustomStrikeSkewPoint_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_CustomStrikeSkewPoint_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



Create Trigger tU_CustomStrikeSkewPoint ON dbo.CustomStrikeSkewPoint
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update CustomStrikeSkewPoint set actionDate=GetDate(),actionID=2
		from CustomStrikeSkewPoint
				join inserted on CustomStrikeSkewPoint.CustomStrikeSkewPointID=inserted.CustomStrikeSkewPointID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update CustomStrikeSkewPoint set actionID=3, actionDate=deleted.actionDate
		from CustomStrikeSkewPoint
				join deleted on CustomStrikeSkewPoint.CustomStrikeSkewPointID=deleted.CustomStrikeSkewPointID
		where deleted.actionID = 3 --for eliminate set 3-->1



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  CLUSTERED  INDEX [CustomStrikeSkewPoint3] ON [CustomStrikeSkewPoint]([actionID], [volaSurfaceDataID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CustomStrikeSkewPoint] ON [CustomStrikeSkewPoint]([volaSurfaceDataID], [strike], [type], [expiryID]) ON [PRIMARY]
GO

 CREATE  INDEX [volaSurfaceDataID_ind] ON [CustomStrikeSkewPoint]([volaSurfaceDataID]) ON [PRIMARY]
GO

 CREATE  INDEX [actionDate_ind] ON [CustomStrikeSkewPoint]([actionDate]) ON [PRIMARY]
GO

 CREATE  INDEX [actionID_ind] ON [CustomStrikeSkewPoint]([actionID]) ON [PRIMARY]
GO

 CREATE  INDEX [volaSurfaceDataID_expDate_ind] ON [CustomStrikeSkewPoint]([volaSurfaceDataID], [expiryID]) ON [PRIMARY]
GO

 CREATE  INDEX [actionID_volaSurfaceDataID_ind] ON [CustomStrikeSkewPoint]([actionID], [volaSurfaceDataID]) ON [PRIMARY]
GO


ALTER TABLE [DataInfo] ADD 
	CONSTRAINT [PK_DataInfo] PRIMARY KEY  NONCLUSTERED 
	(
		[dataInfoID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [Exchange] ADD 
	CONSTRAINT [PK_Exchanges] PRIMARY KEY  CLUSTERED 
	(
		[exchangeID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Exchange] ADD 
	CONSTRAINT [CK_Exchange] CHECK ([actionID] > 0 and [actionID] < 4),
	CONSTRAINT [CK_ExchangeCode] CHECK (len(ltrim(rtrim([exchangeCode]))) <> 0)
GO


ALTER TABLE [Exchange] ADD 
	CONSTRAINT [DF_Exchange_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_Exchange_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_Exchange ON dbo.Exchange
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Exchange set actionDate=GetDate(),actionID=2
		from Exchange
				join inserted on Exchange.ExchangeID=inserted.ExchangeID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update Exchange set actionID=3, actionDate=deleted.actionDate
		from Exchange
				join deleted on Exchange.ExchangeID=deleted.ExchangeID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_ExchangeName] ON [Exchange]([exchangeName]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_ExchangeCode] ON [Exchange]([exchangeCode]) ON [PRIMARY]
GO


ALTER TABLE [ExchangeHoliday] ADD 
	CONSTRAINT [PK_ExchangeHolidays] PRIMARY KEY  NONCLUSTERED 
	(
		[exchangeHolidayID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [ExchangeHoliday] ADD 
	CONSTRAINT [CK_ExchangeHoliday] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [ExchangeHoliday] ADD 
	CONSTRAINT [DF_ExchangeHoliday_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_ExchangeHoliday_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_ExchangeHoliday ON dbo.ExchangeHoliday
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update ExchangeHoliday set actionDate=GetDate(),actionID=2
		from ExchangeHoliday
				join inserted on ExchangeHoliday.ExchangeID=inserted.ExchangeID and ExchangeHoliday.HolidayID=inserted.HolidayID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update ExchangeHoliday set actionID=3, actionDate=deleted.actionDate
		from ExchangeHoliday
				join deleted on ExchangeHoliday.ExchangeID=deleted.ExchangeID and ExchangeHoliday.HolidayID=deleted.HolidayID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



ALTER TABLE [Expiry] ADD 
	CONSTRAINT [PK_Expiry] PRIMARY KEY  CLUSTERED 
	(
		[expiryID]
	)  ON [PRIMARY] 
GO



ALTER TABLE [Expiry] ADD 
	CONSTRAINT [DF__Expiry__actionID__3AAC9BB0] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF__Expiry__actionDa__3BA0BFE9] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_Expiry ON dbo.Expiry
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Expiry set actionDate=GetDate(),actionID=2
		from Expiry
				join inserted on Expiry.expiryID=inserted.expiryID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update Expiry set actionID=3, actionDate=deleted.actionDate
		from Expiry
				join deleted on Expiry.expiryID=deleted.expiryID
		where deleted.actionID = 3 --for eliminate set 3-->1



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



ALTER TABLE [ExpiryCalendar] ADD 
	CONSTRAINT [PK_ExpiryCalendar] PRIMARY KEY  CLUSTERED 
	(
		[expCalendarID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [ExpiryInCalendar] ADD 
	CONSTRAINT [PK_ExpiryInCalendar] PRIMARY KEY  CLUSTERED 
	(
		[expCalendarID],
		[expiryID]
	)  ON [PRIMARY] 
GO





 CREATE  INDEX [IX_ExpiryInCalendar] ON [ExpiryInCalendar]([expiryID]) ON [PRIMARY]
GO


ALTER TABLE [Filter] ADD 
	CONSTRAINT [PK_Filter] PRIMARY KEY  CLUSTERED 
	(
		[filterID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [FilterContent] ADD 
	CONSTRAINT [PK_FilterContent] PRIMARY KEY  CLUSTERED 
	(
		[filterID],
		[filterFieldID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [FilterField] ADD 
	CONSTRAINT [PK_FilterField] PRIMARY KEY  CLUSTERED 
	(
		[filterFieldID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [GenerationRule] ADD 
	CONSTRAINT [PK_Rule] PRIMARY KEY  CLUSTERED 
	(
		[ruleID]
	)  ON [PRIMARY] 
GO



ALTER TABLE [GenerationRule] ADD 
	CONSTRAINT [DF__Generatio__isDef__1AC9DC03] DEFAULT (0) FOR [isDefaultRule],
	CONSTRAINT [DF_GenerationRule_ruleType] DEFAULT (0) FOR [ruleType]
GO




ALTER TABLE [GenerationRuleGroup] ADD 
	CONSTRAINT [PK_StockGroup] PRIMARY KEY  CLUSTERED 
	(
		[groupID]
	)  ON [PRIMARY] 
GO



ALTER TABLE [GenerationRuleGroup] ADD 
	CONSTRAINT [DF_StockGroup_isActive] DEFAULT (1) FOR [isActive]
GO




ALTER TABLE [HistRangesCache] ADD 
	CONSTRAINT [PK_HistRangesCache] PRIMARY KEY  CLUSTERED 
	(
		[contractID],
		[optionExpDate],
		[strike]
	)  ON [PRIMARY] 
GO






ALTER TABLE [Holiday] ADD 
	CONSTRAINT [PK_Holidays] PRIMARY KEY  CLUSTERED 
	(
		[holidayID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Holiday] ADD 
	CONSTRAINT [CK_Holiday] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [Holiday] ADD 
	CONSTRAINT [DF_Holiday_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_Holiday_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_Holiday ON dbo.Holiday
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Holiday set actionDate=GetDate(),actionID=2
		from Holiday
				join inserted on Holiday.holidayID=inserted.holidayID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update Holiday set actionID=3, actionDate=deleted.actionDate
		from Holiday
				join deleted on Holiday.holidayID=deleted.holidayID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_HolidayName] ON [Holiday]([holidayName]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_Holiday] ON [Holiday]([holidayDate]) ON [PRIMARY]
GO


ALTER TABLE [Index] ADD 
	CONSTRAINT [PK_Indexes] PRIMARY KEY  CLUSTERED 
	(
		[indexID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Index] ADD 
	CONSTRAINT [CK_Index] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [Index] ADD 
	CONSTRAINT [DF_Indexes_isHTB] DEFAULT (0) FOR [isHTB],
	CONSTRAINT [DF_Index_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_Index_actionDate] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [DF__Index__calcOptio__7231DAC4] DEFAULT (0) FOR [calcOptionType],
	CONSTRAINT [DF__Index__calcModel__7325FEFD] DEFAULT (1) FOR [calcModelType]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_Index ON dbo.[Index]
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update [Index] set actionDate=GetDate(),actionID=2
		from [Index]
				join inserted on [Index].IndexID=inserted.IndexID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update [Index] set actionID=3, actionDate=deleted.actionDate
		from [Index]
				join deleted on [Index].IndexID=deleted.IndexID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



ALTER TABLE [IndexCorrelation] ADD 
	CONSTRAINT [PK_IndexCorrelation] PRIMARY KEY  CLUSTERED 
	(
		[indexID1],
		[indexID2]
	)  ON [PRIMARY] 
GO





 CREATE  INDEX [indexID1_ind] ON [IndexCorrelation]([indexID1]) ON [PRIMARY]
GO

 CREATE  INDEX [indexID2_ind] ON [IndexCorrelation]([indexID2]) ON [PRIMARY]
GO


ALTER TABLE [IndexDataHistory] ADD 
	CONSTRAINT [PK_IndexDataHistory] PRIMARY KEY  CLUSTERED 
	(
		[indexID],
		[date],
		[tenorID]
	) WITH  FILLFACTOR = 95  ON [PRIMARY] 
GO





/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_c_34_1] ON [IndexDataHistory] ([indexID]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_c_34_2] ON [IndexDataHistory] ([date]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_c_34_3] ON [IndexDataHistory] ([tenorID]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_34_3] ON [IndexDataHistory] ([tenorID]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_34_1] ON [IndexDataHistory] ([indexID]) ')
GO

 CREATE  INDEX [ix_date] ON [IndexDataHistory]([date]) WITH  FILLFACTOR = 95 ON [PRIMARY]
GO

 CREATE  INDEX [fk_tenorID] ON [IndexDataHistory]([tenorID]) WITH  FILLFACTOR = 95 ON [PRIMARY]
GO


ALTER TABLE [IndexDefinition] ADD 
	CONSTRAINT [PK_IndexDefinition] PRIMARY KEY  NONCLUSTERED 
	(
		[indexDefinitionID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [IndexDefinition] ADD 
	CONSTRAINT [CK_IndexDefinition] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [IndexDefinition] ADD 
	CONSTRAINT [DF_IndexDefinition_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_IndexDefinition_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   Trigger tU_IndexDefinition ON dbo.IndexDefinition
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================

	update IndexDefinition set actionDate=GetDate(),actionID=2
		from IndexDefinition
				join inserted on IndexDefinition.indexDefinitionID=inserted.indexDefinitionID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation

	update IndexDefinition set actionID=3, actionDate=deleted.actionDate
		from IndexDefinition
				join deleted on IndexDefinition.indexDefinitionID=deleted.indexDefinitionID
		where deleted.actionID = 3 --for eliminate set 3-->1



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



ALTER TABLE [IRCurve] ADD 
	CONSTRAINT [PK_CurveTypes] PRIMARY KEY  CLUSTERED 
	(
		[curveID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [IRCurve] ADD 
	CONSTRAINT [CK_IRCurve] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [IRCurve] ADD 
	CONSTRAINT [DF_IRCurve_ruleTypeID] DEFAULT (0) FOR [ruleTypeID],
	CONSTRAINT [DF_IRCurve_curveTypeID] DEFAULT (0) FOR [curveTypeID],
	CONSTRAINT [DF_IRCurve_positionThreshold] DEFAULT (0) FOR [positionThreshold],
	CONSTRAINT [DF_IRCurve_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_IRCurve_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_IRCurve ON dbo.IRCurve
    for Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	update IRCurve set actionDate=GetDate(),actionID=2
		from IRCurve
				join inserted on IRCurve.CurveID=inserted.CurveID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update IRCurve set actionID=3, actionDate=deleted.actionDate
		from IRCurve
				join deleted on IRCurve.CurveID=deleted.CurveID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



ALTER TABLE [IRPoint] ADD 
	CONSTRAINT [PK_ZeroCurves] PRIMARY KEY  CLUSTERED 
	(
		[IRpointID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [IRPoint] ADD 
	CONSTRAINT [CK_IRPoint] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [IRPoint] ADD 
	CONSTRAINT [DF_IRPoint_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_IRPoint_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_IRPoint ON dbo.IRPoint
    for Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	update IRPoint set actionDate=GetDate(),actionID=2
		from IRPoint
				join inserted on IRPoint.IRPointID=inserted.IRPointID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update IRPoint set actionID=3, actionDate=deleted.actionDate
		from IRPoint
				join deleted on IRPoint.IRPointID=deleted.IRPointID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [IX_IRPoint] ON [IRPoint]([num]) ON [PRIMARY]
GO












ALTER TABLE [MLState] ADD 
	CONSTRAINT [PK_MLState] PRIMARY KEY  CLUSTERED 
	(
		[mlStateID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [ModelParameter] ADD 
	CONSTRAINT [PK_ModelParameters] PRIMARY KEY  CLUSTERED 
	(
		[parameterID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [ModelParameter] ADD 
	CONSTRAINT [CHK_ModelParameters] CHECK (((not([param1] is null))) or ((not([param2] is null))) or ((not([param3] is null))))
GO





ALTER TABLE [NextDaySurface] ADD 
	CONSTRAINT [PK_NextDaySurface] PRIMARY KEY  CLUSTERED 
	(
		[nextDaySurfaceID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [Option] ADD 
	CONSTRAINT [PK_Options] PRIMARY KEY  CLUSTERED 
	(
		[optionID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Option] ADD 
	CONSTRAINT [CHK_IsCall] CHECK ([isCall] = 1 or [isCall] = 0)
GO


ALTER TABLE [Option] ADD 
	CONSTRAINT [DF_Option_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_Option_actionDate] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [DF__Option__isM] DEFAULT (0) FOR [isManualyEntered]
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE   Trigger tU_Option ON dbo.[Option]
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	update [Option] set actionDate=GetDate(),actionID=2
		from [Option]
				join inserted on [Option].OptionID=inserted.OptionID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation

	update [Option] set actionID=3, actionDate=deleted.actionDate
		from [Option]
				join deleted on [Option].OptionID=deleted.OptionID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_IsCall] ON [Option]([isCall]) ON [PRIMARY]
GO

 CREATE  INDEX [I_Underlying] ON [Option]([underlyingContractID], [isCall], [strike], [expiryID], [actionID]) ON [PRIMARY]
GO

 CREATE  INDEX [underlyingContractID_ind] ON [Option]([underlyingContractID]) ON [PRIMARY]
GO

 CREATE  UNIQUE  INDEX [optionID_actionID_ind] ON [Option]([optionID], [actionID]) ON [PRIMARY]
GO

 CREATE  INDEX [actionDate_ind] ON [Option]([actionDate]) ON [PRIMARY]
GO

 CREATE  INDEX [underlyingContractID_actionID_ind] ON [Option]([underlyingContractID], [actionDate]) ON [PRIMARY]
GO


ALTER TABLE [OptionDataCache] ADD 
	CONSTRAINT [PK_OptionDataCache] PRIMARY KEY  NONCLUSTERED 
	(
		[optionID]
	)  ON [PRIMARY] 
GO
















ALTER TABLE [RMScenario] ADD 
	CONSTRAINT [PK_RMScenario] PRIMARY KEY  CLUSTERED 
	(
		[RMScenarioID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [RMScenario] ADD 
	CONSTRAINT [CK_RMScenario] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [RMScenario] ADD 
	CONSTRAINT [DF_RMScenario_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_RMScenario_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_RMScenario ON dbo.RMScenario
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	update RMScenario set actionDate=GetDate(),actionID=2
		from RMScenario
				join inserted on RMScenario.RMScenarioID=inserted.RMScenarioID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update RMScenario set actionID=3, actionDate=deleted.actionDate
		from RMScenario
				join deleted on RMScenario.RMScenarioID=deleted.RMScenarioID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



ALTER TABLE [RuleItemType] ADD 
	CONSTRAINT [PK_RuleItemType] PRIMARY KEY  CLUSTERED 
	(
		[ruleItemTypeID]
	)  ON [PRIMARY] 
GO






ALTER TABLE [Stock] ADD 
	CONSTRAINT [PK_Stocks] PRIMARY KEY  CLUSTERED 
	(
		[stockID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Stock] ADD 
	CONSTRAINT [CK_Stock] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [Stock] ADD 
	CONSTRAINT [DF__Stocks__DivFreq__31EC6D26] DEFAULT (0) FOR [divFreq],
	CONSTRAINT [DF_Stocks_isHTB] DEFAULT (0) FOR [isHTB],
	CONSTRAINT [DF_Stock_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_Stock_actionDate] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [DF__Stock__isDivCust__607D3EDD] DEFAULT (0) FOR [isDivCustom],
	CONSTRAINT [DF__Stock__calcOptio__70499252] DEFAULT (1) FOR [calcOptionType],
	CONSTRAINT [DF__Stock__calcModel__713DB68B] DEFAULT (1) FOR [calcModelType]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_Stock ON dbo.Stock
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Stock set actionDate=GetDate(),actionID=2
		from Stock
				join inserted on Stock.StockID=inserted.StockID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update Stock set actionID=3, actionDate=deleted.actionDate
		from Stock
				join deleted on Stock.StockID=deleted.StockID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [primaryExchangeID_ind] ON [Stock]([primaryExchangeID]) ON [PRIMARY]
GO

 CREATE  INDEX [stockID_primaryExchangeID_ind] ON [Stock]([stockID], [primaryExchangeID]) ON [PRIMARY]
GO


ALTER TABLE [StockCorrelation] ADD 
	CONSTRAINT [PK_StockCorrelation] PRIMARY KEY  CLUSTERED 
	(
		[stockID1],
		[stockID2]
	)  ON [PRIMARY] 
GO





 CREATE  INDEX [stockID1_stockID2_ind] ON [StockCorrelation]([stockID1], [stockID2]) ON [PRIMARY]
GO

 CREATE  INDEX [stockID1_ind] ON [StockCorrelation]([stockID1]) ON [PRIMARY]
GO

 CREATE  INDEX [stockID2_ind] ON [StockCorrelation]([stockID2]) ON [PRIMARY]
GO


ALTER TABLE [StockDataHistory] ADD 
	CONSTRAINT [PK_StockDataHistory] PRIMARY KEY  CLUSTERED 
	(
		[stockID],
		[date],
		[tenorID]
	)  ON [PRIMARY] 
GO





/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_46_3] ON [StockDataHistory] ([tenorID]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_46_2] ON [StockDataHistory] ([date]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_46_1] ON [StockDataHistory] ([stockID]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_47_2] ON [StockDataHistory] ([date]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_47_1] ON [StockDataHistory] ([stockID]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_47_2_1] ON [StockDataHistory] ([date], [stockID]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_47_2_1_4] ON [StockDataHistory] ([date], [stockID], [IVMean], [IVCall], [IVPut], [currentHV]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_48_7] ON [StockDataHistory] ([currentHV]) ')
GO

/****** The index created by the following statement is for internal use only. ******/
/****** It is not a real index but exists as statistics only. ******/
if (@@microsoftversion > 0x07000000 )
EXEC ('CREATE STATISTICS [hind_48_3] ON [StockDataHistory] ([tenorID]) ')
GO

 CREATE  INDEX [ix_date] ON [StockDataHistory]([date], [tenorID]) ON [PRIMARY]
GO

 CREATE  INDEX [fk_tenorID] ON [StockDataHistory]([tenorID]) ON [PRIMARY]
GO


ALTER TABLE [Strategy] ADD 
	CONSTRAINT [PK_Strategies] PRIMARY KEY  CLUSTERED 
	(
		[strategyID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Strategy] ADD 
	CONSTRAINT [CK_Strategy] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [Strategy] ADD 
	CONSTRAINT [DF_Strategy_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_Strategy_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_Strategy ON dbo.Strategy
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Strategy set actionDate=GetDate(),actionID=2
		from Strategy
				join inserted on Strategy.StrategyID=inserted.StrategyID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update Strategy set actionID=3, actionDate=deleted.actionDate
		from Strategy
				join deleted on Strategy.StrategyID=deleted.StrategyID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_StrategyName] ON [Strategy]([strategyName]) ON [PRIMARY]
GO


ALTER TABLE [Tenor] ADD 
	CONSTRAINT [PK_VolaCalcPeriod] PRIMARY KEY  NONCLUSTERED 
	(
		[tenorID]
	) WITH  FILLFACTOR = 95  ON [PRIMARY] 
GO






ALTER TABLE [Trade] ADD 
	CONSTRAINT [PK_Trade] PRIMARY KEY  CLUSTERED 
	(
		[seqNum]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Trade] ADD 
	CONSTRAINT [CHK_IsBuy] CHECK ([isBuy] = 1 or [isBuy] = 0),
	CONSTRAINT [CHK_IsClose] CHECK ([isClose] = 1 or [isClose] = 0)
GO


ALTER TABLE [Trade] ADD 
	CONSTRAINT [DF_Trade_actionDate] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [DF__Trades__BrokerCo__3BFFE745] DEFAULT (0) FOR [brokerCommissionID],
	CONSTRAINT [DF__Trades__Clearing__3CF40B7E] DEFAULT (0) FOR [clearingBrokerCommissionID],
	CONSTRAINT [DF_Trade_isPosition] DEFAULT (0) FOR [isPosition],
	CONSTRAINT [DF__Trade__lotSize__6F556E19] DEFAULT (1) FOR [lotSize],
	CONSTRAINT [DF__Trade__isSimulat__741A2336] DEFAULT (0) FOR [isSimulated]
GO



 CREATE  INDEX [I_Trades_Contract] ON [Trade]([contractID]) ON [PRIMARY]
GO

 CREATE  INDEX [I_Trades_Book] ON [Trade]([bookID]) ON [PRIMARY]
GO

 CREATE  INDEX [I_Trades_Broker] ON [Trade]([brokerID]) ON [PRIMARY]
GO

 CREATE  INDEX [I_Trades_ClearingBroker] ON [Trade]([clearingBrokerID]) ON [PRIMARY]
GO

 CREATE  INDEX [I_Trades_Strategy] ON [Trade]([strategyID]) ON [PRIMARY]
GO

 CREATE  INDEX [I_Trades_TradeDate] ON [Trade]([tradeDate]) ON [PRIMARY]
GO

 CREATE  INDEX [traderID_ind] ON [Trade]([traderID]) ON [PRIMARY]
GO

 CREATE  INDEX [tradeID_contractID_ind] ON [Trade]([tradeID], [contractID]) ON [PRIMARY]
GO

 CREATE  INDEX [actionDate_ind] ON [Trade]([actionDate]) ON [PRIMARY]
GO


ALTER TABLE [Trader] ADD 
	CONSTRAINT [PK_Traders] PRIMARY KEY  CLUSTERED 
	(
		[traderID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [Trader] ADD 
	CONSTRAINT [CK_Trader_actionID] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [Trader] ADD 
	CONSTRAINT [DF__Trader__MDate__36FD9D0B] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [DF__Trader__MType__37F1C144] DEFAULT (1) FOR [actionID]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE       TRIGGER tU_Trader ON dbo.Trader
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update Trader set actionDate=GetDate(),actionID=2
		from Trader
				join inserted on Trader.traderID=inserted.traderID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update Trader set actionID=3,actionDate=deleted.actionDate
		from Trader
				join deleted on Trader.TraderID=deleted.TraderID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [I_Acronym] ON [Trader]([acronym]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_Trader] ON [Trader]([clearingTraderCode]) ON [PRIMARY]
GO


ALTER TABLE [TraderContract] ADD 
	CONSTRAINT [PK_TraderContract] PRIMARY KEY  CLUSTERED 
	(
		[traderContractID]
	)  ON [PRIMARY] 
GO



ALTER TABLE [TraderContract] ADD 
	CONSTRAINT [DF_TraderContract_actionDate] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [DF_TraderContract_actionID] DEFAULT (1) FOR [actionID]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE trigger tU_TraderContract ON dbo.TraderContract
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update TraderContract set actionDate=GetDate(),actionID=2
		from TraderContract
				join inserted on TraderContract.traderID=inserted.traderID and TraderContract.contractID=inserted.contractID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update TraderContract set actionID=3,actionDate=deleted.actionDate
		from TraderContract
				join deleted on TraderContract.traderID=deleted.TraderID and TraderContract.contractID=deleted.contractID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



ALTER TABLE [TraderRole] ADD 
	CONSTRAINT [PK_TraderRole] PRIMARY KEY  CLUSTERED 
	(
		[traderRoleID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [TraderRole] ADD 
	CONSTRAINT [CK_TraderRole] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [TraderRole] ADD 
	CONSTRAINT [DF_TraderRole_actionDate] DEFAULT (getdate()) FOR [actionDate],
	CONSTRAINT [DF_TraderRole_actionID] DEFAULT (1) FOR [actionID]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE Trigger tU_TraderRole ON dbo.TraderRole
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update TraderRole set actionDate=GetDate(),actionID=2
		from TraderRole
				join inserted on TraderRole.traderRoleID=inserted.traderRoleID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update TraderRole set actionID=3, actionDate=deleted.actionDate
		from TraderRole
				join deleted on TraderRole.traderRoleID=deleted.traderRoleID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





ALTER TABLE [VarCorrelation] ADD 
	CONSTRAINT [DF_VaRCorrelation_fCorrelation] DEFAULT (0.0) FOR [correlation]
GO



 CREATE  INDEX [IX_VarCorrelation_Main] ON [VarCorrelation]([dataSet], [currency1], [currency2]) ON [PRIMARY]
GO




ALTER TABLE [VarReport] ADD 
	CONSTRAINT [DF_VarReport_fVar] DEFAULT (0.0) FOR [varValue]
GO



 CREATE  INDEX [IX_VarReport_Main] ON [VarReport]([currency], [varCode]) ON [PRIMARY]
GO






 CREATE  INDEX [IX_VarVolatility_Main] ON [VarVolatility]([dataSet], [currency]) ON [PRIMARY]
GO


ALTER TABLE [VolaSurfaceData] ADD 
	CONSTRAINT [PK_VolaManagementData] PRIMARY KEY  NONCLUSTERED 
	(
		[volaSurfaceDataID]
	)  ON [PRIMARY] 
GO


ALTER TABLE [VolaSurfaceData] ADD 
	CONSTRAINT [CK_VolaSurfaceData] CHECK ([actionID] > 0 and [actionID] < 4)
GO


ALTER TABLE [VolaSurfaceData] ADD 
	CONSTRAINT [DF_VolaManagementData_basePoint] DEFAULT (0) FOR [baseUnderlinePrice],
	CONSTRAINT [DF_VolaSurfaceData_smileAccelerator] DEFAULT (0) FOR [smileAccelerator],
	CONSTRAINT [DF_VolaSurfaceData_surfaceName] DEFAULT ('<Default>') FOR [surfaceName],
	CONSTRAINT [DF_VolaSurfaceData_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_VolaSurfaceData_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create Trigger tU_VolaSurfaceData ON dbo.VolaSurfaceData
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	
	update VolaSurfaceData set actionDate=GetDate(),actionID=2
		from VolaSurfaceData
				join inserted on VolaSurfaceData.VolaSurfaceDataID=inserted.VolaSurfaceDataID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation
	update VolaSurfaceData set actionID=3, actionDate=deleted.actionDate
		from VolaSurfaceData
				join deleted on VolaSurfaceData.VolaSurfaceDataID=deleted.VolaSurfaceDataID
		where deleted.actionID = 3 --for eliminate set 3-->1


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


 CREATE  INDEX [volaSurfaceDataID_contractID_ind] ON [VolaSurfaceData]([volaSurfaceDataID], [contractID]) ON [PRIMARY]
GO

 CREATE  INDEX [actionDate_ind] ON [VolaSurfaceData]([actionDate]) ON [PRIMARY]
GO

 CREATE  INDEX [contractID_ind] ON [VolaSurfaceData]([contractID]) ON [PRIMARY]
GO



-- FOREIGN KEYS CREATION PART OF SCRIPT


ALTER TABLE [BookRuleItem] ADD 
	CONSTRAINT [FK_BookRuleItem_Book] FOREIGN KEY 
	(
		[bookID]
	) REFERENCES [Book] (
		[bookID]
	),
	CONSTRAINT [FK_BookRuleItem_RuleItemType] FOREIGN KEY 
	(
		[ruleItemTypeID]
	) REFERENCES [RuleItemType] (
		[ruleItemTypeID]
	)
GO


ALTER TABLE [BookRuleItemMask] ADD 
	CONSTRAINT [FK_RiskItemMask_BookRuleItem] FOREIGN KEY 
	(
		[bookRuleItemID]
	) REFERENCES [BookRuleItem] (
		[bookRuleItemID]
	)
GO




ALTER TABLE [Commission] ADD 
	CONSTRAINT [FK_Commission_Brokers] FOREIGN KEY 
	(
		[brokerID]
	) REFERENCES [Broker] (
		[brokerID]
	)
GO


ALTER TABLE [Contract] ADD 
	CONSTRAINT [FK_Contract_GenerationRule] FOREIGN KEY 
	(
		[ruleID]
	) REFERENCES [GenerationRule] (
		[ruleID]
	),
	CONSTRAINT [FK_Contract_GenerationRuleGroup] FOREIGN KEY 
	(
		[groupID]
	) REFERENCES [GenerationRuleGroup] (
		[groupID]
	)
GO


ALTER TABLE [ContractExchangeLink] ADD 
	CONSTRAINT [FK_ContractExchangeLink_Contract] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	),
	CONSTRAINT [FK_ContractExchangeLink_Exchange] FOREIGN KEY 
	(
		[exchangeID]
	) REFERENCES [Exchange] (
		[exchangeID]
	)
GO



ALTER TABLE [ContractInGroup] ADD 
	CONSTRAINT [FK_ContractsInGroups_Contract] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	),
	CONSTRAINT [FK_ContractsInGroups_Group] FOREIGN KEY 
	(
		[groupID]
	) REFERENCES [ContractGroup] (
		[groupID]
	)
GO


ALTER TABLE [ContractMarketData] ADD 
	CONSTRAINT [FK_ContractMarketData_Contract] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	)
GO


ALTER TABLE [ContractPrice] ADD 
	CONSTRAINT [FK_ContractPrice_Contracts] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	),
	CONSTRAINT [FK_ContractPrice_Exchanges] FOREIGN KEY 
	(
		[exchangeID]
	) REFERENCES [Exchange] (
		[exchangeID]
	)
GO


ALTER TABLE [CustomStrikeSkewPoint] ADD 
	CONSTRAINT [FK_CustomStrikeSkewPoint_Expiry] FOREIGN KEY 
	(
		[expiryID]
	) REFERENCES [Expiry] (
		[expiryID]
	),
	CONSTRAINT [FK_CustomStrikeSkewPoint_VolaManagementData] FOREIGN KEY 
	(
		[volaSurfaceDataID]
	) REFERENCES [VolaSurfaceData] (
		[volaSurfaceDataID]
	)
GO




ALTER TABLE [ExchangeHoliday] ADD 
	CONSTRAINT [FK_ExchangeHoliday_Exchanges] FOREIGN KEY 
	(
		[exchangeID]
	) REFERENCES [Exchange] (
		[exchangeID]
	),
	CONSTRAINT [FK_ExchangeHoliday_Holidays] FOREIGN KEY 
	(
		[holidayID]
	) REFERENCES [Holiday] (
		[holidayID]
	)
GO




ALTER TABLE [ExpiryInCalendar] ADD 
	CONSTRAINT [FK_ExpiryInCalendar_Expiry] FOREIGN KEY 
	(
		[expiryID]
	) REFERENCES [Expiry] (
		[expiryID]
	),
	CONSTRAINT [FK_ExpiryInCalendar_ExpiryCalendar] FOREIGN KEY 
	(
		[expCalendarID]
	) REFERENCES [ExpiryCalendar] (
		[expCalendarID]
	)
GO



ALTER TABLE [FilterContent] ADD 
	CONSTRAINT [FK_FilterContent_Filter] FOREIGN KEY 
	(
		[filterID]
	) REFERENCES [Filter] (
		[filterID]
	),
	CONSTRAINT [FK_FilterContent_FilterField] FOREIGN KEY 
	(
		[filterFieldID]
	) REFERENCES [FilterField] (
		[filterFieldID]
	)
GO




ALTER TABLE [GenerationRuleGroup] ADD 
	CONSTRAINT [FK_StockGroup_Rule] FOREIGN KEY 
	(
		[ruleID]
	) REFERENCES [GenerationRule] (
		[ruleID]
	)
GO


ALTER TABLE [HistRangesCache] ADD 
	CONSTRAINT [FK_HistRangesCache_Contract] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	)
GO



ALTER TABLE [Index] ADD 
	CONSTRAINT [FK_Index_Contracts] FOREIGN KEY 
	(
		[indexID]
	) REFERENCES [Contract] (
		[contractID]
	)
GO


ALTER TABLE [IndexCorrelation] ADD 
	CONSTRAINT [FK_IndexCorrelation_Index] FOREIGN KEY 
	(
		[indexID1]
	) REFERENCES [Index] (
		[indexID]
	),
	CONSTRAINT [FK_IndexCorrelation_Index1] FOREIGN KEY 
	(
		[indexID2]
	) REFERENCES [Index] (
		[indexID]
	)
GO


ALTER TABLE [IndexDataHistory] ADD 
	CONSTRAINT [FK_IndexDataHistory_Index1] FOREIGN KEY 
	(
		[indexID]
	) REFERENCES [Index] (
		[indexID]
	),
	CONSTRAINT [FK_IndexDataHistory_Tenor] FOREIGN KEY 
	(
		[tenorID]
	) REFERENCES [Tenor] (
		[tenorID]
	)
GO


ALTER TABLE [IndexDefinition] ADD 
	CONSTRAINT [FK_IndexDefinition_Indexes] FOREIGN KEY 
	(
		[indexID]
	) REFERENCES [Index] (
		[indexID]
	),
	CONSTRAINT [FK_IndexDefinition_Stocks] FOREIGN KEY 
	(
		[stockID]
	) REFERENCES [Stock] (
		[stockID]
	)
GO



ALTER TABLE [IRPoint] ADD 
	CONSTRAINT [FK_IRPoint_IRCurve] FOREIGN KEY 
	(
		[curveID]
	) REFERENCES [IRCurve] (
		[curveID]
	)
GO





ALTER TABLE [ModelParameter] ADD 
	CONSTRAINT [FK_Model_Contracts] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	)
GO


ALTER TABLE [NextDaySurface] ADD 
	CONSTRAINT [FK_NextDaySurface_Contract] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	),
	CONSTRAINT [FK_NextDaySurface_VolaSurfaceData] FOREIGN KEY 
	(
		[volaSurfaceDataID]
	) REFERENCES [VolaSurfaceData] (
		[volaSurfaceDataID]
	)
GO


ALTER TABLE [Option] ADD 
	CONSTRAINT [FK_Option_Contract] FOREIGN KEY 
	(
		[optionID]
	) REFERENCES [Contract] (
		[contractID]
	),
	CONSTRAINT [FK_Option_Contract1] FOREIGN KEY 
	(
		[underlyingContractID]
	) REFERENCES [Contract] (
		[contractID]
	),
	CONSTRAINT [FK_Option_Expiry] FOREIGN KEY 
	(
		[expiryID]
	) REFERENCES [Expiry] (
		[expiryID]
	)
GO


ALTER TABLE [OptionDataCache] ADD 
	CONSTRAINT [FK_OptionDataCache_Option] FOREIGN KEY 
	(
		[optionID]
	) REFERENCES [Option] (
		[optionID]
	)
GO






ALTER TABLE [Stock] ADD 
	CONSTRAINT [FK_Stock_Contracts] FOREIGN KEY 
	(
		[stockID]
	) REFERENCES [Contract] (
		[contractID]
	),
	CONSTRAINT [FK_Stock_Indexes] FOREIGN KEY 
	(
		[baseIndexID]
	) REFERENCES [Index] (
		[indexID]
	),
	CONSTRAINT [FK_Stocks_Exchanges] FOREIGN KEY 
	(
		[primaryExchangeID]
	) REFERENCES [Exchange] (
		[exchangeID]
	)
GO


ALTER TABLE [StockCorrelation] ADD 
	CONSTRAINT [FK_StockCorrelation_Stock] FOREIGN KEY 
	(
		[stockID1]
	) REFERENCES [Stock] (
		[stockID]
	),
	CONSTRAINT [FK_StockCorrelation_Stock1] FOREIGN KEY 
	(
		[stockID2]
	) REFERENCES [Stock] (
		[stockID]
	)
GO


ALTER TABLE [StockDataHistory] ADD 
	CONSTRAINT [FK_StockDataHistory_Stock] FOREIGN KEY 
	(
		[stockID]
	) REFERENCES [Stock] (
		[stockID]
	),
	CONSTRAINT [FK_StockDataHistory_Tenor] FOREIGN KEY 
	(
		[tenorID]
	) REFERENCES [Tenor] (
		[tenorID]
	)
GO




ALTER TABLE [Trade] ADD 
	CONSTRAINT [FK_Trade_Books] FOREIGN KEY 
	(
		[bookID]
	) REFERENCES [Book] (
		[bookID]
	),
	CONSTRAINT [FK_Trade_Brokers] FOREIGN KEY 
	(
		[brokerID]
	) REFERENCES [Broker] (
		[brokerID]
	),
	CONSTRAINT [FK_Trade_ClearingBrokers] FOREIGN KEY 
	(
		[clearingBrokerID]
	) REFERENCES [Broker] (
		[brokerID]
	),
	CONSTRAINT [FK_Trade_Commission] FOREIGN KEY 
	(
		[brokerCommissionID]
	) REFERENCES [Commission] (
		[commissionID]
	),
	CONSTRAINT [FK_Trade_Commission1] FOREIGN KEY 
	(
		[clearingBrokerCommissionID]
	) REFERENCES [Commission] (
		[commissionID]
	),
	CONSTRAINT [FK_Trade_Contracts] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	),
	CONSTRAINT [FK_Trade_Strategies] FOREIGN KEY 
	(
		[strategyID]
	) REFERENCES [Strategy] (
		[strategyID]
	),
	CONSTRAINT [FK_Trade_Traders] FOREIGN KEY 
	(
		[traderID]
	) REFERENCES [Trader] (
		[traderID]
	)
GO


ALTER TABLE [Trader] ADD 
	CONSTRAINT [FK_Trader_Trader] FOREIGN KEY 
	(
		[traderRefID]
	) REFERENCES [Trader] (
		[traderID]
	),
	CONSTRAINT [FK_Trader_TraderRole] FOREIGN KEY 
	(
		[traderRoleID]
	) REFERENCES [TraderRole] (
		[traderRoleID]
	)
GO


ALTER TABLE [TraderContract] ADD 
	CONSTRAINT [FK_TraderContract_Contract] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	),
	CONSTRAINT [FK_TraderContract_Trader] FOREIGN KEY 
	(
		[traderID]
	) REFERENCES [Trader] (
		[traderID]
	)
GO






ALTER TABLE [VolaSurfaceData] ADD 
	CONSTRAINT [FK_VolaSurfaceData_Contract] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	)
GO



