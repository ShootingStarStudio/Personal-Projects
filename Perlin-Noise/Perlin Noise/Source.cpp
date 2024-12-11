#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <fstream>
#include <sstream>

#include "ppm.h"

#include "PerlinNoise.h"
#include "VoronoiNoise.h"

using namespace std;

int main(void) {
	unsigned int width = 500, height = 500, depth = 500;
	unsigned int octave = 4;						//the number of iterations to get the fractal noise, high values are a waste of computation time since the images will be nearly black afterwards
	double lacurinity = 2.0;						//frequncy, the magnification of the number of grids when octaves increment, thus having more detail than the previous noise
	double persistanace = 0.5;						//amplitude, how much the octave influences the noise map, higher persistance will create regular noise
	unsigned int layers = 10;						//seperation of noise into distinct layers, meant for wood grain like noises
	unsigned int seed = 237;						//constant seed for testing and debugging of artefacts
	PerlinNoise pn(seed);

	std::vector<ppm> noiselayers;

	//scale
	double GridX = 10;		//number of cubes on the Xaxis
	double GridY = 10;		//number of cubes on the Yaxis
	double GridZ = 10;		//number of cubes on the Zaxis
	double strength = 1.0;
	//each octave should have 1 image
	for (unsigned int oct = 0; oct < octave; oct++) {
		ppm image(width, height);
		unsigned int kk = 0;
		// Visit every pixel of the image and assign a color generated with Perlin noise
		for (unsigned int i = 0; i < width; ++i) {
			for (unsigned int j = 0; j < height; ++j) {
				double x = (double)i / (double)width;
				double y = (double)j / (double)height;
				double z = 0;								//height of the 2d image
				double n;									// n is a percentage of 0 to 1, 0 being black and 1 being white

				// Typical Perlin noise
				n = pn.noise(GridX * x, GridY * y, GridZ * z) * strength;

				// Wood like structure
				//n = layers * pn.noise(GridX * x, GridY * y, GridZ * z);		//seperates into different layers, similar to topography lines
				//n = n - floor(n);												//gets rid of the int, making it from 0-1 again

				// Map the values to the [0, 255] interval, use of gray scale for topography
				image.r[kk] = image.g[kk] = image.b[kk] = floor(255 * n);

				kk++;		//increment in pixel
			}
		}

		// Save the image in a binary PPM file
		std::string filename = "Noise" + to_string(oct) + ".ppm";
		image.write(filename);

		//push into image vector
		noiselayers.push_back(image);

		//update noise info
		GridX *= lacurinity;
		GridY *= lacurinity;
		GridZ *= lacurinity;
		strength *= persistanace;
	}

	//blending all maps
	ppm fractalimage(width, height);
	std::vector<double> noiseValues(width*height);
	double highest = 0.0, lowest = 255.0;
	for (unsigned int nlayers = 0; nlayers < noiselayers.size(); nlayers++) {
		//opens file from before
		ppm temp_image = noiselayers.at(nlayers);
		unsigned int kk = 0;
		for (unsigned int i = 0; i < width; ++i) {
			for (unsigned int j = 0; j < height; ++j) {
				noiseValues[kk] += temp_image.r[kk];
				highest = max(highest, noiseValues[kk]);
				lowest = min(lowest, noiseValues[kk]);
				kk++;		//increment in pixel
			}
		}
	}
	//casting values
	//find the highest and lowest values
	//scale the values from highest to lowest
	//stretch the values to have an even spread of 0-1
	double highestNormalized = 0.0;
	for (int i = 0; i < noiseValues.size(); ++i) {
		noiseValues[i] -= lowest;									//ensure that the smallest value is negated
		noiseValues[i] = pow(noiseValues[i], 2);					//increase height difference
		highestNormalized = max(highestNormalized, noiseValues[i]); //compares to find highest value
	}

	for (int i = 0; i < noiseValues.size(); ++i) {
		noiseValues[i] /= highestNormalized;						//forces values to be between 0 and 1
		fractalimage.r[i] = fractalimage.g[i] = 
			fractalimage.b[i] = floor(noiseValues[i] * 255);		//assigns values to the pixels
	}

	//exported the image
	fractalimage.write("FractalTerrain.ppm");

	//generate path from fractal noise
	ppm pathimage(width, height);
	//new image with the same resolution

	return 0;
}


/*
Bug list:
Uneven chunking along grid lines causes sharp lines
*/