#include "Work_With_Brisk.h"

//параметры детектора бриск
static int  	thresh       = 200;
static int  	octaves      = 1;
static float  	patternScale = 3.8f;




int main() {

	Mat *img       = new Mat[number_of_image];
	char im[]      = "DataBaseImage/im0.jpg";
	Mat imgg;
	for (int i = 0; i < number_of_image; ++i) {
		im[16]  = char(i + 48+1);
		img[i] = imread(im);
	}


	Ptr<BRISK> detectorBRISK = BRISK::create(thresh, octaves, patternScale);
	cout << "START " << endl;

	funcbild(detectorBRISK,img, number_of_image);

	waitKey(0);
	return 0;
}

