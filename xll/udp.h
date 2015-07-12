// udp.h - send message to udplogger
#pragma once
#include "ensure.h"
#include "utility/socket.h"

#define UDP_HOST "web69.webfaction.com"
#define UDP_PORT 26211

namespace udp {
	inline WSA::dgram& dgram()
	{
		static WSA::dgram s(UDP_HOST, ENSURE_STRZ_(UDP_PORT));

		return s;
	}

}

