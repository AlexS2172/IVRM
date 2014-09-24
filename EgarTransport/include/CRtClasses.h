#ifndef __RTCLASSES_H__
#define __RTCLASSES_H__

#include <winsock.h>

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

namespace __dev__
{

/************************************************************************************************
************************************************************************************************/

class IEgTpRefCounter
{
public:
	IEgTpRefCounter() : m_lRef(0) {};
	virtual ~IEgTpRefCounter(){};

	u_long AddRef()
	{
		return InterlockedIncrement(&m_lRef);
	}

	void Release()
	{
		u_long ul = InterlockedDecrement(&m_lRef);
		
		if(!ul)
			delete this;
	}
protected:
	long		m_lRef;// count of reference
};

template <class T>
class CEgSmartPtr
{
public:
	CEgSmartPtr()
	{
		p=NULL;
	}
	CEgSmartPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	CEgSmartPtr(const CEgSmartPtr<T>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	~CEgSmartPtr()
	{
		if (p)
			p->Release();
	}

	T* operator=(T* lp)
	{
		if (lp != NULL)
			lp->AddRef();
		if (p)
			p->Release();
		p = lp;
		return lp;
	}
	T* operator=(const CEgSmartPtr<T>& lp)
	{
		if (lp != NULL)
			lp->AddRef();
		if (p)
			p->Release();
		p = lp;
		return (T*)lp;
	}
	operator T*() const
	{
		return (T*)p;
	}
	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		return *p;
	}
	T* operator->() const
	{
		ATLASSERT(p!=NULL);
		return (T*)p;
	}
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator<(T* pT) const
	{
		return p < pT;
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	
	void Release()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}

	T* p;
};

class CEgGuid : public GUID
{
public:

	CEgGuid() 
	{
		GUID tempGUID;
		tempGUID.Data1 = 0;
		tempGUID.Data2 = 0;
		tempGUID.Data3 = 0;
		memset(tempGUID.Data4,0,8);

		memcpy(this,&tempGUID,sizeof(GUID));
	}
	
	CEgGuid(const GUID& guid) 
	{
		Data1 = guid.Data1;
		Data2 = guid.Data2;
		Data3 = guid.Data3;
		memcpy(Data4,guid.Data4,8);
	}

	CEgGuid(const CEgGuid& guid) 
	{
		Data1 = guid.Data1;
		Data2 = guid.Data2;
		Data3 = guid.Data3;
		memcpy(Data4,guid.Data4,8);
	}

	CEgGuid& operator = (const CEgGuid& guid) 
	{
		Data1 = guid.Data1;
		Data2 = guid.Data2;
		Data3 = guid.Data3;
		memcpy(Data4,guid.Data4,8);				
		return *this;
	}

	bool operator < (const CEgGuid& Origin) const
	{
		return memcmp(this, &Origin, sizeof(GUID)) < 0;
	}
	
	void Clear()
	{
		memset(this,0,sizeof(GUID));
	}
};

template < class _Key,class _Data >
class CTEgTpList 
{
protected:
	typedef map<_Key, _Data >	TKeyDataMap;

public:
	virtual ~CTEgTpList()
	{
		m_ListMap.clear();
	}

public:	
	_Data GetDataByKey(const _Key& key)
	{
		CAutoLock	Lock(m_Lock);

		TKeyDataMap::iterator it = m_ListMap.find(key);

		if (it == m_ListMap.end())
			return NULL;

		return it->second;
	}
	
	u_long Count()
	{ 
		CAutoLock	Lock(m_Lock);
		return m_ListMap.size();
	}
	
	bool Remove(const _Key& key)
	{
		CAutoLock	Lock(m_Lock);

		TKeyDataMap::iterator it = m_ListMap.find(key);

		if (it == m_ListMap.end())
			return false;

		ATLASSERT(!(it->second) == false);

		m_ListMap.erase(key);

		return true;
	}

	bool Clear()
	{
		CAutoLock	Lock(m_Lock);

		m_ListMap.clear();		

		return true;
	}

	bool First(_Key& keyFirst)
	{
		CAutoLock	Lock(m_Lock);

		TKeyDataMap::iterator it = m_ListMap.begin();

		if (it == m_ListMap.end())
			return false;

		keyFirst = it->first;
		return true;
	}

