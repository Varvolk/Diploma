#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

int Sum_natural_numbers(int n) 
{
	if (n == 1) return 1;
	else if (n < 1) return 0;
	else return n + Sum_natural_numbers(n - 1);
}

Mat CreateBinaryImage(Mat img) {
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
