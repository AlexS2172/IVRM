// Compiler.cpp : Implementation of CcvCompiler

#include "stdafx.h"
#include "cvParser.h"
#include "Compiler.h"
#include <string>
#include <map>
#include "..\cvCV.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <algorithm>
#include <atlsafe.h>

using namespace std;
using namespace EgLib;
using namespace CV;
// CcvCompiler


STDMETHODIMP CcvCompiler::get_ConnectionString(BSTR* pVal)
{
	if( pVal ){	
		*pVal = m_strConnection;
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP CcvCompiler::put_ConnectionString(BSTR newVal)
{
	if( wcslen( newVal ) ) {
		m_strConnection = newVal;
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP CcvCompiler::Compile(LONG iContext, LONG iLevel, BSTR strVarName,BSTR strCaption, BSTR strFormat, 
								  BSTR strSource, BSTR strDescription, SAFEARRAY ** arrAggregation, SAFEARRAY ** arrErrors, SAFEARRAY ** arrCode, LONG* iVariableID)
{
	HRESULT hr = S_OK;
	m_lLevel = iLevel;
	string name = (LPCSTR)(( _bstr_t)strVarName);
	void *ptr = NULL;

	if ( name.empty() ) 
		return E_INVALIDARG;

	try{

		// load context environment
		LoadContext( iContext );

		// find useful variables and functions 
		FindUsefulVars(name);
		FindUsefulFuncs();

		// create parser
		cvScriptParser p;

		// loading available function names into parser 
		for ( cvTRefPtrByNameMap::iterator bF = m_mapFuncs.begin(), bE = m_mapFuncs.end(); bF != bE; bF++ ) {
			pair<unsigned int, cvRefPtr> pr = bF->second;
			p.cvFunctionName.add( pr.second->m_name.c_str());
		}
		
		// load source code into parser 
		p.PrepareParsing( (LPCSTR)(( _bstr_t )strSource) );

		// parse it
		p.Parse();


		if ( !p.info.full || !p.m_vecErrors.empty() ) {
			//if ( p.m_vecErrors.empty() ) 
			p.m_vecErrors.clear();
			p.m_vecErrors.push_back( string("Syntax error") );
			// work up parser errors
			*iVariableID = 0;
			SAFEARRAYBOUND sab = { static_cast<ULONG>(p.m_vecErrors.size()), 0 };
			LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_BSTR, 1, &sab, NULL);
			BSTR *bstrErrs;
			::SafeArrayAccessData( psa, ( void**) &bstrErrs);
			for ( size_t s = 0; s < p.m_vecErrors.size(); s++ ) 
				bstrErrs[s] = SysAllocString( CComBSTR(p.m_vecErrors[s].c_str() ) );
			::SafeArrayUnaccessData( psa );

			*arrErrors = psa;		  
			return S_OK;
		}
		else{
			try{
				const cvTReferable<void, cvtNone> * const pNewVar = compileCV( p.info.trees.begin(), name);
				if ( !m_vecErrors.empty() ) {
					*iVariableID = 0;

					SAFEARRAYBOUND sab = { static_cast<ULONG>(m_vecErrors.size()), 0 };
					LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_BSTR, 1, &sab, NULL);
					BSTR *bstrErrs;
					::SafeArrayAccessData( psa, ( void**) &bstrErrs);
					for ( size_t s = 0; s < m_vecErrors.size(); s++ ) 
						bstrErrs[s] = SysAllocString( CComBSTR(m_vecErrors[s].c_str() ) );
					::SafeArrayUnaccessData( psa );

					*arrErrors = psa;		  
					return S_OK;
				}
				DoListing(p, strVarName);

				// save code into buffer ( constants << functions << variables << code )
				unsigned int sz = m_mapConsts.GetSearilizationSize() + m_mapFuncs.GetSearilizationSize() + 
											m_mapVars.GetSearilizationSize()+ m_Code.GetSearilizationSize();
				ptr = new char[ sz ];
				m_Code.ToBinary(m_mapVars.ToBinary( m_mapFuncs.ToBinary( m_mapConsts.ToBinary(ptr) ) ));

				// build aggregations string
				basic_stringstream<char> a;
				if ( arrAggregation && *arrAggregation ) {
					CComSafeArray<long> arrAggr( *arrAggregation);
					
					for ( LONG i = arrAggr.GetLowerBound(); i <=arrAggr.GetUpperBound(); i++ )
						if ( arrAggr[i]) 
							a << "<" << arrAggr[i] << ">";
				}
				else
					a << "";

				if( pNewVar->getType() != cvtNumeric && !a.str().empty() ){
					delete ptr;
					*iVariableID = 0;
					m_vecErrors.push_back( string("Attempt to aggregate non numeric variable") );
					SAFEARRAYBOUND sab = { static_cast<ULONG>(m_vecErrors.size()), 0 };
					LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_BSTR, 1, &sab, NULL);
					BSTR *bstrErrs;
					::SafeArrayAccessData( psa, ( void**) &bstrErrs);
					for ( size_t s = 0; s < m_vecErrors.size(); s++ ) 
						bstrErrs[s] = SysAllocString( CComBSTR(m_vecErrors[s].c_str() ) );
					::SafeArrayUnaccessData( psa );

					*arrErrors = psa;		  
					return S_OK;
				}

				*iVariableID = SaveCV( name, iContext, iLevel, (LPCSTR)(( _bstr_t)(strCaption ? strCaption : L"" ) ), 
																				(LPCSTR)(( _bstr_t)(strFormat ? strFormat : L""  ) ), 
																				(LPCSTR)(( _bstr_t)(strSource ? strSource : L""  ) ),
																				a.str().c_str(), ptr, (size_t)sz, pNewVar->getType(), pNewVar->GetWeight(),
																				(LPCSTR)(( _bstr_t)(strDescription ? strDescription : L""  )));

				delete ptr;

			}
			catch ( _com_error& e ) {
				if ( ptr ){ 
					delete ptr;
					ptr = NULL;
				}
				hr = e.Error();		
			}
			catch( ...){
				if ( ptr ){ 
					delete ptr;
					ptr = NULL;
				}
				hr = E_FAIL;
			}
		}
	}
	catch ( _com_error& e ) {
		if ( ptr ){ 
			delete ptr;
			ptr = NULL;
		}
		hr = e.Error();		
	}

	return hr;
}


void CcvCompiler::FindUsefulVars(const string& strVarName){
	/*
		Useful variables are:
			1. Global System Variables
			2. System Variables defined for this level
			3. Custom Variables defined for this level
			4. Custom Variables defined on lower levels ( weight is greater than weight of this level ) that are have aggregation on current level
	*/

	cvLevelPtr L;

	cvLevelsByNameMap::iterator bL= c.m_mapLevels.begin();
	cvLevelsByNameMap::iterator eL= c.m_mapLevels.end();
	while ( bL != eL) {
		cvLevelPtr pL = bL->second;

		// 1, 2
		cvSysTypedObjByNameMap::iterator bSV = pL->m_mapSysVars.begin();
		cvSysTypedObjByNameMap::iterator eSV = pL->m_mapSysVars.end();
		while ( bSV != eSV ) {
			cvSysTypedObjPtr pSV = bSV->second;
			if ( ( pSV->m_global && pSV->m_level <= m_lLevel ) || pSV->m_level == m_lLevel ) {
				cvTReferablePtr<double, cvtNumeric> p = cvTReferablePtr<double, cvtNumeric>( new cvTReferable<double, cvtNumeric>() );
				*p = *pSV;
				m_mapVars.Add( p->m_name, p );
			}
			bSV++;
		}

		// 3, 4
		bSV = pL->m_mapCV.begin();
		eSV = pL->m_mapCV.end();
		while ( bSV != eSV ) {
			cvMetaCV *p = static_cast<cvMetaCV*>( bSV->second.get() );
			cvSysTypedObjPtr pSV = bSV->second;
			if (p->m_level == m_lLevel /*3*/|| /*4*/
				search_n( p->m_vecAggregations.begin(), p->m_vecAggregations.end(),1,  m_lLevel ) != p->m_vecAggregations.end() ) {
				cvTReferablePtr<void, cvtNone> pV = cvTReferablePtr<void, cvtNone>( new cvTReferable<void, cvtNone>() );
				*pV = *pSV;
				 pV->SetWeight( p->m_weight );

				// allow change of type of variable being compiled ONLY IF THEREISN'T DEPENDEND VARIABLES
				if ( p->m_strName == strVarName && p->m_level == m_lLevel ) 
					pV->setType( cvtNone );


				m_mapVars.Add( pV->m_name, pV );
			}
			bSV++;
		}

		bL++;
	}
}

void CcvCompiler::FindUsefulFuncs(){
	cvLevelPtr L;

	cvLevelsByNameMap::iterator bL= c.m_mapLevels.begin();
	cvLevelsByNameMap::iterator eL= c.m_mapLevels.end();

	while ( bL != eL) {
		cvLevelPtr pL = bL->second;
		cvSysTypedObjByNameMap::iterator bSF = pL->m_mapSysFunc.begin();
		cvSysTypedObjByNameMap::iterator eSF = pL->m_mapSysFunc.end();
		while ( bSF != eSF ) {
			//cvSysTypedObjPtr pSV = bSV->second;
			cvSysFuncPtr pSF = shared_dynamic_cast<cvSysFunc>(bSF->second);

			// useful functions are functions defined on same level as variable being compiled and
			// global functions defined on higher levels 
			if ( ( pSF->m_global && pSF->m_level <=m_lLevel )  || pSF->m_level == m_lLevel ) {
				cvTReferablePtr<double, cvtNumeric> p = cvTReferablePtr<double, cvtNumeric>( new cvTReferable<double, cvtNumeric>() );
				*p = *pSF;
				m_mapFuncs.Add( p->m_name, p );
			}

			// get next function on level
			bSF++;
		}

		// go to next level within context
		bL++;
	}
}	 

LONG CcvCompiler::SaveCV(const string& name , LONG iContext, LONG iLevel, const string& strCaption, const string& strFormat, const string& strSource, const string& strAggregations, void* pCode, size_t szCodeSize, cvtType Type,int iWeight, const string& strDescription ){
	if ( !m_Conn.IsOpened() ) {
		// open connection 
		m_Conn.Open( m_strConnection, 10, 10, 10, 10 );
	}	

	CStoredProc<> sp( m_Conn, L"usp_cvCV_Save");
	sp << name;											// @Name NVARCHAR
	sp << iContext;									// @contextID INT
	sp << iLevel;										// @levelID INT
	sp << (int) Type;									// @Type INT,
	sp << strSource;									// @Source NVARCHAR
	sp << strCaption;									// @Caption NVARCHAR,
	sp << strFormat;									// @Format NVARCHAR,
	sp << strAggregations;							//	@aggLevels NVARCHAR, 
	sp << CBinaryParam( pCode, szCodeSize );	// @Code IMAGE
	sp << iWeight;										// @Weight
	sp << strDescription;						// @Desc
	sp.Execute();
	return (LONG) sp.GetResult();
}

// loads from database all information regarding context: context info, 
// levels description, system variables and function, custom variables
bool CcvCompiler::LoadContext(int iContextID )
{
	
	if ( !m_Conn.IsOpened() ) {
		// open connection 
		m_Conn.Open( m_strConnection, 120, 120, 120, 120 );
	}	

	CStoredProc<CClientRecordset> sp( m_Conn, L"usp_cvContext_Get" );
	sp << iContextID;
	sp << 6;	// requested info level

	sp.Open();
	if( sp.GetRecordCount() ) {

		// first recordset describes context itself
		for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
			c.m_ID = static_cast<int>( sp[L"ID"]);
			c.m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
			break;
		}

		// second recordset for levels inside context 
		sp.NextRecordset();		
		if( sp.GetRecordCount() ) {
			for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
				cvLevelPtr p = cvLevelPtr( new cvLevel );
				p->m_ID = static_cast<int>( sp[L"ID"] );
				p->m_context = static_cast<int>( sp[L"contextID"] );
				p->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
				p->m_Weight = static_cast<int>( sp[L"Weight"] );
				c.m_mapLevels[ p->m_strName ] = p;
			}
		}
		// third recordset for system variables within levels
		sp.NextRecordset();		
		if( sp.GetRecordCount() ) {
			for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
				cvSysTypedObjPtr p = cvSysTypedObjPtr( new cvSysTypedObj );
				p->m_ID = static_cast<int>( sp[L"ID"] );
				p->m_context = static_cast<int>( sp[L"contextID"] );
				p->m_level = static_cast<int>( sp[L"levelID"] );
				p->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
				p->m_type = static_cast<cvtType>( static_cast<int>(sp[L"Type"]) );
				p->m_global = sp["Global"];
				cvLevelsByNameMap::const_iterator i = c.m_mapLevels.findByID( p->m_level );
				if( i != c.m_mapLevels.end() )
					(*(i->second)).m_mapSysVars[p->m_strName] = p;
			}
		}
		// fourth recordset for system functions within levels
		sp.NextRecordset();		
		if( sp.GetRecordCount() ) {
			for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
				//cvSysTypedObjPtr p = cvSysTypedObjPtr( new cvSysTypedObj );
				cvSysFuncPtr p = cvSysFuncPtr( new cvSysFunc );

				p->m_ID = static_cast<int>( sp[L"ID"] );
				p->m_context = static_cast<int>( sp[L"contextID"] );
				p->m_level = static_cast<int>( sp[L"levelID"] );
				p->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
				p->m_type = static_cast<cvtType>( static_cast<int>(sp[L"Type"]) );
				p->m_global = sp["Global"];
				p->InitParameters( (LPCSTR)((_bstr_t) sp[L"Params"]) );
				cvLevelsByNameMap::const_iterator i = c.m_mapLevels.findByID( p->m_level );
				if( i != c.m_mapLevels.end() )
					(*(i->second)).m_mapSysFunc[p->m_strName] = p;
			}
		}
		// fifth one for predefined custom variables
		sp.NextRecordset();		
		if( sp.GetRecordCount() ) {
			for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
				cvMetaCV *pCV = new cvMetaCV;
				cvSysTypedObjPtr p = cvSysTypedObjPtr( pCV );
				p->m_ID = static_cast<int>( sp[L"ID"] );
				p->m_context = static_cast<int>( sp[L"contextID"] );
				p->m_level = static_cast<int>( sp[L"levelID"] );
				p->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
				p->m_type = static_cast<cvtType>( static_cast<int>(sp[L"Type"]) );
				pCV->InitAggregations( (LPCSTR)( (_bstr_t) sp[L"aggLevels"]));
				pCV->m_caption = (LPCSTR)( (_bstr_t ) sp[L"Caption"] );
				pCV->m_format = (LPCSTR)( (_bstr_t ) sp[L"Format"] );
				pCV->m_source = (LPCSTR)( (_bstr_t ) sp[L"Source"] );
				pCV->m_weight = static_cast<int>( sp[L"Weight"] );

				cvLevelsByNameMap::const_iterator i = c.m_mapLevels.findByID( p->m_level );
				if( i != c.m_mapLevels.end() ) {

					_variant_t vtVal = sp[L"Code"].GetValue();
					size_t v = static_cast<size_t>(vtVal.parray->rgsabound[0].cElements);
					void *ptr = malloc(v);
					if (ptr)
					{
						sp[L"Code"].GetBinaryValue( ptr, v );

						pCV->InitFromBinary( ptr, c, (*(i->second)) /*reference to level*/  );
						(*(i->second)).m_mapCV[p->m_strName] = p;

						free(ptr);
					}

				}


				// now, add 'stub' meta variable to levels, where this one have aggregations
				if ( !pCV->m_vecAggregations.empty() ) {
					for ( size_t s = 0; s < pCV->m_vecAggregations.size(); s++ ) {
						cvLevelsByNameMap::const_iterator iA = c.m_mapLevels.findByID( pCV->m_vecAggregations[s] );
						if( iA != c.m_mapLevels.end() ) {
							cvMetaCV *pCVA = new cvMetaCV;
							cvSysTypedObjPtr pA = cvSysTypedObjPtr( pCVA );
							pA->m_ID = static_cast<int>( sp[L"ID"] );
							pA->m_context = static_cast<int>( sp[L"contextID"] );
							pA->m_level = static_cast<int>( sp[L"levelID"] );
							pA->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
							pA->m_type = static_cast<cvtType>( static_cast<int>(sp[L"Type"]) );
							pCVA->m_caption = (LPCSTR)( (_bstr_t ) sp[L"Caption"] );
							pCVA->m_format = (LPCSTR)( (_bstr_t ) sp[L"Format"] );
							pCVA->m_source = (LPCSTR)( (_bstr_t ) sp[L"Source"] );
							pCVA->m_weight = static_cast<int>( sp[L"Weight"] );
							(*(iA->second)).m_mapCV[pA->m_strName] = pA;
						}
					}
				}
			}
		}
	}
	return true;
}


