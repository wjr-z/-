#include "wtest.h"

#include <time.h>
namespace Math {
	namespace myclock {
		clock::time_point NowTime() {
			static clock::time_point nowtime;
			nowtime = clock::now();
			return nowtime;
		}
		double testTime(const clock::time_point&start,const clock::time_point&end) {
			return std::chrono::duration_cast<mis>(end-start).count()/1000.0;
		}
	}
}
