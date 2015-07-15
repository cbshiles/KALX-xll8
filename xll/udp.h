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
	
	inline int sendto(const char* msg, int len = 0)
	{
		static int result = WSA::Startup{};
		static WSA::Socket s(AF_INET, SOCK_DGRAM, 0);
		static WSA::AddrInfo ai(UDP_HOST, UDP_PORT, WSA::ADDRINFO(0, AF_INET, SOCK_DGRAM, IPPROTO_UDP));

		return ai.sendto(s, msg, len);
	}
}

