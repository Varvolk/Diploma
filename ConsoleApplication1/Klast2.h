#include <stdio.h>
#include <iostream>
#include <bitset>
#include <math.h> 
#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

const int       number_of_image = 5;

namespace Klast {



	double E = 0.0, sumDist = 0.0;
	int kcount = 0;
	int masCount[number_of_image];
	vector<double[64][8]> masCenterKlast(number_of_image);
	///хэминг
	double HammingDistance(double f_bin_descr[64][8], vector<bitset< 8 > >  s_bin_descr) {
		register double distantse = 0;
		for (size_t j = 0; j < 64; j++)
		{
			for (size_t k = 0; k < 8; k++)
			{
				distantse+= abs(f_bin_descr[j][k] - s_bin_descr[j][k]);
			}
		}
		return distantse;
	}

	///распределение дескрипторов по кластерам
	int distributionDescriptors(vector<vector<vector<bitset< 8 > > > >  &Mas_bin_descr,vector<bitset< 8 > > &s_bin_descr)
	{

		double       minDiatance = 10000;
		double       Dist = 0;
		int          iterationMinD = 0;

		for (int i = 0; i < Mas_bin_descr.size(); i++) {
			if ((Dist = HammingDistance(masCenterKlast[i], s_bin_descr)) < minDiatance)
			{
				minDiatance = Dist;
				iterationMinD = i;
			}
		}
		++kcount;
		Mas_bin_descr[iterationMinD].push_back(s_bin_descr);
		for (int j = 0; j < 64; j++)
			for (int s = 0; s < 8; s++)
				masCenterKlast[iterationMinD][j][s] = masCenterKlast[iterationMinD][j][s] + (1.0 /  kcount/*++masCount[iterationMinD]*/)*
				(Mas_bin_descr[iterationMinD][Mas_bin_descr[iterationMinD].size()-1][j][s]- masCenterKlast[iterationMinD][j][s]);

		return iterationMinD;
	}

	void foundE(vector<vector<vector<bitset< 8 > > > >  &Mas_bin_descr, vector<bitset< 8 > > &s_bin_descr)
	{

		double       minDiatance = 10000;
		double       Dist = 0;
		for (int i = 0; i < Mas_bin_descr.size(); i++) {
			if ((Dist = HammingDistance(masCenterKlast[i], s_bin_descr)) < minDiatance)
			{
				minDiatance = Dist;
			}
		}
		sumDist += (minDiatance / 512.0);
	}


	vector<int> distributionMasDescriptors(
		vector<vector<vector<bitset< 8 > > > >   &Mas_bin_descr,
		vector<vector<bitset< 8 > > >             base_mas_bin_descr,
		int                                       id_mas)
	{

		double       counter = 0.0;
		vector<int>  mas;
		for (int i = 0; i < base_mas_bin_descr.size(); i++) {
			mas.push_back(distributionDescriptors(Mas_bin_descr, base_mas_bin_descr[i]));
			if (mas[i] == id_mas)
				++counter;
		}
		for (int i = 0; i < base_mas_bin_descr.size(); i++)
			foundE(Mas_bin_descr, base_mas_bin_descr[i]);
		return mas;
	}

	vector<vector<int> > distributionDescriptors(
		vector<vector<vector<bitset< 8 > > > > &Mas_bin_descr,
		vector<vector<vector<bitset< 8 > > > > base_mas_bin_descr)
	{
		vector<vector<int> > matr;
		E = 0;
		sumDist = 0;

		for (int i = 0; i < base_mas_bin_descr.size(); i++) {
			for (int j = 0; j < 64; j++)
				for (int s = 0; s < 8; s++) {
					masCenterKlast[i][j][s] = Mas_bin_descr[i][0][j][s];
				}
		}

		for (int i = 0; i < base_mas_bin_descr.size(); i++) {
			matr.push_back(distributionMasDescriptors(Mas_bin_descr, base_mas_bin_descr[i], i));
		}

		E = (sumDist) / (255);
		//std::cout << endl << " E= " << E << endl;
		return matr;
	}

	////////////////////////////////////////////////////////////////////////////////

	int findCenterForKlaster(Mat descriptorsImage, int n)
	{
		int AVG = 0;
		int mas[8] = { 0,0,0,0,0,0,0,0 };
		bitset< 8 > a;

		for (int i = 0; i < descriptorsImage.rows; ++i) {
			a = bitset< 8 >(descriptorsImage.at<uchar>(i, n));
			for (int j = 0; j < 8; ++j)
				mas[j] += a[j];
		}
		for (int j = 0; j < 8; ++j) {
			if (descriptorsImage.rows / 2 > mas[j])
				a[j] = 0;
			else
				a[j] = 1;
		}
		AVG = a.to_ullong();
		return (int)AVG;
	}


	///конвекторы
	vector<vector< vector<bitset< 8 > > > > Convert_to_vector_bits(Mat * mas_descriptors, int n) {

		vector<vector< vector<bitset< 8 > > > > binary_descriptors;
		for (int s = 0; s < n; s++) {
			vector< vector<bitset< 8 > > > binary_descriptors2;
			for (int i = 0; i < mas_descriptors[s].rows; i++) {
				vector<bitset< 8 > > current_binary_descriptor;

				for (int j = 0; j < mas_descriptors[s].cols; j++) {
					current_binary_descriptor.push_back(bitset< 8 >(mas_descriptors[s].at<uchar>(i, j)));
				}
				binary_descriptors2.push_back(current_binary_descriptor);
			}
			binary_descriptors.push_back(binary_descriptors2);
		}
		return binary_descriptors;
	}
	Mat* Convert_to_mas_Mat(vector<vector< vector<bitset< 8 > > > >  mas_bin_descriptors) {


		Mat* descriptors = new Mat[mas_bin_descriptors.size()];

		for (int s = 0; s < mas_bin_descriptors.size(); s++) {
			descriptors[s] = Mat(mas_bin_descriptors[s].size(), 64, 0);

			for (int i = 0; i < mas_bin_descriptors[s].size(); i++) {
				for (int j = 0; j < mas_bin_descriptors[s][i].size(); j++) {
					descriptors[s].row(i).col(j) = mas_bin_descriptors[s][i][j].to_ullong();
				}

			}

		}
		return descriptors;
	}

	
}