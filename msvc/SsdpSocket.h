#ifndef __SsdpSocket_h__
#define __SsdpSocket_h__

class SsdpSocket
{
public:
	SsdpSocket(_In_ in_addr _localAddr, _In_ event_base *base);
	~SsdpSocket();

	SsdpSocket(const SsdpSocket&) = delete; // Copy Ctor
	SsdpSocket(SsdpSocket&&) = delete; // Move Ctor
	SsdpSocket& operator=(const SsdpSocket& other) = delete; // Copy assignment
	SsdpSocket& operator=(SsdpSocket&& other) = delete; // Move assignment

	void sendDiscoveryRequest();

private:
	static void recvCb(evutil_socket_t s, short eventType, void *param);

private:
	SOCKET udpSocket;
	sockaddr_in localAddr;
	sockaddr_in targetAddr;
	event recvEvent;
};

#endif // #ifndef __SsdpSocket_h__