	bool Next(_Key& key)
	{
		CAutoLock	Lock(m_Lock);

		TKeyDataMap::iterator it = m_ListMap.upper_bound(key);

		if (it == m_ListMap.end())
			return false;

		key = it->first;
		return true;
	}

protected:
	TKeyDataMap			m_ListMap;
	CLock				m_Lock;
};

template < class _Key,class _Data >
class CTEgTpListSmartPtr
{
protected:
	typedef map<_Key, CEgSmartPtr< _Data > >	TKeyDataMap;

public:
	virtual ~CTEgTpListSmartPtr()
	{
		m_ListMap.clear();
	}

public:	
	_Data* GetDataByKey(const _Key& key)
	{
		CAutoLock	Lock(m_Lock);

		TKeyDataMap::iterator it = m_ListMap.find(key);

		if (it == m_ListMap.end())
			return NULL;

		return it->second;
	}
	
	u_long Count()
	{ 
		CAutoLock	Lock(m_Lock);
		return m_ListMap.size();
	}
	
	bool Remove(const _Key& key)
	{
		CAutoLock	Lock(m_Lock);

		TKeyDataMap::iterator it = m_ListMap.find(key);

		if (it == m_ListMap.end())
			return false;

		ATLASSERT(!(it->second) == false);

		m_ListMap.erase(key);

		return true;
	}

	bool Clear()
	{
		CAutoLock	Lock(m_Lock);

		m_ListMap.clear();		

		return true;
	}

	bool First(_Key& keyFirst)
	{
		CAutoLock	Lock(m_Lock);

		TKeyDataMap::iterator it = m_ListMap.begin();

		if (it == m_ListMap.end())
			return false;

		keyFirst = it->first;
		return true;
	}

	bool Next(_Key& key)
	{
		CAutoLock	Lock(m_Lock);

		TKeyDataMap::iterator it = m_ListMap.upper_bound(key);

		if (it == m_ListMap.end())
			return false;

		key = it->first;
		return true;
	}

protected:
	TKeyDataMap			m_ListMap;
	CLock				m_Lock;
};

/**************************************************************************/
template <class _Data>
class CTEgTpDataSubjectsDatabase
{
private:
	class _DataEx
	{
	public:

		_Data						m_pData;

		bool operator < (const _DataEx& Origin) const
		{
			if(m_pData < Origin.m_pData)
				return true;

			return false;
		}
	};

	typedef set<tstring>						TSubjectSet;			// subjects storage
	typedef set<_DataEx>						TDataSet;				// data storage

	typedef TSubjectSet::iterator				TSubjectSetIt;			// subjects iterator
	typedef typename TDataSet::iterator			TDataSetIt;				// data iterator

	typedef set<tstring*>						TSubjectRefsSet;		// refs to subject
	typedef set<_DataEx*>						TDataRefsSet;			// refs to data

	typedef map<tstring*, TDataRefsSet>			TSubject2TDatas;			// ref subject to refs data
	typedef map<_DataEx*, TSubjectRefsSet>		TData2TSubjects;		// ref data to refs subjects

	typedef typename TSubject2TDatas::iterator	TSubject2TDatasIt;		// ref subject to refs data map iterator
	typedef typename TData2TSubjects::iterator	TData2TSubjectsIt;		// ref data to refs subjects map iterator

	TSubject2TDatas								m_Subject2DatasMap;
	TData2TSubjects								m_Data2SubjectsMap;

	TSubjectSet									m_SubjectSet;
	TDataSet									m_DataSet;

	CLock										m_Lock;

public:

	void GetDataBySubject(	const tstring& sSubject,
							vector<_Data>& vecData)
	{
		CAutoLock	Lock(&m_Lock);

		vecData.clear();

		TSubjectSetIt SubjIt = m_SubjectSet.find(sSubject);
		if(SubjIt == m_SubjectSet.end())
			return;

		tstring* SubjectRef = &(*SubjIt);

		TDataRefsSet& DataRefs = m_Subject2DatasMap[SubjectRef];
		
		for(TDataRefsSet::iterator It = DataRefs.begin(); It != DataRefs.end(); It++)
		{
			_DataEx* pDataRef = *It;

			vecData.push_back(pDataRef->m_pData);
		}
	}

