#pragma once

///////////////////////////////////////////////////////////
//  cv.h
//  Implementation of the Class cvReferable
//  Created on:      20-ieo-2005 19:31:51
///////////////////////////////////////////////////////////

#if !defined(EA_586E7B8C_4A30_4fbf_82D6_4D23BEA7D135__INCLUDED_)
#define EA_586E7B8C_4A30_4fbf_82D6_4D23BEA7D135__INCLUDED_
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <functional>

using namespace std;
using namespace boost;

#define BAD_DOUBLE_VALUE	-1E+308

namespace CV{

	typedef unsigned int cvID;

	#define	CV_PERSIST_ID						( ( unsigned int ) 0xFFFFFFF0 )
	#define	CV_PERSIST_MAP_ID					( CV_PERSIST_ID | 1) 
	#define	CV_PERSIST_CODE_ID				( CV_PERSIST_ID | 2)

	typedef enum{
		cvtNone,
		cvtNumeric,
		cvtBoolean,
		cvtString
	}	cvtType;

	typedef enum{
		cvSO_CV,				// system object - custom variable
		cvSO_Func,			// system object - system function
		cvSO_SV				// system object - system variable
	} cvSysObjType;


	/*
		a referable object - object that can track references from code to itself
	*/
	class cvReferable
	{
	protected:
		vector<unsigned int>		m_vecRefs;		//	indexes of command in code referencing this object
		bool							m_bSystem;		// true if this object is system		
		bool							m_bGlobal;
		int							m_weight;		// weight of referenced object
		unsigned int				m_fromLevel;	// field useful only for variables marked with m_bSystem flag 
															// and shows which level within context 
															// provides this variable. initialized only for variables
															// born from system ones
	public:
		string						m_name;			// name of object

		bool	IsSystem() const { return m_bSystem; }		
		bool	IsGlobal() const { return m_bGlobal & m_bSystem; }		
		// adds reference to this object
		unsigned int Add( const unsigned int Ref ) { 
			m_vecRefs.push_back( Ref );
			return static_cast<unsigned int>( m_vecRefs.size() ) - 1;
		}

		unsigned int GetRefsCount() const { return static_cast<unsigned int>( m_vecRefs.size() ); }
		int GetWeight() const { return m_weight; }
		void SetWeight(int w) { m_weight = w; }
		void MakeNonSystem() { m_bSystem = false; }
		unsigned int GetLevel() const { return m_fromLevel;}
		void SetLevel( cvID levelID) { m_fromLevel = levelID;}
		unsigned int GetRef( const unsigned int i )	const { return m_vecRefs[i];}	
		cvReferable() : m_bSystem(false), m_bGlobal(false), m_weight(0), m_fromLevel(0){};
		cvReferable(const string& name ) : m_name( name), m_bSystem(false), m_bGlobal(false), m_weight(0), m_fromLevel(0) { };
		virtual ~cvReferable() {};
		friend class cvTRefPtrByNameMap;
	};

	typedef shared_ptr<cvReferable> cvRefPtr;

	/*
		an object with type
	*/
	template<typename T, cvtType cvtT>
	class cvTyped{
	protected:
		cvtType					m_Type;					// type ob object
		shared_ptr<T>			m_pValue;				// pointer to value
		
	public:
		cvTyped() : m_Type( cvtT ){};	
		
		void setValue( const T *Value){
			m_pValue.reset();
			T *ptr= new T;
			*ptr = *Value;
			m_pValue = shared_ptr<T>( ptr );
		}
		const cvtType getType() const { 
			return m_Type;
		}

		void setType( cvtType t ){
			if ( t != m_Type ) {
				m_pValue.reset();
				m_Type = t;
			}
		}
		virtual ~cvTyped(){ 
			if ( m_pValue) 
				m_pValue.reset();
		}
	};

	

	/*
		a system object - an object which state managed by cv system
	*/
   struct cvSysObj{
		cvID		m_context;
		string	m_strName;
		cvID		m_ID;
		cvSysObj() : m_strName(""), m_ID(0) {}
		cvSysObj( const cvSysObj& o){
			m_strName = o.m_strName;
			m_ID = o.m_ID;
		}

		const cvSysObj& operator=( const cvSysObj& o){
			m_strName = o.m_strName;
			m_ID = o.m_ID;
		}

		virtual ~cvSysObj() {}
	};

