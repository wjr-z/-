#include <Windows.h>
#include "src/DataStructure/Matrix.h"
#include "src/BigInteger/bint.h"
#include "src/BigInteger/bfloat.h"
#include "src/compress/lz77.h"
using namespace Math;
using namespace std;
#include <random>
std::mt19937 mt_rand(time(NULL));
void randdata(bint& x, size_t n) {
	if (n < 1)return;
	x = 0;
	size_t Size = n - 1 >> 3;
	for (size_t i = 0; i < Size; ++i)
		x[i] = randint(0, bintjw - 1);
	for (size_t i = Size << 3; i < n - 1; ++i)
		x.set(i, randint(0, 9));
	x.set(n - 1, randint(1, 9));
}


int main() {

	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	bint a, b;
	a = randdata(1000000);
	b = randdata(910000);

	START
		bint c = a / b;
	END
		cout << en - sta << endl;

	return 0;

	a = qpow(bint(1234), 56);
	string ttst = a.tostr();
	double ss = currTime();
	cout << "----------\n";
	for (int i = 0; i < 1000000; ++i)
		b = a * a;

	double tt = currTime();
	cout << "----------\n";
	cout << tt - ss << endl;

	//cout<<b<<endl;
	return 0;


	FILE* stream1;
	freopen_s(&stream1, "test.out", "w", stdout);
	cout.setf(ios::left); //设置对齐方式为left

	int HEAD = 10, TAIL = 1000, PY = 10;

	for (int i = HEAD; i <= TAIL; i += PY) {
		for (int j = HEAD; j <= i; j += PY) {
			int len1, len2;
			len1 = ((i - 1) >> 3) + 1;
			len2 = ((j - 1) >> 3) + 1;
			bint x, y;
			randdata(x, i);
			randdata(y, j);
			double stime, ftime, ktime;
			cout << setw(4) << "i=" << setw(4) << i << setw(6) << "  j=" << setw(4) << j << ' ';
			int K = 20;
			bint ans1, ans2, ans3;
		#ifdef TEST
			ans1 = test1(x, y);
			ans2 = test2(x, y);
			ans3 = test3(x, y);
		#endif
			START
			#ifdef TEST
				for (int k = 0; k < K; ++k)
					test1(x, y);
		#endif
			END

				cout << setw(6) << "暴力：" << setw(12) << (stime = en - sta);
			double a1 = stime * 1000000 / (len1 * len2);
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
			double a2 = ftime * 1000000 / ((len1 + len2) * quicklog2(len1 + len2));
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
			if (stime < ftime && stime < ktime) {
				cout << "暴力" << ' ';
			}
			if (ktime < stime && ktime < ftime) {
				cout << "Kar" << ' ';
			}
			if (ftime < stime && ftime < ktime) {
				cout << "FFT" << ' ';
			}
			if (ans1 == ans2 && ans1 == ans3) {
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