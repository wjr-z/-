#include <Windows.h>
#include "src/Matrix.h"
#include "src/bint.h"
#include "src/bfloat.h"
#include "src/lz77.h"
#include "src/wtest.h"
#include <functional>
using namespace Math;
using namespace std;
#include <random>
std::mt19937 mt_rand(time(NULL));
void randdata(bint& x, size_t n) {
	if (n < 1)return;
	x = 0;
	size_t Size = n - 1 >> 3;
	for (size_t i = 0; i < Size; ++i)
		x.at(i) = randint(0, bintjw - 1);
	for (size_t i = Size << 3; i < n - 1; ++i)
		x[i]=randint(0, 9);
	x[n-1]=randint(1, 9);
}

int main() {
	//void (*p)(int,int) =work;
	
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	bint a, b,c;
	a = qpow(bint(12345),5678);
	b = qpow(bint(12345),6000);

	cout<<qtime(
		[&a,&b,&c]() {
			for(int i=0;i<1000;++i)
				c=a*b;
		}
	)<<endl;
	
	return 0;

	FILE* stream1;
	freopen_s(&stream1, "test.out", "w", stdout);
	cout.setf(ios::left); //设置对齐方式为left

	int HEAD = 400000, TAIL = 100000, PY = 10;

	for (int i = HEAD; i <= TAIL; i += PY) {
		for (int j = HEAD; j <= i; j += PY) {
			int len1, len2;
			len1 = ((i - 1) >> 3) + 1;
			len2 = ((j - 1) >> 3) + 1;
			bint x, y;
			randdata(x, i);
			randdata(y, j);
			double stime, ftime, ktime,ttime;
			cout << setw(4) << "i=" << setw(4) << i << setw(6) << "  j=" << setw(4) << j << ' ';
			int K = 4;
			bint ans1, ans2, ans3,ans5;
		#ifdef TEST
			ans1 = test1(x, y);
			ans2 = test2(x, y);
			ans3 = test3(x, y);
			ans5 = testTOOM(x,y);
		#endif
			START
			#ifdef TEST
				//for (int k = 0; k < K; ++k)
					//test1(x, y);
		#endif
			END

				cout << setw(6) << "暴力：" << setw(12) << (stime = en - sta);
			cout << " ";

			START
			#ifdef TEST
				for (int k = 0; k < K; ++k)
					test2(x, y);
		#endif
			END

				cout << setw(6) << "Kar：" << setw(12) << (ktime = en - sta);
			cout << " ";
			START
			#ifdef TEST
				for (int k = 0; k < K; ++k)
					test3(x, y);
		#endif
			END
				cout << setw(6) << "FFT：" << setw(12) << (ftime = en - sta);
			cout << " ";

			START
			#ifdef TEST
				for (int k = 0; k < K; ++k)
					testTOOM(x, y);
		#endif
			END
				cout << setw(6) << "TOOM_COOK_3：" << setw(12) << (ttime = en - sta);
			cout << " ";

			START
			#ifdef TEST
				for (int k = 0; k < K; ++k)
					test4(x, y);
		#endif
			END
				cout << setw(6) << "智能模式：";
			if (mode == 1)cout << setw(6) << "暴力 ";
			else if (mode == 2)cout << setw(6) << "Kar ";
			else cout << setw(6) << "FFT ";

			cout << "  " << setw(14) << en - sta << ' ';
			cout << setw(6) << "最优选择：";
			if (stime < ftime && stime < ktime&&stime<ttime) {
				cout << "暴力" << ' ';
			}
			if (ktime < stime && ktime < ftime&&ktime<ttime) {
				cout << "Kar" << ' ';
			}
			if (ftime < stime && ftime < ktime&&ftime<ttime) {
				cout << "FFT" << ' ';
			}
			if (ttime < stime && ttime < ktime && ttime < ftime) {
				cout<<"TOOM_COOK"<<' ';
			}
			if (ans1 == ans2 && ans1 == ans3&&ans1==ans5) {
				cout << "正确\n";
			}
			else {
				if (ans1 != ans3) {
					cout << "暴力错误\n";
				}
				if (ans1 == ans3 && ans1 != ans2) {
					cout << "Kar错误\n";
				}
				else if (ans1 == ans2 && ans1 != ans3) {
					cout << "FFT错误\n";

					//test3(x, y);
				}

			}
		}
	}
	return 0;

}