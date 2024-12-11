#pragma once

#include <iostream>
#include <vector>
#include <map>

using namespace std;

/*
An overview of the knapsack problem
The 2 most common types of knapsack problems are 01 and fractional
01 can be expanded to bounded, where there are copies of the same item, but with the exact same weight and value
Factional is the use of measurements and quantities when presenting the item, as such the item can be split into multiple portions, or fractions of itself
The value of the split item has the same value to weight ratio as the original item

*/

/*
TO-DO list
Knapsackrecursive should be cleaned and made effecient
Create the algorithm for fractional knapsack problem


*/

/*
Sources:
tutorialspoint.com/0-1-knapsack-problem-in-c#
tutorialspoint.com/cplusplus-program-to-solve-knapsack-problem-using-dynamic-programming
tutorialspoint.com/printing-items-in-0-1-knapsack-in-cplusplus
tutorialspoint.com/Fractional-Knapsack-Problem
tutorialspoint.com/cplusplus-program-to-solve-knapsack-problem-using-dynamic-programming

*/


class Knapsack {
public:
	Knapsack();
	~Knapsack();

	void setcapacity(int newcapacity);

	void additems(int weight, int value);

	int knapsackRecursive(vector<vector<int>> lookupTable, int profits[], int profitsLength, int weights[], int capacity, int currentIndex);
	int knapSack(int profits[], int profitsLength, int weights[]);

	void calculate01(void);
	void calculateFractional(void);

	void printitems();
	void clearitems();

private:
	int capacity = 0;

	std::vector<std::pair<int, int>> items;		//0:weight, 1:value

};

