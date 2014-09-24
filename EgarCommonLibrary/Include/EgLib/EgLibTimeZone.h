#pragma once
//**************************************************************************************************************//
// logic guardians
//**************************************************************************************************************//
#ifndef __EGLIBREG_H__
#error 'EgLibTimeZone.h requires EgLibReg.h to be included first'
#endif


#ifndef __EGLIBMISC_H__
#error 'EgLibTimeZone.h requires EgLibMisc.h to be included first'
#endif


//**************************************************************************************************************//
// header identifier
//**************************************************************************************************************//
#define __EGLIBTZ_H__

//**************************************************************************************************************//
// namespace EgLib
//**************************************************************************************************************//
namespace EgLib 
{

	class CTimeZone
	{
	public:
		CTimeZone():m_bTimeZoneInitialized(false)
		{
		};

	
		vt_date LocalTimeToDestinationTime(vt_date& dtLocal)
		{
			vt_date dtRet = dtLocal;
			if(InitializeTimeZone())
			{
				SYSTEMTIME tmSystemTime;
				SYSTEMTIME LocalTime;
				SYSTEMTIME DestTime;

				dtRet.GetSystemTime(LocalTime);

				if(ConvertTzSpecificLocalTimeToSystemTime(m_tzCurrentTimeZone, LocalTime, &tmSystemTime))
					if(SystemTimeToTzSpecificLocalTime(&m_TimeZoneInfo, &tmSystemTime, &DestTime))
						dtRet = vt_date(DestTime);
			}
			return dtRet;
		}

		vt_date DestinationTimeToLocalTime(vt_date& dtDestTime)
		{
			vt_date dtRet = dtDestTime;
			DWORD dw = 0;
			if(InitializeTimeZone())
			{
				SYSTEMTIME tmSystemTime;
				SYSTEMTIME LocalTime;
				SYSTEMTIME DestTime;
				dtDestTime.GetSystemTime(DestTime);

				if(ConvertTzSpecificLocalTimeToSystemTime(m_TimeZoneInfo, DestTime, &tmSystemTime))
				{
					if(SystemTimeToTzSpecificLocalTime(&m_tzCurrentTimeZone, &tmSystemTime, &LocalTime))
						dtRet = vt_date(LocalTime);
				}
			}
			return dtRet;
		}

		bool LocalTimeToDestinationTime(SYSTEMTIME& LocalTime, SYSTEMTIME& DestTime)
		{
			bool bRet = false;
			if(InitializeTimeZone())
			{
				SYSTEMTIME tmSystemTime;
				TIME_ZONE_INFORMATION tzCurrentTimeZone;
				::GetTimeZoneInformation(&tzCurrentTimeZone);

				if(ConvertTzSpecificLocalTimeToSystemTime(tzCurrentTimeZone, LocalTime, &tmSystemTime))
					if(SystemTimeToTzSpecificLocalTime(&m_TimeZoneInfo, &tmSystemTime, &DestTime))
					{
						DestTime.wMilliseconds = LocalTime.wMilliseconds;
						bRet = true;
					}
			}
			return bRet;
		}

		bool DestinationTimeToLocalTime(SYSTEMTIME& DestTime, SYSTEMTIME& LocalTime)
		{
			bool bRet = false;
			DWORD dw = 0;
			if(InitializeTimeZone())
			{
				SYSTEMTIME tmSystemTime;
				if(ConvertTzSpecificLocalTimeToSystemTime(m_TimeZoneInfo, DestTime, &tmSystemTime))
				{
					TIME_ZONE_INFORMATION tzCurrentTimeZone;
					::GetTimeZoneInformation(&tzCurrentTimeZone);
					if(SystemTimeToTzSpecificLocalTime(&tzCurrentTimeZone, &tmSystemTime, &LocalTime))
					{
						LocalTime.wMilliseconds = DestTime.wMilliseconds;
						bRet = true;
					}
				}
			}
			return bRet;
		}

