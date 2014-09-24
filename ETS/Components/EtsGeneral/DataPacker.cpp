#include "StdAfx.h"
#include "DataPacker.h"
#include "AbstractContract.h"
//--------------------------------------------------------//
DataPacker::DataPacker(){
};
//--------------------------------------------------------//
DataPacker::~DataPacker(){
};
//--------------------------------------------------------//
/*virtual*/
long 
DataPacker::pack_analytics(DataPacker::contract_vector &contracts, 
						   LPSAFEARRAY &data){
							   
	CComRecPtr<IRisksFullInfo, &LIBID_PRICEPROVIDERSLib> RisksFullInfo;
	RisksFullInfo.Init();
	
	IRisksFullInfo* array_item = NULL;

	long array_size = static_cast<long>(contracts.size());

	SAFEARRAYBOUND array_bound = {array_size, 0};
	
	data = SafeArrayCreateEx(VT_RECORD, 1, &array_bound, (PVOID)RisksFullInfo.GetRecordInfo());

	SafeArrayAccessData(data, (void**)&array_item); 
	
	memset(array_item, 0, array_size * sizeof(IRisksFullInfo));

	int j = 0;
	CAbstractContract* base_contract = 0;

	typedef std::vector<CAbstractContract*> container;
	container::iterator it = contracts.begin();
	container::iterator it_end = contracts.end();

	for (; it != it_end; it++) {

		CAbstractContract* contract = *it;

		if (contract->m_spBaseContract) {

			base_contract = 
			   dynamic_cast<CAbstractContract*>(contract->m_spBaseContract.GetInterfacePtr());

		   if (base_contract) {
			   base_contract->GetTicker()->CopyTo(array_item[j].BaseTicker);
		   };
		}
		else {
		   array_item[j].BaseTicker.Symbol	= _com_util::ConvertStringToBSTR("");
		   array_item[j].BaseTicker.Exchange = _com_util::ConvertStringToBSTR("");
		   array_item[j].BaseTicker.Type = enSTK;
		};

		contract->m_spRisks->CopyTo(array_item[j].Risks);
		contract->GetTicker()->CopyTo(array_item[j].Ticker);

		j++;
	};
	::SafeArrayUnaccessData(data);

	return j;
};
//--------------------------------------------------------//
//--------------------------------------------------------//
//--------------------------------------------------------//
//--------------------------------------------------------//
