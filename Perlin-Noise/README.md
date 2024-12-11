# Noise-Maps
 Experimentation with Ken Perlin's implementation of Perlin Noise
 Current version is using the improved algorithm to reduce artefacts
 Features include 3d noise generation, topography lines and fractal noise generation

 Current objective: apply fractal noise generation to current program=
 Explore method of creating a path system from existing noise, thus allowing the player to walk through coherent paths

 Future objectives: 
 implement simplex noise to improve runtime
 link program to map generator to create maps
 implement chunking to save and load portions of the map, reducing render complexity when leaving range
 Moon type map, where the map has craters
 Island type map, where islands are created with a circular gradient filter. 
 implement coord based generation to allow for runtime generation.
 implement chunking to allow for hashing

 apply voronoi noise with the same parameters

stretch image to match high peaks and low valleys