// Compiler.cpp : Implementation of CcvCompiler

#include "stdafx.h"
#include "cvParser.h"
#include "Compiler.h"
#include <string>
#include <map>
#include "..\cvCV.h"
#include <iostream>
//#include <fstream>
//#include <cassert>
//#include <iomanip>
#include <algorithm>
//#include <atlsafe.h>

using namespace std;
using namespace EgLib;
using namespace CV;

void CcvCompiler::CompileComparison(cvScript_tree_iterator& t, JumpsVector *jmpsTrue, JumpsVector *jmpsFalse, bool bFromOr ){
	cvScript_tree_iterator itrChild = t->children.begin();
	string strTokenValue = string( t->value.begin(), t->value.end() ); 

	if ( itrChild != t->children.end() ) {
		CompileNode( itrChild );
		itrChild++;
		CompileNode( itrChild );
		m_Code.Add( *new cvVMCommand(CV::CMP) );
		if ( strTokenValue== ">") {
			if ( bFromOr) 
				jmpsTrue->push_back( m_Code.Add( *new cvVMCommand(CV::jG) ) );
			else
				jmpsFalse->push_back( m_Code.Add( *new cvVMCommand(CV::jLE) ) );
		}
		else if ( strTokenValue== "<"){
			if ( bFromOr) 
				jmpsTrue->push_back( m_Code.Add( *new cvVMCommand(CV::jL) ) );
			else
				jmpsFalse->push_back( m_Code.Add( *new cvVMCommand(CV::jGE) ) );

		}
		else if ( strTokenValue== "!="){
			if ( bFromOr) 
				jmpsTrue->push_back( m_Code.Add( *new cvVMCommand(CV::jNE) ) );
			else
				jmpsFalse->push_back( m_Code.Add( *new cvVMCommand(CV::jE) ) );
		}
		else if ( strTokenValue== "<="){
			if ( bFromOr) 
				jmpsTrue->push_back( m_Code.Add( *new cvVMCommand(CV::jLE) ) );
			else
				jmpsFalse->push_back( m_Code.Add( *new cvVMCommand(CV::jG) ) );
		}
		else if ( strTokenValue== ">="){
			if ( bFromOr) 
				jmpsTrue->push_back( m_Code.Add( *new cvVMCommand(CV::jGE) ) );
			else
				jmpsFalse->push_back( m_Code.Add( *new cvVMCommand(CV::jL) ) );
		}
		else if ( strTokenValue== "=="){
			if ( bFromOr) 
				jmpsTrue->push_back( m_Code.Add( *new cvVMCommand(CV::jE) ) );
			else
				jmpsFalse->push_back( m_Code.Add( *new cvVMCommand(CV::jNE) ) );
		}
	}
	else{
		// error - no enough operands for comparison
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}
}
void CcvCompiler::CompileIf( cvScript_tree_iterator& t ){
	cvScript_tree_iterator itrChild = t->children.begin();
	cvScript_tree_iterator itrLastChild = t->children.end();
	string v;
	v = string( itrChild->value.begin(), itrChild->value.end() );			
	JumpsVector	jmpIn;
	JumpsVector	jmpOut;

	// compile condition
	if( itrChild != itrLastChild ) {
		if ( itrChild->value.id() == PARSER_CMP ) {		
			CompileComparison( itrChild, &jmpIn, &jmpOut );
		}
		else{ 
			if ( itrChild->value.id() == PARSER_LOGIC ) {
				CompileLogic( itrChild, &jmpIn, &jmpOut );
			}
			else {
				CompileNode( itrChild );
				m_Code.Add( *new cvVMCommand(CV::cmpZ) );
				jmpOut.push_back( m_Code.Add( *new cvVMCommand(CV::jE) ) );
			}
		}
	}
	else{
		// error - bad if operator
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}
	v = string( itrChild->value.begin(), itrChild->value.end() );			
	if ( itrChild != itrLastChild ) 
		itrChild++;
	v = string( itrChild->value.begin(), itrChild->value.end() );			
	for ( unsigned int i = 0; i < jmpIn.size(); i++ ) 
		m_Code[jmpIn[i]].m_Operand = static_cast<unsigned int>(m_Code.size());
	CompileNode( itrChild );																		// if true
	if(itrChild != itrLastChild )
		itrChild++;	
	if ( itrChild != itrLastChild ) {

		v = string( itrChild->value.begin(), itrChild->value.end() );			
		unsigned int uOverElseJump = m_Code.Add( *new cvVMCommand( CV::JMP) );
		for ( unsigned int i = 0; i < jmpOut.size(); i++ ) 
			m_Code[jmpOut[i]].m_Operand = static_cast<unsigned int>(m_Code.size());
		CompileNode( itrChild );
		m_Code[ uOverElseJump].m_Operand = static_cast<unsigned int>(m_Code.size());
	}
	else{
		for ( unsigned int i = 0; i < jmpOut.size(); i++ ) 
			m_Code[jmpOut[i]].m_Operand = static_cast<unsigned int>(m_Code.size());
	}
}
void CcvCompiler::CompileLogic(cvScript_tree_iterator& t, JumpsVector *jmpsTrue, JumpsVector *jmpsFalse, bool bFromOr ){
	cvScript_tree_iterator itrChild = t->children.begin();
	string strTokenValue = string( t->value.begin(), t->value.end() ); 			

	if ( itrChild == t->children.end() ) {
		// error - no enough operands
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}

	if ( strTokenValue== "or" ) {
		JumpsVector jmpOR;
		// left part of expression
		if (itrChild->value.id() == PARSER_LOGIC) {
			CompileLogic( itrChild, jmpsTrue, &jmpOR /*, true*/ );
		}
		else {
			if (itrChild->value.id() == PARSER_CMP ) {
				CompileComparison( itrChild, jmpsTrue, &jmpOR/*, true*/ );
			}
			else {
				CompileNode( itrChild );
				m_Code.Add( *new cvVMCommand(CV::cmpZ) );
				jmpsTrue->push_back( m_Code.Add( *new cvVMCommand(CV::jNE) ) );			// jump to true if not 0
			}
		}
		for ( unsigned int i = 0; i < jmpOR.size(); i++ ) 
			m_Code[jmpOR[i]].m_Operand = static_cast<unsigned int>(m_Code.size());

		itrChild++;																					// right part of expression
		if (itrChild->value.id() == PARSER_LOGIC) {
			CompileLogic( itrChild, jmpsTrue, jmpsFalse, true );
		}
		else {
			if (itrChild->value.id() == PARSER_CMP) {
				CompileComparison( itrChild, jmpsTrue, jmpsFalse );
			}
			else {
				CompileNode( itrChild );
				m_Code.Add( *new cvVMCommand(CV::cmpZ) );
				jmpsFalse->push_back( m_Code.Add( *new cvVMCommand(CV::jE) ) );	// jump to false if 0
			}
		}
	}
	else if ( strTokenValue== "and" ) {
		JumpsVector jmpAND;

		if (itrChild->value.id() == PARSER_LOGIC) {
			CompileLogic( itrChild, &jmpAND, jmpsFalse);
		}
		else {
			if (itrChild->value.id() == PARSER_CMP) {
				CompileComparison( itrChild, &jmpAND, jmpsFalse );
			}
			else {
				CompileNode( itrChild );
				m_Code.Add( *new cvVMCommand(CV::cmpZ) );
				jmpsFalse->push_back( m_Code.Add( *new cvVMCommand(CV::jE) ) );		// jump to false if 0
			}
		}

		for ( unsigned int i = 0; i < jmpAND.size(); i++ ) 
			m_Code[jmpAND[i]].m_Operand = static_cast<unsigned int>(m_Code.size());

		itrChild++;

		if (itrChild->value.id() == PARSER_LOGIC) {
			CompileLogic( itrChild, jmpsTrue, jmpsFalse );
		}
		else{
			if (itrChild->value.id() == PARSER_CMP ) {
				CompileComparison( itrChild, jmpsTrue, jmpsFalse );
			}
			else {
				CompileNode( itrChild );
				m_Code.Add( *new cvVMCommand(CV::cmpZ) );
				jmpsFalse->push_back( m_Code.Add( *new cvVMCommand(CV::jE) ) );	// jump to false if 0
			}
		}
	}
	else{
		// error - bad logic
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}
}
void CcvCompiler::CompileScope( cvScript_tree_iterator& i ){
	cvScript_tree_iterator cb = i->children.begin();
	cvScript_tree_iterator ce = i->children.end();

	while ( cb != ce ) {
		m_bPopAfterFunction =  ( cb->value.id() == PARSER_FUNC );
		CompileNode( cb );
		cb++;
	}
}

