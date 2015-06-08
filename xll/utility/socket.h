// socket.h - socket wrappers
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <sstream>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace wsa {

	// Windows only
	class Startup {
		int result;
		WSADATA wsaData;
	public:
		Startup()
			: result(WSAStartup(MAKEWORD(2,2), &wsaData))
		{ }
		Startup(const Startup&) = delete;
		Startup& operator=(const Startup&) = delete;
		~Startup()
		{
			WSACleanup();
		}
		operator int() const
		{
			return result;
		}
	};
	// singleton
	int startup()
	{
		static Startup result;

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
		socket(const socket&) = delete;
		socket& operator=(const socket&) = delete;
		~socket()
		{
			::closesocket(s_);
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
		::sockaddr* operator&()
		{
			return reinterpret_cast<::sockaddr*>(this);
		}
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
			
			::getaddrinfo(host, port, &hints, &pai);
		}
		addrinfo(const addrinfo&) = delete;
		addrinfo& operator=(const addrinfo&) = delete;
		~addrinfo()
		{
			freeaddrinfo(pai);
		}

		::sockaddr* operator&()
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

			return ::sendto(s_, buf, len, 0, &ai_, sizeof(::sockaddr));
		}
	};

	inline int connect(SOCKET s, const char* host, const char* port)
	{
		addrinfo ai(host, port, s);

		int result = ::connect(s, ai.pai->ai_addr, static_cast<int>(ai.pai->ai_addrlen));
		if (result == SOCKET_ERROR)
			throw std::runtime_error("wsa::connect: socket error");

		return result;
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
		std::ostringstream oss{};
		char* buf = static_cast<char*>(_alloca(bufsiz));

		int n;
		do {
			n = ::recv(s, buf, bufsiz, 0);
			if (n > 0)
				oss.write(buf, n);
		} while (n == bufsiz);

		if (n == SOCKET_ERROR)
			throw std::runtime_error("wsa::recv: socket error");

		return oss.str();
	}
	
	struct socketbuf : std::streambuf
	{
		typedef std::streambuf::int_type int_type;
		typedef std::streambuf::traits_type traits_type;
		typedef std::streambuf::char_type char_type;

		static size_t const buffer_size = 0x80;

		socketbuf(SOCKET socket)
			: socket_(socket)
		{
			this->setg(read_buffer_, read_buffer_ + buffer_size,
				read_buffer_ + buffer_size);
			this->setp(write_buffer_, write_buffer_ + buffer_size);
		}

		int_type underflow()
		{
			size_t read;
			if (0 < (read = ::recv(socket_, this->eback(), buffer_size, 0)))
			{
				this->setg(this->eback(), this->eback(), this->eback()
					+ read);
				return traits_type::to_int_type(*this->eback());
			}
			else
			{
				return traits_type::eof();
			}
		}

		int sync()
		{
			this->overflow(traits_type::eof());
			return 0;
		}

		int_type overflow(int_type c)
		{
			if (const int_type chars = this->pptr() - this->pbase())
			{
				::send(socket_, this->pbase(), chars, 0);
				this->setp(this->pbase(), this->epptr());
			}

			if (!traits_type::eq_int_type(c, traits_type::eof()))
			{
				char_type e = traits_type::to_char_type(c);
				::send(socket_, &e, sizeof(char_type), 0);
			}

			return !traits_type::eof();
		}

		SOCKET socket_;
		char read_buffer_[buffer_size];
		char write_buffer_[buffer_size];
	};


	struct socketstream : std::iostream
	{
		socketstream(SOCKET socket)
			: std::iostream(&buffer_)
			, buffer_(socket)
		{}

		socketbuf buffer_;
	};


} // winsock2