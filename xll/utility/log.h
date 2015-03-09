// log.h - Windows Event logging.
// Uses System32\\EventCreate.exe as default message file.
#pragma once
#include "registry.h"

#pragma comment(lib, "advapi32.lib")

namespace Log {

	class EventSource {
		HANDLE hEventLog;
		// Event view looks up registry entry for message file.
		void Register(LPCTSTR lpSourceName, LPCTSTR lpMsgFile, DWORD dwTypesSupported)
		{
			std::basic_string<TCHAR> src(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\"));
			src.append(lpSourceName);

			try {
				Reg::OpenKey<TCHAR> rok(HKEY_LOCAL_MACHINE, src.c_str(), KEY_ALL_ACCESS);
			}
			catch (const std::exception&) {
				Reg::CreateKey<TCHAR> key(HKEY_LOCAL_MACHINE, src.c_str());
				key.SetValue(_T("CustomSource"), 1);
				key.SetValue(_T("EventMessageFile"), lpMsgFile);
				key.SetValue(_T("TypesSupported"), dwTypesSupported);
			}
		}
	public:
		EventSource(LPCTSTR lpSourceName, LPCTSTR lpMsgFile = _T("%SystemRoot%\\System32\\EventCreate.exe"), DWORD dwTypesSupported = 7)
		{
			hEventLog = RegisterEventSource(NULL, lpSourceName);
			if (!hEventLog)
				throw std::runtime_error("Log::EventSource: RegisterEventSource failed");

			//Register(lpSourceName, lpMsgFile, dwTypesSupported);
		}
		EventSource(const EventSource&) = delete;
		EventSource& operator=(const EventSource&) = delete;
		~EventSource()
		{
			DeregisterEventSource(hEventLog);
		}
		// Category is not used. Ever.
		BOOL ReportEvent(WORD wType, DWORD dwEventID, const char* lpString)
		{
			return ::ReportEventA(hEventLog, wType, 0, dwEventID, NULL, 1, 0, &lpString, NULL);
		}
		BOOL ReportError(const char* lpString, DWORD dwEventID = 1)
		{
			return ::ReportEventA(hEventLog, EVENTLOG_ERROR_TYPE, 0, dwEventID, NULL, 1, 0, &lpString, NULL);
		}
		BOOL ReportWarning(const char* lpString, DWORD dwEventID = 1)
		{
			return ::ReportEventA(hEventLog, EVENTLOG_WARNING_TYPE, 0, dwEventID, NULL, 1, 0, &lpString, NULL);
		}
		BOOL ReportInformation(const char* lpString, DWORD dwEventID = 1)
		{
			return ::ReportEventA(hEventLog, EVENTLOG_INFORMATION_TYPE, 0, dwEventID, NULL, 1, 0, &lpString, NULL);
		}
	};

} // namespace Log