net stop "Egar Router"
net stop "Egar Transport"
EgTpSrv.exe /unregserver
EgRtSrv.Exe /unregserver
regsvr32 EgTpPrx.dll /u /s

pause

regsvr32 EgTpPrx.dll /s
EgTpSrv.exe /regserver
EgRtSrv.exe /regserver
EgRtSrv.exe /Service

net start "Egar Router"
net start "Egar Transport"

pause
