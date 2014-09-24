
if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[RMScenario]') and (name = 'VolaSpecificShiftType'))
begin
	ALTER TABLE [RMScenario] 
		ADD [VolaSpecificShiftType] [int] NULL
end
GO

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[RMScenario]') and (name = 'SpotShiftType'))
begin
	ALTER TABLE [RMScenario] 
		ADD [SpotShiftType] [int] NULL
end
GO

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[RMScenario]') and (name = 'SpotSpecificShiftType'))
begin
	ALTER TABLE [RMScenario] 
		ADD [SpotSpecificShiftType] [int] NULL
end
GO

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[RMScenario]') and (name = 'CalcModel'))
begin
	ALTER TABLE [RMScenario] 
		ADD [CalcModel] [int] NULL
end
GO

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[RMScenario]') and (name = 'CorrIndex'))
begin
	ALTER TABLE [RMScenario] 
		ADD [CorrIndex] [int] NULL
end
GO

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[RMScenario]') and (name = 'SpecificShiftParams'))
begin
	ALTER TABLE [RMScenario] 
		ADD [SpecificShiftParams] [varchar] (512) COLLATE Latin1_General_CI_AS NULL
end
GO
