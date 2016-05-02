#include<iostream>
#include<fstream>
#include"opencvLib.h"
using namespace std;
using namespace cv;

#include"setting.h"
#include"myCLib.h"
#include"computeChnFtr.h"
#include"constructFeatureIntHist.h"
#include"AdaboostPredict.h"


#define TEST_IMG_R (128u)
#define TEST_IMG_C (64u)
const int TEST_CHN_FTR_DIM = (TEST_IMG_R / SHRINK)*(TEST_IMG_C / SHRINK) + \
							 (TEST_IMG_R / SHRINK)*(TEST_IMG_C / SHRINK) + \
                             (TEST_IMG_R / BIN_SIZE)* (TEST_IMG_C / BIN_SIZE)*NUM_ORIENT;

static float chnFtrForTest[TEST_CHN_FTR_DIM] = { .0f };
static void checkParamSampleTest();

static string posTestPath = "D:\\testPics\\INRIA_pos_134x70_test\\";
static string negTestPath = "D:\\testPics\\INRIA_neg_128x64_test\\";
static string posNameTxt = posTestPath + "INRIA_pos_134x70_test.txt";
static string negNameTxt = negTestPath + "INRIA_neg_128x64_test.txt";
static string loadChnRectFileName = "savedChnsRect.txt";

#define POS_SAM_NUM 1126    
#define NEG_SAM_NUM 9000    

static unsigned char pImg[TEST_IMG_R * TEST_IMG_C];

void sampleTest(){

	checkParamSampleTest();
	/*using the test set to test the quality of classification*/
	string ImgName;
	ifstream finPos(posNameTxt); if (!finPos){ printf("file fileTest is NULL"); system("pause");}
	ifstream finNeg(negNameTxt); if (!finNeg){ printf("file fileTest is NULL"); system("pause");}

	//POS
	int fnCount = 0;
	for (int num = 0; num < POS_SAM_NUM && getline(finPos, ImgName); num++){
		cout << "Processing£º" << ImgName << endl;
		ImgName = posTestPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		src = src(Rect(3, 3, 64, 128));
		if (src.rows != TEST_IMG_R || src.cols != TEST_IMG_C)
			resize(src, src, Size(TEST_IMG_C, TEST_IMG_R));//TODO
		assert(src.rows == TEST_IMG_R && src.cols == TEST_IMG_C);

		assert(src.rows*src.cols == sizeof(pImg) / sizeof(pImg[0]));
		Mat2ImgPointer(src, pImg);
		constructFeatureIntHist(pImg, TEST_IMG_R, TEST_IMG_C);
		rect r = { 0, 0, TEST_IMG_C, TEST_IMG_R };
		computeChnFtr(pImg, TEST_IMG_R, TEST_IMG_C, r, chnFtrForTest, TEST_CHN_FTR_DIM);

		float score = 0;
		adaboostPredict(chnFtrForTest, 1, TEST_CHN_FTR_DIM, &score,0);
		if (score < 0) fnCount++;

	}

	//NEG
	int fpCount = 0;
	for (int num = 0; num < NEG_SAM_NUM && getline(finNeg, ImgName); num++){

		cout << "Processing£º" << ImgName << endl;
		ImgName = negTestPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		if (src.rows != TEST_IMG_R || src.cols != TEST_IMG_C)
			resize(src, src, Size(TEST_IMG_C, TEST_IMG_R));//TODO
		assert(src.rows == TEST_IMG_R&&src.cols == TEST_IMG_C);

		assert(src.rows*src.cols == sizeof(pImg) / sizeof(pImg[0]));
		Mat2ImgPointer(src, pImg);
		constructFeatureIntHist(pImg, TEST_IMG_R, TEST_IMG_C);
		rect r = { 0, 0, TEST_IMG_C, TEST_IMG_R };
		computeChnFtr(pImg, TEST_IMG_R, TEST_IMG_C, r, chnFtrForTest, TEST_CHN_FTR_DIM);

		float score = 0;
		adaboostPredict(chnFtrForTest, 1, TEST_CHN_FTR_DIM, &score,0);
		if (score > 0) fpCount++;
	}

	printf("fp: %f\n", 1.0*fpCount / NEG_SAM_NUM);
	printf("fn: %f\n", 1.0*fnCount / POS_SAM_NUM);
	system("pause"); exit(0);
}


static void checkParamSampleTest(){

	//Param
	printf("PARAMETER\n");
	cout << "posTrainTxt: " << posNameTxt << endl;
	cout << "negTrainTxt: " << negNameTxt << endl;
	printf("TEST_IMG_SIZE<rows x cols>: %d x %d\n", TEST_IMG_R, TEST_IMG_C);
	printf("POS_SAM_NUM: %d\n", POS_SAM_NUM);
	printf("NEG_SAM_NUM: %d\n", NEG_SAM_NUM);
	printf("FEATURE DIM: %d\n", TEST_CHN_FTR_DIM);

	printf("\nDo you want to continue: y/n?\n");
	char ch = 'c';  scanf("%c", &ch);
	if (ch != 'y' && ch != 'Y'){
		exit(1);
	}

}