#include "ServerMain.h"

#include <Net.h>

#include <ctime>
#include <Windows.h>

int main()
{
 	ServerMain server;
 
 	if(!server.Initialize())
 	{
 		printf("Failed to initialize server.");
 		return 1;
 	}
	long start = 0;
	long elapsedTime = 0;
	DWORD sleepTime = 0;
 	while(server.Working())
 	{
		start = clock();
 		server.Update(10);
		elapsedTime = clock() - start;
		//10 ms sleep
		Sleep( 10 ); 
 	}

	server.Shutdown();
	return 0;
}