void CcvCompiler::CompileFactor( cvScript_tree_iterator& t ) {
	// get factor value
	string strTokenValue = string( t->value.begin(), t->value.end() ); 

	// the only supported factor - unary minus
	if ( strTokenValue == "-" ) {
		cvScript_tree_iterator itrChild = t->children.begin() ;
		if ( itrChild != t->children.end() ) {
			CompileNode( itrChild );
			if ( m_TypeStack.size() > 0 ) {
				cvtType& cvt1 = m_TypeStack.top();		
				if ( cvt1 == cvtNumeric ) {
					m_Code.Add( *new cvVMCommand( CV::NEG ) );
				}
				else{
					// error - negating of non-numeric type is prohibited
					m_TypeStack.pop();
					m_vecErrors.push_back(string("Trying to negate non-numeric expression") );
				}
			}
			else{
				// error - internal, no type info for unari minus operation
				EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
			}
		}
		else{
			// error - internal, bad number of operands for unary minus
			EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
		}
	}
	else{
		// error - internal, unknown factor token
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}
}

void CcvCompiler::CompileNode( cvScript_tree_iterator& t, bool bPop ){
	switch( t->value.id().to_long() ) {
		case PARSER_ASSIGN:
			CompileAssignment( t, bPop );
			break;
		case PARSER_NCONST:
		case PARSER_BCONST:
		case PARSER_SCONST:
			CompileConstant( t );
			break;
		case PARSER_VAR:
			CompileVariable( t );
			break;
		case PARSER_EXP:
		case PARSER_TERM:
			CompileOperation( t );
			break;
		case PARSER_FUNC:
			CompileFunction( t);
			break;
		case PARSER_IF:
			CompileIf( t );
			break;
		case PARSER_SCOPE:
			CompileScope( t);
			break;
		case PARSER_FACTOR:
			CompileFactor( t );
		default:	 ;
	}
}

