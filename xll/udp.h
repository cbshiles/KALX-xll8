// udp.h - send message to logger
#pragma once
#include "ensure.h"
#include "utility/socket.h"

#ifndef UDP_HOST
#define UDP_HOST _T("108.168.134.126")
#endif
#ifndef UDP_PORT
#define UDP_PORT _T("26211")
#endif

#define UDP_SEND udp::sendto(__FUNCTION__);

namespace udp {
	
	inline const WSA::AddrInfo& host()
	{
		static int result = WSA::Startup{};
		static WSA::AddrInfo ai(UDP_HOST, UDP_PORT, WSA::ADDRINFO(0, AF_INET, SOCK_DGRAM, IPPROTO_UDP));

		return ai;
	}

	inline int sendto(SOCKET s, const char* msg, int len = 0)
	{
		return host().sendto(s, msg, len);
	}

	inline int recvfrom(SOCKET s, char* buf, int len)
	{
		return host().recvfrom(s, buf, len);
	}
}

