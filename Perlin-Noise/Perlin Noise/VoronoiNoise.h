#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

class VoronoiNoise {
public:
	VoronoiNoise();
	VoronoiNoise(unsigned int randseed);
	~VoronoiNoise();

	double noise(double x, double y, double z);
private:
	int seed;			//seed
	vector<int> p;		//permutation vector

	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};

/*

creating voronoi noise allows balanced allocation of biomes.
weighting the diagram to create smooth blending locations by creating a fuzzy boundary instead.

realistic fracturing of meshing using a 3d noise map as the planar fracturing

defined as a partition of a plane into regions close to each given set of objects
also known as cellular noise, found in nature as plant cell walls

able to classify the use in machine learning where the differing points seperate groups
and allow the machine to accurately place a new point to find its own region. 

process of generations

may use delaunay triangulation for a given set of discrete points, 
such that no point is inside the circumference of any circle. 
DT maximise the minimum of all angles of the triangles inside the triangulation. 



https://www.baeldung.com/cs/voronoi-diagram#:~:text=This%20algorithm%20works%20by%20iteratively,the%20points%20to%20be%20triangulated.
https://catlikecoding.com/unity/tutorials/pseudorandom-noise/voronoi-noise/



*/