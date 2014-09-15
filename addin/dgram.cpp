// dgram.cpp
#include "../../xll8/xll/xll.h"
#include "../../xll8/xll/utility/socket.h"

#define KALX_HOST "108.168.134.126" 
#define KALX_PORT "26211"

using namespace xll;

int xll_startup(void)
{
	try {
		static wsa::dgram logger(KALX_HOST, KALX_PORT);
		
		int i;
		i = logger.sendto("xll\tstartup");
	}
	catch (const std::exception& ex) {
		int i;
		i = WSAGetLastError();
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<Open> xao_startup(xll_startup);

