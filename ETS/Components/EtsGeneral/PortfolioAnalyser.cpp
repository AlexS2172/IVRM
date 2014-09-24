#include "StdAfx.h"
#include "PortfolioAnalyser.h"
#include "PositionKeeper.h"
#include "DBDataProvider.h"
#include "Position.h"

PortfolioAnalyser::PortfolioAnalyser(void){
	start_thread();
};

PortfolioAnalyser::~PortfolioAnalyser(void){
	stop_thread();
};

/*virtual*/
void 
PortfolioAnalyser::calculate(){
	PositionKeeper* position_keeper = PositionKeeperSingleton::instance();
	if (position_keeper){
		PositionKeeper::position_container positions(position_keeper->get_container());
		PositionKeeper::position_iterator it = positions.begin();
		PositionKeeper::position_iterator it_end = positions.end();

		std::string connection_string = bstr_to_string(CGenegalSettings::GetDBConnectionString());
		DBDataProvider provider(connection_string);
		DBImportPositionAnalytics cmd;
		for ( ; it != it_end; it++){
			it->second->calculate();
			cmd.add_position(it->second);
		};
		cmd.execute(&provider);
	};
};

void 
PortfolioAnalyser::start_thread(){
	unsigned int thread_id = 0;
	recalculate_event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	worker_thread_handle = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, worker_thread, this, 0, &thread_id));
};

void
PortfolioAnalyser::stop_thread(){
	TerminateThread(worker_thread_handle, -1);
	CloseHandle(worker_thread_handle);
	worker_thread_handle = NULL;
};

/*static*/
UINT WINAPI PortfolioAnalyser::worker_thread(LPVOID data_ptr){
	if (data_ptr){
		PortfolioAnalyser* portfolio_analyser = static_cast<PortfolioAnalyser*>(data_ptr);
		if (portfolio_analyser){
			while(true){
				WaitForSingleObject(portfolio_analyser->recalculate_event, INFINITE);
				portfolio_analyser->calculate();
			}
		}
	};
	return 0;
};

/*virtual*/
void PortfolioAnalyser::update(long id){
	SetEvent(recalculate_event);
};

/*static*/
PortfolioAnalyser*
PortfolioAnalyserSingleton::instanse(){
	static PortfolioAnalyser instanse_;
	return &instanse_;
};
