/////////////////////////////////////////////////////////////////////////////
// CLockedMapTemplate

template <class Key, class Type> class CLockedMapTemplate
 {
	
	protected:
		std::map<Key, Type> m_LockedMap;
		//Lock() {}
		//Unlock() {}
		// AutoLock() {}

	public:
		EgLib::CLock         m_csLockedMap;
		typedef pair< std::map<Key, Type>::iterator, bool > inspair;
		typedef std::map<Key, Type>::value_type value_type;
		typedef map<Key, Type>::iterator iterator;
		typedef map<Key, Type>::const_iterator  const_iterator;
		typedef map<Key, Type>::size_type size_type;
		


		 size_type size( )
		 {
			EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.size();
		 }

		iterator insert( iterator _Where,  const value_type & _ValType)
		 {
			EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.insert(_Where, _ValType);
		 }


		 inspair insert(  const value_type & _ValType )
		 {
			EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.insert(_ValType);
		 }
		 


		Type & operator[]( const Key& _Key )
		 {
			 EgLib::CAutoLock lock(m_csLockedMap);
			 return m_LockedMap[_Key];

		 }
		
		iterator end( )
		{
			EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.end();

		}
		
		const_iterator end( ) const
		{
			EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.end();

		}

		iterator find( const Key& Key)
		{
			EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.find(Key);

		}


		iterator begin( )
		 {
		 	EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.begin();
		 }

		const_iterator begin( ) const
		 {
			EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.begin();
		 }


		iterator erase(	iterator _Where	)
		 {
		 	EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.erase(_Where);
		 
		 }
		
		iterator erase( iterator _First, iterator _Last )
		 {
			EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.erase(_First, _Last);

		 }
		
		size_type erase(const Key & _Key )
	    {
	 	 EgLib::CAutoLock lock(m_csLockedMap);
		 return m_LockedMap.erase(_Key);
		}

	   bool empty( )
	    {
		 	EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.empty();
		}
	
		void clear( )
		 {
		 	EgLib::CAutoLock lock(m_csLockedMap);
			m_LockedMap.clear();
			return;
		 }

		 size_type max_size() const
		  {
		 	EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.max_size();
		  }

		size_type count(const Key & _Key) const
		 {
		 	EgLib::CAutoLock lock(m_csLockedMap);
			return m_LockedMap.count(_Key);
		 }

 
 
 };