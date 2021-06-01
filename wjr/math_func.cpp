#include "math_func.h"


/*---测试用高精度计时器---*/
#ifdef TEST
int mode;
double currTime() //使用高精度计时器
{
	LARGE_INTEGER performanceCount;
	double time;
	BOOL result;
	if (freq.QuadPart == 0) {
		BOOL bRet = initFreq();
		if (!bRet)
			return 0;
	}
	result = QueryPerformanceCounter(&performanceCount);
	time = performanceCount.HighPart * 4294967296.0 + performanceCount.LowPart;
	time = time / (freq.HighPart * 4294967296.0 + freq.LowPart);
	return time;
}
double tot, sta, en;

#endif


_MATH_BEGIN



_MATH_END

