#ifndef WTEST_H
#define WTEST_H

#include <utility>
#include <chrono>

namespace Math {

	typedef std::chrono::high_resolution_clock clock;
	typedef std::chrono::microseconds mis;

	namespace myclock {//Мђвз
		clock::time_point NowTime();
		double testTime(const clock::time_point&start,const clock::time_point&end);
	}

	template<typename Fn>
	double qtime(Fn&& ToBeTest) {
		clock::time_point start=myclock::NowTime();
		ToBeTest();
		return myclock::testTime(start,myclock::NowTime());
	}

	template<typename Fn, typename... Args>
	double qtime(Fn&& ToBeTest, Args&&...List) {
		clock::time_point start = myclock::NowTime();
		ToBeTest(std::forward<Args>(List)...);
		return myclock::testTime(start, myclock::NowTime());
	}

	template<typename Fn>
	double qavltime(Fn&& ToBeTest,int kth) {
		clock::time_point start = myclock::NowTime();
		for(int i=0;i<kth;++i)
			ToBeTest();
		return myclock::testTime(start, myclock::NowTime())/kth;
	}

	template<typename Fn, typename... Args>
	double qavltime(Fn&& ToBeTest, int kth, Args&&...List) {
		clock::time_point start = myclock::NowTime();
		for (int i = 0; i < kth; ++i)
			ToBeTest(std::forward<Args>(List)...);
		return myclock::testTime(start, myclock::NowTime()) / kth;
	}

}
#endif