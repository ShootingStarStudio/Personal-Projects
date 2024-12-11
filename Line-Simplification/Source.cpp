#include <iostream>




using namespace std;

int main() {


	/*
	https://en.wikipedia.org/wiki/Ramer–Douglas–Peucker_algorithm
	https://medium.com/mlearning-ai/applications-of-ramer-douglas-peucker-algorithm-in-machine-learning-that-you-might-not-have-heard-63b0c4f15a43#:~:text=Ramer-Douglas-Peucker%20Algorithm%2C,%27epsilon%27%20(ε).
	https://gist.github.com/yageek/287843360aeaecdda14cb12f9fbb60dc


	2 step process
	1. run a direction comparison with angle based comparison, threshold appropriate for the model
		the points with strong angular changes will be marked as points of interest
		hold the current and previous points to form the line segments, where the resultant values will create line segments
		check the distance of the path against constant value to ensure that the player is not just standing still or just idling around a area
	2. run RDP or VW algorithm to simplify the points into reduced lines via distance comparison
	3. detect the number of vertices after and compare reduction value.  


	expected time complexity = (0) = n*log(n)


	suggested use cases
	1. vr magic game where the player draws the runes and chains them into "sentences" to cast spells, ie noita
	2. machine learning where the data sets are reduced in complexity to reduce computation costs
	3. robotics where the radial approximation of the world can be reduced in quality as a fuzzy object/distance detection graph

	*/



	return 0;
}