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

bool isrightint(const char* s) {
	int Length = strlen(s);
	int head = 0;
	if (s[0] == '+' || s[0] == '-')
		++head;
	for (; head < Length; ++head) {
		if (s[head] == '+' || s[head] == '-')return false;
		if (s[head] < '0' || s[head]>'9')return false;
	}
	return true;
}

bool isrightdouble(const char* s) {
	int Length = strlen(s);
	int head = 0;
	if (s[0] == '+' || s[0] == '-')
		++head;
	int pointpos = -1;
	for (; head < Length; ++head) {
		if (s[head] == 'e' || s[head] == 'E')
			break;
		if (s[head] == '.') {
			if (pointpos != -1)return false;
			pointpos = head;
		}
		else if (s[head] < '0' || s[head]>'9')return false;
	}
	if (pointpos == head - 1)return false;
	if (s[head] == 'e' || s[head] == 'E') {
		++head;
		if (head < Length && (s[head] == '+' || s[head] == '-'))
			++head;
		for (; head < Length; ++head)
			if (s[head] < '0' || s[head]>'9')return false;
	}

	return true;
}


_MATH_END

