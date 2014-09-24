#include "StdAfx.h"
#include "Controller.h"
#include <fstream>
//--------------------------------------------------------------------------------------------------------
CController::CController(void)
{
	m_pMain = NULL;
}
//--------------------------------------------------------------------------------------------------------
CController::~CController(void)
{
	UnSubscribeSettingsUpdate();
}
//--------------------------------------------------------------------------------------------------------
bool CController::SetMain(IEtsMainPtr spMain)
{
	if (static_cast<bool>(spMain))
	{
		if (m_pMain == NULL)	
			m_pMain = spMain.GetInterfacePtr();
		return true;
	}
	return false;
};
//--------------------------------------------------------------------------------------------------------
IEtsMainPtr		CController::GetMain()
{
	ATLASSERT(m_pMain != NULL);

	IEtsMainPtr spRet = m_pMain;

	return spRet;
}
//--------------------------------------------------------------------------------------------------------
void CController::SetSettingsImpl(CSettingsPtr spSettings)
{
	m_spSettings = spSettings;
};
//--------------------------------------------------------------------------------------------------------
CSettingsPtr CController::GetSettingsImpl()
{
	return m_spSettings;
};
//--------------------------------------------------------------------------------------------------------
void CController::LoadSettings()
{
	try
	{
		CCalculationSettingsPtr spCalcSettings = CCalculationSettingsPtr(new CCalculationSettings());

		spCalcSettings->SetUseTimePrecision(m_spSettings->GetUseTimePrecision());
		spCalcSettings->SetGreeksCalculationModel(m_spSettings->GetGreeksCalculationModel());
		spCalcSettings->SetGreeksCalculationMode(m_spSettings->GetGreeksCalculationMode() > 0 ? enGcmTheoreticalVola : enGcmImpliedVola);
		spCalcSettings->SetPriceTolerance(0.01);
		spCalcSettings->SetPriceRoundingRule(0);
		spCalcSettings->SetUseTheoVolaIfNoBid(0);
		spCalcSettings->SetUseTheoVolaIfBadMarketVola(1);
		spCalcSettings->set_extrapolation_type(m_spSettings->curve_extrapolation_type());
		spCalcSettings->set_interpolation_type(m_spSettings->curve_interpolation_type());

		OnSettingsUpdate(spCalcSettings);
	}
	catch (_com_error& err)
	{
		err.Description();
		ATLASSERT(FALSE);
	}
	catch (...)
	{
		ATLASSERT(FALSE);
	}
};
//--------------------------------------------------------------------------------------------------------
void CController::OnSettingsUpdate(CCalculationSettingsPtr spSettings)
{
	try
	{
		ICalculationSettingsPtr settings = NULL;
		
		GetMain()->get_CalculationSettings(&settings);

		if (static_cast<bool>(settings))
		{
			settings->put_GreeksCalculationMode(static_cast<LONG>(spSettings->GetGreeksCalculationMode()));
			settings->put_GreeksCalculationModel(spSettings->GetGreeksCalculationModel());
			settings->put_UseTimePrecision(spSettings->GetUseTimePrecision() == true ? 1 : 0 );
			settings->put_UseTheoVolaIfNoBid(spSettings->GetUseTheoVolaIfNoBid() == true ? 1 : 0 );
			settings->put_UseTheoVolaIfBadMarketVola(spSettings->GetUseTheoVolaIfBadMarketVola() == true ? 1 : 0 );
			settings->put_PriceTolerance(spSettings->GetPriceTolerance());
			settings->put_PriceRoundingRule(spSettings->GetPriceRoundingRule());
			settings->put_CurveExtrapolationType(spSettings->extrapolation_type());
			settings->put_CurveInterpolationType(spSettings->interpolation_type());
		}
	}
	catch (_com_error&e)
	{
		e.Description();
		ATLASSERT(FALSE);
	};
};
//--------------------------------------------------------------------------------------------------------
void CController::OnUnderlyingUpdate(struct IUnderlyingUpdate* Data)
{
	try
	{
		UNDERLYING_UPDATE_STATUS updStatus;
		Data->get_UpdStatus(&updStatus);
		if (enUndDividendUpdate & updStatus)
			GetMain()->ReloadDividends(Data->GetUndID());

	}
	catch (_com_error&e)
	{
		e.Description();
		ATLASSERT(FALSE);
	};
};
//--------------------------------------------------------------------------------------------------------//
void CController::LoadAssetList()
{
	try 
	{
		std::vector<CString> __params;
		std::ifstream ContractListFile;

		char* pToken = NULL;
		char* pNextToken = NULL;
		char line[512];
		memset(line, 0, static_cast<size_t>(sizeof(char) * 512));
		char seps[] = ";";

		std::string assetFileName = GetSettingsImpl()->GetAssetListFileName();

		if (assetFileName.length() > 0)
		{
			ContractListFile.open(assetFileName.c_str());

			if (ContractListFile.is_open())
			{
				while (!ContractListFile.eof())
				{
					ContractListFile.getline(line, 256, '\n');

					pToken = strtok_s( line, seps, &pNextToken ); 

					//separate line values
					while( pToken != NULL )
					{
						CString strValue(pToken);
						strValue.TrimLeft().TrimRight();
						__params.push_back(strValue);
						pToken = strtok_s( NULL, seps, &pNextToken );
					};

					if (static_cast<long>(__params.size()) > 1)
					{
						CTicker contract;
						contract.m_enType = static_cast<InstrumentTypeEnum>(-1);

						contract.m_sSymbol = __params[0];
						if (__params[1] == CString("STOCK"))
							contract.m_enType = enSTK;
						else if (__params[1] == CString("INDEX"))
							contract.m_enType = enIDX;

						if (contract.m_enType >= 0 ){
							ITicker ticker;
							contract.CopyTo(ticker);

							GetMain()->LoadContracts(&ticker);

							::SysFreeString(ticker.Symbol);
							::SysFreeString(ticker.Exchange);
						};
					}
					//clear for new line parse
					__params.clear();
				}
			}
			else{
				LOG4CPLUS_WARN(VS::Log, _T("Can't read Asset List file: ") << assetFileName.c_str());	
			}
		}
		else{
			LOG4CPLUS_INFO(VS::Log, _T("Asset List file not defined."));	
		};
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(VS::Log, _T("Unknown exception occured while trying to read Asset List file."));
	};
};
//--------------------------------------------------------------------------------------------------------//