	void GetDataBySubjectWithMask(	const tstring& sSubject,
									set<_Data>& setData)
	{
		CAutoLock	Lock(m_Lock);
		
		for(TSubjectSetIt SubjIt = m_SubjectSet.begin(); SubjIt != m_SubjectSet.end(); SubjIt++)
		{
			if(EgLib::Comparable::CompareStringByMask( sSubject.c_str(), /* As real subject */
									(*SubjIt).c_str()  /* As subject mask*/))
			{

				tstring* SubjectRef = &(*SubjIt);

				TDataRefsSet& DataRefs = m_Subject2DatasMap[SubjectRef];
				
				for(TDataRefsSet::iterator It = DataRefs.begin(); It != DataRefs.end(); It++)
				{
					_DataEx* pDataRef = *It;

					setData.insert(pDataRef->m_pData);
				}
			}
		}


	}

	void GetSubjectsByData( _Data pData, 
							vector<tstring>& vecSubjects)
	{
		CAutoLock	Lock(m_Lock);

		vecSubjects.clear();

		_DataEx Data;
		Data.m_pData = pData;

		TDataSetIt& DataIt = m_DataSet.find(Data);
		if(DataIt == m_DataSet.end())
			return;

		_DataEx* DataRef = &(*DataIt);

		TSubjectRefsSet& SubjRefs = m_Data2SubjectsMap[DataRef];
		
		for(TSubjectRefsSet::iterator It = SubjRefs.begin(); It != SubjRefs.end(); It++)
		{
			tstring* pSubjRef = *It;
			vecSubjects.push_back(*pSubjRef);
		}
	}


	void UnregisterData(_Data pData)
	{
		CAutoLock	Lock(m_Lock);

		_DataEx Data;
		Data.m_pData = pData;
		
		TDataSetIt DataIt = m_DataSet.find(Data);
		if(DataIt == m_DataSet.end())
			return;

		_DataEx* DataRef = &(*DataIt);
		
		TData2TSubjectsIt It1 = m_Data2SubjectsMap.find(DataRef);
		if(It1 != m_Data2SubjectsMap.end())
		{
			TSubjectRefsSet& SubjectSet = It1->second;

			// for each subject delete reference to Data
			for(TSubjectRefsSet::iterator It2 = SubjectSet.begin(); It2 != SubjectSet.end(); It2++)
			{
				tstring* SubjectRef = *It2;

				TDataRefsSet& DataRefs = m_Subject2DatasMap[SubjectRef];

				TDataRefsSet::iterator It3 = DataRefs.find(DataRef);
				if(It3 != DataRefs.end())
				{
					DataRefs.erase(It3);
				}

				if(DataRefs.empty())// no Data with this subject -> remove subject
				{
					m_Subject2DatasMap.erase(SubjectRef);// remove reference
						
					TSubjectSetIt SubjIt = m_SubjectSet.find(*SubjectRef);
					if(SubjIt != m_SubjectSet.end())
						m_SubjectSet.erase(SubjIt);//physically remove subject
				}

			}

			m_Data2SubjectsMap.erase(It1);
		}
		m_DataSet.erase(DataIt);//physically remove Data				
	}

