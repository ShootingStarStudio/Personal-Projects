#include "Knapsack.h"

Knapsack::Knapsack() {
	//null
}
Knapsack::~Knapsack() {
	//null
}

void Knapsack::setcapacity(int newcapacity) {
	capacity = newcapacity;
}

void Knapsack::additems(int weight, int value) {
	std::pair<int, int> temp = std::pair<int, int>(weight, value);
	items.push_back(temp);
}

int Knapsack::knapsackRecursive(vector<vector<int>> lookupTable, int profits[], int profitsLength, int weights[], int capacity,  int currentIndex) {
	// base checks  
	if (capacity <= 0 || currentIndex >= profitsLength || currentIndex < 0) return 0;

	// if we have already solved the problem, return the result from the table  
	if (lookupTable[currentIndex][capacity] != 0) return lookupTable[currentIndex][capacity];

	// recursive call after choosing the element at the currentIndex
	// if the weight of the element at currentIndex exceeds the capacity, we shouldn't process this
	int profit1 = 0;
	if (weights[currentIndex] <= capacity)
		profit1 = profits[currentIndex] + knapsackRecursive(lookupTable, profits, profitsLength, weights, capacity - weights[currentIndex], currentIndex + 1);

	// recursive call after excluding the element at the currentIndex
	int profit2 = knapsackRecursive(lookupTable, profits, profitsLength, weights, capacity, currentIndex + 1);

	lookupTable[currentIndex][capacity] = max(profit1, profit2);
	return lookupTable[currentIndex][capacity];
}

int Knapsack::knapSack(int profits[], int profitsLength, int weights[]) {
	vector< vector<int> > lookupTable( profitsLength, std::vector<int>(capacity + 1, 0));
	return knapsackRecursive(lookupTable, profits, profitsLength, weights, capacity, 0);
}
void Knapsack::calculate01(void) {
	int profits[] = { 1, 6, 10, 16 };
	int weights[] = { 1, 2, 3, 5 };
	cout << "Total knapsack profit = " << knapSack(profits, 4, weights) << endl;
	cout << "Knapsack contains = " << endl;		//add item list

}

void Knapsack::calculateFractional(void) {

}

void Knapsack::printitems() {
	for (auto it = items.begin(); it != items.end(); ++it) {
		std::cout << "{" << (*it).first << ": " << (*it).second << "}\n";
	}
}

void Knapsack::clearitems() {
	items.clear();
}