	typedef shared_ptr<cvSysObj> cvSysObjPtr;

	

	struct cvSysTypedObj : public cvSysObj {
		cvID		m_level;
		bool		m_global;
		cvtType	m_type;
		cvSysTypedObj() : m_type( cvtNone ), m_global( false), m_level(0) {}
		virtual void f() {};
		virtual ~cvSysTypedObj() {}
	};

	typedef shared_ptr<cvSysTypedObj> cvSysTypedObjPtr;

	struct cvSysFunc : public cvSysTypedObj{
		vector<cvtType>	m_vecParamTypes;
		void	InitParameters( const string& s){
			string::size_type p = s.find_first_of( '<' );

			while ( p != string::npos ) {
				p = s.find_first_of( '<', p );
				if ( p != string::npos ) {
					string number( "" );
					p++;
					while ( isdigit( s[p] ) ) 
						number += s[p++];
					if ( !number.empty() ) {
						m_vecParamTypes.push_back( static_cast<cvtType>( atoi(number.c_str() ) ) );
						number.clear();
					}
				}
			}
		}
	};

	typedef shared_ptr<cvSysFunc>	cvSysFuncPtr;


	template<typename T>
	class cvSysObjByNameMap : public map< string, T>{
	public:
		bool Contains( const string& s) const {
			return find( s ) != end();
		}

		const_iterator findByID( const cvID id) const {
			const_iterator b = begin();
			const_iterator e = end();
			while ( b!= e ) {
				if( b->second->m_ID == id )
					return b;
				b++;
			}
			return e;
		}
		virtual ~cvSysObjByNameMap(){
			clear();
		}
	};

	class cvSysTypedObjByNameMap : public cvSysObjByNameMap<cvSysTypedObjPtr>{

		cvSysTypedObjPtr FindOnLevel( const string& name, cvID LevelID ){
			cvSysObjByNameMap<cvSysTypedObjPtr>::iterator b = begin();
			cvSysObjByNameMap<cvSysTypedObjPtr>::iterator e = end();

			for ( ;b!=e; b++)
				if ( b->second->m_level == LevelID && b->second->m_strName == name ) 
					return b->second;
			return cvSysTypedObjPtr();
		}

	};

	struct cvLevel;
	typedef shared_ptr<cvLevel> cvLevelPtr;

	struct cvLevel : public cvSysObj{
		cvID							m_context;
		unsigned int				m_Weight;
		cvSysTypedObjByNameMap	m_mapSysVars;		
		cvSysTypedObjByNameMap	m_mapSysFunc;
		cvSysTypedObjByNameMap	m_mapCV;
		const bool	IsNameFree( const string& s) const { return true;}
		static bool less ( cvLevelPtr& _X, cvLevelPtr& _Y ){
			return (_X->m_Weight < _Y->m_Weight); 
		}
	};

	class cvLevelsByNameMap : public cvSysObjByNameMap<cvLevelPtr>{
	};

	typedef enum 
	{
		STOP = 0, POP, pushG, pushC, pushS, popV, pushV, stoV,
		CMP, cmpZ, ADD, SUB, DIV, MUL, iDIV, MOD, NEG, 
		JMP = 0x80000000, jE = 0x80000001, jGE = 0x80000002, jLE = 0x80000003,
		jG= 0x80000004, jNE= 0x80000005, jL= 0x80000006, callF = 0x40000000 
	} cvVMOpCode;

	class cvVMCommand{
	public:
		cvVMOpCode		m_OpCode;
		unsigned int	m_Operand;
		cvVMCommand( const cvVMOpCode OpCode, const unsigned int Operand = 0 ) : m_OpCode(OpCode), m_Operand(Operand) {}
		cvVMCommand(): m_OpCode(STOP), m_Operand(0) {}
		friend ostream& operator<<(ostream& o, const cvVMCommand& c);

	};

