// server.cpp - simple socket server
#include <iostream>
#include "../../xll/utility/socket.h"

using namespace WSA;

int main()
{
	try {
		int err;
		Startup wsa;
		Bind s(_T("localhost"), _T("3456"), WSA::ADDRINFO(AI_PASSIVE, AF_UNSPEC));

		err = listen(s, 10);
		if (err) {
			err = WSAGetLastError();
			throw err;
		}

		sockaddr_storage ss;
		socklen_t nss = sizeof(ss);
		SOCKET si = accept(s, (sockaddr*)&ss, &nss);

		std::string str;
		do {
			str = WSA::recv(si);
			std::cout << str << std::endl;
			WSA::send(si, str);
		} while (str != "q\r\n");
	}
	catch (int err) {
		return err;
	}

	return 0;
}