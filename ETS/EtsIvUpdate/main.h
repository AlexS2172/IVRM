#pragma once

class CIVOptionData ;
class CIVStockData ;

class BIV_Data_Update_Service
{
public:
	virtual bool SaveIVOptionData(const CIVOptionData& _IVData) = 0 ;
	virtual bool SaveIVStockData(const CIVStockData& _IVData) = 0 ;

	virtual bool IsEndOfDay () = 0 ;
	virtual void SendUpdate() = 0 ; 
};