	struct cvContext : public cvSysObj {
		cvLevelsByNameMap			m_mapLevels;
		const bool	IsNameFree( const LONG lvl, const string& s) const { return true;}
		cvID	FindSysObjID( const string& name, cvID level, cvSysObjType type ){
			cvLevelsByNameMap::const_iterator litr = m_mapLevels.findByID( level );
			if ( litr == m_mapLevels.end() ) 
				return false;

			cvLevelPtr p = litr->second;
			if ( p ) {
				cvSysTypedObjByNameMap::iterator itr;
				switch( type ) {
				case cvSO_SV:
					itr = p->m_mapSysVars.find( name );
					if ( itr != p->m_mapSysVars.end() )
						return itr->second->m_ID;
					break;
				case cvSO_CV:
					itr = p->m_mapCV.find( name );
					if ( itr != p->m_mapCV.end() )
						return itr->second->m_ID;
					break;
				case cvSO_Func:
					itr = p->m_mapSysFunc.find( name );
					if ( itr != p->m_mapSysFunc.end() )
						return itr->second->m_ID;
					break;
				default:
					return 0;
				}
			}
			return 0;
		}
	};

	ostream& operator<<(ostream& o, const cvVMCommand& c);

	class cvCode: public vector<cvVMCommand>
	{

	public:
		unsigned int Add( const cvVMCommand& c) {
			push_back( c );
			return static_cast<unsigned int>(size()) - 1;
		}

		unsigned int GetSearilizationSize(){
			unsigned int sz = sizeof( CV_PERSIST_CODE_ID );
			sz += sizeof( size_type );
			sz += static_cast<unsigned int>( size() * sizeof( cvVMCommand) );
			return sz;
		}

		void *ToBinary( void *p ){
			unsigned int *pUint = reinterpret_cast<unsigned int*>(p);
			*pUint = CV_PERSIST_CODE_ID;
			pUint++;
			*pUint = static_cast<unsigned int>(size());
			pUint++;
			for ( size_t i = 0; i < size(); i++ ) {
				*pUint = static_cast<unsigned int> ( (operator[]( i )).m_OpCode );
				pUint++;
				*pUint = ( (operator[]( i )).m_Operand );
				pUint++;
			}
			return pUint;
		}

		// makes all jumps instructions in code relative to itself, so they don't depend on real position of jump
		// instruction in code
		void MakeJumpsRelative(){
			for( size_t	CurPos = 0; CurPos < size(); CurPos++ ){
				if ( operator[]( CurPos).m_OpCode & JMP ) {
					operator[]( CurPos).m_Operand -= static_cast<unsigned int>(CurPos); 					
				}
			}
		}
	};
	/*
	a typed, referable object - typed object that tracks references to itself from code
	*/
	template<typename T, cvtType cvtT>
	class cvTReferable : public cvTyped<T, cvtT>, public cvReferable
	{
	public:
		cvTReferable( ) : cvTyped<T, cvtT>(), cvReferable() {} 
		virtual unsigned int	GetSerializationSize() const { 
			unsigned int	sz = 0;

			// cvReferable data:
			sz += sizeof( string::size_type );																						// length of string
			sz += static_cast<unsigned int > ( (m_name.length() + 1) );														// string
			sz += sizeof( cvReferable::m_bSystem );
			sz += sizeof( cvReferable::m_bGlobal);
			sz += sizeof( cvReferable::m_weight);
			sz += sizeof( cvReferable::m_fromLevel);
			// references
			sz += sizeof( vector<unsigned int>::size_type );																	// size of vector
			sz += static_cast<unsigned int>( m_vecRefs.size() ) * sizeof( vector<unsigned int>::value_type );	// vector

			// CVTyped data:
			sz += sizeof( cvtType );																									// type
			return sz;
		};
		cvTReferable( const string& name ) : cvTyped< T, cvtT>(), cvReferable( name ) {}
		cvTReferable& operator=( const cvSysTypedObj& o){
			m_name = o.m_strName;
			m_Type = o.m_type;
			m_fromLevel = o.m_level;
			m_bSystem = true;
			m_bGlobal = o.m_global;
			return *this;
		}
		void*	ToBinary( void *p){
			unsigned int *pUint = reinterpret_cast<unsigned int*>(p);

			// save size of name
			*pUint = static_cast<unsigned int>(m_name.length());
			pUint++;

			// save name
			char *pChar = reinterpret_cast<char*>(pUint);
			string::size_type i = 0;
			while (  i < m_name.length() ) {
				*( pChar ) = m_name[i++];
				pChar++;
			}
			*pChar = 0;		// zero byte 
			pChar++;

			// save system & global flags
			bool *pBool = reinterpret_cast<bool*>(pChar);
			*pBool = m_bSystem;
			pBool++;
			*pBool = m_bGlobal;
			pBool++;

			//save weight and from level info
			pUint = reinterpret_cast<unsigned int*>(pBool);
			*pUint = m_weight;
			pUint++;
			*pUint = m_fromLevel;
			pUint++;

			//save references
			*pUint = GetRefsCount();
			pUint++;

			for ( unsigned int j = 0; j < GetRefsCount(); j++ ) {
				*pUint = m_vecRefs[j];
				pUint++;
			}

			// save type info
			cvtType *pT = reinterpret_cast<cvtType*>(pUint);
			*pT = getType();
			pT++;
			return pT;
		}

