#include "stdafx.h"
#include <string>
#include <vector>
#include <sstream>
#include "cvParser.h"

/*
SEMANTIC ACTIONS FOR GRAMMAR
*/

// remembers starting position of current statement
void sa_cvStmntBegin( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last){
	const_cast<cvScriptParser*>(p)->itrCurrentStmnt = first;	
}

void sa_cvStmntEOF( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last){
	//const_cast<cvScriptParser*>(p)->itrCurrentStmnt = first;	
	//return;
}
void sa_errRCBracketMiss( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last){
	basic_stringstream<char>	sErr;
	string str(pParser->itrCurrentStmnt, last );
	string err;

	sErr << "Line " << first.get_position().line << ": Curly bracket missed near\'" << str << "\'";
	const_cast<cvScriptParser*>(pParser)->m_vecErrors.push_back( sErr.str() );

	// will continue parsing from where error was
	const_cast<cvScriptParser*>(pParser)->itrStart = first;

	// stop current parsing
	//cerr << sErr.str();
	throw 1;	

}

void sa_errLCBracketMiss( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last){
	basic_stringstream<char>	sErr;
	string str(pParser->itrCurrentStmnt, last );
	string err;

	sErr << "Line " << first.get_position().line << ": Curly bracket missed near\'" << str << "\'";
	const_cast<cvScriptParser*>(pParser)->m_vecErrors.push_back( sErr.str() );

	// will continue parsing from where error was
	const_cast<cvScriptParser*>(pParser)->itrStart = first;

	// stop current parsing
	//cerr << sErr.str();
	throw 1;	

}

void sa_errRBracketMiss( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last){
	basic_stringstream<char>	sErr;
	string str(pParser->itrCurrentStmnt, last );
	string err;

	sErr << "Line " << first.get_position().line << ": Bracket missed near\'" << str << "\'";
	const_cast<cvScriptParser*>(pParser)->m_vecErrors.push_back( sErr.str() );

	// will continue parsing from where error was
	const_cast<cvScriptParser*>(pParser)->itrStart = first;

	// stop current parsing
	//cerr << sErr.str();
	throw 1;	

}

void sa_errLBracketMiss( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last){
	basic_stringstream<char>	sErr;
	string str(pParser->itrCurrentStmnt, last );
	string err;

	sErr << "Line " << first.get_position().line << ": Bracket missed near\'" << str << "\'";
	const_cast<cvScriptParser*>(pParser)->m_vecErrors.push_back( sErr.str() );

	// will continue parsing from where error was
	const_cast<cvScriptParser*>(pParser)->itrStart = first;

	// stop current parsing
	//cerr << sErr.str();
	throw 1;	

}
void sa_errExpMiss( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last){
	basic_stringstream<char>	sErr;
	string str(pParser->itrCurrentStmnt, last );
	string err;

	sErr << "Line " << first.get_position().line << ": Expression missed near\'" << str << "\'";
	const_cast<cvScriptParser*>(pParser)->m_vecErrors.push_back( sErr.str() );

	// will continue parsing from where error was
	const_cast<cvScriptParser*>(pParser)->itrStart = first;

	// stop current parsing
	//cerr << sErr.str();
	throw 1;	
}

//	reporting error
void sa_errSemiMiss( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last )
{
	basic_stringstream<char>	sErr;
	string str(pParser->itrCurrentStmnt, last );
	string err;

	sErr << "Line " << first.get_position().line << ": ';' expected after \'" << str << "\'";
	const_cast<cvScriptParser*>(pParser)->m_vecErrors.push_back( sErr.str() );

	// will continue parsing from where error was
	const_cast<cvScriptParser*>(pParser)->itrStart = first;

	// stop current parsing
	//cerr << sErr.str();
	throw 1;	
}

void sa_no_bracket(const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last ){
	basic_stringstream<char>	sErr;
	string str(pParser->itrCurrentStmnt, last );
	string err;

	sErr << "Line " << first.get_position().line << ": ')' expected after \'" << str << "\'";
	const_cast<cvScriptParser*>(pParser)->m_vecErrors.push_back( sErr.str() );

	// will continue parsing from where error was
	const_cast<cvScriptParser*>(pParser)->itrStart = first;

	// stop current parsing
	//cerr << sErr.str();
	throw 1;	
}

void sa_errSyntax(const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last ){
	basic_stringstream<char>	sErr;
	string str(pParser->itrCurrentStmnt, last );
	string err;

	sErr << "Line " << first.get_position().line << ": Syntax error";
	const_cast<cvScriptParser*>(pParser)->m_vecErrors.push_back( sErr.str() );

	// will continue parsing from where error was
	const_cast<cvScriptParser*>(pParser)->itrStart = first;

	//cerr << sErr.str();

	// stop current parsing
	if( !pParser->m_bInScope)
		throw PARSER_EX_EOF;	
}

void sa_errAssignExpected( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last){
	basic_stringstream<char>	sErr;
	string str(pParser->itrCurrentStmnt, last );
	string err;

	sErr << "Line " << first.get_position().line << ": '=' expected after \'" << str << "\'";
	const_cast<cvScriptParser*>(pParser)->m_vecErrors.push_back( sErr.str() );

	// will continue parsing from where error was
	const_cast<cvScriptParser*>(pParser)->itrStart = first;

	// stop current parsing
	//cerr << sErr.str();
	throw 1;	
}

//	reporting error
void sa_EnterScope( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last )
{
	const_cast<cvScriptParser*>(pParser)->m_bInScope = true;	
}
void sa_LeaveScope( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last )
{
	const_cast<cvScriptParser*>(pParser)->m_bInScope = false;	
}

