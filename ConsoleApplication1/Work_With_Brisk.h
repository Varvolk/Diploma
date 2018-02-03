#include "myMatche.h"

#include "kasterization.h"
#include <ctime>

//#include "Klast2.h"


void funcbild(Ptr<BRISK> &detectorBRISK, Mat *img, const int &n) {


	int numberOFmatche = Sum_natural_numbers(n - 1);
	int colMathes      = 0;


	BFMatcher matcher(NORM_HAMMING, true);
	Mat                   * BinaryImg         = new Mat[n];
	vector<KeyPoint>      * keypointsImage    = new vector<KeyPoint>[n];
	Mat                   * descriptorsImage  = new Mat[n];

	vector<KeyPoint>      * keypointsKlaster  = new vector<KeyPoint>[n];           //кластеризаци€   “
	Mat                   * klasterDescripIm  = new Mat[n];                        //кластеризаци€ дескриптора
	Mat                   * klasterDescripIm2 = new Mat[n];

	vector<DMatch>        * matches           = new vector<DMatch>[numberOFmatche];
	Mat                   * imgMatch          = new Mat[n];
	MySort                  mySort;
	MySort                  mySort2;
	Mat                     Gistogramma      = imread("256gist.png");

	
	for (int i = 0; i < n; ++i)
	{
		BinaryImg[i] = CreateBinaryImage(img[i]); // —оздаем бинарные изображени€ к которым будем примен€ть преобразовани€
		imgMatch[i] = img[i].clone();            //создаем копию изображени€ с которой буде работать

		// «апись ключевых точек и дескрипторов дл€ изображений
		detectorBRISK->detectAndCompute(BinaryImg[i], Mat(), keypointsImage[i], descriptorsImage[i]);

		// ороктеровки числа ключевых точек
		if(keypointsImage[i].size()>51)
		sortDescriptorsAndKP(keypointsImage[i], descriptorsImage[i],51);
	}


	// Ќахождение св€зей между точками разных изображений 
	for (int j = 0; j<n - 1; ++j)
		for (int i = 1 + j; i < n; ++i)
			matcher.match(descriptorsImage[j], descriptorsImage[i], matches[colMathes++]);

	cout << "Before:" << endl;
	for (int i = 0; i < n; ++i)
	cout <<i+1 <<" descriptor:"<< descriptorsImage[i].size()<<endl;
	
	/*printGistogram(descriptorsImage[0], FIRSTSTRINPRINT);
	printDescriptorBinary(descriptorsImage[0], FIRSTSTRINPRINT);
	drawGistogram(descriptorsImage[1], FIRSTSTRINPRINT);*/

	//отоброжение ключевых точек на базовых изображени€х.
	for (int i = 0; i < n; ++i) {
		for (int n = 0; n < keypointsImage[i].size(); ++n) {
			circle(imgMatch[i], keypointsImage[i][n].pt, 4, Scalar(0, 0, 0), -1, 8);
			circle(imgMatch[i], keypointsImage[i][n].pt, 2, Scalar(0, 0, 255), -1, 8);
		}
	}


	//кластеризаци€ по расто€нию
	//Clustering_by_coordinates_KP(keypointsImage, keypointsKlaster, n);
	//IzobrajeniechtoKlastereziryem(Gistogramma, keypointsKlaster);
	

	colMathes = 0;
	int a = 0;

	for (int j = 0; j<n - 1; ++j)
		for (int i = 1 + j; i < n; ++i)
		{
		  Draw_matches_upon_Picture(matches[colMathes++], imgMatch[j], imgMatch[i], keypointsImage[j], keypointsImage[i], Scalar(0, 255, 0));
		}
	
	//нахождени€ центра масс кластеризации
	for (int i = 0; i < n; ++i)
	{
		klasterDescripIm[i] = Mat(1, 64, 0);
		for (int j = 0; j < 64; ++j)
		{
			klasterDescripIm[i].row(0).col(j) = Klast::findCenterForKlaster(descriptorsImage[i], j);
		}
	}
	srand(time(0));
	vector<vector<vector<bitset< 8 > > > > klaster = Klast::Convert_to_vector_bits(klasterDescripIm, n);
	mySort.matr= Klast::distributionDescriptors(klaster, Klast::Convert_to_vector_bits(descriptorsImage, n));
	klasterDescripIm= Klast::Convert_to_mas_Mat(klaster);
	cout << "runtime = " << clock() << endl;
	//вывод дескрипторов
	cout << endl << "new klasters:" << endl;
	for (int i = 0; i < n; ++i)
		cout << i + 1 << "-descriptor:  " << klasterDescripIm[i].rows - 1 << endl;

	mySort.sort();
	mySort.show();
	/*//второй круг
	//нахождени€ центра масс кластеризации
	for (int i = 0; i < n; ++i)
	{
		klasterDescripIm2[i] = Mat(1, 64, 0);
		for (int j = 0; j < 64; ++j)
		{
			klasterDescripIm2[i].row(0).col(j) = Klast::findCenterForKlaster(descriptorsImage[i], j);
		}
	}
	vector<vector<vector<bitset< 8 > > > > klaster2 = Klast::Convert_to_vector_bits(klasterDescripIm2, n);
	mySort2.matr = Klast::distributionDescriptors(klaster2, Klast::Convert_to_vector_bits(descriptorsImage, n));
	klasterDescripIm2 = Klast::Convert_to_mas_Mat(klaster2);


	//вывод непон€тно зачем нужных дескрипторов
	cout << endl << "2 round" << endl;
	for (int i = 0; i < n; ++i)
	cout <<i+1<< "-descriptor:  " << klasterDescripIm2[i].rows - 1 << endl;

	mySort2.sort();
	mySort2.show();
	*/
	for (int i = 0; i < n; ++i)
		imshow("matches" + i, imgMatch[i]);

}



