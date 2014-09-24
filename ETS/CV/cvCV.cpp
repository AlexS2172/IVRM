#include "stdafx.h"
#include <iostream>
#include "cvCV.h"


using namespace std;

namespace CV{

ostream& operator<<(ostream& o, const cvVMCommand& c){

	switch( c.m_OpCode ) {
		case STOP:
			o << "STOP";
			break;
		case POP:
			o << "POP";
			break;
		case pushG:
			o << "pushG" << "\t\t" << c.m_Operand;
			break;
		case pushC:
			o << "pushC" << "\t\t" << c.m_Operand;
			break;
		case pushS:
			o << "pushS" << "\t\t" << c.m_Operand;
			break;
		case	popV:
			o <<  "popV" << "\t\t" << c.m_Operand;
			break;
		case	pushV:
			o << "pushV" << "\t\t" << c.m_Operand;
			break;
		case	stoV:
			o << "stoV" << "\t\t" << c.m_Operand;
			break;
		case	CMP:
			o <<  "CMP";
			break;
		case	cmpZ:
			o <<	"cmpZ";
			break;
		case	ADD:
			o <<	"ADD";
			break;
		case	SUB:
			o <<  "SUB";
			break;
		case	DIV:
			o << "DIV";
			break;
		case	MUL:
			o << "MUL";
			break;
		case	iDIV:
			o << "iDIV";
			break;
		case	MOD:
			o <<  "MOD";
			break;
		case	jE:
			o << "jE" << "\t\t" << c.m_Operand;
			break;
		case	JMP:
			o <<  "JMP" << "\t\t" << c.m_Operand;
			break;
		case	jGE:
			o << "jGE" << "\t\t" << c.m_Operand;
			break;
		case	jLE:
			o << "jLE" << "\t\t" << c.m_Operand;
			break;
		case	jG:
			o << "jG" << "\t\t" << c.m_Operand;
			break;
		case	jNE:
			o << "jNE" << "\t\t" << c.m_Operand;
			break;
		case	jL:
			o << "jL" << "\t\t" << c.m_Operand;
		default:
			{
				if ( static_cast<unsigned int>( c.m_OpCode )  & static_cast<unsigned int>( callF )  ) 
					o << "callF[" << ( static_cast<unsigned int>( c.m_OpCode ) & 0x00FFFFFF ) << "]\t\t" << c.m_Operand;
				else
					o << "*** error *** ";
			}
	}
	return o;
}

ostream& operator<<( ostream& o, cvTRefPtrByNameMap& m){
	cvTRefPtrByNameMap::iterator b = m.begin();
	cvTRefPtrByNameMap::iterator e = m.end();
	while ( b!= e ) {
		pair<unsigned int, cvRefPtr>&	p = b->second;
		cvRefPtr	pt=p.second;
		o << pt->m_name; 
		for( unsigned int s = 0 ; s < ( 35 - pt->m_name.length() ); s++ )
			o << " ";
		streampos p1 = o.tellp();
		o << "[";
		o << p.first <<"] ";
		streampos p2 = o.tellp();
		for( int s = 0 ; s < ( 10 - ( p2 -p1 )) && s > 0; s++ )
			o << " ";
		for( unsigned int i = 0; i < pt->GetRefsCount(); i++)
			o << pt->GetRef(i) << " ";
		o << endl;
		b++;
	}
	return o;
}

}