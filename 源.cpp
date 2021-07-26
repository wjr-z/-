#include <Windows.h>
#include "src/Matrix.h"
#include "src/bint.h"
#include "src/bfloat.h"
#include "src/lz77.h"
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

class qmatrix {
public:
	bint a[2];
	bint&operator [](int index){return a[index]; }
	const bint&operator[](int index)const{return a[index]; }
	qmatrix(){}
	qmatrix(const qmatrix&other):a{other[0],other[1]}{}
	qmatrix(qmatrix&& other)noexcept {
		a[0]=std::move(other[0]);
		a[1]=std::move(other[1]);
	}
	qmatrix(bint&& A, bint&& B) :a{ (std::move(A)),(std::move(B))} {}
	qmatrix& operator=(const qmatrix& other) {
		a[0] = other[0];
		a[1] = other[1];
		return*this;
	}
	qmatrix& operator=(qmatrix&& other)noexcept {
		a[0]=std::move(other[0]);
		a[1]=std::move(other[1]);
		return*this;
	}
};

qmatrix qmul(const qmatrix& lhs, const qmatrix& rhs) {
	bint A=lhs[0]*rhs[0];
	return qmatrix{lhs[1] * rhs[1] + A,(lhs[0] + lhs[1]) * (rhs[0] + rhs[1]) -A};
}

bint fib(int n) {
	++n;
	bint ini[2];
	ini[1]=1;
	bint ans[2];
	ans[0]=1;
	bint A,B,C;
	while (n) {
		if (n & 1) {
			A=ini[0]*ans[0],B=ini[1]*ans[1],C=(ini[0]+ini[1])*(ans[0]+ans[1]);
			ans[0]=A+B;
			ans[1]=C-A;
		}
		A=ini[0]*ini[0];
		B=ini[1]*ini[1];
		C=(ini[0]+ini[1])*(ini[0]+ini[1]);
		ini[0]=A+B;
		ini[1]=C-A;
		n>>=1;
	}
	return ans[0];
}

bint slowfib(int n) {
	--n;
	matrix<bint>ini(2,2),ans(2,2);
	ans[0][0]=ans[1][1]=1;
	ini[0][0]=ini[0][1]=ini[1][0]=1;
	while (n) {
		if(n&1)ans=ans*ini;
		ini=ini*ini;
		n>>=1;
	}return ans[0][0];
}

#include <unordered_map>
unordered_map<int,bint>que;
bint f(int n) {
	if(que.count(n))return que[n];
	return que[n]=(n&1?f(n>>1)*f(n>>1)+f((n>>1)+1)*f((n>>1)+1):f(n>>1)*(f((n>>1)+1)+f((n>>1)-1)));
}


std::unordered_map<int, bint2>fibmap;
bint2 quickfib2(int n) {
	if (fibmap.count(n))return fibmap[n];
	return fibmap[n] =
		(n & 1 ? quickfib2(n >> 1) * quickfib2(n >> 1) + quickfib2((n >> 1) + 1) * quickfib2((n >> 1) + 1)
			: quickfib2(n >> 1) * (quickfib2((n >> 1) + 1) + quickfib2((n >> 1) - 1)));
}
bint2 fibonacci2(int n) {
	fibmap[0] = 0;
	fibmap[1] = fibmap[2] = 1;
	fibmap[3] = 2;
	bint2 ans = quickfib2(n);
	fibmap.clear();
	return ans;
}

int main() {

	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	int n;

	while (cin >> n) {
		START
			fibonacci(n);
		END
			cout<<en-sta<<endl;
		START
			fibonacci2(n);
		END
			cout << en - sta << endl;
	}

	bint a, b;
	a = qpow(bint(12345), 5678);
	string ttst = a.tostr();
	double ss = currTime();
	cout << "----------\n";
	for (int i = 0; i < 1000; ++i)
		b = a * a;
	double tt = currTime();
	cout << "----------\n";
	cout << tt - ss << endl;
	cout<<b<<endl;
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