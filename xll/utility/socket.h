// socket.h - socket wrappers
// Throws int that can be fed to gai_strerror().
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <sstream>
#include <vector>
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
		ADDRINFO(SOCKET s)
		{
			WSAPROTOCOL_INFO pi;
			int npi = sizeof(pi);

			if (::getsockopt(s, SOL_SOCKET, SO_PROTOCOL_INFO, (char*)&pi, &npi))
				throw WSAGetLastError();

			ai_flags = 0;
			ai_family = pi.iAddressFamily;
			ai_socktype = pi.iSocketType;
			ai_protocol = pi.iProtocol;
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
		int sendto(SOCKET s, const char* msg, size_t len = 0, int flags = 0)
		{
			if (!len)
				len = strlen(msg);

			return ::sendto(s, msg, (int)len, flags, pai->ai_addr, (int)pai->ai_addrlen);
		}
		int recvfrom(SOCKET s, char* buf, int len, int flags = 0)
		{
			int addrlen = (int)pai->ai_addrlen;

			return ::recvfrom(s, buf, len, flags, pai->ai_addr, &addrlen);
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
				
				if (-1 == bind(s, pai->ai_addr, (int)pai->ai_addrlen)) {
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
n					continue;
				if (-1 == connect(s, pai->ai_addr, (int)pai->ai_addrlen)) {
					closesocket(s);
					continue;
				}

				break;
			}

			if (!pai || s == INVALID_SOCKET)
				throw WSAGetLastError();
		}
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

	inline std::vector<char> recv(SOCKET s, int nbuf = 4096)
	{
		std::vector<char> buf;
		buf.resize(nbuf);

		int off = 0, n;
		do {
			n = ::recv(s, &buf[off], nbuf, 0);
			if (n > 0) {
				off += n;
				buf.resize(off + nbuf);
			}
		} while (n == nbuf); //!!! recv blocks if data is muliple of nbuf

		if (n == SOCKET_ERROR)
			throw WSAGetLastError();
		else
			buf.resize(off);

		return buf;
	}

	template<size_t nbuf = 0x80>
	struct socketbuf : std::streambuf
	{
		typedef std::streambuf::int_type int_type;
		typedef std::streambuf::traits_type traits_type;
		typedef std::streambuf::char_type char_type;

		socketbuf(SOCKET s)
			: s(s)
		{
			this->setg(rbuf, rbuf + nbuf, rbuf + nbuf);
			this->setp(wbuf, wbuf + nbuf);
		}

		int_type underflow()
		{
			size_t read;
			if (0 < (read = ::recv(s, this->eback(), nbuf, 0)))
			{
				this->setg(this->eback(), this->eback(), this->eback() + read);

				return traits_type::to_int_type(*this->eback());
			}

			return traits_type::eof();
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
				::send(s, this->pbase(), chars, 0);
				this->setp(this->pbase(), this->epptr());
			}

			if (!traits_type::eq_int_type(c, traits_type::eof()))
			{
				char_type e = traits_type::to_char_type(c);
				::send(s, &e, sizeof(char_type), 0);
			}

			return !traits_type::eof();
		}

		SOCKET s;
		char rbuf[nbuf];
		char wbuf[nbuf];
	};

	// operator<<() and operator>>() for sockets
	template<size_t nbuf = 0x80>
	struct socketstream : std::iostream
	{
		socketstream(SOCKET socket)
			: std::iostream(&buf), buf(socket)
		{}

		socketbuf<nbuf> buf;
	};


} // winsock2