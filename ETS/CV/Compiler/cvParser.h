#include "stdafx.h"
#include <stdio.h>
//#define BOOST_SPIRIT_DEBUG	
//#define BOOST_SPIRIT_DEBUG_FLAGS ( BOOST_SPIRIT_DEBUG_FLAGS_NODES )
#include "stdafx.h"
#include <string>
#include <vector>
#include <boost/spirit.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/ast.hpp>
#include <boost/spirit/tree/tree_to_xml.hpp>
#include <functional>
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <sstream>

#define __rule( id, name ) rule<ScannerT, parser_context<>, parser_tag<id> >  name
#define __debug_rule( name )  BOOST_SPIRIT_DEBUG_NODE( name )

#define	PARSER_CONST		( 1 << 31 )
#define	PARSER_NCONST		PARSER_CONST | 1
#define	PARSER_BCONST		PARSER_CONST | 2
#define	PARSER_SCONST		PARSER_CONST | 3
#define	PARSER_IF			4
#define	PARSER_VAR			1
#define	PARSER_FUNC			5
#define	PARSER_EXP			6
#define	PARSER_LOGIC		7
#define	PARSER_CMP			8
#define	PARSER_TERM			9
#define	PARSER_ATOM			10
#define	PARSER_FACTOR		11
#define	PARSER_ASSIGN		12
#define	PARSER_STMNT		13
#define	PARSER_SCOPE		14
#define	PARSER_PROGRAM		15

#define	PARSER_EX_EOF			0x0fac

#define	CV_ERR( SA ) (    ( eps_p ) [ bind( &SA,&self, _1, _2 ) ]   )

using namespace std;
using namespace boost::spirit;
using boost::mem_fn;

typedef position_iterator<char const*>	cvScript_iterator;
typedef tree_match<cvScript_iterator, node_iter_data_factory<> > cvScriptParser_tree_match;
typedef cvScriptParser_tree_match::tree_iterator cvScript_tree_iterator;

struct cvScriptParser;

void	sa_cvStmntBegin(	const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last );
void	sa_errSemiMiss(	const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last );
void	sa_no_bracket(		const cvScriptParser *pParser,cvScript_iterator first, cvScript_iterator last );
void	sa_errRCBracketMiss( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last);
void	sa_errLCBracketMiss( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last);
void	sa_errRBracketMiss( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last);
void	sa_errLBracketMiss( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last);
void	sa_errExpMiss( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last);
void	sa_errSyntax( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last);
void	sa_errAssignExpected( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last);
void	sa_EnterScope( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last );
void	sa_LeaveScope( const cvScriptParser *pParser, cvScript_iterator first, cvScript_iterator last );
void	sa_cvStmntEOF( const cvScriptParser *p, cvScript_iterator first, cvScript_iterator last);

/*

GRAMMAR DEFINITION
*/



struct cvScriptParser : public grammar<cvScriptParser>{
private:
	struct cvFunctionNames : public symbols<>{};	
	cvScript_iterator m_ErrPos;

public:
	vector<string>									m_vecErrors;
	cvScript_iterator								itrStart;
	cvScript_iterator								itrEnd;
	cvScript_iterator								itrCurrentStmnt;
	tree_parse_info<cvScript_iterator, node_iter_data_factory<> >		info;
	string											strTextToParse;
	cvFunctionNames								cvFunctionName;
	bool												m_bInScope;
	void PrepareParsing( const string& s ){
		m_bInScope = false;
		strTextToParse = s;
		itrStart = cvScript_iterator( strTextToParse.c_str(), strTextToParse.c_str() + strTextToParse.length() );
		itrEnd = cvScript_iterator( strTextToParse.c_str() + strTextToParse.length(), strTextToParse.c_str() + strTextToParse.length() );
	}

	void	Parse(){
		//while( !info.full && itrStart != itrEnd ){
			try{
				info = ast_parse< node_iter_data_factory<> >( itrStart, itrEnd, *this, space_p );
				/*if( info.full)
					break;
				itrStart++;*/
			}
			catch( int ){
				// semantic action has raise an exception
				// typically this means an error occurred during parsing
				//if ( e == PARSER_EX_EOF ) 
				//	return;
				return;
			}
		//}
	}

