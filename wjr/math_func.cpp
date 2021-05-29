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

int sqrt(int A) {
	int ans[2] = { -1,1 };
	int now = 1;
	while (std::abs(ans[0] - ans[1]) > 1) {
		now ^= 1;
		ans[now] = (ans[now ^ 1] + A / ans[now ^ 1]) / 2;
	}return ans[now];
}

_MATH_END