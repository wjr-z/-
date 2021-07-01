#include <Windows.h>
#include "src/BigInteger/bint.h"
using namespace Math;
using namespace std;
#include <random>
std::mt19937 mt_rand(time(NULL));
void randdata(bint& x,int n) {
	x=0;
	int Size = n - 1 >> 3;
	for (int i = 0; i < Size; ++i)
		x[i] = randint(0, bintjw - 1);
	for (int i = Size << 3; i < n - 1; ++i)
		x.set(i, randint(0, 9));
	x.set(n - 1, randint(1, 9));
}

const int N = 1e8 + 10;
int prime[N];
bool f[N];


inline int dd(int x,int v) {
	return x/v;
}

int main() {

	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	/*int cnt=0;
	double s1=currTime();
	int maxn = 1e7;
	for (int i = 2; i <= maxn; ++i) {
		if (!f[i])prime[++prime[0]] = i;
		for (int j = 1; j <= prime[0] && i * prime[j] <= maxn; ++j) {
			f[i * prime[j]] = true;
			if (i % prime[j] == 0)break;
		}
		if (f[i] == isprime(i)) {
			++cnt;
			cout<<"WA\n";
			cout<<i<<endl;
		}
	}
	double t1=currTime();
	cout<<t1-s1<<endl;*/
	//cout<<tot<<endl;
	//if(!cnt)
	//cout<<"AC\n";
	//else
	//cout<<"WA\n"<<cnt<<endl;
	//return 0;
	
	//while (true) {
	//	bint a,b;
	//	randdata(a,10000);
	//	randdata(b,1000);
	//	bint2 c,d;
	//	c=a;
	//	d=b;
	//	int k=randdata(0,bintjw);
	//	if(a/b==c/d)
	//		cout<<"AC\n";
	//	else {
	//		cout<<a+b<<endl;
	//		cout<<c+d<<endl;
	//		cout << "WA\n";
	//		return 0;
	//	}
	//}

	//bint test,qwq;

	//cout<<"----\n";

	//qwq=(test*test)*test;


	//cout<<"----\n";

	//return 0;


	bint a,b;
	a=qpow(bint(1234),56);
	double ss=currTime();
	cout << "----------\n";
	for(int i=0;i<1000000;++i)
		b=a*a;
	double tt=currTime();
	cout << "----------\n";
	cout<<tt-ss<<endl;
	cout<<tot<<endl;
	//cout<<b<<endl;
	return 0;



	freopen("test.out", "w", stdout); 
	cout.setf(ios::left); //设置对齐方式为left

	int HEAD=10,TAIL=1000,PY=10;

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
			bint ans1,ans2,ans3;
			ans1=test1(x,y);
			ans2=test2(x,y);
			ans3=test3(x,y);
			START
				for (int k = 0; k < K; ++k)
					test1(x, y);
			END

				cout << setw(6) << "暴力：" << setw(12) << (stime = en - sta);
			double a1 = stime * 1000000 / (len1 * len2);
			cout << " ";

			START
				for (int k = 0; k < K; ++k)
					test2(x, y);
			END

				cout << setw(6) << "Kar：" << setw(12) << (ktime = en - sta);
			cout << " ";
			START
				for (int k = 0; k < K; ++k)
					test3(x, y);
			END
				cout << setw(6) << "FFT：" << setw(12) << (ftime = en - sta);
			double a2 = ftime * 1000000 / ((len1 + len2) * quicklog2(len1 + len2));
			cout << " ";
			START
				for (int k = 0; k < K; ++k)
					test4(x, y);
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
				cout<<"正确\n";
			}
			else {
				if (ans1 != ans3) {
					cout<<"暴力错误\n";
				}
				if (ans1 == ans3 && ans1 != ans2) {
					cout<<"Kar错误\n";
				}
				else if (ans1 == ans2 && ans1 != ans3) {
					cout<<"FFT错误\n";
					test3(x,y);
				}
			
			}
		}
	}
	return 0;

	 
	return 0;
}