EXEC sp_addlinkedserver @server='TESTSERVER'
GO

EXEC sp_addlinkedsrvlogin 'TESTSERVER', false, NULL, 'iv_user', 'dbrx*p6f4h'
GO

EXEC sp_serveroption 'TESTSERVER', 'rpc', 'on'
GO

EXEC sp_serveroption 'TESTSERVER', 'rpc out', 'on'
GO