#include <stdio.h>
#include <iostream>
#include <bitset>
#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

const int       number_of_image = 5;

namespace Klast {
	
	double E = 0.0, sumDist=0.0;
	int kcount = 0;

	int HammingDistance(vector<bitset< 8 > >  f_bin_descr, vector<bitset< 8 > >  s_bin_descr);

	///распределение дескрипторов по кластерам
	int distributionDescriptors(
		vector<vector<vector<bitset< 8 > > > >  &Mas_bin_descr,
		vector<bitset< 8 > >                    &s_bin_descr) {

		int          minDiatance      = 10000;
		int          Dist             = 0;
		int          iterationMinD    = 0;

		for (int i = 0; i < Mas_bin_descr.size(); i++) {
			if ((Dist = HammingDistance(Mas_bin_descr[i][0], s_bin_descr)) < minDiatance)
			{
				minDiatance = Dist;
				iterationMinD = i;
			}
		}
		++kcount;
		sumDist += (minDiatance/512.0);
		Mas_bin_descr[iterationMinD].push_back(s_bin_descr);
		return iterationMinD;
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

		return mas;
	}

	vector<vector<int> > distributionDescriptors(
		vector<vector<vector<bitset< 8 > > > > &Mas_bin_descr,
		vector<vector<vector<bitset< 8 > > > > base_mas_bin_descr)
	{
		vector<vector<int> > matr;

		E = 0;
		for (int i = 0; i < base_mas_bin_descr.size(); i++) {
			matr.push_back(distributionMasDescriptors(Mas_bin_descr, base_mas_bin_descr[i],i));
		}
		
		E = (sumDist)/(kcount);
		//std::cout << endl << " E= " << E << endl;
		return matr;
	}

	////////////////////////////////////////////////////////////////////////////////


	int findCenterForKlaster(Mat descriptorsImage,int n)
	{
		int AVG=0;
		int mas[8] = {0,0,0,0,0,0,0,0};
		bitset< 8 > a;

		for (int i = 0; i < descriptorsImage.rows; ++i) {
			a=bitset< 8 >(descriptorsImage.at<uchar>(i, n));
			for (int j = 0; j < 8; ++j)
				mas[j] += a[j];
		}
		for (int j = 0; j < 8; ++j) {
			if (descriptorsImage.rows/2 > mas[j])
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

	///хэминг
	int HammingDistance(vector<bitset< 8 > >  f_bin_descr, vector<bitset< 8 > >  s_bin_descr) {
		register int distantse = 0;
		register bitset< 8 > resXOR;
		for (size_t j = 0; j < f_bin_descr.size(); j++)
		{
			resXOR = f_bin_descr[j] ^ s_bin_descr[j];
			for (size_t k = 0; k < f_bin_descr[j].size(); k++)
			{
				distantse += resXOR[k];
			}
		}
		return distantse;
	}
}