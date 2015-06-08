// server.cpp - simple socket server
#include <iostream>
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
		std::cout << str << std::endl;
		wsa::send(si, str);
	} while (str != "q\r\n");


	return 0;
}