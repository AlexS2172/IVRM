#pragma once

namespace Trace
{
	class CEgHyprFeedTrace : public	EgLib::CEgLibTraceManager
	{
	public:
		CEgHyprFeedTrace(void)
			:EgLib::CEgLibTraceManager(EgLib::CEgLibTraceManager::enRelativeMyDocuments, _T("ETS\\Logs"))
		{
			EgLib::CEgLibTraceManager::FileName = L"ivrm_activ_feed";
		}

		virtual ~CEgHyprFeedTrace(void)
		{
		}
	};
}
