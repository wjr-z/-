#ifndef POLLARDRHO_H
/**
 * 对int/long long 实现快速质数判断或者质因数分解
 */
#define POLLARDRHO_H

#include <vector>

namespace Math {

	bool is_prime(int);//int判别质数
	std::vector<int>pollard_rho(int);
	int max_prime(int);

	bool is_prime(long long);
	std::vector<long long>pollard_rho(long long);
	long long max_prime(long long);

}

#endif
