#include "myMatche.h"





void funcbild(Ptr<BRISK> &detectorBRISK, Mat *img, const int &n) {


	int numberOFmatche = Sum_natural_numbers(n - 1);
	int colMathes      = 0;
	int s1             = 0;
	int s2             = 0;

	BFMatcher matcher(NORM_HAMMING, true);
	Mat imgFinal, imgMatch3;

	Mat                   * BinaryImg        = new Mat[n];
	std::vector<KeyPoint> * keypointsImage   = new std::vector<KeyPoint>[n];
	Mat                   * descriptorsImage = new Mat[n];
	std::vector<DMatch>   * matches          = new std::vector<DMatch>[numberOFmatche];
	Mat                   * imgMatch = new Mat[n];


	
	for (int i = 0; i < n; ++i) 
	{
		// —оздаем бинарные изображени€ к которым будем примен€ть преобразовани€
		BinaryImg[i] = CreateBinaryImage(img[i]);
		//создаем копию изображени€ с которой буде работать
		imgMatch[i]  = img[i].clone();
	}

	// «апись ключевых точек и дескрипторов дл€ обоих изображений
	for (int i = 0; i<n; ++i)
		detectorBRISK->detectAndCompute(BinaryImg[i], Mat(), keypointsImage[i], descriptorsImage[i]);
	// Ќахождение св€зей между точками разных изображений 
	for (int j = 0; j<n - 1; ++j)
		for (int i = 1 + j; i < n; ++i)
			matcher.match(descriptorsImage[j], descriptorsImage[i], matches[colMathes++]);



	for (int i = 0; i < n; ++i) {
		cout << i + 1 << " img - number of keypoints: " << keypointsImage[i].size() << endl;
		for (int n = 0; n < keypointsImage[i].size(); ++n) {
			circle(imgMatch[i], keypointsImage[i][n].pt, 4, Scalar(0, 0, 0), -1, 8);
			circle(imgMatch[i], keypointsImage[i][n].pt, 2, Scalar(0, 0, 255), -1, 8);
		}
	}

	colMathes = 0;
	for (int j = 0; j<n - 1; ++j)
		for (int i = 1 + j; i < n; ++i)
		{
		  Draw_matches_upon_Picture(matches[colMathes++], imgMatch[j], imgMatch[i], keypointsImage[j], keypointsImage[i], Scalar(0, colMathes * 10, 0));
		}












	//cout << "mathes:  " << matches.size()<< endl;
	for (int i = 0; i < n; ++i)
		imshow("matches" + i, imgMatch[i]);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*drawMatches(img1, keypointsImageTemple, img2, keypointsImage, matches, imgMatch3);
	imwrite("matches.jpeg", imgMatch3);
	std::cout << endl <<"Size of 1-st img Descriptor:   "<< descriptorsImageTemple.size()<<endl;
	std::cout << endl <<"Size of 2-nd img Descriptor:   " << descriptorsImage.size() << endl;
	std::cout << "first descriptors of the first image:_"<<endl;


	for (int i = 0; i < descriptorsImageTemple.cols; ++i) {
	if (i % 8 == 0)cout << endl;
	std::cout << descriptorsImageTemple.row(0).col(i) << " ";

	}*/
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



