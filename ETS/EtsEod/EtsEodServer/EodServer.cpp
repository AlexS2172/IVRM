// EodServer.cpp : Implementation of CEodServer

#include "stdafx.h"
#include "EodServer.h"
#include "Task.h"


// CEodServer
CEodServer* CEodServer::s_pEodServer = NULL;

STDMETHODIMP CEodServer::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] =
	{
		&IID_IEodServer
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CEodServer::FinalConstruct()
{
	s_pEodServer = this;
	m_spSettingsMgr.reset();
	m_spMktStrMgr.reset();
	m_spStepsMgr.reset();
	//m_spIndexFutures.reset();
	m_hStartEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hStopEodEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hTaskStartTimer = CreateWaitableTimer(NULL, FALSE, NULL);

	m_XMLParams.LoadXMLParams();

	//init report

	try
	{
		EODCO::CEgReportManagerPtr spReport(new EODCO::CEgReportManager);

		EODCO::CEgReportManager::s_spEodReport = spReport;

		m_spSettingsMgr = EODSM::CSmSettingsMgrPtr(new EODSM::CSmSettingsMgr);

		m_spMktStrMgr =  EOD::CEodMktStructureMgrPtr(new EOD::CEodMktStructureMgr);

		m_spStepsMgr = EOD::CEodStepManagerPtr(new EOD::CEodStepManager);

		//m_spIndexFutures = EOD::CEodStepLoadIndexFuturesPtr (new EOD::CEodStepLoadIndexFutures);

		CThread::Start();
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("%s, %s"), typeid(e).name(), e.what());
		return E_FAIL;
	}
	return S_OK;
}

void CEodServer::FinalRelease()
{
	CThread::Stop();

	if(m_hStartEvent)
		CloseHandle(m_hStartEvent);
	if(m_hStopEodEvent)
		CloseHandle(m_hStopEodEvent);

	if(m_hTaskStartTimer)
	{
		CancelWaitableTimer(m_hTaskStartTimer);
		CloseHandle(m_hTaskStartTimer);
	}

	m_XMLParams.SaveXMLParams();

	m_hStartEvent = NULL;
	m_hStopEodEvent = NULL;
	m_hTaskStartTimer = NULL;
	//m_spSettingsMgr.reset();
	//m_spMktStrMgr.reset();
	s_pEodServer = NULL;
	//m_spStepsMgr.reset();

	//uninit report
	EODCO::CEgReportManager::s_spEodReport.reset();
}

STDMETHODIMP CEodServer::get_EodMask(ULONG* pVal)
{
	long nVal = enSmDefSteps;
	m_XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("StepMask"), &nVal, enSmDefSteps);
	*pVal = nVal;

	return S_OK;
}

STDMETHODIMP CEodServer::put_EodMask(ULONG newVal)
{
	HRESULT hr = m_XMLParams.SetXMLLong(c_strSettingsXMLKey, _T("StepMask"), newVal);
	if(FAILED(hr))
		return Error(L"Can't set xml value", IID_IEodServer, hr);

	return S_OK;
}

STDMETHODIMP CEodServer::AddTask(ITask* pTask)
{
	DATE dtStart;
	pTask->get_StartTime(&dtStart);
	COleDateTime dtStartTime(dtStart);
	HRESULT hr = m_XMLParams.SetXMLString(c_strTaskXMLKey, _T("RestartTime"), (LPCTSTR)dtStartTime.Format(VAR_TIMEVALUEONLY));
	if(FAILED(hr))
		return Error(L"Can't set xml value", IID_IEodServer, hr);

	_QueueSetTaskStartTimer(dtStartTime);
	return S_OK;
}

STDMETHODIMP CEodServer::RemoveTask()
{
	HRESULT hr = m_XMLParams.SetXMLString(c_strTaskXMLKey, _T("RestartTime"), "");
	if(FAILED(hr))
		return Error(L"Can't set xml value", IID_IEodServer, hr);

	CancelWaitableTimer(m_hTaskStartTimer);
	return S_OK;
}

