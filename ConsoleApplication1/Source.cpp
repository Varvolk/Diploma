#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

void funcbild(Ptr<BRISK> &detectorBRISK, Mat img1, Mat img2) {
	std::vector<KeyPoint> keypointsImageTemple, keypointsImage;
	Mat descriptorsImageTemple, descriptorsImage;
	std::vector<DMatch> matches;

	// Запись ключевых точек и дескрипторов для обоих изображений
	detectorBRISK->detectAndCompute(img1, Mat(), keypointsImageTemple, descriptorsImageTemple);
	detectorBRISK->detectAndCompute(img2, Mat(), keypointsImage, descriptorsImage);

	BFMatcher matcher(NORM_HAMMING, true);
	matcher.match(descriptorsImageTemple, descriptorsImage, matches);
	// Отобразить на изображении
	Mat imgMatch= img1.clone(), imgMatch2 = img2.clone();
	Mat imgFinal, imgMatch3;
   cout << "First img - number of keypoints: "<< keypointsImageTemple.size() <<endl;
   for (int n = 0; n < keypointsImageTemple.size(); ++n) {
	   circle(imgMatch, keypointsImageTemple[n].pt, 4, Scalar(0, 0, 0), -1, 8);
	   circle(imgMatch, keypointsImageTemple[n].pt, 2, Scalar(0, 0, 255), -1, 8);
   }
	cout << "Second img - number of keypoints: " << keypointsImage.size() << endl;
	for (int n = 0; n < keypointsImage.size(); ++n) {
		circle(imgMatch2, keypointsImage[n].pt, 4, Scalar(0, 0, 0), -1, 8);
		circle(imgMatch2, keypointsImage[n].pt, 2, Scalar(0, 0, 255), -1, 8);
	}
	double persentoftrueKeypoint= keypointsImage.size();


			for (int z = 0; z<matches.size(); ++z)	
		   {
				circle(imgMatch2, keypointsImage[matches[z].trainIdx].pt, 2, Scalar(0, 255, 0), -1, 8);
				circle(imgMatch, keypointsImageTemple[matches[z].queryIdx].pt, 2, Scalar(0, 255, 0), -1, 8);
				
		   }
	cout << "mathes:  " << matches.size()<< endl;
	imshow("matches", imgMatch);
	imshow("matches2", imgMatch2);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	drawMatches(img1, keypointsImageTemple, img2, keypointsImage, matches, imgMatch3);
	imwrite("matches.jpeg", imgMatch3);
	std::cout << endl <<"Size of 1-st img Descriptor:   "<< descriptorsImageTemple.size()<<endl;
	std::cout << endl <<"Size of 2-nd img Descriptor:   " << descriptorsImage.size() << endl;
	std::cout << "first descriptors of the first image:_"<<endl;


	for (int i = 0; i < descriptorsImageTemple.cols; ++i) {
		if (i % 8 == 0)cout << endl;
		std::cout << descriptorsImageTemple.row(0).col(i) << " ";
		
	}
	////////////////////////////////////////
/*	std::vector<Point2f> obj;
	std::vector<Point2f> scene;
	for (int i = 0; i < matches.size(); i++)
	{
		obj.push_back(keypointsImageTemple[matches[i].queryIdx].pt);
		scene.push_back(keypointsImage[matches[i].trainIdx].pt);
	}
	Mat H = findHomography(obj, scene, CV_RANSAC);       
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(img1.cols, 0);
	obj_corners[2] = cvPoint(img1.cols, img1.rows); 
	obj_corners[3] = cvPoint(0, img1.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(imgMatch3, scene_corners[0] + Point2f(img1.cols, 0), scene_corners[1] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
	line(imgMatch3, scene_corners[1] + Point2f(img1.cols, 0), scene_corners[2] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
	line(imgMatch3, scene_corners[2] + Point2f(img1.cols, 0), scene_corners[3] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
	line(imgMatch3, scene_corners[3] + Point2f(img1.cols, 0), scene_corners[0] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
	imwrite("matches3.jpeg", imgMatch);
	*/
}



int main() {
	int  	thresh = 60;
	int  	octaves = 0;
	float  	patternScale = 0.5f;
	Ptr<BRISK> detectorBRISK = BRISK::create(thresh, octaves, patternScale);
	cout << "START " << endl;
	Mat img1 = imread("im1.jpg");
	Mat img2 = imread("im2.jpg");
	funcbild(detectorBRISK,img1, img2);
	waitKey(0);
	return 0;
}