	public:
		TIME_ZONE_INFORMATION m_TimeZoneInfo;
		bool                  m_bTimeZoneInitialized;
		TIME_ZONE_INFORMATION m_tzCurrentTimeZone;

		virtual bool InitializeTimeZone() = 0;
	protected:
		bool	Initialize(std::string strTimeZoneName)
		{
			bool bRet = false;
			if(m_bTimeZoneInitialized)
				bRet = true;
			else
			{
				ZeroMemory(&m_tzCurrentTimeZone, sizeof(m_tzCurrentTimeZone));
				ZeroMemory(&m_TimeZoneInfo, sizeof(m_TimeZoneInfo));
				::GetTimeZoneInformation(&m_tzCurrentTimeZone);
			
				EgLib::CEgRegKey reg;
				OSVERSIONINFO osV;
				osV.dwOSVersionInfoSize = sizeof(osV);
				GetVersionEx(&osV);
				std::string sTZ;
				if(osV.dwPlatformId == VER_PLATFORM_WIN32_NT)
					sTZ = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\");
				else
					sTZ = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Time Zones\\");

				sTZ += strTimeZoneName; //Pacific Standard Time;
				reg.Open(HKEY_LOCAL_MACHINE,sTZ.c_str(),KEY_READ);

				if(reg!=NULL)
				{
					struct REGTIMEZONEINFORMATION {
						long       Bias;
						long       StandardBias;
						long	   DaylightBias;
						SYSTEMTIME StandardDate;
						SYSTEMTIME DaylightDate;
					} tzI;
					ULONG ulBufferSize = sizeof(tzI);

					if(RegQueryValueEx(reg, _T("TZI"),NULL,NULL,reinterpret_cast<LPBYTE>(&tzI), &ulBufferSize)== ERROR_SUCCESS)
					{
						m_TimeZoneInfo.Bias = tzI.Bias;
						m_TimeZoneInfo.StandardBias = tzI.StandardBias;
						m_TimeZoneInfo.DaylightBias = tzI.DaylightBias;
						m_TimeZoneInfo.StandardDate = tzI.StandardDate;
						m_TimeZoneInfo.DaylightDate = tzI.DaylightDate;
						m_bTimeZoneInitialized = true;
						bRet = true;

						_bstr_t bsStdName;
						_bstr_t bsDLName;
						reg.QueryValue( bsStdName, _T("Std"));
						reg.QueryValue( bsDLName, _T("Dlt"));
#if (_MSC_VER >= 1400)
						wcsncpy_s(m_TimeZoneInfo.DaylightName, bsDLName, 32);
						wcsncpy_s(m_TimeZoneInfo.StandardName, bsStdName, 32);
#else
						wcsncpy(m_TimeZoneInfo.DaylightName, bsDLName, 32);
						wcsncpy(m_TimeZoneInfo.StandardName, bsStdName, 32);
#endif
					}

				}
			}
			return bRet;
		}

	protected:

		static BOOL ConvertTzSpecificLocalTimeToSystemTime(	
			const TIME_ZONE_INFORMATION &TimeZoneInformation,
			const SYSTEMTIME &LocalTime,
			LPSYSTEMTIME lpUniversalTime)
		{
			if(lpUniversalTime == NULL)
			{
				ATLASSERT(lpUniversalTime);
				return FALSE;
			}
			try
			{

				TIME_ZONE_INFORMATION tz;
				memcpy(&tz,&TimeZoneInformation, sizeof(tz));

				EgLib::vt_date_span spBias(0, 0, TimeZoneInformation.Bias, 0);

				EgLib::vt_date_span spStandardBias(0, 0,TimeZoneInformation.Bias + TimeZoneInformation.StandardBias, 0);
				EgLib::vt_date_span spDaylightBias(0, 0, TimeZoneInformation.Bias +	TimeZoneInformation.DaylightBias, 0);

				EgLib::vt_date local(LocalTime);
				EgLib::vt_date utc, utcTemp1, utcTemp2, localTemp1, localTemp2;
				
				SYSTEMTIME stTemp1 = {0}, stTemp2 = {0};
				SYSTEMTIME ltTemp1 = {0}, ltTemp2 = {0};

				// We need to reverse engineer what the time would be, 	by using the SDK: SystemTimeToTzSpecificLocalTime() function
				// From TZI doc:  UTC = local time + bias 
				// NOTE: use (compare) Daylight first, so when hour repeats, we find first occurrence
					utcTemp1 = local; 
					utcTemp1 += spDaylightBias; 
					utcTemp2 = local; 
					utcTemp2 += spStandardBias;
				// Get the raw SYSTEMTIME values

					utcTemp1.GetSystemTime(stTemp1);
					utcTemp2.GetSystemTime(stTemp2);
				// Convert both to LocalTime
					SystemTimeToTzSpecificLocalTime(&tz, &stTemp1, &ltTemp1);
					SystemTimeToTzSpecificLocalTime(&tz, &stTemp2, &ltTemp2);
					localTemp1 = EgLib::vt_date(ltTemp1);
					localTemp2 = EgLib::vt_date(ltTemp2);
				// Find the Call that gave us back the original LocalTime, and that is the correct SystemTime
					if(local == localTemp1)
						utcTemp1.GetSystemTime(*lpUniversalTime);
					else if(local == localTemp2)
						utcTemp2.GetSystemTime(*lpUniversalTime);
					else if(localTemp1 < local && local < localTemp2)
					{
						// 1st Special case for hour that we skip (nothern 	hemisphere)
							// i.e. 2:30am CST on day when we skip ahead from 2:00am to 3:00am
							// we use the earliest time of the two, just like WinXP function TzSpecificLocalTimeToSystemTime() does
							utcTemp1.GetSystemTime(*lpUniversalTime);
					}
					else if(localTemp1 > local && local > localTemp2)
					{
						// 2nd Special case for hour that we skip (southern hemisphere)
							utcTemp2.GetSystemTime(*lpUniversalTime);
					}
					else
					{
						// Should NEVER get in here
						return FALSE;						
					}
					lpUniversalTime->wMilliseconds = LocalTime.wMilliseconds;
			}catch(...)
			{
				ATLASSERT(0);
				return FALSE;
			}
			return TRUE;
		}
	};

	class CEasternTimeZone: public CTimeZone
	{
	public:
		virtual bool InitializeTimeZone()
		{
			return Initialize(_T("Eastern Standard Time"));
		}
	};

	//------------------------------------------------------------------------------------------
	class CGMTTimeZone
	{
	public:
		CGMTTimeZone()
			:m_nTimeZoneBias(0)
		{
			Initialize();
		}
	private:
		long m_nTimeZoneBias;

		void Initialize()
		{
			TIME_ZONE_INFORMATION tzi;
			ZeroMemory(&tzi, sizeof(tzi));
			const DWORD dwResult = ::GetTimeZoneInformation(&tzi);

			if(TIME_ZONE_ID_INVALID!= dwResult)
			{
				m_nTimeZoneBias = tzi.Bias;
				if(TIME_ZONE_ID_DAYLIGHT == dwResult)
					m_nTimeZoneBias +=  tzi.DaylightBias;  //+ tzi.DaylightDate.wMinute;

			}
		}
	public:
		vt_date LocalTimeToGMT(vt_date dtLocal)
		{
			vt_date vtRet = dtLocal;
			vtRet += vt_date_span(0L,0L,m_nTimeZoneBias);
			return vtRet;

		}
		vt_date GMTTimeZoneToLocal(vt_date dtGMT)
		{
			vt_date vtRet = dtGMT;
			vtRet -= vt_date_span(0L,0L,m_nTimeZoneBias);
			return vtRet;
		}
	};
};