STDMETHODIMP CEodServer::GetTask(ITask** ppTask)
{
	*ppTask = NULL;

	_bstr_t sbsTime;
	m_XMLParams.GetXMLString(c_strTaskXMLKey, _T("RestartTime"), &sbsTime);
	if(sbsTime.length() == 0)
		return S_OK;

	CComObject<CTask>* pTask;
	CComObject<CTask>::CreateInstance(&pTask);

	CString sTime = (LPCTSTR)sbsTime;

	COleDateTime dtStartTime;
	if(dtStartTime.ParseDateTime(sTime, VAR_TIMEVALUEONLY))
	{
		pTask->put_StartTime(dtStartTime);
		pTask->AddRef();
		*ppTask = pTask;
	}
	return S_OK;
}

STDMETHODIMP CEodServer::StartEod(void)
{
	ATLASSERT(m_spStepsMgr);
	if(m_spStepsMgr->Status() == enStStopped)
	{
		m_spStepsMgr->Status(enStStartPending);
		SetEvent(m_hStartEvent);
	}
	return S_OK;
}

STDMETHODIMP CEodServer::StopEod(void)
{
	ATLASSERT(m_spStepsMgr);
	if(m_spStepsMgr->Status() == enStStarted)
	{
		m_spStepsMgr->Status(enStStopPending);
		SetEvent(m_hStopEodEvent);
	}
	return S_OK;
}

STDMETHODIMP CEodServer::get_EodStatus(EodStatusEnum* pVal)
{
	if(!m_spStepsMgr)
		*pVal = enStStopped;
	else
		*pVal = m_spStepsMgr->Status();
	return S_OK;
}

STDMETHODIMP CEodServer::get_EodCalcModel(EodCalcModelTypeEnum* pVal)
{
	long nVal = 0;
	m_XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("CalcModel"), &nVal, enEodCmBinomial);
	
	*pVal = static_cast<EodCalcModelTypeEnum>(nVal);

	return S_OK;
}

STDMETHODIMP CEodServer::put_EodCalcModel(EodCalcModelTypeEnum newVal)
{
	HRESULT hr = m_XMLParams.SetXMLLong(c_strSettingsXMLKey, _T("CalcModel"), newVal);
	if(FAILED(hr))
		return Error(L"Can't set xml value", IID_IEodServer, hr);

	return S_OK;
}

STDMETHODIMP CEodServer::get_EodLogLevel(int* pVal)
{
	long nVal = EODCO::enTrLogNone;
	m_XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("LogLevel"), &nVal, EODCO::enTrLogNone);

	*pVal = static_cast<int>(nVal);

	return S_OK;
}

STDMETHODIMP CEodServer::put_EodLogLevel(int newVal)
{
	HRESULT hr = m_XMLParams.SetXMLLong(c_strSettingsXMLKey, _T("LogLevel"), newVal);
	if(FAILED(hr))
		return Error(L"Can't set xml value", IID_IEodServer, hr);

	return S_OK;
}

STDMETHODIMP CEodServer::get_EodLogHistory(int* pVal)
{
	long nVal = DAYS_IN_HISTORY_DEFAULT;
	m_XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("LogHistory"), &nVal, DAYS_IN_HISTORY_DEFAULT);

	*pVal = static_cast<int>(nVal);

	return S_OK;
}

STDMETHODIMP CEodServer::put_EodLogHistory(int newVal)
{
	HRESULT hr = m_XMLParams.SetXMLLong(c_strSettingsXMLKey, _T("LogHistory"), newVal);
	if(FAILED(hr))
		return Error(L"Can't set xml value", IID_IEodServer, hr);

	return S_OK;
}

_COM_SMARTPTR_TYPEDEF(ITask, __uuidof(ITask));

