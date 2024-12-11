#pragma once

#include <iostream>
#include <vector>
#include <map>

using namespace std;

/*
An overview of the teleporting ant
An ant wishes to travel between 2 points, each step taking unit t of time
The path allows the ant to travel right
The path has portals between the origin and the goal
When the ant steps on an open portal, the ant will be teleported to the the exit of the portal, which is always on the left of the opening
The portal then closes. When the ant next steps on the closed portal, the portal would be opened. 
Calculate the time taken for the ant to travel from the origin to the goal. 

Explanation, upon drawing the question, the ant will only move right, and the state of the portal is toggled upon touching it. 
This ensures that the portals on the left of the ant, will always be open. 
The time taken would then be equal to the cost of moving through the portal added to the actual distance of the path. 
This now brings forth the question as to how to calculate the cost of the moving through the portals. 
 
https://codeforces.com/problemset/problem/1552/F
https://www.youtube.com/watch?v=_DaTsI42Wvo

*/



class TeleportingAnt {
public:
	TeleportingAnt();
	~TeleportingAnt();

	void setroadlength(int length);

	void addPortal(int entrance, int exit, bool open);

	void calculate();

	void reset();

private:



};

