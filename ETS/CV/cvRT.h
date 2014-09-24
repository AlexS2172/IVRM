#pragma once 
#include "cvCV.h"
#include <vector>
#include <stack>
#include "boost/shared_ptr.hpp"
#include "CV.h"
using namespace std;
using namespace boost;
using namespace CV;

namespace cvRuntime{


	// System provider interface
	class IcvSystemProvider{
	public:
		virtual void	GetVarible(unsigned int, cvtType ) =0;
		virtual void	CallFunction( unsigned int, cvtType ) = 0;
	};

	class cvDataAtomBase{
	public:
		cvtType		m_Type;
		virtual ~cvDataAtomBase() {
		}
		virtual size_t GetSize()const = 0;
		
	};

	typedef shared_ptr<cvDataAtomBase> cvDataAtomBasePtr;

	template<typename T, cvtType cvtT>
	class cvDataAtom : public cvDataAtomBase {
	protected:
		shared_ptr<T>	m_spData;
	public:
		cvDataAtom() { m_Type = cvtT; m_spData = shared_ptr<T>(new T);}
		cvtType GetType() const { return m_Type; };
		//virtual void SetData( T  *Value ) = 0;
	
	};

	class cvNDataAtom : public cvDataAtom<double, cvtNumeric>{
	public:
		void SetData( double&  Value ){
			*m_spData = Value;
		}
		double& GetData() const { return (double&) *m_spData;}

		size_t GetSize() const { return sizeof(double) + sizeof(m_spData)+ sizeof(cvDataAtomBase); }
	};

	class cvSDataAtom : public cvDataAtom<string, cvtString>{
	public:
		void SetData( string&  Value ){
			*m_spData = Value;
		}
		string& GetData() const { return (string&) *m_spData;}

		size_t GetSize() const { return m_spData->size() + sizeof(m_spData)+ sizeof(cvDataAtomBase) + sizeof(string); }
	};

	class cvBDataAtom : public cvDataAtom<bool, cvtBoolean>{
	public:
		void SetData( bool&  Value ){
			*m_spData = Value;
		}
		bool& GetData() const { return (bool&) *m_spData;}

		size_t GetSize() const { return sizeof(bool) + sizeof(m_spData) + sizeof(cvDataAtomBase); }
	};
	
	typedef shared_ptr<cvNDataAtom> cvNDataAtomPtr;
	typedef shared_ptr<cvSDataAtom> cvSDataAtomPtr;
	typedef shared_ptr<cvBDataAtom> cvBDataAtomPtr;
	typedef shared_ptr<cvDataAtom<void, cvtNone> > cvDataAtomPtr;
	
	class cvStaticStorage : public vector<cvDataAtomBasePtr >{
	public:
		unsigned int Add( cvDataAtomBasePtr& p ){
			push_back( p );
			return static_cast<unsigned int>( size() - 1);
		}

		size_t GetSize() {

			m_nSize = 0;

			for(const_iterator it = begin(); it != end(); ++it)
			{
				m_nSize += (*it)->GetSize() ;
			}
			return m_nSize;
		}

		size_t m_nSize;
	};

	class cvDynamicStorage{
		vector<cvNDataAtomPtr>	m_vecNData;
		stack<unsigned int>		m_stckNData;
		vector<cvSDataAtomPtr>	m_vecSData;
		stack<unsigned int>		m_stckSData;
		vector<cvBDataAtomPtr>	m_vecBData;
		stack<unsigned int>		m_stckBData;
	public:
		cvDynamicStorage(){
			for ( unsigned int i = 0; i < 300; i++) {
				m_vecNData.push_back( cvNDataAtomPtr(new cvNDataAtom) );
				m_vecBData.push_back( cvBDataAtomPtr(new cvBDataAtom) );
				m_vecSData.push_back( cvSDataAtomPtr(new cvSDataAtom) );
				m_stckBData.push( i );
				m_stckSData.push( i );
				m_stckNData.push( i );
			}
		}
		
		cvNDataAtomPtr& GetNData( unsigned int index ){
			return m_vecNData[ index ];
		}
		cvSDataAtomPtr& GetSData( unsigned int index ){
			return m_vecSData[ index ];
		}
		cvBDataAtomPtr& GetBData( unsigned int index ){
			return m_vecBData[ index ];
		}

