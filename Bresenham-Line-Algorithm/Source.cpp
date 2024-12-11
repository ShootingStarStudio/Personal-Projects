#include <iostream>
#include <vector>

using namespace std;

/*
This is a line drawing algorithm that determines the points of an n-dimensional raster 
that should be selected in order to form a close approximation to a straight line between 2 points.

Approach:
	The algorithm is provided 2 points on a line, this is the start and end points of the line.
	The algorithm calculates the gradient and increments in the direction of the line.
	This should return an approximate line that matches a grid. 

Source:
https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

Uses:
	Line of sight on a 2d top down grid based game
	Straight line on a diagonal

NOTE:
The graph has a origin of 0,0, at the bottom left
Most computers have a origin of 0,0 at the top left the the screen
*/

/*
	slope intercept form: y = mx + c
	function is then written as Ax + By + C
	where,
	A = y1-y0
	B = -(x1-x0)
	C = x1y0-x0y1

	int A = y1 - y0, B = -(x1 - x0), C = (x1 * y0) - (x0 * y1);

	the constraints return a value
	if the value is on the line, it returns 0
	else itll not be 0;
	negatives will be in the positive half-plane
	positives will be in the negative half-plane
*/

class BLine {
private:
	std::vector<std::pair<int, int>> POL;			//points on a line

public:
	BLine() {
		//NULL
	}
	~BLine() {
		//NULL
	}

	void generalLine(int x0, int y0, int x1, int y1) {
		int dx = x1 - x0;
		int dy = y1 - y0;
		int D = 2 * dy - dx;
		/*
		algorithm for integer arithmetic
		this method is faster than floating-point arithmetic
		the difference value D = f(x0 + 1, y0 + 0.5) - f(x0, y0)
		the simplification of D = A + 0.5B
		where the change in y - half of the change in x
		*/
		int tempy = y0;

		for (int tempx = x0; tempx <= x1; tempx++) {
			POL.push_back({ tempx,tempy });
			if (D > 0) {
				tempy += 1;
				D -= 2 * dx;
			}
			D += 2 * dy;
		}
	}

	void lowLine(int x0, int y0, int x1, int y1) {
		clearVector();
		int dx = x1 - x0;
		int dy = y1 - y0;
		int yi = 1;
		if (dy < 0) {
			yi = -1;
			dy *= -1;
		}
		int D = 2 * dy - dx;
		int tempy = y0;

		for (int tempx = x0; tempx <= x1; tempx++) {
			POL.push_back({ tempx,tempy });
			if (D > 0) {
				tempy += yi;
				D += 2 * (dy - dx);
			}
			else D += 2 * dy;
		}
	}

	void highLine(int x0, int y0, int x1, int y1) {
		clearVector();
		int dx = x1 - x0;
		int dy = y1 - y0;
		int xi = 1;
		if (dy < 0) {
			xi = -1;
			dx *= -1;
		}
		int D = 2 * dx - dy;
		int tempx = y0;

		for (int tempy = x0; tempy <= x1; tempy++) {
			POL.push_back({ tempx,tempy });
			if (D > 0) {
				tempx += xi;
				D += 2 * (dx - dy);
			}
			else D += 2 * dx;
		}
	}

	void completeLine(int x0, int y0, int x1, int y1) {
		if (abs(y1 - y0) < abs(x1 - x0)){
			if (x0 > x1)
				lowLine(x1, y1, x0, y0);
			else
				lowLine(x0, y0, x1, y1);
		}
		else {
			if (y0 > y1)
				highLine(x1, y1, x0, y0);
			else
				highLine(x0, y0, x1, y1);
		}
	}

	//this algorithm does not garuntee order between points, line may be drawn from 0 to 1, or 1 to 0
	void improvedLine(int x0, int y0, int x1, int y1) {
		clearVector();
		int dx = abs(x1 - x0);
		int dy = -abs(y1 - y0);
		int sx = x0 < x1 ? 1 : -1;
		int sy = y0 < y1 ? 1 : -1;
		int error = dx + dy;
		int tempx = x0, tempy = y0;

		while (true) {
			POL.push_back({ tempx,tempy });
			if (tempx == x1 && tempy == y1) {		//reached the end of the line
				break;
			}
			int e2 = 2 * error;
			if (e2 >= dy) {
				error += dy;
				tempx += sx;
			}
			if (e2 <= dx) {
				error += dx;
				tempy += sy;
			}
		}
	}

	void clearVector() {
		POL.clear();
	}

	void printline() {
		for (auto i : POL) {
			std::cout << i.first << ", " << i.second << std::endl;
		}
	}
};

int main(void) {
	BLine bLine;

	int x0 = 0, y0 = 1, x1 = 6, y1 = 4;
	bLine.generalLine(x0, y0, x1, y1);
	std::cout << "General Line: " << std::endl;
	bLine.printline();

	bLine.completeLine(x0, y0, x1, y1);
	std::cout << "Complete Line: " << std::endl;
	bLine.printline();

	bLine.improvedLine(x0, y0, x1, y1);
	std::cout << "Improved Line: " << std::endl;
	bLine.printline();

	return 0;
}

