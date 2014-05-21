// google.h - Google analytics reporting
// google::analytics ga; ga.type("xlAutoOpen").post();
#pragma once
#include <strstream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <sddl.h>
#include "../ensure.h"

#pragma comment(lib, "Ws2_32.lib")

#ifndef GOOGLE_USER_AGENT
#define GOOGLE_USER_AGENT "Xll/1.0"
#endif
#define GOOGLE_ANALYTICS_HOST "www.google-analytics.com"
#ifndef GOOGLE_TRACKING_ID
#define GOOGLE_TRACKING_ID "UA-48552649-1"
#endif
#ifndef GOOGLE_PROTOCOL_VERSION
#define GOOGLE_PROTOCOL_VERSION "1"
#endif

namespace google {

	class analytics {
		struct startup {
			static int init(void)
			{
				static WSADATA wsaData;
				static int i = WSAStartup(MAKEWORD(2,2), &wsaData);
				
				return i;
			}
			startup()
			{
				ensure (0 == init());
			}
			startup(const startup&) = delete;
			startup& operator=(const startup&) = delete;
			~startup()
			{
				WSACleanup();
			}
		};

		// unique client id
		static LPCTSTR cid(void)
		{
			static LPTSTR pcid(nullptr);

			if (!pcid) {
				TCHAR account[1024];
				DWORD naccount(1024);
				GetUserName(account, &naccount);

				PSID psid = nullptr;
				BYTE sid[SECURITY_MAX_SID_SIZE];
				DWORD nsid(SECURITY_MAX_SID_SIZE);
				psid = (PSID)&sid;

				SID_NAME_USE snu;
				TCHAR domain[1024];
				DWORD ndomain(1024);

				LookupAccountName(0, account, psid, &nsid, domain, &ndomain, &snu);
				ConvertSidToStringSid(psid, &pcid);

			}

			return pcid;
		}

		int init_;
		SOCKET s_;
		std::ostringstream post_, data_;
	public:
		analytics()
			: init_(google::analytics::startup::init()), s_(::socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP))
		{
			ensure (s_ != INVALID_SOCKET);
			::addrinfo *pai, hints;

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			ensure (0 == ::getaddrinfo(GOOGLE_ANALYTICS_HOST, "http", &hints, &pai));
			int result = ::connect(s_, pai->ai_addr, pai->ai_addrlen);
			::freeaddrinfo(pai);

			ensure (result != SOCKET_ERROR);
		}
		analytics(const analytics&) = delete;
		analytics& operator=(const analytics&) = delete;
		~analytics()
		{
			::closesocket(s_);
		}

		analytics& post(LPCTSTR type)
		{
			post_.clear();
			post_ << "POST /collect HTTP/1.1\r\n";
			post_ << "Host: www.google-analytics.com\r\n";
			post_ << "User-Agent: " << GOOGLE_USER_AGENT << "\r\n";

			data_.clear();
			data_ << "v=" GOOGLE_PROTOCOL_VERSION << "\r\n";
			data_ << "&tid=" << GOOGLE_TRACKING_ID << "\r\n";
			data_ << "&cid=" << cid() << "\r\n";
			data_ << "&t=" << type << "\r\n";

			return *this;
		}
		void send()
		{
			post_ << "Content-Length: " << data_.str().length() << "\r\n\r\n";

			::send(s_, post_.str().c_str(), post_.str().length(), 0);
			::send(s_, data_.str().c_str(), data_.str().length(), 0);
		}
		std::string get()
		{
			post_ << "Content-Length: " << data_.str().length() << "\r\n\r\n";

			return data_.str() + post_.str();
		}
	};
	
};

