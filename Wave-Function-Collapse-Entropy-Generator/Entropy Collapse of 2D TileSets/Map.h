#pragma once
#include <iostream>
#include <fstream>
#include "frame.h"
#include "Node.h"

using namespace std;

class Map
{
public:
	Map(int x, int y);
	~Map();

	void printmap(void);
	void exportmap(void);

	int getwidth(void);
	int getheight(void);

	void Update();

private:
	int width, height;
};