	template<typename ScannerT>
	struct definition{
		definition( const cvScriptParser& self )
		{
			/*boolean constant */
			BConst		=	leaf_node_d[
				as_lower_d[
					str_p("true") | str_p("false")
				]
			];

			/*numeric constant*/
			NConst		=	leaf_node_d[
				lexeme_d[	
					real_p 
				]
			];

			/*string constant */
			SConst		=	leaf_node_d[
				no_node_d[ 
					ch_p('\"')
				] >> *( 
				anychar_p-ch_p('\n') - ch_p( '\"')
				) 
				>> 
				no_node_d[
					ch_p('\"')
				] 
			];

			/*constant*/
			Constant		=	NConst | BConst | SConst;

			/*variable*/
			Var		=	leaf_node_d[
				lexeme_d[
					+( alpha_p | ch_p('_') ) >> *( digit_p | alpha_p | ch_p('_') )
				]
			];
			/*function*/
			Func		=	root_node_d[ self.cvFunctionName	 ]	>> 
				inner_node_d[ 
					( ch_p('(') >> !( Exp ) >> ch_p( ')' ) ) 
				];

			/*atom*/
			Atom		=	Constant | Func | Var;	

			Factor	=	Atom																					|
				inner_node_d[		ch_p('(') >> Exp >> /*(*/ ch_p(')') /*| CV_ERR(sa_errRBracketMiss)  )*/		]					|
				(	root_node_d[	ch_p('-')	] >> /*(*/ Factor /*| CV_ERR(sa_errExpMiss)	)*/ ) ;

			Term		=	Factor >>
				*(
				(	root_node_d[	ch_p('*')	]	>>	/*(*/Factor /*| CV_ERR(sa_errExpMiss) 	)*/)	|
				(	root_node_d[	ch_p('/')	]	>>	/*(*/Factor /*| CV_ERR(sa_errExpMiss)  )*/)	
				);

			Exp		=	Term >>
				*( 
				( root_node_d[ ch_p( '+' ) ]	>> /*(*/ Term /*| CV_ERR(sa_errExpMiss) )*/ )	| 
				( root_node_d[ ch_p( '-' ) ]	>> /*(*/ Term /*| CV_ERR(sa_errExpMiss) ) */)	|
				( root_node_d[ Cmp ]			   >> /*(*/ Term /*| CV_ERR(sa_errExpMiss) ) */)	|
				( root_node_d[ Logic ]		   >> /*(*/ Exp  /*| CV_ERR(sa_errExpMiss) )*/ )	|
				( root_node_d[ ch_p( ',' ) ]	>> /*(*/ Exp  /*| CV_ERR(sa_errExpMiss) )*/ )  
				);

			Logic		=	as_lower_d[ 
				lexeme_d[ 
					str_p( "and" ) | str_p( "or" ) 
				]
			];

			Cmp		=	str_p( "==" ) | str_p( "<=" ) | str_p( ">=" ) | str_p( "!=" ) | ch_p( '<' )	| ch_p( '>' );

			Assign	=  Var >> 
				/*(*/ root_node_d[ ch_p( '=' ) ] /*| CV_ERR( sa_errAssignExpected ) )*/
				>> Exp >> 
				/*(*/
				no_node_d[ ch_p( ';' ) ]	/*|
				CV_ERR(sa_errSemiMiss)
				)*/;

			Stmnt		=	CV_ERR(sa_cvStmntBegin) >>	/*save start position*/
				(
				/*( end_p >> CV_ERR(sa_cvStmntEOF )		)															|*/
				(	Func >>	/*(*/  no_node_d[ ch_p( ';' ) ]	/*| CV_ERR( sa_errSemiMiss ) )*/ )				|
				_If																											|
				Assign																										/*|
				(		ch_p(';') >> CV_ERR( sa_errExpMiss ) )														|  
				(		lexeme_d[ as_lower_d[ str_p('else' ) ] ] >> CV_ERR( sa_errSyntax ) )				|
				(		anychar_p >> CV_ERR( sa_errSyntax )	)			  */
				);

			Scope		=  no_node_d[ ch_p( '{' ) ] >> CV_ERR(sa_EnterScope )
				>> *Stmnt >> 
				no_node_d[ ch_p( '}' ) ];


			_If			=	no_node_d[
				lexeme_d[
					as_lower_d[
						str_p("if")
					]
				]
			] >>
				no_node_d[ 
					ch_p( '(' )	/*| CV_ERR(sa_errLBracketMiss)*/
				] >> (Exp /*| CV_ERR(sa_errExpMiss) */) >> 
				(
				no_node_d[
					ch_p( ')' ) 
				]/*									| 
				CV_ERR( sa_no_bracket) */
				) >>		// ( expression )
				( Scope | Stmnt  ) >>																// true part
				!( no_node_d[ lexeme_d[ as_lower_d[ str_p("else") ] ] ] >> (Scope|Stmnt) );		// else part

					Program	= *Stmnt;
		}

		__rule( PARSER_CONST, Constant );
		__rule( PARSER_NCONST, NConst );
		__rule( PARSER_BCONST, BConst );
		__rule( PARSER_SCONST, SConst );
		__rule( PARSER_VAR, Var );
		__rule( PARSER_FUNC, Func );
		__rule( PARSER_IF, _If);
		__rule( PARSER_EXP, Exp );
		__rule( PARSER_LOGIC, Logic );
		__rule( PARSER_CMP, Cmp );	
		__rule( PARSER_TERM, Term );	
		__rule( PARSER_ATOM, Atom );	
		__rule( PARSER_FACTOR, Factor );	
		__rule( PARSER_ASSIGN, Assign );	
		__rule( PARSER_STMNT, Stmnt );
		__rule( PARSER_SCOPE, Scope );
		__rule( PARSER_PROGRAM, Program );

		const rule<ScannerT, parser_context<>, parser_tag<PARSER_PROGRAM> >& start() const { return Program; } ;
	};
};
