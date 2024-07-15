#include "morphology.h"



image complement(image im)
{
	int image_size = im.h * im.w;

	unsigned char binary_0 = 0;
	unsigned char binary_1 = 255;

	unsigned char* complement_data = new unsigned char[image_size];
	for (int i = 0; i < image_size; i++)
	{
		complement_data[i] = (im.data[i] == binary_0) ? binary_1 : binary_0; // Complementi hesapla
		im.data[i] = complement_data[i];
	}

	return im;

}




image erosion(image im, int struct_matris_dim)
{
	int im_column = im.w;
	int im_row = im.h;
	unsigned char* im_data = im.data;

	int m_dim = struct_matris_dim; // 3 ile denendi  // matrisin ka�a ka�l�k bir kare dimension olaca��n�n atanmas�
	int m_size = m_dim * m_dim; // matris boyutu
	int image_size = im_column * im_row; //imaj�n boyutu

	unsigned char* new_data = new unsigned char[image_size]; // erosion uygulanan binary resmin yeni de�erlerinin girilece�i dizi
	for (int i = 0; i < image_size; i++)
	{
		new_data[i] = 0;
	}
	unsigned char* current_region = new unsigned char[m_size]; // resmin i�lenecek pikseli ve o pikselin kom�ular�n� tutacak matris/dizi
	for (int i = 0; i < m_size; i++)
	{
		current_region[i] = 0;
	}

	
	unsigned char* erosion_matris = new unsigned char[m_size]; // erosion uygulanacak yap�sal eleman matrisi/dizisi
	for (int i = 0; i < m_size; i++)
	{
		erosion_matris[i] = 255;
	}


	int* ands_result = new int[m_size]; // yap�sal eleman ve resmin hedef b�lgesinin or i�lemi sonu�lar�n� tutacak matris /dizi
	for (int i = 0; i < m_size; i++)
	{
		ands_result[i] = 1;
	}
	int and_result = 1; // or i�lemi sonucunun and i�lemi yap�ld�ktan sonraki sonucunu tutacak de�i�ken



	// Kom�u piksellerin indislerini hesapla
	for (int row = 0; row < im_row; row++) {
		for (int col = 0; col < im_column; col++) {

			// sadece beyaz olanlar �zerinde i�lem yap
			if (int(im_data[row * im_column + col]) == 255)
			{
				and_result = 1; // Her piksel i�in and_result'i s�f�rla

				// Kenar piksel kontrol�
				bool isLeftEdge = (col == 0);
				bool isRightEdge = (col == im_column - 1);
				bool isTopEdge = (row == 0);
				bool isBottomEdge = (row == im_row - 1);

				for (int r = 0; r < m_dim; r++) {
					for (int c = 0; c < m_dim; c++) {
						int imgRow = row - 1 + r;
						int imgCol = col - 1 + c;

						// �ndislerin s�n�rlar�n� kontrol et
						if (imgRow >= 0 && imgRow < im_row && imgCol >= 0 && imgCol < im_column) {
							current_region[r * m_dim + c] = im_data[imgRow * im_column + imgCol];
						}
						else {
							// Kenar piksel kontrol�
							if (isLeftEdge && c == 0) { // buralara 0 ata bir de 
								current_region[r * m_dim + c] = 0; // Sol kenar pikseli i�in 1 de�eri atan�r
							}
							else if (isRightEdge && c == m_dim - 1) {
								current_region[r * m_dim + c] = 0; // Sa� kenar pikseli i�in 1 de�eri atan�r
							}
							else if (isTopEdge && r == 0) {
								current_region[r * m_dim + c] = 0; // ! // �st kenar pikseli i�in 1 de�eri atan�r
							}
							else if (isBottomEdge && r == m_dim - 1) {
								current_region[r * m_dim + c] = 0; // Alt kenar pikseli i�in 1 de�eri atan�r
							}
							else {
								// �ndis ge�erli de�il, d��ar�da kalan b�lgeler i�in iste�e ba�l� i�lemler yap�labilir
								// �rne�in, -1 veya farkl� bir de�er atanabilir
								current_region[r * m_dim + c] = 0;
							}
						}
					}
				}

				for (int i = 0; i < m_size; i++)
				{ // yap�sal elaman or image hedef b�lgesi
					ands_result[i] = int(current_region[i]) & int(erosion_matris[i]);
				}
				for (int i = 0; i < m_size; i++)
				{ // erosion sonucu de�er
					and_result = and_result & ands_result[i];
				}

				// e�er erosion sonucu 0 ise hedef pikselin de�eri azalt�l�r
				if (and_result == 0)
				{
					new_data[row * im_column + col] = 0; // ! 				
				}
				else
				{
					new_data[row * im_column + col] = 255;
				}
				// de�ilse ayn� kal�r
			}
		}
	}

	image erosion_image;
	erosion_image.h = im_row;
	erosion_image.w = im_column;
	erosion_image.c = im.c;
	erosion_image.data = new unsigned char[image_size];

	for (int i = 0; i < image_size; i++)
	{
		erosion_image.data[i] = new_data[i];
	}

	delete[] im_data;
	delete[] current_region;
	delete[] erosion_matris;
	delete[] ands_result;
	delete[] new_data;

	return erosion_image;
}

