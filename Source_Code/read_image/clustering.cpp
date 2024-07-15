#include "clustering.h"


int* Histogram(image im)
{
	int* hist;
	if (im.c == 1)
	{
		hist = new int[256];

		// içeriði temizle
		for (int i = 0; i < 256; i++)
			hist[i] = 0;

		for (int i = 0; i < im.h * im.w; i++)
		{
		hist[im.data[i]]++;
		}

		return hist;
	}
	else hist = NULL;
}


unsigned char EuclideanDistance(float data, float* kmeans, int k)
{
    if (k != 2)
    {
        return 0; 
    }

    float distance0 = std::abs(data - kmeans[0]); // Ýlk küme merkezine olan uzaklýk
    float distance1 = std::abs(data - kmeans[1]); // Ýkinci küme merkezine olan uzaklýk

    if (distance0 <= distance1)
    {
        return 1; // Ýlk küme merkezine daha yakýnsa 0 döndür
    }
    else
    {
        return 2; // Ýkinci küme merkezine daha yakýnsa 1 döndür
    }
}


float* KMeans_Euclidean(image im, int k)
{
	int* hist = Histogram(im);
	int MaxIntensity = 256;
	if (k == 2)
	{
		float Tlow = 20, Thigh = 150;
		float Tlow_new, Thigh_new;
		// intensity deðerlerinini etiketle 
		float low, high, number_low, number_high;
		bool State = true;
		while (State)
		{
			low = high = number_low = number_high = 0.0;
			for (int i = 0; i < MaxIntensity; i++)
			{
                if (std::abs(i - Tlow) <= std::abs(i - Thigh))
                {
                    low += (hist[i] * i);
                    number_low += hist[i];
                }
                else
                {
                    high += (hist[i] * i);
                    number_high += hist[i];
                }
            }

            // Yeni eþik deðerlerini hesapla
            Tlow_new = low / number_low;
            Thigh_new = high / number_high;

            // Eþik deðerleri deðiþiklik oranýný kontrol et
            if (std::abs(Tlow - Tlow_new) < 1.0 && std::abs(Thigh - Thigh_new) < 1.0)
            {
                State = false; // Deðiþiklik oraný yeterince küçükse döngüden çýk
            }
            else
            {
                Tlow = Tlow_new;
                Thigh = Thigh_new;
            }
        }

        // Küme merkezlerini oluþtur
        float* clusterCenters = new float[k];
        clusterCenters[0] = Tlow;
        clusterCenters[1] = Thigh;


        delete[] hist;
        return clusterCenters;
    }

    return nullptr; // Kümeleri sadece 2 olarak destekliyoruz, baþka bir k deðeri girildiðinde nullptr döndür
}


image KBasedSegmentation(image im, float* kmean, int k)
{
    float* means = new float[2]; // kmeans deðerleri, back-fore yapýldýðý için 2 adet deðer içerir
    means[0] = 0.0;
    means[1] = 0.0;

    means[0] = kmean[0];
    means[1] = kmean[1];
    std::cout << "küme merkezi low: " << means[0] << std::endl;
    std::cout << "küme merkezi high: " << means[1] << std::endl;

    unsigned char cluster = -1; // bu böyle kalmalý mý bilmiyorum?
    unsigned char binary_1 = 255; // background
    unsigned char binary_0 = 0; // foreground , object


    for (int i = 0; i < im.h * im.w; i++)
    {
        cluster = EuclideanDistance(im.data[i], means, 2);

        if (int(cluster) == 1)
        {
            im.data[i] = binary_1;
        }
        else
        {
            if (int(cluster) == 2)
            {
                im.data[i] = binary_0;
            }
        }
    }
    
    image binary_im;
    binary_im.w = im.w;
    binary_im.h = im.h;
    binary_im.c = im.c;
    binary_im.data = im.data;

    return binary_im;
}








// feature vector deðerleri için 
std::vector<double> KMeans_Euclidean(std::vector<double> data, int k)
{
    int dataSize = data.size();
    std::vector<double> clusterCenters(k, 0.0); // Küme merkezlerini tutan vektör

    // Rastgele baþlangýç küme merkezlerini ata
    for (int i = 0; i < k; i++)
    {
        clusterCenters[i] = data[i];
    }

    bool converged = false; // Yakýnsama durumu
    int maxIterations = 100; // Maksimum iterasyon sayýsý
    int iterations = 0; // Geçerli iterasyon sayýsý

    while (!converged && iterations < maxIterations)
    {
        std::vector<double> clusterSums(k, 0.0); // Küme elemanlarýnýn toplamlarýný tutan vektör
        std::vector<int> clusterCounts(k, 0); // Küme elemanlarýnýn sayýsýný tutan vektör

        // Her veriyi en yakýn küme merkezine ata
        for (int i = 0; i < dataSize; i++)
        {
            double dataPoint = data[i];
            double minDistance = std::abs(dataPoint - clusterCenters[0]);
            int closestCluster = 0;

            for (int j = 1; j < k; j++)
            {
                double distance = std::abs(dataPoint - clusterCenters[j]);

                if (distance < minDistance)
                {
                    minDistance = distance;
                    closestCluster = j;
                }
            }

            // Kümeye veriyi ata
            clusterSums[closestCluster] += dataPoint;
            clusterCounts[closestCluster]++;
        }

        // Yeni küme merkezlerini hesapla
        bool anyChanges = false;
        for (int i = 0; i < k; i++)
        {
            double newCenter = clusterSums[i] / clusterCounts[i];

            if (std::abs(clusterCenters[i] - newCenter) > 0.001) // Yakýnsama kontrolü
            {
                clusterCenters[i] = newCenter;
                anyChanges = true;
            }
        }

        // Yakýnsama durumunu kontrol et
        if (!anyChanges)
        {
            converged = true;
        }

        iterations++;
    }

    std::vector<double> result;
    for (int i = 0; i < k; i++)
    {
        result.push_back(clusterCenters[i]);
    }

    return result;
}


int EuclideanDistance(double data, std::vector<double> kmeans, int k)
{
    std::vector<float> distances;
    for (int i = 0; i < k; i++)
    {
        distances.push_back(std::abs(data - kmeans[i])); // kaç tane sýnýf varsa o kadar k, o kadar uzaklýk deðeri
    }

    float min_d = distances[0];
    int cluster = 0;

    for (int d = 0; d < distances.size(); d++)
    {
        if (distances[d] < min_d)
        {
            min_d = distances[d];
            cluster = d; // 1. sýnýf, 0. sýnýf .... vs.
        }
    }

    return cluster;
}


std::vector<std::vector<double>> KBasedSegmentation(std::vector<double> data, std::vector<double> kmean, int k)
{
    std::vector<double> means;
    for (int i = 0; i < k; i++)
    {
        means.push_back( kmean[i]);
        std::cout << "Kume merkezi " << i << ": " << means[i] << std::endl;
    }

    std::vector<std::vector<double>> clustered_datas;
    clustered_datas.resize(k);

    for (int i = 0; i < data.size(); i++)
    {
        // bir sýnýf deðeri döner
        int cluster = EuclideanDistance(data[i], means, k);

        // datayý dönen sýnýfa ekle
        clustered_datas[cluster].push_back(data[i]);

    }

    return clustered_datas;
}