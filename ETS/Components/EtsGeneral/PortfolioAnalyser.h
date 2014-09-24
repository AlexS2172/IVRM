#pragma once

class PortfolioAnalyser{
public:
	PortfolioAnalyser(void);
	
	virtual 
	~PortfolioAnalyser(void);

	virtual 
	void calculate();

	void start_thread();
	void stop_thread();

	/*virtual*/
	void update(long id);

private:
	HANDLE	
	worker_thread_handle;

	HANDLE
	recalculate_event;

	static 
	UINT WINAPI worker_thread(LPVOID data_ptr);
};

class PortfolioAnalyserSingleton{
public:
	static PortfolioAnalyser* instanse();
};
