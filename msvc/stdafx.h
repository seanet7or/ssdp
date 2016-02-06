#pragma once

#include "targetver.h"

#pragma warning (push)
#pragma warning (disable:4365)
#pragma warning (disable:4574)
#pragma warning (disable:4668)
#pragma warning (disable:4710)
#pragma warning (disable:4820)

#include <assert.h>

#include <stdio.h>



#include <vector>

#ifdef _WIN32
#include <tchar.h>
#include <winsock2.h>
#include <Mstcpip.h>
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#define _In_
#endif


/*#include <event2/util.h>
#include <event2/event.h>
#include <event2/event_struct.h>*/

#pragma warning (pop)