image dilation(image im, int struct_matris_dim)
{
	int im_column = im.w;
	int im_row = im.h;
	unsigned char* im_data = im.data;

	int m_dim = struct_matris_dim; // 3 ile denendi  // matrisin ka�a ka�l�k bir kare dimension olaca��n�n atanmas�
	int m_size = m_dim * m_dim; // matris boyutu
	int image_size = im_column * im_row; //imaj�n boyutu

	unsigned char* new_data = new unsigned char[image_size]; // dilation uygulanan binary resmin yeni de�erlerinin girilece�i dizi
	for (int i = 0; i < image_size; i++)
	{
		new_data[i] = 0;
	}
	unsigned char* current_region = new unsigned char[m_size]; // resmin i�lenecek pikseli ve o pikselin kom�ular�n� tutacak matris/dizi
	for (int i = 0; i < m_size; i++)
	{
		current_region[i] = 0;
	}
	unsigned char* dilation_matris = new unsigned char[m_size]; // dilation uygulanacak yap�sal eleman matrisi/dizisi
	for (int i = 0; i < m_size; i++)
	{
		dilation_matris[i] = 0;
	}

	int* ors_result = new int[m_size]; // yap�sal eleman ve resmin hedef b�lgesinin or i�lemi sonu�lar�n� tutacak matris /dizi
	for (int i = 0; i < m_size; i++)
	{
		ors_result[i] = 0;
	}
	int or_result = 0; // or i�lemi sonucunun and i�lemi yap�ld�ktan sonraki sonucunu tutacak de�i�ken

	// Kom�u piksellerin indislerini hesapla
	for (int row = 0; row < im_row; row++) {
		for (int col = 0; col < im_column; col++) {

			or_result = 0; // Her piksel i�in and_result'i s�f�rla

			// Kenar piksel kontrol�
			bool isLeftEdge = (col == 0);
			bool isRightEdge = (col == im_column - 1);
			bool isTopEdge = (row == 0);
			bool isBottomEdge = (row == im_row - 1);

			for (int r = 0; r < m_dim; r++) {
				for (int c = 0; c < m_dim; c++) {
					int imgRow = row - 1 + r;
					int imgCol = col - 1 + c;

					// �ndislerin s�n�rlar�n� kontrol et
					if (imgRow >= 0 && imgRow < im_row && imgCol >= 0 && imgCol < im_column) {
						current_region[r * m_dim + c] = im_data[imgRow * im_column + imgCol];
					}
					else {
						// Kenar piksel kontrol�
						if (isLeftEdge && c == 0) { // buralara 0 ata bir de 
							current_region[r * m_dim + c] = 0; // Sol kenar pikseli i�in 1 de�eri atan�r
						}
						else if (isRightEdge && c == m_dim - 1) {
							current_region[r * m_dim + c] = 0; // Sa� kenar pikseli i�in 1 de�eri atan�r
						}
						else if (isTopEdge && r == 0) {
							current_region[r * m_dim + c] = 0; // ! // �st kenar pikseli i�in 1 de�eri atan�r
						}
						else if (isBottomEdge && r == m_dim - 1) {
							current_region[r * m_dim + c] = 0; // Alt kenar pikseli i�in 1 de�eri atan�r
						}
						else {
							// �ndis ge�erli de�il, d��ar�da kalan b�lgeler i�in iste�e ba�l� i�lemler yap�labilir
							// �rne�in, -1 veya farkl� bir de�er atanabilir
							current_region[r * m_dim + c] = 0;
						}
					}
				}
			}

			for (int i = 0; i < m_size; i++)
			{ // yap�sal elaman or image hedef b�lgesi
				ors_result[i] = int(current_region[i]) | int(dilation_matris[i]);
			}
			for (int i = 0; i < m_size; i++)
			{ // dilation sonucu de�er
				or_result = or_result | ors_result[i];
			}

			// e�er dilation sonucu 0 ise hedef pikselin de�eri azalt�l�r
			if (or_result == 0)
			{
				new_data[row * im_column + col] = 0; // ! 
			}
			else
			{
				new_data[row * im_column + col] = 255;
			}
			// de�ilse ayn� kal�r

		}
	}

	image dilation_image;
	dilation_image.h = im_row;
	dilation_image.w = im_column;
	dilation_image.c = im.c;
	dilation_image.data = new unsigned char[image_size];

	for (int i = 0; i < image_size; i++)
	{
		dilation_image.data[i] = new_data[i];
	}

	delete[] im_data;
	delete[] current_region;
	delete[] dilation_matris;
	delete[] ors_result;
	delete[] new_data;

	return dilation_image;
}




