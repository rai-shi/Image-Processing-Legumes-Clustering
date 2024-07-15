#include "moment.h"



double find_eta(int i_val, int j_val, int* imgarr, int m00, double m10m00, double m01m00, int width, int height) {
    double lamda = ((i_val + j_val) / 2.0) + 1.0;
    double mu = find_mu(i_val, j_val, imgarr, m10m00, m01m00, width, height);
    return mu / pow(m00, lamda);
}

double find_mu(int i_val, int j_val, int* imgarr, double m10m00, double m01m00, int width, int height) {
    double cnt = 0.0;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            cnt += pow((i + 1) - m10m00, i_val) * pow((j + 1) - m01m00, j_val) * imgarr[i * width + j];
        }
    }
    return cnt;
}


FeatureVector MomentExtractions(image im)
{
    int width = im.w;
    int height = im.h;
    int* data = new int[width * height];

    for (int i = 0; i < width * height; i++)
    {
        data[i] = int(im.data[i]);
    }


    double m00 = 0.0;
    double m10m00 = 0.0;
    double m01m00 = 0.0;

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            m00 += data[i * width + j];
            m10m00 += (j + 1) * data[i * width + j];
            m01m00 += (i + 1) * data[i * width + j];
        }
    }

    m10m00 /= m00;
    m01m00 /= m00;

    double eta0_2 = find_eta(0, 2, data, m00, m10m00, m01m00, width, height);
    double eta0_3 = find_eta(0, 3, data, m00, m10m00, m01m00, width, height);
    double eta1_1 = find_eta(1, 1, data, m00, m10m00, m01m00, width, height);
    double eta1_2 = find_eta(1, 2, data, m00, m10m00, m01m00, width, height);
    double eta2_0 = find_eta(2, 0, data, m00, m10m00, m01m00, width, height);
    double eta2_1 = find_eta(2, 1, data, m00, m10m00, m01m00, width, height);
    double eta3_0 = find_eta(3, 0, data, m00, m10m00, m01m00, width, height);

    double phi1 = eta2_0 + eta0_2;

    double phi2 = std::pow(eta2_0 + eta0_2, 2) + 4 * std::pow(eta1_1, 2);

    double phi3 = std::pow(eta3_0 - (3 * eta1_2), 2) + std::pow((3 * eta2_1) - eta0_3, 2);
   
    double phi4 = std::pow(eta3_0 + eta1_2, 2) + std::pow(eta2_1 + eta0_3, 2);
    
    double phi5 = (3 * eta2_1 - eta0_3) * (eta2_1 + eta0_3) * (std::pow(eta3_0 + eta1_2, 2) - std::pow(eta2_1 + eta0_3, 2))
        + (eta3_0 - (3 * eta1_2)) * (eta3_0 + eta1_2) * (std::pow(eta3_0 + eta1_2, 2) - 3 * std::pow(eta2_1 + eta0_3, 2));
    
    double phi6 = (3 * eta2_1 - eta0_3) * (std::pow(eta3_0 + eta1_2, 2) - std::pow(eta2_1 + eta0_3, 2))
        + 4 * eta1_1 * (eta3_0 + eta1_2) * (eta2_1 + eta0_3);
    
    double phi7 = (3 * eta2_1 - eta0_3) * (eta3_0 + eta1_2) * (std::pow(eta3_0 + eta1_2, 2) - 3 * std::pow(eta2_1 + eta0_3, 2))
        - (eta3_0 - (3 * eta1_2)) * (eta2_1 + eta0_3) * (3 * std::pow(eta3_0 + eta1_2, 2) - std::pow(eta2_1 + eta0_3, 2));

    std::cout << "phi1: " << phi1 << " " << std::endl;
    std::cout << "phi2: " << phi2 << " " << std::endl;
    std::cout << std::endl;
    //std::cout << "phi3: " << phi3 << " ";
    //std::cout << "phi4: " << phi4 << " ";
    //std::cout << "phi5: " << phi5 << " ";
    //std::cout << "phi6: " << phi6 << " ";
    //std::cout << "phi7: " << phi7 << std::endl;

    FeatureVector vector;
    vector.phi1 = phi1;
    vector.phi2 = phi2;
    vector.phi3 = phi3;
    vector.phi4 = phi4;
    vector.phi5 = phi5;
    vector.phi6 = phi6;
    vector.phi7 = phi7;

    return vector;
}



