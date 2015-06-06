// server.cpp - simple socket server
#include "../../xll/utility/socket.h"

int main()
{
	wsa::socket s{};
	wsa::sockaddr sa(AF_INET, 3456, "localhost");

	bind(s, &sa, sizeof(sa));
	listen(s, 3);

	int san =sizeof(sa);
	SOCKET si = accept(s, &sa, &san);

	std::string str;
	do {
		str = wsa::recv(si);
		wsa::send(si, str);
	} while (str != "q\r\n");


	return 0;
}