#include "Node.h"



Node::Node(int x, int y, int z) {
	//populate the tile choice
	//check for surrounding tiles and collapse tile choice
	//set coord
	c.X = x; c.Y = y;
	tile = -1;
	/*for (int i = 0; i < 10; ++i) {
		tilechoice.push_back(i);
	}*/
}

Node::~Node() {
	//delete everything and return coords and tile

}

void Node::Update()
{
	//get UDLR tiles, if filled, update tile choice
	//tilechoice.erase(std::find(tilechoice.begin(), tilechoice.end(), item to remove));
	
	tile = rand() % 10;

}

void Node::setTile(int type) {
	tile = type;
}

int Node::getTile(void) {
	return tile;
}