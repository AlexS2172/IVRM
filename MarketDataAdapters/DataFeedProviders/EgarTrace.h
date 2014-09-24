#include "stdafx.h"

class CEgTrace: public EgLib::CEgLibTraceManager {
public:

	CEgTrace(void)
	:EgLib::CEgLibTraceManager(EgLib::CEgLibTraceManager::enRelativeMyDocuments, _T("Core")) {
		EgLib::CEgLibTraceManager::FileName = L"ivrm_iv_feed";
	};

	virtual 
	~CEgTrace(void) { 
	};
	
	static 
	void Trace() {
	};
};