image opening(image im, int struct_matris_dim)
{
	image im_erosion;
	im_erosion = erosion(im, struct_matris_dim);

	image im_dilation;
	im_dilation = dilation(im_erosion, struct_matris_dim);

	return im_dilation;
}

image closing(image im, int struct_matris_dim)
{
	image im_dilation;
	im_dilation = dilation(im, struct_matris_dim);

	image im_erosion;
	im_erosion = erosion(im_dilation, struct_matris_dim);


	return im_erosion;
}




image edge_detection(image im, int struct_matris_dim)
{
	int im_column = im.w;
	int im_row = im.h;
	unsigned char* im_data = im.data;

	int m_dim = struct_matris_dim; // 3 ile denendi  // matrisin ka�a ka�l�k bir kare dimension olaca��n�n atanmas�
	int m_size = m_dim * m_dim; // matris boyutu
	int image_size = im_column * im_row; //imaj�n boyutu

	unsigned char* new_data = new unsigned char[image_size]; // dilation uygulanan binary resmin yeni de�erlerinin girilece�i dizi
	for (int i = 0; i < image_size; i++)
	{
		new_data[i] = 0;
	}
	unsigned char* current_region = new unsigned char[m_size]; // resmin i�lenecek pikseli ve o pikselin kom�ular�n� tutacak matris/dizi
	for (int i = 0; i < m_size; i++)
	{
		current_region[i] = 0;
	}


	unsigned char* dilation_matris = new unsigned char[m_size]; // dilation uygulanacak yap�sal eleman matrisi/dizisi
	for (int i = 0; i < m_size; i++)
	{
		dilation_matris[i] = 0;
	}


	int* ors_result = new int[m_size]; // yap�sal eleman ve resmin hedef b�lgesinin or i�lemi sonu�lar�n� tutacak matris /dizi
	for (int i = 0; i < m_size; i++)
	{
		ors_result[i] = 0;
	}
	int or_result = 0; // or i�lemi sonucunun and i�lemi yap�ld�ktan sonraki sonucunu tutacak de�i�ken



	// Kom�u piksellerin indislerini hesapla
	for (int row = 0; row < im_row; row++) {
		for (int col = 0; col < im_column; col++) {

			if (int(im_data[row * im_column + col]) == 0)
			{
				or_result = 0; // Her piksel i�in and_result'i s�f�rla

				// Kenar piksel kontrol�
				bool isLeftEdge = (col == 0);
				bool isRightEdge = (col == im_column - 1);
				bool isTopEdge = (row == 0);
				bool isBottomEdge = (row == im_row - 1);

				for (int r = 0; r < m_dim; r++) {
					for (int c = 0; c < m_dim; c++) {
						int imgRow = row - 1 + r;
						int imgCol = col - 1 + c;

						// �ndislerin s�n�rlar�n� kontrol et
						if (imgRow >= 0 && imgRow < im_row && imgCol >= 0 && imgCol < im_column) {
							current_region[r * m_dim + c] = im_data[imgRow * im_column + imgCol];
						}
						else {
							// Kenar piksel kontrol�
							if (isLeftEdge && c == 0) { // buralara 0 ata bir de 
								current_region[r * m_dim + c] = 0; // Sol kenar pikseli i�in 1 de�eri atan�r
							}
							else if (isRightEdge && c == m_dim - 1) {
								current_region[r * m_dim + c] = 0; // Sa� kenar pikseli i�in 1 de�eri atan�r
							}
							else if (isTopEdge && r == 0) {
								current_region[r * m_dim + c] = 0; // ! // �st kenar pikseli i�in 1 de�eri atan�r
							}
							else if (isBottomEdge && r == m_dim - 1) {
								current_region[r * m_dim + c] = 0; // Alt kenar pikseli i�in 1 de�eri atan�r
							}
							else {
								// �ndis ge�erli de�il, d��ar�da kalan b�lgeler i�in iste�e ba�l� i�lemler yap�labilir
								// �rne�in, -1 veya farkl� bir de�er atanabilir
								current_region[r * m_dim + c] = 0;
							}
						}
					}
				}

				for (int i = 0; i < m_size; i++)
				{ // yap�sal elaman or image hedef b�lgesi
					ors_result[i] = int(current_region[i]) | int(dilation_matris[i]);
				}
				for (int i = 0; i < m_size; i++)
				{ // dilation sonucu de�er
					or_result = or_result | ors_result[i];
				}

				// e�er dilation sonucu 0 ise hedef pikselin de�eri azalt�l�r
				if (or_result == 0)
				{
					new_data[row * im_column + col] = 0; // ! 
				}
				else
				{
					new_data[row * im_column + col] = 255;
				}
				// de�ilse ayn� kal�r
			}
		}
	}

	image dilation_image;
	dilation_image.h = im_row;
	dilation_image.w = im_column;
	dilation_image.c = im.c;
	dilation_image.data = new unsigned char[image_size];

	for (int i = 0; i < image_size; i++)
	{
		dilation_image.data[i] = new_data[i];
	}

	delete[] im_data;
	delete[] current_region;
	delete[] dilation_matris;
	delete[] ors_result;
	delete[] new_data;

	return dilation_image;
}




