#include "stdafx.h"

class CEgTrace: public EgLib::CEgLibTraceManager {
public:
	CEgTrace(void)
	:EgLib::CEgLibTraceManager(EgLib::CEgLibTraceManager::enRelativeMyDocuments, _T("core")) {
		EgLib::CEgLibTraceManager::FileName = L"ivrm_core";
	};

	virtual 
	~CEgTrace(void) {
	};
	
	static 
	void Trace() {
	};
};