#ifndef POLLARDRHO_H
#define POLLARDRHO_H

#include <vector>

namespace Math {

	bool is_prime(int);//intÅÐ±ðÖÊÊý
	std::vector<int>pollard_rho(int);
	int max_prime(int);

	bool is_prime(long long);
	std::vector<long long>pollard_rho(long long);
	long long max_prime(long long);

}

#endif
