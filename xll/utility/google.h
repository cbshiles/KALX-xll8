// google.h - Google analytics reporting
// google::analytics ga; ga.type("xlAutoOpen").post();
#pragma once
#include <string>
#include "inet.h"
#include <sddl.h>

#ifndef GOOGLE_USER_AGENT
#define GOOGLE_USER_AGENT _T("Xll Add-in Library")
#endif
#define GOOGLE_ANALYTICS_URL _T("http://www.google.com/collect")
#ifndef GOOGLE_TRACKING_ID
#define GOOGLE_TRACKING_ID _T("UA-48552649-1")
#endif
#ifndef GOOGLE_PROTOCOL_VERSION
#define GOOGLE_PROTOCOL_VERSION _T("1")
#endif

namespace google {

	class analytics {
		static const Inet::Open& session(void)
		{
			static Inet::Open io(GOOGLE_USER_AGENT);

			return io;
		}
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
	public:
		static void post(LPCTSTR type)
		{
			std::basic_string<TCHAR> s(_T("/connect"));
			s.append(_T("?v="));
			s.append(GOOGLE_PROTOCOL_VERSION);
			s.append(_T("&tid="));
			s.append(GOOGLE_TRACKING_ID);
			s.append(_T("&cid="));
			s.append(cid());
			s.append(_T("&t="));
			s.append(type);

			std::string buf;
			Inet::Open::Connection ioc = session().Connect(_T("www.google-analytics.com"));
			Inet::Open::Connection::Request iocr = ioc.OpenRequest(_T("GET"), s.c_str());

			BOOL b;
			DWORD err;
			b = HttpSendRequest(iocr, nullptr, 0, nullptr, 0);
			err = GetLastError();
			b = HttpEndRequest(iocr, nullptr, 0, 0);
			err = GetLastError();

			char hdr[1024];
			DWORD n(1024);
			b = HttpQueryInfo(iocr, HTTP_QUERY_RAW_HEADERS, hdr, &n, 0);
			err = GetLastError();

			
		}
	};
	
};

