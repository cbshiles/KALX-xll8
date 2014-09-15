// socket.h - socket wrappers
#pragma once
#include <string>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace wsa {

	// Windows only
	int startup()
	{
		static int result(-1);
		static WSADATA wsaData;

		if (-1 == result) {
			result = WSAStartup(MAKEWORD(2,2), &wsaData);
		}

		return result;
	}

	class socket {
		SOCKET s_;
	public:
		// default to IPv4 TCP
		socket(int af = AF_INET, int type = SOCK_STREAM, int proto = IPPROTO_TCP)
		{
			s_ = (0 != startup()) ? INVALID_SOCKET : ::socket(af, type, proto);
		}
		socket(const socket&) = default;
		socket& operator=(const socket&) = default;
		~socket()
		{
		}
		operator const SOCKET&() const
		{
			return s_;
		}
	};

	struct sockaddr : ::sockaddr_in {
		sockaddr(ADDRESS_FAMILY family = AF_UNSPEC, USHORT port = 0, const char* host = 0)
		{
			sin_family = family;
			sin_port = htons(port);
			if (host && *host) {
				if (isdigit(host[0])) {
					sin_addr.s_addr = inet_addr(host); 
				}
				else {
					hostent* ph = gethostbyname(host);
					if (ph)
						memcpy(&sin_addr, ph->h_addr, ph->h_length);
				}
			}
			else {
				sin_addr.s_addr = INADDR_ANY;
			}
		}
		sockaddr(const sockaddr&) = delete;
		sockaddr operator=(const sockaddr&) = delete;
		~sockaddr()
		{ }
	};

	struct addrinfo {
		::addrinfo* pai;
		addrinfo(const char* host, const char* port, SOCKET s = INVALID_SOCKET)
		{
			::addrinfo hints;

			memset(&hints, 0, sizeof(hints));
			if (INVALID_SOCKET != s) {
				WSAPROTOCOL_INFO pi;

				int npi(sizeof(pi));
				if (0 != getsockopt(s, SOL_SOCKET, SO_PROTOCOL_INFO, (char*)&pi, &npi)) {
					hints.ai_family = pi.iAddressFamily;
					hints.ai_socktype = pi.iSocketType;
					hints.ai_protocol = pi.iProtocol;
				}
			}
			else {
				hints.ai_family = AF_UNSPEC;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;
			}
			
			getaddrinfo(host, port, &hints, &pai);
		}
		addrinfo(const addrinfo&) = delete;
		addrinfo& operator=(const addrinfo&) = delete;
		~addrinfo()
		{
			freeaddrinfo(pai);
		}

		const ::sockaddr* sock() const
		{
			return pai->ai_addr;
		}
	};

	class dgram  {
		SOCKET s_;
		addrinfo ai_;
	public:
		dgram(const char* host, const char* port)
			: s_(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)), ai_(host, port, s_)
		{
		}
		dgram(const dgram&) = delete;
		dgram& operator=(const dgram&) = delete;
		~dgram()
		{ 
			closesocket(s_);
		}

		int sendto(const char* buf, int len = 0)
		{
			if (!len)
				len = static_cast<int>(strlen(buf));

			return ::sendto(s_, buf, len, 0, ai_.sock(), sizeof(::sockaddr));
		}
	};

	inline int connect(SOCKET s, const char* host, const char* port)
	{
		addrinfo ai(host, port, s);

		return ::connect(s, ai.pai->ai_addr, static_cast<int>(ai.pai->ai_addrlen));
	}

	inline int send(SOCKET s, const char* buf, int len = 0, int flags = 0)
	{
		if (!len)
			len = static_cast<int>(strlen(buf));

		return ::send(s, buf, len, flags);
	}
	inline int send(SOCKET s, const std::string& buf, int flags = 0)
	{
		return ::send(s, buf.c_str(), static_cast<int>(buf.length()), flags);
	}

	inline std::string recv(SOCKET s, int bufsiz = 4096)
	{
		std::string buf;
		buf.resize(bufsiz);

		for (int n = bufsiz, off = 0; n == bufsiz || n == 0; off += bufsiz) {
			buf.reserve(off + bufsiz);
			n = ::recv(s, &buf[off], bufsiz, 0);
			buf.resize(off + n);
		}

		return buf;
	}


} // winsock2