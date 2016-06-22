#include"setting.h"
#include"myCLib.h"
#include"computeFtr.h"
#include"constructFtrIntHist.h"
#include"classify.h"
#include<iostream>
#include<fstream>
#include"opencvLib.h"
using namespace std;
using namespace cv;


#define TEST_IMG_R (128u)
#define TEST_IMG_C (64u)
#define POS_SAM_NUM 1126    
#define NEG_SAM_NUM 9000

static string posTestPath = "D:\\testPics\\INRIA_pos_134x70_test\\";
static string negTestPath = "D:\\testPics\\INRIA_neg_128x64_test\\";
static string posNameTxt = posTestPath + "INRIA_pos_134x70_test.txt";
static string negNameTxt = negTestPath + "INRIA_neg_128x64_test.txt";

static void checkParamSampleTest();

void sampleTest(){
	/*在测试集上测试效果，主要通过漏检率和误检率来判断*/
	checkParamSampleTest();
	ifstream finPos(posNameTxt); if (!finPos){ printf("file fileTest is NULL"); system("pause");}
	ifstream finNeg(negNameTxt); if (!finNeg){ printf("file fileTest is NULL"); system("pause");}

	string ImgName;
	static const int TEST_CHN_FTR_DIM = getFtrDim(TEST_IMG_R, TEST_IMG_C);
	static float *pFtrForTest = new float[TEST_CHN_FTR_DIM];

	//POS
	int fnCount = 0;
	for (int num = 0; num < POS_SAM_NUM && getline(finPos, ImgName); num++){
		cout << "Processing：" << ImgName << endl;
		ImgName = posTestPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		src = src(Rect(3, 3, 64, 128));
		if (src.rows != TEST_IMG_R || src.cols != TEST_IMG_C)
			resize(src, src, Size(TEST_IMG_C, TEST_IMG_R));//TODO
		assert(src.rows == TEST_IMG_R && src.cols == TEST_IMG_C);

		constructFtrIntHist(src);
		rect r = { 0, 0, TEST_IMG_C, TEST_IMG_R };
		computeFtr(TEST_IMG_R, TEST_IMG_C, r, pFtrForTest, TEST_CHN_FTR_DIM);

		float score = 0;
		//adaPredictPerson(pFtrForTest, 1, TEST_CHN_FTR_DIM, &score,0);
		if (score < 0) fnCount++;

	}

	//NEG
	int fpCount = 0;
	for (int num = 0; num < NEG_SAM_NUM && getline(finNeg, ImgName); num++){

		cout << "Processing：" << ImgName << endl;
		ImgName = negTestPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		if (src.rows != TEST_IMG_R || src.cols != TEST_IMG_C)
			resize(src, src, Size(TEST_IMG_C, TEST_IMG_R));//TODO
		assert(src.rows == TEST_IMG_R&&src.cols == TEST_IMG_C);

		constructFtrIntHist(src);
		rect r = { 0, 0, TEST_IMG_C, TEST_IMG_R };
		computeFtr(TEST_IMG_R, TEST_IMG_C, r, pFtrForTest, TEST_CHN_FTR_DIM);

		float score = 0;
		//adaPredictPerson(pFtrForTest, 1, TEST_CHN_FTR_DIM, &score,0);
		if (score > 0) fpCount++;
	}
	delete[] pFtrForTest;

	printf("fp: %f\n", 1.0*fpCount / NEG_SAM_NUM);
	printf("fn: %f\n", 1.0*fnCount / POS_SAM_NUM);
	system("pause"); exit(0);
}


static void checkParamSampleTest(){

	//Param
	printf("sampleTest() PARAMETER\n");
	printf("TEST_IMG_SIZE<rows x cols>: %d x %d\n", TEST_IMG_R, TEST_IMG_C);
	printf("FEATURE DIM: %d\n", getFtrDim(TEST_IMG_R, TEST_IMG_C));
	printf("POS_SAM_NUM: %d\n", POS_SAM_NUM);
	printf("NEG_SAM_NUM: %d\n", NEG_SAM_NUM);
	cout << "posTrainTxt: " << posNameTxt << endl;
	cout << "negTrainTxt: " << negNameTxt << endl;

	printf("\nDo you want to continue: y/n?\n");
	char ch = 'c';
	while (cin >> ch){
		if (tolower(ch) == 'n')   exit(1);
		else if (tolower(ch) == 'y') break;
		else continue;
	}
}