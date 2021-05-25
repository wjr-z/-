#include <Windows.h>
#include "Math.h"
using namespace Math;
using namespace std;
#include <random>
#include <bitset>
std::mt19937 mt_rand(time(NULL));
void randdata(bint& x,int n) {
	x=0;
	for(int i=0;i<n;++i)
		x.set(i,mt_rand()%8+1);
}

int main() {
	bint x,y;
	x=qpow(bint(1234),5678);
	y=qpow(bint(1237),5678);
	START
		bint z;
		for(int i=0;i<1000;++i)
			z=x*y;
	
	END
	cout<<en-sta;
	
	return 0;
	

	



	
	freopen("test.out","w",stdout);
	cout.setf(ios::left); //设置对齐方式为left 
	for (int i = 8000; i <= 8000; i += 50) {
		for (int j = 20; j <= i; j += 20) {
			int len1,len2;
			len1=((i-1)>>3)+1;
			len2=((j-1)>>3)+1;
			bint x,y;
			randdata(x,i);
			randdata(y,j);
			double stime,ftime,ktime;
			cout<<setw(4)<<"i="<<setw(4)<<i<<setw(6)<<"  j="<<setw(4)<<j<<' ';
			int K=10;
			START
			for(int k=0;k<K;++k)
				test1(x,y);
			END
			cout<<setw(6)<<"暴力："<<setw(12)<<(stime=en-sta);
			double a1= stime * 1000000 / (len1*len2);
			cout<<" ";
			START
				for (int k = 0; k < K; ++k)
					test2(x, y);
			END
				cout << setw(6) << "Kar：" << setw(12) << (ktime = en - sta);
			cout<<" ";
			START
				for (int k = 0; k < K; ++k)
					test3(x, y);
			END
				cout << setw(6) << "FFT：" << setw(12) << (ftime=en - sta);
			double a2= ftime * 1000000 / ((len1+len2)*quicklog2(len1+len2));
			cout<<" ";
			START
				for (int k = 0; k < K; ++k)
					test4(x, y);
			END
				cout << setw(6) << "智能模式：";
			if(mode==1)cout<< setw(6) << "暴力 ";
			else if(mode==2)cout<<setw(6) << "Kar ";
			else cout<< setw(6) << "FFT ";
			cout<<"  "<<setw(14)<<en-sta<<' ';
			cout<<setw(6)<<"最优选择：";
			if (stime < ftime&&stime<ktime) {
				cout<<"暴力"<<' ';
			}
			if (ktime < stime && ktime < ftime) {
				cout<<"Kar"<<' ';
			}
			if (ftime < stime&&ftime<ktime) {
				cout<<"FFT"<<' ';
			}
			cout<<"比值:"<<a2/a1<<endl;
		}
	}
	return 0;


	//mt19937 mt_rand(time(NULL));
	////system("fc wjr.out wjr2.out");
	//int N=1e6;
	//bint a,b;
	//for(int i=0;i<N;++i)
	//	a[i]=(mt_rand()%(100000000-1)+1);
	//for(int i=0;i<N;++i)
	//	b[i]= (mt_rand() % (100000000 - 1) + 1);
	//cout<<"rand done\n";
	//START
	//bint2 c=a.to2bit();
	//END
	//cout<<"done in "<<en-sta<<"seconds"<<endl;
	//START
	//return 0;
	/*a*=b;
	END
	cout<<en-sta<<endl;
	START
	c*=d;
	END
	cout<<en-sta<<endl;
	freopen("wjr.out","w",stdout);
	cout<<a<<endl;
	freopen("wjr2.out","w",stdout);
	cout<<c.to10bit()<<endl;
	system("fc wjr.out wjr2.out");*/

	return 0;
}