#include <stdio.h>
#include <iostream>
#include <bitset>
#include <algorithm>
#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


#define FIRSTSTRINPRINT 0
#define ALLPRINT 1


using namespace cv;
using namespace std;


class MySort {
	public:
		vector<vector<int> > matr;
		vector<vector<int> > matr2;

	void sort() {
		vector<int> g;
		for (int i = 0; i < matr.size(); ++i)
			g.push_back(0);
		for (int i = 0; i < matr.size(); ++i) {
			matr2.push_back(g);
		}
		for (int i = 0; i < matr.size(); ++i)
			for (int j = 0; j < matr[i].size(); ++j)
			{
				++matr2[i][matr[i][j]];
			}
	}

	void show() {

		for (int i = 0; i < matr.size(); ++i)
		{
			std::cout << endl << i + 1 << "im  1    2    3    4    5    6    7" << endl;
			for (int j = 0; j < matr.size(); ++j)
			{
				std::cout << "    " << matr2[i][j];
			}
			std::cout << endl;
		}
	}
};

__int64 calcHammingDistance(__int64 x, __int64 y)
{
	__int64 dist = 0, val = x ^ y;
	// Count the number of set bits
	while (val)
	{
		++dist;
		val &= val - 1;
	}

	return dist;
}



void sortDescriptorsAndKP(vector<KeyPoint> &kp, Mat &descriptor,int MaxcolElem) {

	int *            Masmindist     = new int[kp.size()-1];
	Mat              descriporSizeN = descriptor.clone();
	vector<KeyPoint> newKP;
	int              k              = 0;

	int delElem = descriporSizeN.rows - MaxcolElem;
	descriporSizeN.pop_back(delElem);


	for (int i = 0; i < kp.size() - 1; ++i) {
		Masmindist[i] = calcHammingDistance(kp[i].pt.x + kp[i].pt.y, kp[i + 1].pt.x + kp[i + 1].pt.y);
	}
	for (int ss = 9; ss >= 0; --ss) {
		for (int i = 0; i < kp.size() - 1; ++i) {
			if (ss == Masmindist[i]) {
				descriporSizeN.row(k) = descriptor.row(i);
				newKP.push_back(kp[i]);
				++k;	
				if (k == MaxcolElem) { goto a; }
			}
		}
	}

a:
	kp         = newKP;
	descriptor = descriporSizeN;

}


int Sum_natural_numbers(int n) 
{
	if (n == 1) return 1;
	else if (n < 1) return 0;
	else return n + Sum_natural_numbers(n - 1);
}

Mat CreateBinaryImage(const Mat &img) {
	Mat newimg;
	//GaussianBlur(img, newimg, Size(3, 3), 4, 4);
	cvtColor(img, newimg, CV_RGB2GRAY);
	threshold(newimg, newimg, 180, 255, CV_THRESH_BINARY);
	return newimg;
}

void Draw_matches_upon_Picture(
	std::vector<DMatch> matches,
	Mat   &firstimgMatch,
	Mat   &secondimgMatch,
	std::vector<KeyPoint>  FkeypointsImage,  //first
	std::vector<KeyPoint>  SkeypointsImage,  //second
	Scalar collor
)
{
	for (int z = 0; z<matches.size(); ++z)
	{
		circle(firstimgMatch, FkeypointsImage[matches[z].queryIdx].pt, 2, collor, -1, 8);
		circle(secondimgMatch, SkeypointsImage[matches[z].trainIdx].pt, 2, collor, -1, 8);
	}

}


void printDescriptorBinary(Mat descriptors, const bool &isAllprint) {
	int itaration = 0;


	vector< vector<bitset< 8 > > > binary_descriptors;

	for (int i = 0; i < descriptors.rows; i++) {
		vector<bitset< 8 > > current_binary_descriptor;

		for (int j = 0; j < descriptors.cols; j++) {
			current_binary_descriptor.push_back(bitset< 8 >(descriptors.at<uchar>(i, j)));
		}
		binary_descriptors.push_back(current_binary_descriptor);
	}

	for (size_t i = 0; i < binary_descriptors.size(); i++) {
		for (size_t j = 0; j < binary_descriptors[i].size(); j++) {
			for (size_t k = 0; k < binary_descriptors[i][j].size(); k++) {
					std::cout << binary_descriptors[i][j][k] << " ";
			}
			cout << endl;
		}
		if (FIRSTSTRINPRINT == isAllprint) break;
	}
	cout << endl;
	vector<vector<bitset< 8 > > >().swap(binary_descriptors);
}


void printGistogram(Mat descriptors, const bool &isAllprint) {
	Mat Gistogramma = imread("256gist.png");
	vector< vector<int > > int_descriptors;

	for (int i = 0; i < descriptors.rows; i++) {
		vector<int > current_int_descriptor;

		for (int j = 0; j < descriptors.cols; j++) {
			current_int_descriptor.push_back(int(descriptors.at<uchar>(i, j)));
		}
		int_descriptors.push_back(current_int_descriptor);
	}

		for (size_t j = 0, k = 0; k < int_descriptors[0].size(); j+=4,++k) {
			for (int jj = Gistogramma.rows - int_descriptors[0][k]; jj < Gistogramma.rows; jj++) {
				Gistogramma.at<Vec3b>(jj, j)     = Vec3b(0, 0, 0);
				Gistogramma.at<Vec3b>(jj, j + 1) = Vec3b(0, 0, 0);
				Gistogramma.at<Vec3b>(jj, j + 2) = Vec3b(0, 0, 0);
				Gistogramma.at<Vec3b>(jj, j + 3) = Vec3b(0, 0, 0);

			}
		}
	vector<vector<int > >().swap(int_descriptors);
	imshow("gist", Gistogramma);
}


