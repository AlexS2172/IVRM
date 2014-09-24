----------------------------------------------------------------------------------------------------
-- usp_AspUserStatus_Get
----------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspUserStatus_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspUserStatus_Get]
GO

CREATE PROC dbo.usp_AspUserStatus_Get
	@vcUserName varchar(63),
	@iStatus int = null out
as
	set nocount on

	exec TESTSERVER.IV.dbo.sp_ETS_User_Status_get @vcUserName, 500, @iStatus out

	return @@error
GO

