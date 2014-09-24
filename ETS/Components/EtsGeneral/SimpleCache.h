#pragma once

#include "DataProvider.h"

template<class T, typename Key>
class SimpleCache
{
public:

	typedef Key container_key;
	typedef std::map<container_key, T*> container;
	typedef typename container::iterator container_iterator;

	SimpleCache(DataProvider* data_provider){
		this->data_provider = data_provider;		
	};

	virtual
	~SimpleCache(){
		clear();
	};

	void set_data_provider(DataProvider* data_provider){
		this->data_provider = data_provider;
	};

	void clear(){
		container_iterator it = cache_container.begin();
		container_iterator it_end = cache_container.end();

		for (; it != it_end; it++){
			erase_item(it->second);
		};
		cache_container.clear();
	};

	virtual
	void erase_item(T* item) = 0;

	T* get_item(Key key){
		container_iterator it = cache_container.find(key);
		if (it != cache_container.end()){
			return it->second;
		};
		return 0;
	};

	void initialize(DataProvider::Command* command){
		if (data_provider && command){
			data_provider->open("");
			Table* table = data_provider->execute_command(command);
			if (table){
				Table::container& rows = table->get_rows();
				Table::container_iterator it = rows.begin();
				Table::container_iterator itEnd = rows.end();

				for ( ; it != itEnd; it++){
					T* item = create_item(*it);
					if (item){
						add_item(get_key(item), item);
					};
				}
				delete table;
			}
			data_provider->close();
		}
	};

	virtual
	container_key get_key(T* item) = 0;

	virtual 
	T* create_item(Row* row) = 0;

	void add_item(container_key key, T* item){
		cache_container[key] = item;
	};
	
	container& get_items(){
		return cache_container;
	};

private:
	container cache_container;
	DataProvider* data_provider;
};
