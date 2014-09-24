#include "stdafx.h"
#include "RttfClient.h"

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("usage: %s <host> <port> <user_id>\n", argv[0]);
		exit(-1);
	}

	RttfClient client;

	// Connect to CS
	if (!client.connect(argv[1], atoi(argv[2])))
		exit(-1);	
	
	printf("connection with %s:%s established\n", argv[1], argv[2]);

	// Initialize the last active trade
	client.m_LastTrade.init("rttf.dat");
		
	// Queue a login message to CS
	client.login(argv[3], argv[3], argv[3]);

	// Run the Rttf client
	client.run();

	return 0;
}