void connectedComponent(image im, int row, int column, unsigned char label) {
	if (row < 0 || column < 0 || row >= im.h || column >= im.w) {
		return;
	}

	if (im.data[row * im.w + column] != 255) {
		return;
	}

	im.data[row * im.w + column] = label;

	connectedComponent(im, row - 1, column, label);
	connectedComponent(im, row + 1, column, label);
	connectedComponent(im, row, column - 1, label);
	connectedComponent(im, row, column + 1, label);
}

image& labeling(image& im)
{
	int row = im.h;
	int column = im.w;

	int label = 20;


	for (int r = 0; r < row; r++) {
		for (int c = 0; c < column; c++) {
			if (im.data[r*column+c] == 255) {
				connectedComponent(im, r, c, label);
				label = label+10;
			}
		}
	}

	return im;
}



// �al��mad�
image& regionFilling(image& im) {
	int row = im.h;
	int column = im.w;

	unsigned char label = 255;

	image filled_image;
	filled_image.h = row;
	filled_image.w = column;
	filled_image.c = 1;
	
	filled_image.data = new unsigned char[row * column]{ 0 };

	for (int r = 0; r < row; r++) {
		for (int c = 0; c < column; c++) {

			// imajdaki beyaz noktalar
			if (im.data[r * column + c] == 255) 
			{

				// center
				if (!(r < 0 || c < 0 || r >= row || c >= column)) {
					filled_image.data[r * column + c] = label;
				}
				// top
				if (!(r - 1 < 0 || c < 0 || r - 1 >= row || c >= column)) {
					filled_image.data[(r - 1) * column + c] = label;
				}
				// bottom
				if (!(r + 1 < 0 || c < 0 || r + 1 >= row || c >= column)) {
					filled_image.data[(r + 1) * column + c] = label;
				}
				// left
				if (!(r < 0 || c - 1 < 0 || r >= row || c - 1 >= column)) {
					filled_image.data[r * column + (c - 1)] = label;
				}
				// right
				if (!(r < 0 || c + 1 < 0 || r >= row || c + 1 >= column)) {
					filled_image.data[r * column + (c + 1)] = label;
				}

			}
		}
	}

	return filled_image;
}