	void UnregisterData(_Data pData, vector<tstring>& vecSubjectsToUnsubscribe)
	{
		CAutoLock	Lock(m_Lock);

		vecSubjectsToUnsubscribe.clear();

		_DataEx Data;
		Data.m_pData = pData;
		
		TDataSetIt DataIt = m_DataSet.find(Data);
		if(DataIt == m_DataSet.end())
			return;

		_DataEx* DataRef = &(*DataIt);
		
		TData2TSubjectsIt It1 = m_Data2SubjectsMap.find(DataRef);
		if(It1 != m_Data2SubjectsMap.end())
		{
			TSubjectRefsSet& SubjectSet = It1->second;

			// for each subject delete reference to Data
			for(TSubjectRefsSet::iterator It2 = SubjectSet.begin(); It2 != SubjectSet.end(); It2++)
			{
				tstring* SubjectRef = *It2;

				TDataRefsSet& DataRefs = m_Subject2DatasMap[SubjectRef];

				TDataRefsSet::iterator It3 = DataRefs.find(DataRef);
				if(It3 != DataRefs.end())
				{
					DataRefs.erase(It3);
				}

				if(DataRefs.empty())// no Data with this subject -> remove subject
				{
					m_Subject2DatasMap.erase(SubjectRef);// remove reference
	
					vecSubjectsToUnsubscribe.push_back(*SubjectRef);// pushing subject to unsubscribe vector
					
					TSubjectSetIt SubjIt = m_SubjectSet.find(*SubjectRef);
					if(SubjIt != m_SubjectSet.end())
						m_SubjectSet.erase(SubjIt);//physically remove subject
				}

			}

			m_Data2SubjectsMap.erase(It1);
		}
		m_DataSet.erase(DataIt);//physically remove Data				
	}
	void RegisterDataSubject(	const tstring& sSubject, 
								_Data pData)
	{
		// bind
		CAutoLock	Lock(m_Lock);


		TSubjectSetIt SubjIt = m_SubjectSet.find(sSubject);
		if(SubjIt == m_SubjectSet.end())
		{
			pair<TSubjectSetIt , bool> InsIt = m_SubjectSet.insert(sSubject);
			SubjIt = InsIt.first;
		}

		_DataEx Data;
		Data.m_pData = pData;

		TDataSetIt DataIt = m_DataSet.find(Data);
		if(DataIt == m_DataSet.end())
		{
			pair<TDataSetIt, bool> InsIt = m_DataSet.insert(Data);
			DataIt = InsIt.first;
		}

		_DataEx*		DataRef = &(*DataIt);
		tstring*		SubjectRef = &(*SubjIt);

		TSubjectRefsSet& SubjectRefs = m_Data2SubjectsMap[DataRef];		
		// for each subject delete reference to Data
		SubjectRefs.insert(SubjectRef);

		TDataRefsSet& DataRefs = m_Subject2DatasMap[SubjectRef];
		DataRefs.insert(DataRef);
	};

	/* return true, if no object referenced to this subject
		else - return false*/

	bool UnregisterDataSubject(	const tstring& sSubject, 
								_Data pData)
	{
		CAutoLock	Lock(m_Lock);
		
		TSubjectSetIt SubjIt = m_SubjectSet.find(sSubject);
		if(SubjIt == m_SubjectSet.end())
			return true;

		_DataEx Data;
		Data.m_pData = pData;

		TDataSetIt DataIt = m_DataSet.find(Data);
		if(DataIt == m_DataSet.end())
			return false;

		_DataEx*	DataRef = &(*DataIt);
		tstring*	SubjectRef = &(*SubjIt);

		TData2TSubjectsIt It1 = m_Data2SubjectsMap.find(DataRef);
		if(It1 != m_Data2SubjectsMap.end())
		{
			TSubjectRefsSet& SubjectRefsSet = It1->second;

			TSubjectRefsSet::iterator it = SubjectRefsSet.find(SubjectRef);
			if(it != SubjectRefsSet.end())
				SubjectRefsSet.erase(it);
		}

		TSubject2TDatasIt It2 = m_Subject2DatasMap.find(SubjectRef);
		if(It2 != m_Subject2DatasMap.end())
		{
			TDataRefsSet& DataRefsSet = It2->second;

			TDataRefsSet::iterator it = DataRefsSet.find(DataRef);
			if(it != DataRefsSet.end())
				DataRefsSet.erase(it);

			if(DataRefsSet.empty())// no Data with this subject -> remove subject
			{
				m_Subject2DatasMap.erase(It2);// remove reference
				
				m_SubjectSet.erase(SubjIt);//physically remove subject

				return true; // subject is clear
			}
		}
		return false;
	}
	bool GetAllSubjects(vector<tstring>& vecSubjects)
	{
		CAutoLock	Lock(m_Lock);

		vecSubjects.clear();
		if(m_SubjectSet.empty())
			return false;
		
		for(TSubjectSetIt iter = m_SubjectSet.begin(); iter != m_SubjectSet.end(); iter++)
		{
			vecSubjects.push_back(*iter);
		}
		return true;
	};
	
	void ClearAllStorage()
	{
		CAutoLock	Lock(m_Lock);
		m_Subject2DatasMap.clear();
		m_Data2SubjectsMap.clear();
		m_SubjectSet.clear();
		m_DataSet.clear();
	}

};

};
#endif //__RTCLASSES_H__