/*
	DB version: 3.37.027
*/
set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexDefinition]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[IndexDefinition]
GO

CREATE TABLE [IndexDefinition] (
	[indexDefinitionID] [int] IDENTITY (1, 1) NOT NULL ,
	[indexID] [int] NOT NULL ,
	[stockID] [int] NOT NULL ,
	[weight] [float] NULL ,
	[beta] [float] NULL ,
	[correlationPrice] [float] NULL ,
	[correlationVola] [float] NULL ,
	[actionID] [tinyint] NOT NULL CONSTRAINT [DF_IndexDefinition_actionID] DEFAULT (1),
	[actionDate] [datetime] NOT NULL CONSTRAINT [DF_IndexDefinition_actionDate] DEFAULT (getdate()),
	CONSTRAINT [PK_IndexDefinition] PRIMARY KEY  NONCLUSTERED 
	(
		[indexDefinitionID]
	)  ON [PRIMARY] ,
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
	),
	CONSTRAINT [CK_IndexDefinition] CHECK ([actionID] > 0 and [actionID] < 4)
) ON [PRIMARY]
GO






--===================================================================================
--change DB version
--===================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.37.027')
	begin
		update DataInfo set keyValue = '3.37.027' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.37.027'
	end
