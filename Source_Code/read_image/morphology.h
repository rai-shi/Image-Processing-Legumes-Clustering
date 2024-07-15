#pragma once

#include "image.h"
#include "iostream"

// opening;

image& regionFilling(image& im);



image erosion(image im, int struct_matris_dim);

image dilation(image im, int struct_matris_dim); 



image opening(image im, int struct_matris_dim);

image closing(image im, int struct_matris_dim);



image edge_detection(image im, int struct_matris_dim);



#include <vector> 

image& labeling(image& im);




struct Point {
	int row; // |
	int column; // --
};



std::vector<unsigned char> getLabels(image im);
std::vector<std::vector<int>> bounding(image im);
void drawBoundingBox(image& im, int max_x, int min_x, int max_y, int min_y, int width, int height);
image  drawBoundary(image im);




