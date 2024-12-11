#include <iostream>
#include <random>
#include "frame.h"
#include "Map.h"
#include "Node.h"

using namespace std;

/*
An overview of 2D tile sets
Creating a map can be made using wang tiles, where tiles have specific edges to match other tiles
This makes the map be able to have smooth edges while still being rather random
This can also be expanded to have 3D applications, such as a castle or a house with modular pieces and thus easily creating assets for a village
Similarily, the algorithm can be used during runtime to ensure that new nodes added fufill requirements or to merge the edges smoothly. 

*/

int update() {
	return 0;
}

int main(void) {
	srand(NULL);
	//create link list and subsequent attachments for 4 sides(UDLR)
	// CNode requires 4 links, data for tile and update for allowed links, and coords(if needed)
	// links require update function to remove certain entropies
	// when a tile is generated, all surrounding tiles should be updated
	// when no other updates can be made, find link with the smallest number of entropies
	// and choose random option to collapse. 
	// overall update function should find "updateable" tiles to save on computational power
	// ie only calculate tiles with a filled linked tile
	
	//create tiles allowed,
	// land, 
	// water, coastU, coastD, coastL, coastR
	// mountain, cliffU, cliffD, cliffL, cliffD, 
	// enviroments: rock, tree, bush
	// rocks can spawn on land and mountains
	// tree and bush can spawn on land
	// optional: a tree tile has a higher chance to spawn in a group, using weighted randomness
	
	//default generation is to choose random point on map and randomly choose their tile, then start the generation
	//create constraints for generation
	//export generation to excel sheet as CSV
	//display generation on screen

	/*while (true) {
		update();
	}*/

	//set map boundaries
	Map TileMap1(50, 50);

	return 0;
}