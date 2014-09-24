IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = 'RegUserAction'
	   AND 	  type = 'P')
    DROP PROCEDURE RegUserAction
GO

/*
ѕроцедура создает запись о ¬ходе\¬ыходе пользователей ETS.
@i_idtTime 		- врем€ входа\выхода;
@i_bLogOnOut 	- тип действи€ пользовател€, 1 - LogOn, 0 - LogOut;
@i_DBLogin		- Login пользовател€;	
@i_DBName 		- название базы;
@i_ServerName  - название сервера;
@i_ServerPath  - путь к серверу;
@i_PersonName	- им€ пользовател€;
@i_PersonLName - фамили€ пользовател€;
@i_CompanyName - название компании, в которой работает пользователь
*/

CREATE PROCEDURE dbo.RegUserAction
  (@i_dtTime 		datetime,
   @i_bLogOnOut 	smallint,
	@i_DBLogin 		varchar(50),		
	@i_ServerName	varchar(50),
	@i_CompanyName varchar(50))

AS 

BEGIN 
	Declare 	@iUserId 	Int

	Select @iUserId = [id] From Ets_User Where Login = @i_DBLogin 	

	Begin transaction	
	
	If @iUserId is null
		-- ≈сли такой пользователь не заведен в базе
		BEGIN
			Execute CreateUser @i_DBLogin, @i_CompanyName, @i_ServerName, @iUserId OUTPUT 
			If @iUserId = -1
				GOTO DEBUG
		END

	-- «анесение данных в таблицу USER_ACTIVITY
	insert into activity (act_time, LogOnOut, r_user_id)
				Values (@i_dtTime,@i_bLogOnOut,@iUserId)	   

	If @@Error <> 0 or @@RowCount <> 1
		GOTO DEBUG
			
	commit tran
	
RETURN(0)
	
DEBUG:
	RollBack
	RETURN(-1)
END

GO

--------------------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = 'CreateUser'
	   AND 	  type = 'P')
    DROP PROCEDURE CreateUser
GO

/*
ѕроцедура создает нового пользовател€ ETS
@i_DBLogin		- Login пользовател€;	
@i_CompanyName - название компании, в которой работает пользователь
@i_ServerName  - название сервера;
*/

CREATE PROCEDURE dbo.CreateUser	@i_DBLogin 		varchar(50),		
											@i_CompanyName varchar(50),
											@i_ServerName		varchar(50),
											@o_iUserId 		int	OUTPUT 
AS 

BEGIN

	-- «анесение нового пользовател€
	insert into ets_user (Login, CompanyName, ServerName)
				Values (@i_DBLogin, @i_CompanyName, @i_ServerName)	   

	If @@Error <> 0
		Goto Debug
	Else 
		set @o_iUserId = scope_identity()

	RETURN(0)
	
DEBUG:
	SET @o_iUserId = -1
	RETURN(-1)
END

Go

