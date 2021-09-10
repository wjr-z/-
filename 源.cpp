#include <bits/stdc++.h>

#include "src/bfloat.h"
#include "src/bint.h"
#include "src/mtool.h"

using namespace Math;
using namespace std;

int main() {
	
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);


	int n=1e5;
	bfloat x(1);
	cout<<x<<endl;
	double y=1;

	auto s=GetTime();
	for(int i=1;i<n;++i)
		x+=bfloat(1.0/i);
	auto t=GetTime();
	cout<<t-s<<'\n';

	s=GetTime();
	for(int i=1;i<n;++i)
		y+=1.0/i;
	t=GetTime();
	cout<<t-s<<'\n';

	cout<<x<<' '<<y<<'\n';

	return 0;
}