void CcvCompiler::CompileConstant( cvScript_tree_iterator& t ){
	string strTokenValue = string( t->value.begin(), t->value.end() ); 
	cvTRefPtrByNameMap::iterator itr = m_mapConsts.find( strTokenValue );
	if ( itr != m_mapConsts.end() ) {
		pair<unsigned int, cvRefPtr >& p = itr->second;
		p.second->Add(	m_Code.Add( *new cvVMCommand( CV::pushC, p.first) ) );
		cvNConstant *c = static_cast<cvNConstant*>( p.second.get() );
		m_TypeStack.push( c->getType() );
	}
}

void CcvCompiler::CompileFunction( cvScript_tree_iterator& t ){
	string strTokenValue = string( t->value.begin(), t->value.end() ); 
	cvTRefPtrByNameMap::iterator itr = m_mapFuncs.find( strTokenValue );
	if ( itr != m_mapFuncs.end() ) {

		// find system function object being called
		pair<unsigned int, cvRefPtr> pr = itr->second;

		// get level
		cvLevelsByNameMap::const_iterator itrLevel = c.m_mapLevels.findByID( pr.second->GetLevel() );
		if ( itrLevel == c.m_mapLevels.end() ) {
			// error - internal, function being called not found in available system function list
			EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
		}

		// get system function object
		cvSysTypedObjByNameMap::iterator itrSysFunc = itrLevel->second->m_mapSysFunc.find( strTokenValue );

		if ( itrSysFunc == itrLevel->second->m_mapSysFunc.end() ) {
			// error - - internal, function being called not found in available system function list
			EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
		}

		cvSysFuncPtr pSysFunc = shared_dynamic_cast<cvSysFunc>( itrSysFunc->second );

		// get expected parameters count
		unsigned int uExpectParamsCount = static_cast<unsigned int>(pSysFunc->m_vecParamTypes.size() );
		unsigned int uSaveFCallParamsCount = m_uFCallParamsCount;
		m_uFCallParamsCount = 0;

		cvScript_tree_iterator itrChild = t->children.begin() ;
		cvScript_tree_iterator itrLastChild = t->children.end() ;

		// determine is there parameters list passed to the function or just one parameter
		if ( itrChild != itrLastChild ) {
			string strChildValue = string( itrChild->value.begin(), itrChild->value.end() ); 
			if ( strChildValue != "," ) 
				m_uFCallParamsCount = 1;	// else - got a list, comma operator will set number of actual parameters passed to function
		}

		bool bSavePopAfterCall = m_bPopAfterFunction;
		bool bCommaAllowed = m_bCommaAllowed;
		m_bPopAfterFunction = false;
		m_bCommaAllowed = true;
		while ( itrChild != itrLastChild ){
			CompileNode( itrChild++);
		}
		m_bPopAfterFunction = bSavePopAfterCall;
		m_bCommaAllowed = bCommaAllowed ;

		// now check parameters count and types
		if ( uExpectParamsCount != m_uFCallParamsCount || m_TypeStack.size() < static_cast<size_t>( uExpectParamsCount ) ) {
			// error - wrong parameters number
			m_uFCallParamsCount = uSaveFCallParamsCount;
			basic_stringstream<char> strErr;
			strErr << "ERROR(" << t->value.begin().get_position().line << "): Function " << strTokenValue << " called with wrong parameters count";
			m_vecErrors.push_back( strErr.str() );
			
		}


		// check types 
		cvtType cvtParamType = cvtNone;
		stack<cvtType>	ParamsTypesStack;
		for ( unsigned int uActualParamCount= 0; uActualParamCount < uExpectParamsCount && uExpectParamsCount == m_uFCallParamsCount ; uActualParamCount++ ) {
			ParamsTypesStack.push( m_TypeStack.top() );
			m_TypeStack.pop();
		}

		for ( size_t Param = 0; Param < pSysFunc->m_vecParamTypes.size() && uExpectParamsCount == m_uFCallParamsCount ; Param++ ) {
			if ( ParamsTypesStack.top() != pSysFunc->m_vecParamTypes[ Param ] ) {
				// error - parameter of wrong type passed to the function
				m_uFCallParamsCount = uSaveFCallParamsCount;
				basic_stringstream<char> strErr;
				strErr << "ERROR(" << t->value.begin().get_position().line << "): Incorrect parameters types for function " << strTokenValue;
				m_vecErrors.push_back( strErr.str() );
				break;
			}
			ParamsTypesStack.pop();
		}


		pair<unsigned int, cvRefPtr >& p = itr->second;
		cvVMOpCode CallOpCOde = CV::callF;
		CallOpCOde = static_cast<cvVMOpCode>( static_cast<unsigned int>( CallOpCOde) | m_uFCallParamsCount );
		p.second->Add(	m_Code.Add( *new cvVMCommand( CallOpCOde, p.first) ) );
		cvTReferable<double, cvtNumeric> *f = static_cast<cvTReferable<double, cvtNumeric>*>(p.second.get());
		if ( m_bPopAfterFunction) 
			m_Code.Add( *new cvVMCommand( CV::POP ) );		// pop return value from stack 
		else
			m_TypeStack.push( f->getType() );					// push function type into types stack
		m_uFCallParamsCount = uSaveFCallParamsCount;
	}
}