UINT CEodServer::Run()
{
	::CoInitializeEx(NULL,COINIT_MULTITHREADED);

	{
		ITaskPtr spTask;
		HRESULT hr = GetTask(&spTask);
		if(SUCCEEDED(hr) && spTask != NULL)
		{
			DATE dtStart;
			hr = spTask->get_StartTime(&dtStart);
			if(SUCCEEDED(hr))
				_QueueSetTaskStartTimer(COleDateTime(dtStart));
		}
		spTask = NULL;

		bool bStop = false;
		while(!bStop)
		{
			vector<HANDLE> vecHandles;
			vecHandles.push_back(GetStopEventHandle());
			vecHandles.push_back(m_hStopEodEvent);
			vecHandles.push_back(m_hTaskStartTimer);
			vecHandles.push_back(m_hStartEvent);

			m_spStepsMgr->RescheduleSteps(vecHandles);
			DWORD dwRes = ::WaitForMultipleObjects(static_cast<DWORD>(vecHandles.size()),&vecHandles.front(),FALSE,INFINITE);

			switch(dwRes)
			{
				case WAIT_OBJECT_0: //stop
				{
					bStop = true;
				}
				case WAIT_OBJECT_0 + 1: //stop Eod
				{
					m_spStepsMgr->Stop();
				}
				break;
				case WAIT_OBJECT_0 + 2: //start by timer
				{
					hr = GetTask(&spTask);
					if(SUCCEEDED(hr) && spTask != NULL)
					{
						DATE dtStart;
						hr = spTask->get_StartTime(&dtStart);
						if(SUCCEEDED(hr))
							_QueueSetTaskStartTimer(COleDateTime(dtStart));
					}
					spTask = NULL;
					if(m_spStepsMgr->Status() != enStStopped)
						break;
				}
				case WAIT_OBJECT_0 + 3: //start
				{
					try
					{
						ULONG nMask = 0L;
						get_EodMask(&nMask);

						EODCO::CEgReportManager::s_spEodReport->Init();
						m_spSettingsMgr->Init();
						m_spMktStrMgr->Init(m_spSettingsMgr);
						//m_spIndexFutures->init(enSmLoadIndexFutures, 0);

						m_spStepsMgr->Start(nMask);
					}
					catch(exception& e)
					{
						Fire_OnError(_bstr_t(e.what()));

						m_spStepsMgr->Status(enStStopped);
						Fire_OnEodStatusChanged(enStStopped);

						Trace(EODCO::enTrLogError,__FUNCTION__,_T("%s, %s"), typeid(e).name(), e.what());
					}
					catch(_com_error& e)
					{
						Trace(EODCO::enTrLogError,__FUNCTION__,(LPCSTR)e.Description());
						
						MessageBox(0,_T((LPCSTR)e.Description()), NULL, MB_ICONEXCLAMATION);
						 		
						m_spStepsMgr->Status(enStStopped);
						Fire_OnEodStatusChanged(enStStopped);	
					}
				}
				break;
			}
		}
	}
	::CoUninitialize();
	return 0;
}

bool CEodServer::_QueueSetTaskStartTimer(const COleDateTime& dtStartTime)
{
	COleDateTime dtCurrentTime(COleDateTime::GetCurrentTime());
	COleDateTime dtStartDateTime;

	dtStartDateTime.SetDateTime(dtCurrentTime.GetYear(), dtCurrentTime.GetMonth(), dtCurrentTime.GetDay(),
		dtStartTime.GetHour(), dtStartTime.GetMinute(), dtStartTime.GetSecond());

	COleDateTimeSpan dtSpanOneDay(1L,0,0,0);
	COleDateTimeSpan dtSpanOneMinute(0L,0,1,0);
	if(dtCurrentTime + dtSpanOneMinute > dtStartDateTime)
		dtStartDateTime += dtSpanOneDay;


	SYSTEMTIME sysTime;
	dtStartDateTime.GetAsSystemTime(sysTime);
	FILETIME ftLoc, ftUTC;
	SystemTimeToFileTime(&sysTime, &ftLoc);
	LocalFileTimeToFileTime(&ftLoc,&ftUTC);

	LARGE_INTEGER liDueTime;
	memcpy(&liDueTime.QuadPart,&ftUTC,sizeof(FILETIME));

	if(SetWaitableTimer(m_hTaskStartTimer, &liDueTime, 0, NULL, NULL, 0) == TRUE)
		return true;
	else return false;
}
