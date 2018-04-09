#include <iostream>

int main()
{
	float S;
	float Fp;

	std::cout << "Enter your 4-thread-to-one-thread speedup S: ";
	std::cin >> S;
	std::cout << "You entered: " << S << "\n";

	/* Calculate parallel fraction with user input */
	Fp = (4./3.)*(1. - (1./S) );
	std::cout << "Parallel fraction, Fp = " << Fp << "\n";

	return 0;

}
