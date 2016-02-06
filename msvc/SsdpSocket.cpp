#include "stdafx.h"
#include "SsdpSocket.h"
#include "SocketHelper.h"

#define SSDP_PORT				1900
#define SSDP_MULTICAST_IPV4		"239.255.255.250"

SsdpSocket::SsdpSocket(_In_ in_addr _localAddr, _In_ event_base *base)
{
	udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	assert(udpSocket != INVALID_SOCKET);

	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_addr = _localAddr;
	localAddr.sin_port = htons(SSDP_PORT);
	localAddr.sin_family = AF_INET;

	if (0 != SocketHelper::makeNonBlocking(static_cast<intptr_t>(udpSocket)))
	{
		return/* false*/;
	}

	BOOL reuseAddress = TRUE;
	if (0 != setsockopt(udpSocket, SOL_SOCKET, SO_REUSEADDR, static_cast<char*>(static_cast<void*>(&reuseAddress)), sizeof(reuseAddress)))
	{
		return/* false*/;
	}

	BOOL broadcast = TRUE;
	if (0 != setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, static_cast<char*>(static_cast<void*>(&broadcast)), sizeof(broadcast)))
	{
		return/* false*/;
	}

	if (0 != bind(udpSocket, static_cast<sockaddr*>(static_cast<void*>(&localAddr)), sizeof(localAddr)))
	{
		return/* false*/;
	}

	DWORD loop = TRUE;
	if (setsockopt(udpSocket, IPPROTO_IP, IP_MULTICAST_LOOP, static_cast<char*>(static_cast<void*>(&loop)), sizeof(loop)) < 0)
	{
		return/* false*/;
	}

	in_addr mcast;
	mcast.s_addr = inet_addr(SSDP_MULTICAST_IPV4);
	IP_MREQ mreq;
	mreq.imr_multiaddr = mcast;
	mreq.imr_interface = localAddr.sin_addr;
	if (setsockopt(udpSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, static_cast<char*>(static_cast<void*>(&mreq)), sizeof(mreq)) < 0)
	{
		return/* false*/;
	}

	targetAddr.sin_family = AF_INET;
	targetAddr.sin_port = htons(SSDP_PORT);
	targetAddr.sin_addr.s_addr = inet_addr(SSDP_MULTICAST_IPV4);

	event_assign(&recvEvent, base, static_cast<intptr_t>(udpSocket), EV_READ | EV_PERSIST, &SsdpSocket::recvCb, this);
	event_add(&recvEvent, NULL);
}

SsdpSocket::~SsdpSocket()
{
	shutdown(udpSocket, SD_BOTH);
	closesocket(udpSocket);
}

void SsdpSocket::sendDiscoveryRequest()
{
	const char* header = "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 3\r\nST: ssdp:all\r\n\r\n";
	auto res = sendto(udpSocket, header, static_cast<int>(strlen(header)), 0, static_cast<sockaddr*>(static_cast<void*>(&targetAddr)), static_cast<int>(sizeof(targetAddr)));
	assert(res != SOCKET_ERROR);
}

void SsdpSocket::recvCb(evutil_socket_t s, short /*eventType*/, void * /*param*/)
{
	int unFromAddrLen;
	int nByte = 0;
	char aReqBuffer[512];
	ZeroMemory(aReqBuffer, 512);
	struct sockaddr_in stFromAddr;

	unFromAddrLen = sizeof(stFromAddr);

	if ((nByte = recvfrom(static_cast<SOCKET>(s), aReqBuffer, sizeof(aReqBuffer), 0,
		(struct sockaddr *)&stFromAddr, &unFromAddrLen)) == -1)
	{
		printf("error occured while receivingn");
	}

	printf("Function called buffer is %s",aReqBuffer);
	
}