		unsigned int Add( cvtType type, void *pData ){
			unsigned int DataIndex = 0;

			switch( type ) {
			case cvtNumeric:
				{
					DataIndex = m_stckNData.top();
					m_stckNData.pop();
					cvNDataAtomPtr ptr = cvNDataAtomPtr( new cvNDataAtom );
					ptr->SetData( *(static_cast<double*>(pData) ) );
					m_vecNData[ DataIndex ] = ptr;
				}
				break;
			case cvtString:
				{
					DataIndex = m_stckSData.top();
					m_stckSData.pop();
					cvSDataAtomPtr ptr = cvSDataAtomPtr( new cvSDataAtom );
					ptr->SetData( *(static_cast<string*>(pData) ) );
					m_vecSData[ DataIndex ] = ptr;
				}
				break;
			case cvtBoolean:
				{
					DataIndex = m_stckBData.top();
					m_stckBData.pop();
					cvBDataAtomPtr ptr = cvBDataAtomPtr( new cvBDataAtom );
					ptr->SetData( *(static_cast<bool*>(pData) ) );
					m_vecBData[ DataIndex ] = ptr;
				}
				break;
			default:
				assert(0);
			}

			return DataIndex;
		}

		void Remove( cvtType type, unsigned int Index ){
			try{
				switch(type) {
				case cvtString:
					{
						m_vecSData[Index].reset();
						m_stckSData.push( Index);
					}
					break;
				case cvtNumeric:
					{
						m_vecNData[Index].reset();
						m_stckNData.push( Index);
					}
					break;
				case cvtBoolean:
					{
						m_vecBData[Index].reset();
						m_stckBData.push( Index);
					}
					break;
				default:;
				}
			}
			catch (...) {
				assert(0);
			}
		}
	};

	typedef enum {
		cvStatic,
		cvDynamic
	} cvDataStorageType;

	struct cvDataReference{
		cvDataStorageType		m_StorageType;
		cvtType					m_Type;
		unsigned int			m_DataIndex;
	};

	typedef stack<cvDataReference> cvExecutionStack;

	//class CcvRTPool;
	class cvMetaPool;
	typedef shared_ptr<cvMetaPool>	cvMetaPoolPtr;

	typedef map<cvID, cvMetaPoolPtr>	cvMetaPoolMap;		// this map, associates level id and meta pools
	
	class cvMetaPool {
		public:		
		cvID															m_levelID;		// ID of level for which this meta pool created
		vector<cvMetaCVPtr>										m_vecMCV;		// meta data about custom variables defined for level
		cvSysTypedObjByNameMap									m_mapSysVars;	// system variables defined on this level
		cvSysTypedObjByNameMap									m_mapSysFuncs;	// system functions defined on this level
		map<string, pair<unsigned int/*index in m_vecMCV*/,cvID> >		m_mapCV;			// custom variables defined on this level

		cvMetaPool( cvID levelID, cvSysTypedObjByNameMap& mapSysVars, cvSysTypedObjByNameMap& mapSysFuncs, 
			cvSysTypedObjByNameMap& mapCV, cvMetaPoolMap& metaPoolMap, cvStaticStorage& sd  );
		void AddMetaCV(cvMetaCVPtr pMetaCV, cvMetaPoolMap& metaPoolMap, cvStaticStorage& sd);
		void GetRequiredDataProviderLevels( vector<cvID>& levels);
		void GetRequiredAggregationLevels( vector<cvID>& levels );
		const vector<cvMetaCVPtr> &GetMetaCVVector() const { return m_vecMCV; }
	};
  /*
	#define CV_DATAPROVIDER_BAD_ID	0xFFFFFFFFFF

	typedef map<cvID, int>	cvIDToIDMap;
	typedef struct {
		const char* m_strName;
		int			m_DataId;
	} cvDataProviderID;

	#define CV_DATANOTSUPPORTED ( size_t) -1

	class CcvDataProviderBase : public IcvDataProvider{
	protected:
		vector<cvDataProviderID>	m_vecLocalID;
		cvIDToIDMap						m_mapID;

		size_t FindDataByName( const string& strDataName )const{
			for ( size_t s = 0; s < m_vecLocalID; s++ ) {
				if ( strDataName == m_vecLocalID[s].m_strName) 
					return s;
			}		

			return CV_DATANOTSUPPORTED;
		}

	public:
		int	GetID( cvID _cvID){
			cvIDToIDMap::iterator itr = m_mapID.find(_cvID) 
			if ( itr == m_mapID.end() ) {
				return CV_DATAPROVIDER_BAD_ID;
			else
				return itr->second;
		}

		bool SupportsAll(){
			return m_vecLocalID.empty();
		}

		STDMETHOD(Check)( SAFEARRAY *arrSysVars, SAFEARRAY*arrSysFuncs);
	};*/
}