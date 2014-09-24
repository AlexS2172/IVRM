#pragma once

#ifdef EGLIB_DB_BACKWARD_COMPATIBLE
	#undef EGLIB_DB_CLIENT_ONLY
	#import <msado25.tlb> implementation_only rename ("EOF", "AdoEof")  rename ("BOF", "AdoBof") EGLIB_ADO_IMPORT_SPECIFIER
#else
	#ifndef EGLIB_DB_CLIENT_ONLY
		#import <msado15.dll> implementation_only rename ("EOF", "AdoEof")  rename ("BOF", "AdoBof") EGLIB_ADO_IMPORT_SPECIFIER
	#else
		#import <msador15.dll> implementation_only rename ("EOF", "AdoEof") rename ("BOF", "AdoBof") EGLIB_ADO_IMPORT_SPECIFIER
	#endif //EGLIB_DB_CLIENT_ONLY
#endif
