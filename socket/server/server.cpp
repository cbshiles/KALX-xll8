// server.cpp - simple socket server
#include <iostream>
#include <future>
#include "../../xll/utility/socket.h"

using namespace WSA;

void echo(SOCKET s)
{
	while (true) {
		std::vector<char> str = WSA::recv(s);
		if (0 == strncmp(&str[0], "\r\n", 2)) {
			shutdown(s, SD_BOTH);

			return;
		}

		WSA::send(s, &str[0], str.size());
	}
}

#ifndef UDP_HOST
#define UDP_HOST _T("108.168.134.126")
#endif
#ifndef UDP_PORT
#define UDP_PORT _T("26211")
#endif

int main()
{
	try {
		int err;
		Startup wsa;
		Bind s(_T("localhost"), _T("3456"), WSA::ADDRINFO(AI_PASSIVE));

		err = listen(s, 10);
		if (err) {
			throw WSAGetLastError();
		}

		Socket so(AF_INET, SOCK_DGRAM, 0);
		AddrInfo ai(UDP_HOST, UDP_PORT, WSA::ADDRINFO(0, AF_INET, SOCK_DGRAM, IPPROTO_UDP));

		ai.sendto(so, "test");

		while (1) {
			sockaddr_storage ss;
			socklen_t nss = sizeof(ss);
			SOCKET si = accept(s, (sockaddr*)&ss, &nss);
			DWORD n = 64;
			TCHAR addr[64];
			WSAAddressToString((sockaddr*)&ss, nss, 0, &addr[0], &n);

			std::wcout << _T("accept: ") << addr << std::endl;

			auto t = std::async(std::launch::async, echo, si);
		}
	}
	catch (int err) {
		return err;
	}

	return 0;
}