void CcvCompiler::CompileAssignment( cvScript_tree_iterator& t, bool bPop ){
	cvScript_tree_iterator itrChild = t->children.begin();
	if (	
		t->children.begin() != t->children.end()		/* there are should be children in tree for this token*/ 
		&& itrChild->value.id() == PARSER_VAR				/* first children should be variable */
		) {
			// get name of variable to be assigned 
			string strTokenValue = string( itrChild->value.begin(), itrChild->value.end() ); 
			// check that lvalue is a local variable
			if ( IsNameFree( strTokenValue) ) {
				// find variable in local variables map
				cvTRefPtrByNameMap::iterator i = m_mapVars.find(strTokenValue);
				if ( i == m_mapVars.end() ) {
					// first time 
					cvTReferablePtr<void, cvtNone> p = cvTReferablePtr<void, cvtNone>( new cvTReferable<void, cvtNone>(strTokenValue) );
					m_mapVars.Add( strTokenValue, p);
				}

				// compile rvalue
				itrChild++;
				// as rvalue we can obtain constant, variable, function, term, factor  or expression
				if ( true/*t->m_pChildren->m_pNext && t->m_pChildren->m_pNext->m_Type != _token::_end*/ ) {
					CompileNode( itrChild, itrChild->value.id() == PARSER_ASSIGN  );
				}		
				else{
					// error - missed rvalue or bad
					EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
				}

				// compile lvalue
				i = m_mapVars.find( strTokenValue);
				pair< unsigned int, cvRefPtr>& pr = i->second;
				cvTReferable<void, cvtNone> *pV = static_cast<cvTReferable<void, cvtNone>*>( pr.second.get() );
				if ( m_TypeStack.size() > 0 ) {
					cvtType& cvt1 = m_TypeStack.top();
					if (pV->getType() == cvt1 || pV->getType() == cvtNone ) {
						pV->Add( m_Code.Add( *new cvVMCommand( bPop ? CV::stoV : CV::popV, pr.first)));
						if( !bPop )
							m_TypeStack.pop();
						pV->setType( cvt1 );
					}
					else{
						// error - can not assign to variable value of different type
						basic_stringstream<char> strErr;
						strErr << "ERROR(" << t->value.begin().get_position().line << "): Variable" << strTokenValue << " can not be assigned due of type difference";
						m_vecErrors.push_back( strErr.str() );
					}
				}
				else{
					// error - no type for lvalue	in stack
					EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
				}
			}
			else{
				// error - can not assign to variable value of different type
				basic_stringstream<char> strErr;
				strErr << "ERROR(" << t->value.begin().get_position().line << "): Variable" << strTokenValue << " can not be assigned ";
				m_vecErrors.push_back( strErr.str() );
			}
		}
	else{
		// error - bad lvalue
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}
}

