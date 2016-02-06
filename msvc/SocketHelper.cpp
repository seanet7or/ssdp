#include "stdafx.h"
#include "SocketHelper.h"

int SocketHelper::makeNonBlocking(intptr_t socket)
{
#ifdef WIN32
{
	unsigned long nonblocking = 1;
	ioctlsocket(socket, FIONBIO, (unsigned long*)&nonblocking);
}
#else
	if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1) 
	{
		event_warn("fcntl(O_NONBLOCK)");
		return -1;
	}
#endif
	return 0;
}
