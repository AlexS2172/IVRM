/*
	this script reduces the server loading, 
	when you work with "Load IV Bands" in "End Of Day" procedure.

	DB version: 3.37.029
*/
set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaBandsEOD_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaBandsEOD_Complete_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaBandsEOD_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaBandsEOD_Init_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaBandsEOD_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaBandsEOD_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    proc dbo.usp_VolaBandsEOD_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	Begin tran 
		if object_id('tempdb..##VolaBandsEOD') is null
		   begin
		       raiserror ('Cannot complete EOD. Cannot find save data of IV bands.', 16, 1)
		       return (-1)
		   end

		delete VolaBands
			where contractID in 
				(select distinct contractID from ContractView CV 
					join ##VolaBandsEOD EOD on CV.symbol = EOD.Symbol)
 		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
 	
		insert into VolaBands (contractID,ExpPeriod,strikeMoneyness,IVBandLow,IVBandHi)
			select contractID,ExpPeriod,StrikeMoneyness,IVBandLow,IVBandHi
				from ##VolaBandsEOD EOD
					join ContractView CV on CV.symbol = EOD.Symbol

 		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit tran

    set @error = 0

finish:
	if object_id('tempdb..##VolaBandsEOD') is not null
		drop table ##VolaBandsEOD

Return(@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE   proc dbo.usp_VolaBandsEOD_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if object_id('tempdb..##VolaBandsEOD') is not null
		begin
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			return (-1) 
		end

	create table ##VolaBandsEOD(
		volaBandID int identity(1,1), 
		Symbol varchar (20) not null,
		ExpPeriod int not null,
		StrikeMoneyness float not null,
		IVBandLow float not null,
		IVBandHi float not null) 

	ALTER TABLE ##VolaBandsEOD WITH NOCHECK ADD CONSTRAINT PK_VolaBand 
		PRIMARY KEY CLUSTERED (volaBandID)

	CREATE INDEX symbol_ind ON ##VolaBandsEOD(Symbol) ON [PRIMARY]

	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE   proc dbo.usp_VolaBandsEOD_Save
	@vcSymbol varchar(20),
	@iExpPeriod int,
	@fStrikeMoneyness float,
	@fIVBandLow float,
	@fIVBandHi float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1
--	if (select object_id('tempdb..##VolaBandsEOD')) is not null
--		begin
	insert into ##VolaBandsEOD (Symbol,ExpPeriod,StrikeMoneyness,IVBandLow,IVBandHi)
		values (@vcSymbol, @iExpPeriod, @fStrikeMoneyness, @fIVBandLow,@fIVBandHi)


	if (select count(*) from ##VolaBandsEOD(nolock)) > 1999
		begin
			delete VolaBands
				where contractID in 
					(select distinct contractID from ContractView CV 
						join ##VolaBandsEOD EOD on CV.symbol = EOD.Symbol)
	 		if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end
	 	
			insert into VolaBands (contractID,ExpPeriod,strikeMoneyness,IVBandLow,IVBandHi)
				select contractID,ExpPeriod,StrikeMoneyness,IVBandLow,IVBandHi
					from ##VolaBandsEOD EOD
						join ContractView CV on CV.symbol = EOD.Symbol
	 		if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end

			truncate table  ##VolaBandsEOD
	 		if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end
		end
--		end
finish:
    set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--==================================================================================
--change DB version
--===================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.37.029')
	begin
		update DataInfo set keyValue = '3.37.029' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.37.029'
	end
