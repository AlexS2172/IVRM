CREATE DATABASE [EgarOne.BackOffice]  ON (NAME = N'EgarOne.BackOffice_Data', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL\data\EgarOne.BackOffice_Data.MDF' , SIZE = 3, FILEGROWTH = 10%) LOG ON (NAME = N'EgarOne.BackOffice_Log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL\data\EgarOne.BackOffice_Log.LDF' , SIZE = 1, FILEGROWTH = 10%)
 COLLATE Cyrillic_General_CI_AS
GO

exec sp_dboption N'EgarOne.BackOffice', N'autoclose', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'bulkcopy', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'trunc. log', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'torn page detection', N'true'
GO

exec sp_dboption N'EgarOne.BackOffice', N'read only', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'dbo use', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'single', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'autoshrink', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'ANSI null default', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'recursive triggers', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'ANSI nulls', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'concat null yields null', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'cursor close on commit', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'default to local cursor', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'quoted identifier', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'ANSI warnings', N'false'
GO

exec sp_dboption N'EgarOne.BackOffice', N'auto create statistics', N'true'
GO

exec sp_dboption N'EgarOne.BackOffice', N'auto update statistics', N'true'
GO

if( ( (@@microsoftversion / power(2, 24) = 8) and (@@microsoftversion & 0xffff >= 724) ) or ( (@@microsoftversion / power(2, 24) = 7) and (@@microsoftversion & 0xffff >= 1082) ) )
	exec sp_dboption N'EgarOne.BackOffice', N'db chaining', N'false'
GO