		void* FromBinary( void *ptr){
			unsigned int *pUint = static_cast<unsigned int*>(ptr);
			unsigned int namelen = *pUint;


			// restore name of element
			pUint++;
			char * pChar = reinterpret_cast< char*>(pUint);
			for ( unsigned int i = 0; i < namelen; i++, pChar++)
				m_name += *pChar;
			pChar++; //skip zero byte

			//restore system and global flags
			bool *pBool = reinterpret_cast<bool*>( pChar);
			m_bSystem = *pBool;
			pBool++;
			m_bGlobal = *pBool;
			pBool++;

			// restore weight and from level fields
			pUint = reinterpret_cast< unsigned int*>(pBool);
			m_weight = *pUint;
			pUint++;
			m_fromLevel = *pUint;
			pUint++;

			// restore references
			unsigned int refcount = *pUint;
			pUint++;
			for ( unsigned int r = 0; r < refcount; r++, pUint++) 
				m_vecRefs.push_back( *pUint);

			// restore type
			cvtType *pType = reinterpret_cast<cvtType*>(pUint);
			m_Type = *pType;
			pType++;
			return pType;
		}
	};

	template<typename T, cvtType cvtT>
	struct cvTReferablePtr :  shared_ptr< cvTReferable<T, cvtT> >{
		cvTReferablePtr( cvTReferable< T, cvtT> *p) : shared_ptr< cvTReferable<T, cvtT> >( p) { }
	};


	class cvTRefPtrByNameMap : public map<string, pair<unsigned int, cvRefPtr> >{
		unsigned int __ref_id;
	public:
		cvTRefPtrByNameMap() { __ref_id = 0; }

		void Add( const string& s, const cvRefPtr& r ){
			if ( find( s) == end() ) 
				operator[](r->m_name) = pair<unsigned int, cvRefPtr>( /* 0 means  error */ ++__ref_id, r);
		}

		void AddWithId( const string& s, const cvRefPtr& r, unsigned int ID ){
			if ( find(s) == end()) {
				operator[](r->m_name) = pair<unsigned int, cvRefPtr>( ID, r);
			}
		}
		iterator FindById( const unsigned int id ){
			iterator b = begin();
			iterator e = end();
			while ( b != e) {
				pair<unsigned int, cvRefPtr > p = b->second;
				if ( p.first== id) 
					return b;
				b++;
			}
			return b;
		}
		unsigned int	GetSearilizationSize(){
			/*
			Size of map for serialization = size_of( map id ) + size_of( entry ) * size_of( map )
			*/
			iterator	b = begin();
			iterator e = end();
			unsigned int sz = 0;

			sz += sizeof(CV_PERSIST_MAP_ID);
			sz += sizeof( unsigned int );			// size of map
			while ( b != e) {
				pair<unsigned int, cvRefPtr > pr = b->second;
				cvTReferable<void, cvtNone> *p = static_cast<cvTReferable<void, cvtNone> *>( pr.second.get() );
				if (p->GetRefsCount() ) 
					sz += p->GetSerializationSize();
				b++;
			}
			return sz;
		}
		void* ToBinary( void *p ){
			// save signature
			unsigned int* pInt = static_cast<unsigned int*>(p);

			*pInt = CV_PERSIST_MAP_ID;
			pInt++;

			// save size of map
			unsigned int uToSaveCnt = 0;
			for ( iterator b = begin(); b != end(); b++ ) {
				pair<unsigned int, cvRefPtr> pr = b->second;
				cvTReferable<void, cvtNone> *ptr = static_cast<cvTReferable<void, cvtNone>*>( pr.second.get() );
				if ( ptr->GetRefsCount() ) 
					uToSaveCnt++;
			}
			*pInt = uToSaveCnt;
			pInt++;

			// save map
			void *p1 = pInt;
			for ( iterator b = begin(); b != end(); b++ ) {
				pair<unsigned int, cvRefPtr> pr = b->second;
				cvTReferable<void, cvtNone> *ptr = static_cast<cvTReferable<void, cvtNone>*>( pr.second.get() );
				if ( ptr->GetRefsCount() ) 
					p1 = ptr->ToBinary( p1 );
			}

			return p1;
		}