const cvTReferable<void, cvtNone>* const CcvCompiler::compileCV(cvScript_tree_iterator& root, const string& name )
{
	cvTReferable<void, cvtNone> *pV = NULL;

	cvScript_tree_iterator itrChild = root->children.begin();
	cvScript_tree_iterator itrLastChild = root->children.end();
	CollectConsts( root );

	// add variable being compiled to variables map
	cvTRefPtrByNameMap::iterator itr = m_mapVars.begin();
	itr = m_mapVars.find( name );
	if ( itr == m_mapVars.end() ) {
		// first time compiling variable
		cvTReferablePtr<void, cvtNone> p = cvTReferablePtr<void, cvtNone>( new cvTReferable<void, cvtNone>(name) );
		p->SetWeight(0);
		p->SetLevel(m_lLevel);
		m_mapVars.Add( name, p);
	}
	else{
		// this variable already in map, this means it was already compiled and saved in database
		// just clear system flag for it
		pair<unsigned int, cvRefPtr> pr = itr->second;
		pV = static_cast<cvTReferable<void, cvtNone>*> ( pr.second.get() );
		pV->SetWeight(0);
		pV->MakeNonSystem();
	}

	// perform compile
	if (root->value.id() == PARSER_PROGRAM ) {
		while ( itrChild != itrLastChild ){ 
			m_bPopAfterFunction =  ( itrChild->value.id() == PARSER_FUNC );
			CompileNode( itrChild );
			itrChild++;
		}
	}
	else{
		m_bPopAfterFunction =  ( root->value.id() == PARSER_FUNC );
		CompileNode( root );
	}

	/*
		!!!!!!!!!!!!!!!!!!!!!!!!!!
		CHECK COMPILER ERRORS HERE
		!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/


	// check that compiled code has provided value for variable being compiled 
	cvTReferable<void, cvtNone> *p1 = NULL;
	itr = m_mapVars.find( name );
	if ( itr != m_mapVars.end() ) {
		pair<unsigned int, cvRefPtr> pr = itr->second;
		p1 = static_cast<cvTReferable<void, cvtNone>*> ( pr.second.get() );
		if ( p1->getType()== cvtNone ) {
			// error - code hasn't assign any value to variable being compiled 
			basic_stringstream<char> strErr;
			strErr << "ERROR: This code doesn't provide value for custom variable " << name;
			m_vecErrors.push_back( strErr.str() );
			return NULL;
		}
	}
	else{
		// internal error
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}
	m_Code.Add( *new cvVMCommand( CV::STOP) );


	m_Code.MakeJumpsRelative();


	p1->SetWeight( m_mapVars.GetReferencedWeight() );

	// "localize" variables
	cvTRefPtrByNameMap::iterator b = m_mapVars.begin();
	cvTRefPtrByNameMap::iterator e = m_mapVars.end();
	for ( ; b != e; b++ ) {
		pair<unsigned int, cvRefPtr> pr = b->second;
		pV = static_cast<cvTReferable<void, cvtNone>*> ( pr.second.get() );
		// only  non-system, referenced variables except being compiled
		if ( pV->GetRefsCount() && pV->m_name != name && !pV->IsSystem() ) 
			pV->m_name = name + "$"+ pV->m_name;
	}

	return p1;
}


bool CcvCompiler::IsNameFree( const string& s ){
	// !!!!!!!!!!!!
	// additionally, if this variable have aggregation on upper levels
	// we need to search these levels to insure that they do not have 
	// variables with name of s
	// !!!!!!!!!!!!
	cvTRefPtrByNameMap::iterator i = m_mapVars.find( s); 
	if( i == m_mapVars.end() )
		return true;		// variable not in map yet
	pair< unsigned int, cvRefPtr>& p = i->second;
	return !p.second->IsSystem();
}
void CcvCompiler::CollectConsts(cvScript_tree_iterator& treeNodeStart)
{
	cvScript_tree_iterator beginItr = treeNodeStart->children.begin();
	cvScript_tree_iterator endItr = treeNodeStart->children.end();
	while ( beginItr != endItr )  {
		if ( beginItr->value.id().to_long() &  PARSER_CONST ) {
			string strConstValue(beginItr->value.begin(), beginItr->value.end() ); 
			switch( beginItr->value.id().to_long() ) {
				case PARSER_BCONST:
					m_mapConsts.Add( strConstValue, cvBConstantPtr( new cvBConstant( strConstValue, strConstValue== "true" ? true : false) ) );
					break;
				case PARSER_SCONST:
					m_mapConsts.Add( strConstValue, cvSConstantPtr( new cvSConstant( strConstValue, strConstValue) ) );		
					break;
				default:
					m_mapConsts.Add( strConstValue,  cvNConstantPtr( new cvNConstant( strConstValue, atof(strConstValue.c_str()) ) ) );
			}
		}
		CollectConsts( beginItr );
		beginItr++;
	}
}

void CcvCompiler::DoListing(cvScriptParser& p, BSTR& s){
	
	ofstream lst;
	string lstfname = (LPCSTR)((_bstr_t)(s));
	lstfname += ".listing";
	lst.open( lstfname.c_str(), ios::out | ios::trunc );
	if ( lst ) {
		map<parser_id, string> rule_names;
		rule_names[PARSER_CONST] =	"CONST"     ;
		rule_names[PARSER_NCONST] = "NUMCONST" ; 
		rule_names[PARSER_BCONST]	=	"BOOLCONST" ;
		rule_names[PARSER_SCONST]	=	"STRINGCONST" ;
		rule_names[PARSER_FUNC] = "FUNCTION";
		rule_names[PARSER_VAR] = "VARIABLE";
		rule_names[PARSER_TERM] = "TERM";
		rule_names[PARSER_EXP] =	"EXPRESSION";
		rule_names[PARSER_CMP]=	"COMPARASION";		
		rule_names[PARSER_FACTOR] = "FACTOR";
		rule_names[PARSER_TERM] = "TERM";
		rule_names[PARSER_STMNT]=	"STATEMENT";
		rule_names[PARSER_ASSIGN] = "ASSIGNMENT";
		rule_names[PARSER_IF] = "IF";
		rule_names[PARSER_SCOPE] = "SCOPE";
		rule_names[PARSER_LOGIC] =	"LOGIC";
		rule_names[PARSER_PROGRAM] =	"PROGRAM";
		lst << "cvCompiler listing file for variable: " << string( (LPCSTR)((_bstr_t)s) )<< endl << endl;
		lst << "-----------------------------------------------------------------------" << endl;
		lst << "Context: " << c.m_strName << "[" << c.m_ID << "]" << endl << endl;
		cvLevelsByNameMap::const_iterator i = c.m_mapLevels.findByID( m_lLevel );
		lst << "-----------------------------------------------------------------------" << endl;
		lst << "Level: " << i->first << "[" << m_lLevel << "] " << endl<< endl;
		lst << "-----------------------------------------------------------------------" << endl;
		lst << "Source code:" << endl;
		lst << p.strTextToParse << endl<< endl;
		lst << "-----------------------------------------------------------------------" << endl;					
		lst << "Constants:" << endl;
		lst << m_mapConsts << endl << endl;
		lst << "-----------------------------------------------------------------------" << endl;					
		lst << "Functions:" << endl;
		lst << m_mapFuncs << endl << endl;					
		lst << "-----------------------------------------------------------------------" << endl;					
		lst << "Variables:" << endl;
		lst << m_mapVars << endl << endl;					
		lst << "-----------------------------------------------------------------------" << endl;					
		lst << "Code" << endl;
		for ( size_t i = 0; i < m_Code.size(); i++ ) {
			lst << static_cast<unsigned int>(i) <<"\t\t"<< m_Code[i] << endl;
		}
		lst << endl << endl;
		lst << "-----------------------------------------------------------------------" << endl;					
		lst << "cvScripParser Tree" << endl;
		tree_to_xml( lst, p.info.trees, p.strTextToParse, rule_names);
	}
}
