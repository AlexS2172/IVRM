
	iF EXISTS(SELECT name 
		FROM 	 sysobjects 
		WHERE  name = 'ACTIVITY'
		AND 	 type = 'U')
	DROP TABLE ACTIVITY
go
	iF EXISTS(SELECT name 
		FROM 	 sysobjects 
		WHERE  name = 'ETS_USER'
		AND 	 type = 'U')
	DROP TABLE ETS_USER
go	
	Create Table
		ETS_USER (
			Id INT IDENTITY (1,1) PRIMARY KEY,
			Login 		Varchar(50)	NOT NULL,
			CompanyName	Varchar(50) NOT NULL,
			ServerName	Varchar(50) NOT NULL);
go
	Create Table
		ACTIVITY (
			Id INT IDENTITY (1,1) PRIMARY KEY,
			act_time DATETIME			NOT NULL,
			LogOnOut	SMALLINT			NOT NULL,  -- 1  - LogOn, 0 - LogOut
			r_user_id	INT 			NOT NULL FOREIGN KEY REFERENCES ETS_USER (Id) ON UPDATE CASCADE);


go




		
					
		