		unsigned int GetReferencedWeight(){
			unsigned int w = 0;
			for ( iterator b = begin(); b != end(); b++ ) {
				pair<unsigned int, cvRefPtr> pr = b->second;
				cvTReferable<void, cvtNone> *ptr = static_cast<cvTReferable<void, cvtNone>*>( pr.second.get() );
				if ( ptr->GetRefsCount() )		// count weight only for referenced objects
					w += ptr->GetWeight();
			}			
			return w+1;
		}
	};

	struct cvMetaCV;
	typedef shared_ptr<cvMetaCV> cvMetaCVPtr;

	struct cvMetaCV : public cvSysTypedObj{
		string					m_caption;
		string					m_format;
		string					m_source;
		vector<cvID>			m_vecAggregations;
		cvCode					m_code;
		cvTRefPtrByNameMap	m_mapVars;
		cvTRefPtrByNameMap	m_mapConsts;
		cvTRefPtrByNameMap	m_mapFuncs;
		int						m_weight;
		vector<unsigned int> m_vecReferences;

		cvMetaCV() : m_caption(""), m_format(""), m_source(""), m_weight(0){}
		void	InitAggregations( const string& s){
			string::size_type p = s.find_first_of( '<' );

			while ( p != string::npos ) {
				p = s.find_first_of( '<', p );
				if ( p != string::npos ) {
					string number( "" );
					p++;
					while ( isdigit( s[p] ) ) 
						number += s[p++];
					if ( !number.empty() ) {
						m_vecAggregations.push_back( atoi(number.c_str()));
						number.clear();
					}
				}
			}
		}

		bool InitFromBinary( void *pCode, cvContext& context, cvLevel& level ){
			unsigned int *p = static_cast<unsigned int*>(pCode);

			if ( *p != CV_PERSIST_MAP_ID ) 
				return false;
			// restore constants map
			p++; // now it points to the number of elements in map
			void *pMap = static_cast<void*>(p+1);
			for ( unsigned int i = 0; i < *p; i++ ) {
				cvTReferablePtr<void, cvtNone> pTR = cvTReferablePtr<void, cvtNone>( new cvTReferable<void, cvtNone>());
				pMap = pTR->FromBinary( pMap );
				m_mapConsts.Add( pTR->m_name, pTR );
			}
			
			// restore functions map
			p = static_cast<unsigned int*>( pMap );
			if ( *p != CV_PERSIST_MAP_ID ) 
				return false;
			p++; // now it points to the number of elements in map
			pMap = static_cast<void*>(p+1);
			for ( unsigned int i = 0; i < *p; i++ ) {
				cvTReferablePtr<void, cvtNone> pTR = cvTReferablePtr<void, cvtNone>( new cvTReferable<void, cvtNone>());
				pMap = pTR->FromBinary( pMap );
				if ( pTR->IsGlobal() ) 
					m_mapFuncs.AddWithId( pTR->m_name, pTR, context.FindSysObjID( pTR->m_name, pTR->GetLevel(), cvSO_Func) );				
				else
					m_mapFuncs.AddWithId( pTR->m_name, pTR, context.FindSysObjID( pTR->m_name, level.m_ID, cvSO_Func) );				
			}

			// restore variables map
			p = static_cast<unsigned int*>( pMap );
			if ( *p != CV_PERSIST_MAP_ID ) 
				return false;
			p++; // now it points to the number of elements in map
			pMap = static_cast<void*>(p+1);
			for ( unsigned int i = 0; i < *p; i++ ) {
				cvTReferablePtr<void, cvtNone> pTR = cvTReferablePtr<void, cvtNone>( new cvTReferable<void, cvtNone>());
				// add variable to map only if it isn't description of THIS variable
				pMap = pTR->FromBinary( pMap );
				// variable with name equal to name of variable being restored from binary and 
				// level id equal to zero considered as entry in variables map for this variable ( references from code
				// to variable being restored ). do not add it to variables map of this meta variable
				// and save references separately
				if ( !(pTR->m_name == m_strName && pTR->GetLevel() == level.m_ID ) ){ 
					if( pTR->IsGlobal() )
						m_mapVars.AddWithId( pTR->m_name, pTR, context.FindSysObjID( pTR->m_name, pTR->GetLevel(), cvSO_CV) );				
					else
						m_mapVars.AddWithId( pTR->m_name, pTR, context.FindSysObjID( pTR->m_name, level.m_ID, cvSO_CV) );				
				}
				else{
					// load references to THIS variable from code
					for ( unsigned int r = 0; r < pTR->GetRefsCount(); r++) 
						m_vecReferences.push_back( pTR->GetRef(r) );
				}
			}

			// restore code
			p = static_cast<unsigned int*>( pMap );
			if ( *p != CV_PERSIST_CODE_ID ) 
				return false;
			p++;
			unsigned int commandscount = *p;
			p++;
			m_code.reserve( commandscount );
			for ( unsigned int i =0; i < commandscount; i++ ) {
				cvVMCommand cmd;
				cmd.m_OpCode = static_cast<cvVMOpCode>(*p);
				p++;
				cmd.m_Operand = *p;
				p++;
				m_code.push_back(cmd);
			}


			return true;
		}
		void GetRequiredDataProviderLevels( vector<cvID>& levels){
			levels.clear();
			map<unsigned int, bool> mapLevels;
			cvTRefPtrByNameMap::iterator b = m_mapVars.begin();
			cvTRefPtrByNameMap::iterator e = m_mapVars.end();
			while ( b != e ) {
				pair<unsigned int, cvRefPtr> pr = b->second;
				cvTReferable<void, cvtNone> *p = static_cast<cvTReferable<void, cvtNone>*>( pr.second.get());
				if ( p->GetLevel() )
					mapLevels[ p->GetLevel()] = true;
				b++;
			}
			b = m_mapFuncs.begin();
			e = m_mapFuncs.end();
			while ( b != e ) {
				pair<unsigned int, cvRefPtr> pr = b->second;
				cvTReferable<void, cvtNone> *p = static_cast<cvTReferable<void, cvtNone>*>( pr.second.get());
				mapLevels[ p->GetLevel()] = true;
				b++;
			}
			map<unsigned int, bool>::iterator b1 = mapLevels.begin();
			map<unsigned int, bool>::iterator e1 = mapLevels.end();
			while ( b1 != e1 ) {
				levels.push_back( b1->first );
				b1++;
			}									
		}

