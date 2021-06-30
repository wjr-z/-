#ifndef POLLARDRHO_H
#define POLLARDRHO_H


#include <vector>

namespace Math{

	bool isprime(int);//int判别质数，已验证
	std::vector<int>pollard_rho(int);

	bool isprime(long long);
	std::vector<long long>pollard_rho(long long);

}

#endif POLLARDRHO_H
