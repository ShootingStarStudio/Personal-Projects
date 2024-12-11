#pragma once
#include <vector>
#include <random>
#include "frame.cpp"

using namespace std;

class Node
{
public:
	Node(int x, int y, int z);
	~Node();
	void Update();
	void setTile(int type);
	int getTile(void);

private:
	int tile;
	vector<int> tilechoice;
	Coord c;
};

