#include "Map.h"


Map::Map(int x, int y) {
	width = x; height = y;
	Node head(rand() % width + 1, rand() % height + 1);
}
Map::~Map() {

}

void Map::printmap(void) {
	for (int i = 0; i <= height; ++i) {
		for (int j = 0; j <= width; ++j) {
			//cout<<printnode;
			
		}
		cout << "" << endl;
	}
}
void Map::exportmap(void) {
	ofstream Tilesheet("Mapped.csv");
	for (int i = 0; i <= height; ++i) {
		for (int j = 0; j <= width; ++j) {
			//Tilesheet << i + j << ",";
		}
		Tilesheet << "" << endl;
	}
}

int Map::getwidth(void) {
	return width;
}
int Map::getheight(void) {
	return height;
}

void Map::Update() {

}