void drawGistogram(Mat descriptors, const bool &isAllprint) {
	Mat Gistogramma = imread("512gist.png");
	int itaration = 0;


	vector< vector<bitset< 8 > > > binary_descriptors;

	for (int i = 0; i < descriptors.rows; i++) {
		vector<bitset< 8 > > current_binary_descriptor;

		for (int j = 0; j < descriptors.cols; j++) {
			current_binary_descriptor.push_back(bitset< 8 >(descriptors.at<uchar>(i, j)));
		}
		binary_descriptors.push_back(current_binary_descriptor);
	}

	for (size_t i = 0; i < binary_descriptors.size(); i++) {
		for (size_t j = 0; j < binary_descriptors[i].size(); j++) {
			for (size_t k = 0; k < binary_descriptors[i][j].size(); k++) {

				if (binary_descriptors[i][j][k])
					for (int jj = Gistogramma.rows / 2; jj < Gistogramma.rows; jj++)
						Gistogramma.at<Vec3b>(jj, itaration) = Vec3b(0, 0, 0);
				++itaration;
			}
		}
		if (FIRSTSTRINPRINT == isAllprint) break;
	}
	vector<vector<bitset< 8 > > >().swap(binary_descriptors);
	imshow("gist", Gistogramma);
}

void IzobrajeniechtoKlastereziryem(const Mat &Gistogramma2, vector<KeyPoint>  * keypointsKlaster) {

	Mat Gistogramma = Gistogramma2.clone();
		for (int n = 0; n < keypointsKlaster[0].size(); ++n) {
			circle(Gistogramma, keypointsKlaster[0][n].pt, 4, Scalar(0, 0, 0), -1, 8);
			circle(Gistogramma, keypointsKlaster[0][n].pt, 2, Scalar(0, 255, 0), -1, 8);
		}
	imshow("Klaster1", Gistogramma);
	Gistogramma = Gistogramma2.clone();
		for (int n = 0; n < keypointsKlaster[1].size(); ++n) {
			circle(Gistogramma, keypointsKlaster[1][n].pt, 4, Scalar(0, 0, 0), -1, 8);
			circle(Gistogramma, keypointsKlaster[1][n].pt, 2, Scalar(255, 0, 0), -1, 8);
		}
	imshow("Klaster2", Gistogramma);
	Gistogramma = Gistogramma2.clone();
		for (int n = 0; n < keypointsKlaster[2].size(); ++n) {
			circle(Gistogramma, keypointsKlaster[2][n].pt, 4, Scalar(0, 0, 0), -1, 8);
			circle(Gistogramma, keypointsKlaster[2][n].pt, 2, Scalar(0, 0, 255), -1, 8);
		}
	imshow("Klaster3", Gistogramma);
	Gistogramma = Gistogramma2.clone();
		for (int n = 0; n < keypointsKlaster[3].size(); ++n) {
			circle(Gistogramma, keypointsKlaster[3][n].pt, 4, Scalar(0, 0, 0), -1, 8);
			circle(Gistogramma, keypointsKlaster[3][n].pt, 2, Scalar(255, 255, 0), -1, 8);
		}
	imshow("Klaster4", Gistogramma);
	Gistogramma = Gistogramma2.clone();
		for (int n = 0; n < keypointsKlaster[4].size(); ++n) {
			circle(Gistogramma, keypointsKlaster[4][n].pt, 4, Scalar(0, 0, 0), -1, 8);
			circle(Gistogramma, keypointsKlaster[4][n].pt, 2, Scalar(0, 255, 255), -1, 8);
		}
	imshow("Klaster5", Gistogramma);
	}



//не та кластеризация
void Clustering_by_coordinates_KP(vector<KeyPoint> * kpImage, vector<KeyPoint> * keypointsKlaster, int n)
{
	KeyPoint  *  masKP         =  new KeyPoint[n];
	int          minDiatance   =  10000;
	int          iterationMinD =  0;
	int          count         =  0;
	//выбираем центральные точки наших базовых кластеров КТ
	for (int i = 0; i < n; ++i)
	{
		masKP[i] = kpImage[i][0];
		keypointsKlaster[i].push_back(kpImage[i][0]);
	}

	//находим минимальную дистанцию по хэмингe



		for (int j = 0; j < n; ++j)
			for (int k = 1; k < kpImage[j].size(); ++k) {
				for (int i = 0; i < n; ++i)
				{
					if (calcHammingDistance(masKP[i].pt.x + masKP[i].pt.y, kpImage[j][k].pt.x+ kpImage[j][k].pt.y) < minDiatance)
					{
						minDiatance = calcHammingDistance(masKP[i].pt.x + masKP[i].pt.y, kpImage[j][k].pt.x + kpImage[j][k].pt.y);
						iterationMinD = i;
					}
				}
				minDiatance = 10000;
				keypointsKlaster[iterationMinD].push_back(kpImage[j][k]);
				++count;
			}
		std::cout << endl;
		std::cout << "Num of KP: " << count +5<<endl;
	delete  masKP;
}
