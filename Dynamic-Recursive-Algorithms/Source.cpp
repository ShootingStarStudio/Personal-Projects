#include <iostream>

#include "Fibonacci.h"
#include "Knapsack.h"

using namespace std;

//dynamic recursion with memonization

int main(void) {
	/*Fibonacci fib;
	int n = 0;
	do {
		std::cout << fib.fib_mem(n) << std::endl;
		std::cout << "Enter your number:";
		std::cin >> n;
	} while (n > -1);

	fib.printmap();*/

	Knapsack sack;
	sack.setcapacity(7);
	/*sack.additems(2, 3);
	sack.additems(3, 4);
	sack.additems(4, 3);
	sack.additems(4, 5);
	sack.additems(5, 8);
	sack.additems(9, 10);*/
	sack.calculate01();
	//sack.calculatebounded();
	//sack.printitems();
	//sack.printmemo();
	//sack.clearitems();
	//sack.clearmemo();


	return 0;
}