// codec.h - encode/decode
#pragma once
#include "../ensure.h"
#include <string>
#include <vector>
#define _ATL_ALLOW_CHAR_UNSIGNED
#include <atlenc.h>

#pragma 

namespace Base64 {

	inline std::string Encode(const BYTE* src, int len, DWORD flags = ATL_BASE64_FLAG_NONE)
	{
		std::string enc;

		int n = Base64EncodeGetRequiredLength(len, flags);
		enc.resize(n);
		ensure (Base64Encode(src, len, &enc[0], &n, flags));
		enc.resize(n);

		return enc;
	}
	inline std::vector<BYTE> Decode(const char* src, int len = 0)
	{
		std::vector<BYTE> dec;

		if (!len)
			len = strlen(src);

		int n = Base64DecodeGetRequiredLength(len);
		dec.resize(n);
		ensure (Base64Decode(src, len, &dec[0], &n));
		dec.resize(n);

		return dec;
	}
} // Base64