void CcvCompiler::CompileVariable( cvScript_tree_iterator& t ){
	string strTokenValue = string( t->value.begin(), t->value.end() ); 

	cvTRefPtrByNameMap::iterator itr = m_mapVars.find( strTokenValue );
	if ( itr != m_mapVars.end() ) {
		pair<unsigned int, cvRefPtr >& p = itr->second;
		cvTReferable<double, cvtNumeric>* o = static_cast<cvTReferable<double, cvtNumeric>*>( p.second.get() );
		if ( o->IsSystem() ) {
			// system variable
			if ( o->getType() != cvtNone ) {
				if (o->IsGlobal()) 
					p.second->Add(	m_Code.Add( *new cvVMCommand( CV::pushG, p.first) ) );
				else
					p.second->Add(	m_Code.Add( *new cvVMCommand( CV::pushS, p.first) ) );
				m_TypeStack.push( o->getType() );
			}
			else{
				// error - uninitialized system variable used in expression
				// should never be there, only mistake in database can drive to here
				EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
			}

		}
		else{
			// user variable
			cvTReferable<void, cvtNone> *v = static_cast<cvTReferable<void, cvtNone> *>(p.second.get());
			if ( v->getType() != cvtNone ){		
				p.second->Add(	m_Code.Add( *new cvVMCommand( CV::pushV, p.first) ) );
				m_TypeStack.push( v->getType() );
			}
			else{
				// error - uninitialized variable used in expression
				// practically, should never be there. 
				// all uninitialized variables should be filtered out by first 'if'
				basic_stringstream<char> strErr;
				strErr << "ERROR(" << t->value.begin().get_position().line << "): Variable " << strTokenValue << " is used without being initialized";
				m_vecErrors.push_back( strErr.str() );
			}
		}
	}
	else{
		// error - uninitialized variable used in expression
		// report error
		basic_stringstream<char> strErr;
		strErr << "ERROR(" << t->value.begin().get_position().line << "): Variable " << strTokenValue << " is used without being initialized";
		m_vecErrors.push_back( strErr.str() );
	}
}

void CcvCompiler::CompileOperation( cvScript_tree_iterator& t ){
	string strTokenValue( t->value.begin(), t->value.end() );
	if ( t->value.id() == PARSER_EXP ) {
		if ( strTokenValue == "," ) {
			if ( m_bCommaAllowed) 
				CompileComma( t );
			else{
				// error - no comma allowed in this statement
				EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
			}
		}
		else if ( t->value.id() == PARSER_CMP || t->value.id() == PARSER_LOGIC ) {
			// error - logic or compare operation are not allowed here  
			EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
		}
		else 
			CompileAddSub( t );
	}
	else if ( t->value.id() == PARSER_TERM ) {
		CompileMulDiv( t );
	}
}

