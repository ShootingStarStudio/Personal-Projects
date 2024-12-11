#pragma once

#include <iostream>
#include <map>

using namespace std;

//largest number is 46
//f(47) breaks the int limit

class Fibonacci {
public:
	Fibonacci();
	~Fibonacci();

	int fib_mem(int n, int tab = 0);

	void printmap(void);
private:
	std::map<int, int> memo;

};
