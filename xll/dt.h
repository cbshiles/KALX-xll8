// dt.h - lightweight date and time functions
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <ctime>

namespace xll {

	#define EXCEL_EPOCH 25569 // Jan 1, 1970 - start of unix time
	#define SECS_PER_DAY 60*60*24
	
	inline void tzset(const char* tz = nullptr)
	{
		static bool first(true);

		if (tz) {
			ensure (0 == _putenv_s("TZ", tz));
			first = true;
		}

		if (first) {
			_tzset();
			first = false;
		}
	}

	// daylight savings time adjustment
	inline long 
	dst(time_t t) 
	{
		struct tm tm;
		
		tzset();
		ensure (0 == ::localtime_s(&tm, &t)); // slow

		return tm.tm_isdst*3600;
	}

	// Excel local time to UTC.
	// Should agree with mktime for broken down time.
	inline time_t
	excel2time_t(double d, bool nodst = false)
	{
		ensure (EXCEL_EPOCH <= d);

		tzset();
		time_t t = static_cast<time_t>(0.5 + _timezone + (d - EXCEL_EPOCH)*SECS_PER_DAY);

		return t - (nodst ? 0 : dst(t));
	}
	// UTC to Excel local time.
	inline double
	time_t2excel(time_t t, bool nodst = false)
	{
		tzset();
		return static_cast<double>(EXCEL_EPOCH + (t - _timezone + (nodst ? 0 : dst(t)))/SECS_PER_DAY); 
	}

} // namespace xll