std::vector<unsigned char> getLabels(image im)
{
	int im_row = im.h;
	int im_column = im.w;

	unsigned char binary0 = 0;

	std::vector<unsigned char> colors;
	for (int row = 0; row < im_row; row++)
	{
		for (int col = 0; col < im_column; col++)
		{
			if (!(im.data[row * im_column + col] == binary0))
			{
				unsigned char color = im.data[row * im_column + col];
				bool hasColor = false;
				for (int i = 0; i < colors.size(); i++) {
					if (colors[i] == color) {
						hasColor = true;
						break;
					}
				}

				if (!hasColor) {
					colors.push_back(color);

				}
			}
		}
	}

	return colors;
}

// image -> std::vector<std::vector<int>> [min_x, max_x, min_y, max_y, width, height]
std::vector<std::vector<int>> bounding(image im)
{
	int im_row = im.h;
	int im_column = im.w;

	int max_x = 0; // --
	int min_x = 0; // --

	int max_y = 0; // |
	int min_y = 0; // |

	unsigned char binary0 = 0;

	// resimdeki etiket de�erlerini al
	std::vector<unsigned char> colors = getLabels(im);

	std::vector<std::vector<int>> bounds; // her bir nesnenin s�n�r de�erlerini tutacak de�i�ken


	for (int i = 0; i < colors.size(); i++)
	{
		unsigned char color = colors[i]; // bir etiketi al

		std::vector<Point> object_points; // tek bir nesnenin pikselleri

		// t�m resmi tara, color de�erindeki t�m pikselleri topla
		for (int row = 0; row < im_row; row++)
		{
			for (int col = 0; col < im_column; col++)
			{
				unsigned char pixel_color = im.data[row * im_column + col];
				Point point;
				if (pixel_color == color)
				{
					point.row = row;
					point.column = col;
					object_points.push_back(point);
				}
			}
		}

		// s�n�rlar� bul
		min_x = max_x = object_points[0].column;
		min_y = max_y = object_points[0].row;

		for (int j = 0; j < object_points.size(); j++)
		{
			if (object_points[j].column < min_x)
				min_x = object_points[j].column;
			if (object_points[j].column > max_x)
				max_x = object_points[j].column;

			if (object_points[j].row < min_y)
				min_y = object_points[j].row;
			if (object_points[j].row > max_y)
				max_y = object_points[j].row;
		}

		int new_height = abs(max_y - min_y);
		int new_width = abs(max_x - min_x);

		//t�m s�n�r de�i�kenlerini tutan vector
		std::vector<int> bound;

		bound.push_back(min_x);
		bound.push_back(max_x);
		bound.push_back(min_y);
		bound.push_back(max_y);
		bound.push_back(new_width);
		bound.push_back(new_height);

		bounds.push_back(bound); // nesnenin s�n�r de�erlerini t�m nesnelerin s�n�r de�erlerini tutacak de�i�kene ata

	}
	return bounds;
}


image drawBoundary(image im)
{
	image bounded_im;
	bounded_im.w = im.w;
	bounded_im.h = im.h;
	bounded_im.c = 1;
	bounded_im.data = new unsigned char[im.w * im.h];
	for (int i = 0; i < im.h * im.w; i++)
	{
		bounded_im.data[i] = im.data[i];
	}
	unsigned char binary1 = 255;
	std::vector<std::vector<int>> bounds = bounding(im);

	for (int i = 0; i < bounds.size(); i++)
	{
		//drawBoundingBox(bounded_im, bounds[i][0], bounds[i][1], bounds[i][2], bounds[i][3], bounds[i][4], bounds[i][5]);
		int min_x = bounds[i][0];
		int max_x = bounds[i][1];
		int min_y = bounds[i][2];
		int max_y = bounds[i][3];
		int width = bounded_im.w;

		// yatay �izgiler
		for (int col = min_x; col <= max_x; col++)
		{
			bounded_im.data[min_y * width + col] = binary1;
			bounded_im.data[max_y * width + col] = binary1;
		}
		// dikey �izgiler
		for (int row = min_y; row <= max_y; row++)
		{
			bounded_im.data[row * width + min_x] = binary1;
			bounded_im.data[row * width + max_x] = binary1;
		}

		std::cout << bounds[i][0] << " " << bounds[i][1] << " " << bounds[i][2] << " " << bounds[i][3] << " " << bounds[i][4] << " " << bounds[i][5] << std::endl;
	}

	return bounded_im;
}
