#pragma once

#include "image.h"
#include "morphology.h"
#include "clustering.h"


//#include <iostream>
//#include <vector>



struct FeatureVector
{
    double phi1; // kütle merkezi ve daðýlýmý, alan, büyüklük gibi özellikleri belirtir.
    double phi2; // yuvarlak olma derecesini belirtir.
    double phi3;
    double phi4;
    double phi5;
    double phi6;
    double phi7;
};



FeatureVector MomentExtractions(image im);
double find_mu(int i_val, int j_val, int* imgarr, double m10m00, double m01m00, int width, int height);
double find_eta(int i_val, int j_val, int* imgarr, int m00, double m10m00, double m01m00, int width, int height);
std::vector<image> getObjectImageOneByOne(image im);
void getMoments(image im);







std::vector<std::vector<double>> phi2clustering();

double calculateMean(const std::vector<double> values);

double calculateVariance(const std::vector<double> values, double mean);

double calculateStandardDeviation(double variance);