		static bool greater(cvMetaCVPtr& _X, cvMetaCVPtr& _Y) {
			if ( _X->m_code.empty() && !_Y->m_code.empty()) 
				return true;
			if ( _Y->m_code.empty() && !_X->m_code.empty()) 
				return false;
			return (_X->m_weight < _Y->m_weight); 
		};
	};

	

	

	template<typename T, cvtType cvtT >
	class cvConstant : public cvTReferable< T, cvtT > {
	public:
		cvConstant( const string& name,  const T& Value) : cvTReferable< T, cvtT >( name ) { cvTyped< T, cvtT >::setValue( &Value); }
	};

	class cvNConstant : public cvConstant< double, cvtNumeric >{
	public:
		cvNConstant() : cvConstant<double, cvtNumeric>( "" , 0.0 ) {}
		cvNConstant( const string& name,  const double Value) : 
			cvConstant<double, cvtNumeric>( name , Value) {}
	};

	class cvBConstant : public cvConstant< bool, cvtBoolean>{
	public:
		cvBConstant() : cvConstant<bool, cvtBoolean>( "" , false ) {}
		cvBConstant( const string& name,  const bool Value) : cvConstant<bool, cvtBoolean>( name , Value) {}
	};
	class cvSConstant : public cvConstant< string, cvtString>{
	public:
		cvSConstant() : cvConstant<string, cvtString>( "" , "" ) {}
		cvSConstant( const string& name,  const string& Value) : cvConstant<string, cvtString>( name , Value) {}
	};

	typedef shared_ptr<cvBConstant>	cvBConstantPtr;
	typedef shared_ptr<cvNConstant>	cvNConstantPtr;
	typedef shared_ptr<cvSConstant>	cvSConstantPtr;

	
	ostream& operator<<( ostream& o, cvTRefPtrByNameMap& m);
}
#endif // !defined(EA_586E7B8C_4A30_4fbf_82D6_4D23BEA7D135__INCLUDED_)
