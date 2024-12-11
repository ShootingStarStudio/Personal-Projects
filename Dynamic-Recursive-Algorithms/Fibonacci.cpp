#include "Fibonacci.h"

Fibonacci::Fibonacci() {
	//null
}
Fibonacci::~Fibonacci() {
	//null
}

int Fibonacci::fib_mem(int n, int tab) {
	if (n < 0) return false;
	else if (n < 2) return n;
	else {
		if (memo.find((n - 1, n - 2)) == memo.end()) {
			//not found
			std::cout << "Summing fib(" << n - 1 << ") and fib(" << n - 2 << ")" << std::endl;
			memo[(n - 1, n - 2)] = fib_mem(n - 1, tab + 1) +
				fib_mem(n - 2, tab + 1);
		}
		return memo[(n - 1, n - 2)];
	}
	return 0;
}

void Fibonacci::printmap(void) {
	for (auto it = memo.begin(); it != memo.end(); ++it) {
		std::cout << "{" << (*it).first << ": " << (*it).second << "}\n";
	}
}