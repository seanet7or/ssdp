#include "stdafx.h"
#include "SsdpSocket.h"


#define UDP_PORT 6000 
#include <iostream>
#include <event2/event-config.h>
#include <event2/event_struct.h>
#include <event2/listener.h>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int g_count = 0;
struct timeval stTv;

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// TIMER FUNCTION
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//void timer_function(int x, short int y, void *pargs)
//{
//	printf("nn**********Count is %dnn", g_count);
//	g_count = 0;
//	event_add((struct event*)pargs, &stTv);
//}



//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Event Function
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//void func_for_eve1(int x, short int y, void *pargs)
//{
//	int unFromAddrLen;
//	int nByte = 0;
//	char aReqBuffer[512];
//	struct sockaddr_in stFromAddr;
//
//	unFromAddrLen = sizeof(stFromAddr);
//
//	if ((nByte = recvfrom(x, aReqBuffer, sizeof(aReqBuffer), 0,
//		(struct sockaddr *)&stFromAddr, &unFromAddrLen)) == -1)
//	{
//		printf("error occured while receivingn");
//	}
//
//	//printf("Function called buffer is %sn",aReqBuffer);
//	g_count++;
//
//}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// MAIN
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
int main(int argc, char **argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	WSADATA d;
	WSAStartup(MAKEWORD(2, 2), &d);

	event_base *base = event_base_new();

	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) 
	{
		cerr << "Error " << WSAGetLastError() << " when getting local host name." << endl;
		return 1;
	}
	
	hostent *phe = gethostbyname(ac);
	if (phe == 0) 
	{
		cerr << "Yow! Bad host lookup." << endl;
		return 1;
	}

	std::vector<SsdpSocket*> sockets;

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) 
	{
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));

		SsdpSocket* s = new SsdpSocket(addr, base);
		//s->sendDiscoveryRequest();
		sockets.push_back(s);
	}
		
	event_base_dispatch(base);

	WSACleanup();
	return 0;
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^