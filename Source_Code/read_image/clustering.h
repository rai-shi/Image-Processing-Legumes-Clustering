#include "image.h"
#include <vector>
#include <iostream>
#include <cmath>

int* Histogram(image im);

float* KMeans_Euclidean(image im, int k);

image KBasedSegmentation(image im, float* kmean, int k);

unsigned char EuclideanDistance(float data, float* kmeans, int k);



//int* Histogram(unsigned char* data);



std::vector<double> KMeans_Euclidean(std::vector<double> data, int k); // returns means 
int EuclideanDistance(double data, std::vector<double> kmeans, int k);
std::vector<std::vector<double>> KBasedSegmentation(std::vector<double> data, std::vector<double> kmean, int k); // returns clustered-labeled data