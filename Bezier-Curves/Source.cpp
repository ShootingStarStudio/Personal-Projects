#include <iostream>
#include <vector>


using namespace std;

/*
This algorithm creates smooth curves between control points based on linear interpolation. 

Sources:
https://en.wikipedia.org/wiki/B%C3%A9zier_curve
https://medium.com/geekculture/2d-and-3d-b%C3%A9zier-curves-in-c-499093ef45a9
http://blog.sklambert.com/finding-the-control-points-of-a-bezier-curve/
http://scaledinnovation.com/analytics/splines/splines.html
https://www.geeksforgeeks.org/cubic-spline-interpolation/
https://en.wikipedia.org/wiki/Smoothness

Approach:
	De Casteljau's Algorithm-recursive method
	The algorithm gathers all the control points and "draws" an imaginary line between every 2 neighbouring point
	This approach uses time as the iterator, with a precision of 0.01t
	Then it uses linear interpolation to figure out the coordinate of the new point.
	This repeats until there is only 2 points left, constructing the last 

	Bezier's formula- blending and derivatives
	Using an equation to blend together control points
	cubic curve
	p = (1-t)^3*p0 + 3*t*(1-t)^2*p1 + 3*t^2*(1-t)*p2 + t^3*p3
	quadratic curve
	p = (t-t)^2*p0 + 2*(1-t)*t*p1 + t^2*p2

	other formulas can be derived from the formula

Uses:
 smooth curves with non best fit properties
 used to draw complete images using purely control points
 looks nice
 recursive

*/

class DeCasteljau {
private:
	//stores the original control points
	std::vector<std::pair<int, int>> ControlPoints;
	//uses vector of vectors since each layer has less points than the previous layer
	std::vector<std::vector<std::pair<int, int>>> LerpPoints;
	//stores the actual position of the points
	std::vector<std::pair<int, int>> BezierPoints;		//should have a size of 100

public:
	DeCasteljau() {
		//NULL
	}
	~DeCasteljau() {
		//NULL
	}

	void generateSpline(std::vector<std::pair<int, int>> vecpoints) {
		//derivative approach
		ControlPoints = vecpoints;



		//clear LerpPoints at the end
		LerpPoints.clear();

	}

	void clearmemory() {
		ControlPoints.clear();
		BezierPoints.clear();
	}

	void printpoints() {
		for (auto i : BezierPoints) {
			std::cout << i.first << ", " << i.second << std::endl;
		}
	}

};

class Bezier {
private:
	//stores the original control points
	std::vector<std::pair<int, int>> ControlPoints;
	int size;											//number of points you want to calculate
	//stores the actual position of the points
	std::vector<std::pair<int, int>> BezierPoints;		//^^

public:
	Bezier() {
		//NULL
	}
	~Bezier() {
		//NULL
	}

	void generateSpline(std::vector<std::pair<int, int>> vecpoints) {
		//derivative approach
		ControlPoints = vecpoints;


	}

	void clearmemory() {
		ControlPoints.clear();
		BezierPoints.clear();
	}

	void printpoints() {
		for (auto i : BezierPoints) {
			std::cout << i.first << ", " << i.second << std::endl;
		}
	}

};

int main(void) {
	std::vector<std::pair<int, int>> ControlPoints;
	DeCasteljau dCurve;
	dCurve.generateSpline(ControlPoints);
	dCurve.printpoints();
	//draw here

	//export here
	dCurve.clearmemory();

	Bezier bCurve;
	bCurve.generateSpline(ControlPoints);
	bCurve.printpoints();
	//draw here
	
	//export here
	bCurve.clearmemory();

	return 0;
}

