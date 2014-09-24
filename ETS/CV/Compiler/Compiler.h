// Compiler.h : Declaration of the CcvCompiler

#pragma once
#include "resource.h"       // main symbols

#include "cvCompiler.h"
#include "_IcvCompilerEvents_CP.h"
#include "..\cvCV.h"
#include <string>
#include <stack>

using namespace std;
using namespace EgLib;
using namespace CV;

// CcvCompiler

class ATL_NO_VTABLE CcvCompiler : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CcvCompiler, &CLSID_cvCompiler>,
	public ISupportErrorInfoImpl<&IID_IcvCompiler>,
	public IConnectionPointContainerImpl<CcvCompiler>,
	public CProxy_IcvCompilerEvents<CcvCompiler>, 
	public IDispatchImpl<IcvCompiler, &IID_IcvCompiler, &LIBID_cvCompilerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CcvCompiler():
		m_iContextID(0),
		m_bCommaAllowed(false),
		m_bPopAfterFunction(false),
		m_bLogicAllowed( false ),
		m_bBoolAllowed(false),
		m_uFCallParamsCount(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CVCOMPILER1)


BEGIN_COM_MAP(CcvCompiler)
	COM_INTERFACE_ENTRY(IcvCompiler)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CcvCompiler)
	CONNECTION_POINT_ENTRY(__uuidof(_IcvCompilerEvents))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(get_ConnectionString)(BSTR* pVal);
	STDMETHOD(put_ConnectionString)(BSTR newVal);
	STDMETHOD(Compile)(LONG iContext, LONG iLevel, BSTR strVarName, BSTR strCaption, BSTR strFormat, BSTR strSource, BSTR strDescription, SAFEARRAY ** arrAggregation, SAFEARRAY ** arrErrors, SAFEARRAY ** arrCode, LONG* iVariableID);
protected:
	_bstr_t m_strConnection;
private:

	vector<string>	m_vecErrors;

	typedef vector<unsigned int> JumpsVector;

	int m_iContextID;
	bool IsNameFree( const string& s );
	//  loads from database all context information regarding context: context info, levels description, system variables and function, custom variables
	bool LoadContext(int);
	LONG SaveCV( const string& name , LONG iContext, LONG iLevel, const string& strCaption, const string& strFormat, const string& strSource, const string& strAggregations, void* pCode, size_t szCodeSize, cvtType Type,int iWeight, const string& strDescription  );
	// builds map of system and custom variables that can be used in definition of variables for which compiler charged
	void FindUsefulVars(const string& );
	void FindUsefulFuncs();
	const cvTReferable<void, cvtNone>* const	compileCV(cvScript_tree_iterator&, const string& name );
	CDBConnection m_Conn;
	bool	m_bCommaAllowed;				// true if commas allowed in expression ( only in function parameters )
	bool	m_bPopAfterFunction;			// forced pop instruction after function call ( only if function STATEMENT( not function in expression compile ) )
	bool	m_bLogicAllowed;				// true if logic-relative operations allowed (comparison, and/or ) 
	bool	m_bBoolAllowed;				// true if usage of true/false allowed
	unsigned int	m_uFCallParamsCount;
public:
	
private:
	void DoListing(cvScriptParser& , BSTR& );
	void CollectConsts(cvScript_tree_iterator& t);
	void CcvCompiler::CompileFactor( cvScript_tree_iterator& t );
	void CompileNode( cvScript_tree_iterator& , bool bPop = false );
	void CompileConstant( cvScript_tree_iterator& );
	void CompileVariable( cvScript_tree_iterator& );
	void CompileAssignment( cvScript_tree_iterator& t, bool bPop = false );
	void CompileOperation( cvScript_tree_iterator& t );
	void CompileAddSub( cvScript_tree_iterator&);
	void CompileMulDiv( cvScript_tree_iterator&);
	void CompileComma( cvScript_tree_iterator&);
	void CompileFunction( cvScript_tree_iterator& t );
	void CompileLogic(cvScript_tree_iterator& t, JumpsVector *jmpsTrue, JumpsVector *jmpsFalse, bool bFromOr = false);
	void CompileComparison(cvScript_tree_iterator& t, JumpsVector *jmpsTrue, JumpsVector *jmpsFalse, bool bFromOr = false);
	void CompileIf( cvScript_tree_iterator& i );
	void CompileScope( cvScript_tree_iterator& i );
	cvTRefPtrByNameMap m_mapConsts;
	cvTRefPtrByNameMap m_mapVars;
	cvTRefPtrByNameMap m_mapFuncs;
	cvContext c;
	cvCode	m_Code;
	ULONG		m_lLevel;
	stack<cvtType>		m_TypeStack;
};

OBJECT_ENTRY_AUTO(__uuidof(cvCompiler), CcvCompiler)
