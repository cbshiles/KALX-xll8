// socket.h - socket wrappers
// Throws int that can be fed to gai_strerror().
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <sstream>
#include <tchar.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace WSA {

	// Must be called before any other socket functions.
	class Startup {
		static int init()
		{
			static int result = -1;
			static WSADATA wsaData;

			if (-1 == result) {
				result = WSAStartup(WINSOCK_VERSION, &wsaData);
			}

			return result;
		}
		int result;
	public:
		Startup()
			: result(init())
		{
			if (0 != result)
				throw result;
		}
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

	// used for hints
	struct ADDRINFO : public ::ADDRINFOT {
		ADDRINFO(int flags = 0, int family = AF_INET, int socktype = SOCK_STREAM, int protocol = IPPROTO_TCP)
		{
			ZeroMemory(this, sizeof(*this));

			ai_flags = flags;
			ai_family = family;
			ai_socktype = socktype;
			ai_protocol = protocol;
		}
	};

	class AddrInfo {
		::ADDRINFOT* pai;
	public:
		AddrInfo(LPCTSTR host, LPCTSTR port, const ADDRINFO& hints = ADDRINFO())
		{
			int status = ::GetAddrInfo(host, port, &hints, &pai);
			if (0 != status)
				throw WSAGetLastError();
		}
		AddrInfo(const AddrInfo&) = delete;
		AddrInfo& operator=(const AddrInfo&) = delete;
		~AddrInfo()
		{
			FreeAddrInfo(pai);
		}
		::ADDRINFOT* operator&()
		{
			return pai;
		}
	};

	class Socket {
	protected:
		SOCKET s;
	public:
		Socket(int af = AF_INET, int type = SOCK_STREAM, int proto = IPPROTO_TCP)
			: s(::socket(af, type, proto))
		{ }
		Socket(const ::ADDRINFOT& ai)
			: Socket(ai.ai_family, ai.ai_socktype, ai.ai_protocol)
		{ }
		Socket(const Socket&) = default;
		Socket& operator=(const Socket&) = default;
		// must call closesocket() explicitly
		~Socket()
		{ }
		// act like a SOCKET
		explicit Socket(SOCKET s)
			: s(s)
		{ }
		operator SOCKET()
		{
			return s;
		}
	};

	struct Bind : public Socket {
		Bind(LPCTSTR host, LPCTSTR port, const ADDRINFO& hints = ADDRINFO(AI_PASSIVE))
			: Socket(INVALID_SOCKET)
		{
			AddrInfo ai(host, port, hints);

			ADDRINFOT* pai{0};
			for (pai = &ai; pai; pai = pai->ai_next) {
				s = Socket(*pai);
				if (s == INVALID_SOCKET)
					continue;
				
				int yes = 1;
				if (-1 == setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes))) {
					closesocket(s);
					continue;
				}
				
				if (-1 == bind(s, pai->ai_addr, pai->ai_addrlen)) {
					closesocket(s);
					continue;
				}

				break;
			}

			if (!pai || s == INVALID_SOCKET)
				throw WSAGetLastError();
		}
		operator ::SOCKET()
		{
			return s;
		}
	};
	struct Connect : public Socket {
		Connect(LPCTSTR host, LPCTSTR port)
		{
			AddrInfo ai(host, port, ADDRINFO{});
			ADDRINFOT* pai{0};
			for (pai = &ai; pai; pai = pai->ai_next) {
				s = Socket(*pai);
				if (s == INVALID_SOCKET)
					continue;
				if (-1 == connect(s, pai->ai_addr, pai->ai_addrlen)) {
					closesocket(s);
					continue;
				}
			}

			if (!pai || s == INVALID_SOCKET)
				throw WSAGetLastError();
		}
	};
	class Dgram : public Socket {
	};


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
			throw std::runtime_error("WSA::recv: socket error");

		return oss.str();
	}

	struct socketbuf : std::streambuf
	{
		typedef std::streambuf::int_type int_type;
		typedef std::streambuf::traits_type traits_type;
		typedef std::streambuf::char_type char_type;

		static size_t const nbuf = 0x80;

		socketbuf(SOCKET socket)
			: socket_(socket)
		{
			this->setg(rbuf, rbuf + nbuf,
				rbuf + nbuf);
			this->setp(wbuf, wbuf + nbuf);
		}

		int_type underflow()
		{
			size_t read;
			if (0 < (read = ::recv(socket_, this->eback(), nbuf, 0)))
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
		char rbuf[nbuf];
		char wbuf[nbuf];
	};

	// operator<<() and operator>>() for sockets
	struct socketstream : std::iostream
	{
		socketstream(SOCKET socket)
			: std::iostream(&buf)
			, buf(socket)
		{}

		socketbuf buf;
	};


} // winsock2