std::vector<image> getObjectImageOneByOne(image im)
{
    image colored_im = labeling(im);
    std::vector<std::vector<int>> bounds = bounding(colored_im);

    std::vector<image> objectImages;


    for (int i = 0; i < bounds.size(); i++)
    {
        int min_x = bounds[i][0];
        int max_x = bounds[i][1];
        int min_y = bounds[i][2];
        int max_y = bounds[i][3];
        int width = bounds[i][4];
        int height = bounds[i][5];
        int big_width = im.w;

        image new_im;
        new_im.h = height;
        new_im.w = width;
        new_im.c = 1;
        new_im.data = new unsigned char[height * width];

        // row = y, column = x
        for (int row = min_y; row <= max_y; row++)
        {
            int new_row = 0;
            for (int col = min_x; col <= max_x; col++)
            {
                int new_col = 0;
                new_im.data[new_row*width+new_col] = im.data[row * big_width + col];
                new_col++;
            }
            new_row++;
        }
        objectImages.push_back(new_im);
        delete[] new_im.data;
    }

    return objectImages;
}



#include <fstream>
#include <string>

// buradan dönen verileri txt'de tutacaðýz.
void getMoments(image im) // std::vector<FeatureVector>
{
    std::vector<image> objectImages = getObjectImageOneByOne(im);

    std::vector<FeatureVector> all_phis;

    for (int image = 0; image < objectImages.size(); image++)
    {
        FeatureVector fvector;
        fvector = MomentExtractions(objectImages[image]);

        all_phis.push_back(fvector);
    }

    //return all_phis;

    std::ofstream write2("phi2.txt", std::ios::app);
    if (write2.is_open())
    {
        for (int vector = 0; vector < all_phis.size(); vector++)
        {
            if ( !std::isnan(all_phis[vector].phi2) )
            {
                write2 << all_phis[vector].phi2 << std::endl;
            }
        }
        write2.close();
        std::cout << std::endl << "phi2 degerleri kayit edildi";
    }

    std::ofstream write1("phi1.txt", std::ios::app);
    if (write1.is_open())
    {
        for (int vector = 0; vector < all_phis.size(); vector++)
        {
            if (!std::isnan(all_phis[vector].phi2))
            {
                write1 << all_phis[vector].phi2 << std::endl;
            }
        }
        write1.close();
        std::cout << std::endl << "phi2 degerleri kayit edildi";
    }

}











double calculateMean(const std::vector<double> values) {
    double sum = 0.0;
    for (const auto& value : values) {
        sum += value;
    }
    return sum / values.size();
}

double calculateVariance(const std::vector<double> values, double mean) {
    double variance = 0.0;
    for (const auto& value : values) {
        variance += std::pow(value - mean, 2);
    }
    return variance / values.size();
}

double calculateStandardDeviation(double variance) {
    return std::sqrt(variance);
}






// txt ye den okunup classification yapýlýyor
std::vector<std::vector<double>> phi2clustering()
{
    std::ifstream read("phi2.txt");
    std::vector<double> phi2;

    std::vector<std::vector<double>> phi2_feature_vector; // return vector

    if (read.is_open())
    {
        std::string line;

        // Dosyadan verileri satýr satýr oku ve ekrana yazdýr
        while (std::getline(read, line))
        {
            double value = std::stod(line);
            phi2.push_back(value);
            std::cout << value << std::endl;
        }

    }
    read.close();

    if (!phi2.empty())
    {
        std::vector<double> means = KMeans_Euclidean(phi2, 2);

            std::vector<std::vector<double>> clustered_data = KBasedSegmentation(phi2, means, 2); // 2 küme ve ph deðerleri bunu 

            double phi2_mean;
            double phi2_variance;
            double phi2_standartdev;


            for (int i = 0; i < clustered_data.size(); i++)
            {
                phi2_mean = calculateMean(clustered_data[i]);
                phi2_variance = calculateVariance(clustered_data[i], phi2_mean);
                phi2_standartdev = calculateStandardDeviation(phi2_variance);
                std::vector<double> phi2_feature;
                phi2_feature.push_back(phi2_mean);
                phi2_feature.push_back(phi2_variance);
                phi2_feature.push_back(phi2_standartdev);

                phi2_feature_vector.push_back(phi2_feature);
            }

        return phi2_feature_vector;
    } 
    else
    {
        return phi2_feature_vector;
    }
}