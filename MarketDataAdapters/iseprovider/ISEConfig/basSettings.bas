Attribute VB_Name = "basSettings"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Const APP_REG_KEY = "SOFTWARE\Egar\Common\Connectivity\ISE Connector\"

Public Enum enStatus
    enStatus_New = 0
    enStatus_Edit = 1
    enStatus_Del = 2
End Enum

'//////////////////////////////////////////////////////////////////////////
'Bins
'Bins\1]
'Bins\1\Account 1]
'"User"="EG15M-8001"
'"Password"="EGA"
'
'Gateways
'Gateways\1
'"Addr"="209.140.121.66"
'"Port"="20024"
'

'//////////////////////////////////////////////////////////////////////////
'
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector]
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Bins]
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Bins\1]
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Bins\1\Account 1]
'"User"="EG15M-8001"
'"Password"="EGA"
'
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Bins\1\Account 2]
'"User"="EG15M-8002"
'"Password"="EGA"
'
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Bins\1\Account 3]
'"User"="EG15M-8003"
'"Password"="EGA"
'
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Gateways]
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Gateways\1]
'"Addr"="209.140.121.66"
'"Port"="20024"
'
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Gateways\2]
'"Addr"="209.140.121.66"
'"Port"="20025"
'
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Gateways\3]
'"Addr"="209.140.121.67"
'"Port"="20024"
'
'[HKEY_LOCAL_MACHINE\SOFTWARE\Egar\ETS\ISE Connector\Gateways\4]
'"Addr"="209.140.121.67"
'"Port"="20025"
'
'//////////////////////////////////////////////////////////////////////////

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSettingString( _
    ByVal SettingsGroup As String, _
    ByVal SettingsName As String, _
    Optional ByVal DefaultValue As String = "" _
) As String
    Dim Value As String
    If GetStrKeyValue(HKEY_LOCAL_MACHINE, APP_REG_KEY & SettingsGroup, SettingsName, Value) Then
        GetSettingString = Value
    Else
        GetSettingString = DefaultValue
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SetSettingString( _
    ByVal SettingsGroup As String, _
    ByVal SettingsName As String, _
    ByVal Value As String _
) As Boolean
    SetStrKeyValue HKEY_LOCAL_MACHINE, APP_REG_KEY & SettingsGroup, SettingsName, Value
    SetSettingString = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DelSettingsKey( _
    ByVal SettingsGroup As String _
) As Boolean
    DeleteKey HKEY_LOCAL_MACHINE, APP_REG_KEY & SettingsGroup
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function EnumSettingsKey( _
    ByVal SettingsGroup As String _
) As Collection
    Set EnumSettingsKey = GetSubkeys(HKEY_LOCAL_MACHINE, APP_REG_KEY & SettingsGroup)
End Function