void CcvCompiler::CompileAddSub( cvScript_tree_iterator& t){
	string strTokenValue = string( t->value.begin(), t->value.end() ); 
	cvScript_tree_iterator itrChild = t->children.begin() ;

	if (	t->children.begin() != t->children.end() ) {
		CompileNode( itrChild);
		itrChild++;
		CompileNode( itrChild);
		if ( m_TypeStack.size() > 1 ) {
			cvtType& cvt1 = m_TypeStack.top();
			m_TypeStack.pop();
			cvtType& cvt2 = m_TypeStack.top();
			m_TypeStack.pop();
			if ( cvt1 == cvt2 && cvt1 != cvtNone ) {
				if ( cvt1 == cvtNumeric || ( cvt1 == cvtString && strTokenValue == "+") ) {
					m_Code.Add( *new cvVMCommand(strTokenValue == "+" ? CV::ADD : CV::SUB ) );
					m_TypeStack.push( cvt1 );
				}
				else{
					// error - all arithmetic operations on booleans and strings, except for '+' on strings, are prohibited
					basic_stringstream<char> strErr;
					strErr << "ERROR(" << t->value.begin().get_position().line << "): all arithmetic operations on booleans and strings, except for '+' on strings, are prohibited";
					m_vecErrors.push_back( strErr.str() );
					return;
				}
			}
			else{
				// error - either one operand of operation is not initialized or type of operands
				// are different
				basic_stringstream<char> strErr;
				strErr << "ERROR(" << t->value.begin().get_position().line << "): either one operand of addition or subtraction is not initialized or type of operands are different";
				m_vecErrors.push_back( strErr.str() );
				return;
			}
		}
		else{
			// error - internal, no enough operands for operation
			EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
		}
	}
	else{
		// error - bad number of operands 
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}
}

void CcvCompiler::CompileMulDiv(cvScript_tree_iterator& t){
	cvVMOpCode opcode = CV::MUL;
	string strTokenValue = string( t->value.begin(), t->value.end() ); 
	cvScript_tree_iterator itrChild = t->children.begin();

	if ( strTokenValue == "*" ) 
		opcode = CV::MUL;
	else if ( strTokenValue == "\\" ) 
		opcode = CV::iDIV;
	else if ( strTokenValue  == "/" ) 
		opcode = CV::DIV;
	else if ( strTokenValue  == "%" ) 
		opcode =CV::MOD;

	if ( t->children.begin() != t->children.end() ) {
		CompileNode( itrChild);
		itrChild++;
		CompileNode( itrChild);
		if ( m_TypeStack.size() > 1 ) {
			cvtType& cvt1 = m_TypeStack.top();
			m_TypeStack.pop();
			cvtType& cvt2 = m_TypeStack.top();
			m_TypeStack.pop();
			if ( cvt1 == cvt2 && cvt1 != cvtNone ) {
				if ( cvt1 == cvtNumeric ) {
					m_Code.Add( *new cvVMCommand( opcode ) );
					m_TypeStack.push( cvt1 );
				}
				else{
					// error - all arithmetic operations on booleans and strings, except for '+' on strings, are prohibited
					basic_stringstream<char> strErr;
					strErr << "ERROR(" << t->value.begin().get_position().line << "): all arithmetic operations on booleans and strings, except for '+' on strings, are prohibited";
					m_vecErrors.push_back( strErr.str() );
					return;
				}
			}
			else{
				// error - either one operand of operation is not initialized or type of operands
				// are different
				basic_stringstream<char> strErr;
				strErr << "ERROR(" << t->value.begin().get_position().line << "): either one operand of multiplication or division is not initialized or type of operands are different";
				m_vecErrors.push_back( strErr.str() );
				return;
			}
		}
		else{
			// error - internal, no enough operands for operation
			EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
		}
	}
	else{
		// error - bad number of operands 
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}
}

void CcvCompiler::CompileComma( cvScript_tree_iterator& t){
	/*
	comma tokens should contain 2 children.
	expression 'a, b, c' presented as
	,(root)
	|--a
	|__,
	|--b
	|__c

	in parsing tree. checking of existence of expression after comma performed by parser
	*/
	string strTokenValue = string( t->value.begin(), t->value.end() ); 
	cvScript_tree_iterator itrChild = t->children.begin() ;		// first child

	if (	t->children.begin() != t->children.end() ) {
		CompileNode( itrChild);
		m_uFCallParamsCount++;
		itrChild++;
		if ( itrChild != t->children.end() ) {
			string strChildValue = string( itrChild->value.begin(), itrChild->value.end() ); 
			if ( strChildValue != "," ) 
				m_uFCallParamsCount++;
			CompileNode( itrChild);
		}
		else{
			// error - expression missed after comma
			EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
		}
	}
	else{
		// error - bad number of operands 
		EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Internal error") );
	}
}

