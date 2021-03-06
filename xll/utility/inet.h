// inet.h - Wininet wrappers
#pragma once
#include "../ensure.h"
#include "scoped_handle.h"
#include <initializer_list>
#include <memory>
#include <Windows.h>
#include <tchar.h>
#include <WinInet.h>

#pragma comment(lib, "wininet.lib")

template<>
struct ::scoped_handle::traits<HINTERNET> {
	static HINTERNET invalid(void) 
		{ return nullptr; }
	static void close(HINTERNET h) 
		{ InternetCloseHandle(h); }
};


namespace Inet {

	using handle = ::scoped_handle::base<HINTERNET>;

	// read all bytes from InternetOpenUrl
	std::basic_string<TCHAR> ReadFile(HINTERNET h)
	{
		DWORD n;
		
		ensure (InternetQueryDataAvailable(h, &n, 0, 0));

		std::basic_string<TCHAR> buf;
		buf.reserve(n + 1);

		ensure (InternetReadFile(h, &buf[0], n, &n));

		return buf;
	}


	struct Open : public handle {
//		Open() : handle() { }
		Open(LPCTSTR lpszAgent = _T("WinInet"), DWORD access = INTERNET_OPEN_TYPE_DIRECT)
			: handle(InternetOpen(lpszAgent, access, nullptr, nullptr, 0))
		{
			ensure (h_);
		}
		Open(const Open&) = delete;
		Open& operator=(const Open&) = delete;
		~Open()
		{ }
		struct Connection : public handle {
			Connection() : handle() { }
			Connection(const Open& h, LPCTSTR lpszServerName, INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTP_PORT,
				LPCTSTR lpszUsername = nullptr, LPCTSTR lpszPassword = nullptr,
				DWORD dwService = INTERNET_SERVICE_HTTP, DWORD dwFlags = 0, DWORD dwContext = 0)
				: handle(InternetConnect(h, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext))
			{
				ensure (h_);
			}
			Connection(const Connection&) = delete;
			Connection(Connection&& c)
			{
				h_ = c.h_;
			}
			Connection operator=(const Connection&) = delete;
			~Connection()
			{ }
			struct Request : public handle {
//				Request() : handle() { }
				Request(const Connection& h, LPCTSTR lpszVerb = _T("GET"), LPCTSTR lpszObjectName = _T("/"),
					LPCTSTR lpszVersion = nullptr, LPCTSTR lpszReferer = nullptr, 
					LPCTSTR *lplpszAcceptTypes = nullptr, DWORD dwFlags = 0, DWORD_PTR dwContext = 0)
					: handle(HttpOpenRequest(h, lpszVerb, lpszObjectName, lpszVersion, lpszReferer, lplpszAcceptTypes, dwFlags, dwContext))
				{
					ensure (h_);
				}
				Request(const Request&) = delete;
				Request(Request&& r)
				{
					h_ = r.h_;
				}
				Request operator=(const Request&) = delete;
				~Request()
				{ }
				BOOL AddHeader(LPCTSTR header, DWORD flags = 0) const
				{
					DWORD len = static_cast<DWORD>(_tcsclen(header));

					return HttpAddRequestHeaders(*this, header, len, flags);
				}
				BOOL AddHeader(LPCTSTR key, LPCTSTR value, DWORD flags = 0)
				{
					std::basic_string<TCHAR> header(key);

					header.append(_T(" "));
					header.append(value);
					DWORD len = static_cast<DWORD>(header.length());

					return HttpAddRequestHeaders(*this, header.c_str(), len, flags);
				}
				BOOL Send(LPCTSTR data, DWORD ndata = 0)
				{
					if (!ndata)
						ndata = static_cast<DWORD>(_tcslen(data));

					return HttpSendRequest(h_, 0, 0, (LPVOID)data, ndata);
				}
				std::basic_string<TCHAR> Read()
				{
					return ReadFile(h_);
				}
			};
			Request&& OpenRequest(LPCTSTR lpszVerb = _T("GET"), LPCTSTR lpszObjectName = _T("/"),
					LPCTSTR lpszVersion = _T("HTTP/1.1"), LPCTSTR lpszReferer = nullptr, 
					LPCTSTR *lplpszAcceptTypes = nullptr, DWORD dwFlags = 0, DWORD_PTR dwContext = 0) const
			{
				return std::move(Request(*this, lpszVerb, lpszObjectName, lpszVersion, lpszReferer, lplpszAcceptTypes, dwFlags, dwContext));
			}
		};
		struct URL : public handle {
			URL()
				: handle()
			{ }
			URL(const Open& h, LPCTSTR lpszUrl, LPCTSTR lpszHeaders = nullptr, DWORD dwHeadersLength = 0,
				DWORD dwFlags = 0, DWORD_PTR dwContext = 0)
				: handle(InternetOpenUrl(h, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext))
			{
				ensure (h_ || !"InternetOpenUrl failed");
			}
			URL(const URL&) = delete;
			URL(URL&& u)
			{
				h_ = u.h_;
			}
			URL& operator=(const URL&) = delete;
			~URL()
			{ }

			// read all bytes from InternetOpenUrl
			std::basic_string<TCHAR> Read()
			{
				return ReadFile(h_);
			}

		};

		Connection&& Connect(LPCTSTR lpszServerName, INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTP_PORT,
				LPCTSTR lpszUsername = nullptr, LPCTSTR lpszPassword = nullptr,
				DWORD dwService = INTERNET_SERVICE_HTTP, DWORD dwFlags = 0, DWORD dwContext = 0) const
		{
			return std::move(Connection(*this, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext));
		}
		URL&& OpenUrl(LPCTSTR lpszUrl, LPCTSTR lpszHeaders = nullptr, DWORD dwHeadersLength = 0,
				DWORD dwFlags = 0, DWORD_PTR dwContext = 0) const
		{
			return std::move(URL(*this, lpszUrl, lpszHeaders, dwHeadersLength, dwFlags, dwContext));
		}
		/*
		handle FtpFile(Connection c, LPCTSTR lpszFilename, DWORD dwAccess = GENERIC_READ, 
			DWORD dwFlags = FTP_TRANSFER_TYPE_UNKNOWN, DWORD_PTR dwContext = 0)
		{
			return FtpOpenFile(c, lpszFilename, dwAccess, dwFlags, dwContext);
		}
		*/
	};

} // Inet