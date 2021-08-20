
#include <bits/stdc++.h>
#include "src/bint.h"
#include "src/mtool.h"

using namespace Math;
using namespace std;
int main() {

	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	
	bint a, b, c;
	
	a = qpow(bint(12345), 5678);
	cout << qtime(
		[&a,&c]() {
			for (int i = 0; i < 1000; ++i) 
				c = a * a;
		}
	) << endl;
	return 0;

	FILE* stream1;
	freopen_s(&stream1, "test.out", "w", stdout);
	cout.setf(ios::left); //设置对齐方式为left

	int HEAD = 40000, TAIL = 40000, PY = 10;

	for (int i = HEAD; i <= TAIL; i += PY) {
		for (int j = HEAD; j <= i; j += PY) {
			int len1, len2;
			len1 = ((i - 1) >> 3) + 1;
			len2 = ((j - 1) >> 3) + 1;
			bint x, y;
			x = randbint(i);
			y = randbint(j);
			double stime, ftime, ktime, ttime;
			cout << setw(4) << "i=" << setw(4) << i << setw(6) << "  j=" << setw(4) << j << ' ';
			int K = 20;
			bint ans1, ans2, ans3, ans5;
#ifdef TEST
			ans1 = test1(x, y);
			ans2 = test2(x, y);
			ans3 = test3(x, y);
			ans5 = testTOOM(x, y);
#endif
			auto sta = GetTime();
#ifdef TEST
			for (int k = 0; k < K; ++k)
				test1(x, y);
#endif

			auto en = GetTime();
			cout << setw(6) << "暴力：" << setw(12) << (stime = en - sta);
			cout << " ";

			sta = GetTime();
#ifdef TEST
			for (int k = 0; k < K; ++k)
				test2(x, y);
#endif
			en = GetTime();

			cout << setw(6) << "Kar：" << setw(12) << (ktime = en - sta);
			cout << " ";
			sta = GetTime();
#ifdef TEST
			for (int k = 0; k < K; ++k)
				test3(x, y);
#endif
			en = GetTime();
			cout << setw(6) << "FFT：" << setw(12) << (ftime = en - sta);
			cout << " ";

			sta = GetTime();
#ifdef TEST
			for (int k = 0; k < K; ++k)
				testTOOM(x, y);
#endif
			en = GetTime();
			cout << setw(6) << "TOOM_COOK_3：" << setw(12) << (ttime = en - sta);
			cout << " ";

			sta = GetTime();
#ifdef TEST
			for (int k = 0; k < K; ++k)
				test4(x, y);
#endif
			en = GetTime();
			cout << setw(6) << "智能模式：";
			if (mode == 1)cout << setw(6) << "暴力 ";
			else if (mode == 2)cout << setw(6) << "Kar ";
			else cout << setw(6) << "FFT ";

			cout << "  " << setw(14) << en - sta << ' ';
			cout << setw(6) << "最优选择：";
			double mintime = min({stime, ftime, ktime, ttime});

			if (mintime == stime) {
				cout << "暴力" << ' ';
			}
			if (mintime == ktime) {
				cout << "Kar" << ' ';
			}
			if (mintime == ftime) {
				cout << "FFT" << ' ';
			}
			if (mintime == ttime) {
				cout << "TOOM_COOK" << ' ';
			}
			if (check(ans1, ans3, ans5)) {
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
				}

			}
		}
	}
	return 0;

}
