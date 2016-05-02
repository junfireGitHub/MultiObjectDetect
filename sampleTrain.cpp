#include<iostream>
#include<fstream>
#include"opencvLib.h"
using namespace std;
using namespace cv;

#include"setting.h"
#include"myCLib.h"
#include"computeChnFtr.h"
#include"constructFeatureIntHist.h"


static int appendAdaboostData2File(const char * filename, const float * arrayData, int dimension, int num);
static void checkParamSampleTrain();

string posTrainPath = "D:\\trainPics\\pos\\";
string negTrainPath = "D:\\trainPics\\neg\\";
string hardTrainPath = "D:\\trainPics\\hardForChnsFtr\\192x96_acf\\";
string posTrainTxt = posTrainPath + "pos.txt";
string negTrainTxt = negTrainPath + "neg.txt";
string hardTrainTxt = hardTrainPath + "hard.txt";

#define TRAIN_IMG_R (128u)
#define TRAIN_IMG_C  (64u)
#define   POS_SAM_NUM  0   
#define   NEG_SAM_NUM  0
#define   HARD_SAM_NUM 1053
static const int TRAIN_CHN_FTR_DIM = (TRAIN_IMG_R / SHRINK)*(TRAIN_IMG_C / SHRINK) + \
									 (TRAIN_IMG_R / SHRINK)*(TRAIN_IMG_C / SHRINK) + \
									 (TRAIN_IMG_R / BIN_SIZE)* (TRAIN_IMG_C / BIN_SIZE)*NUM_ORIENT;


static char ftrPosFileName[256] = "ftrPos192x96_acf_2416p_5000n.txt";
static char ftrNegFileName[256] = "ftrNeg192x96_acf_2416p_5000n.txt";
static char ftrHardFileName[256] = "ftrHard192x96_acf_2416p_5000n_1053h.txt";

void sampleTrain(){

	checkParamSampleTrain();
	string ImgName;
	ifstream finPos(posTrainTxt); if (!finPos){ printf("file finPos is NULL"); system("pause"); }
	ifstream finNeg(negTrainTxt);  if (!finNeg){ printf("file finNegt is NULL"); system("pause"); }
	ifstream finHard(hardTrainTxt); if (!finHard){ printf("file finHard is NULL"); system("pause"); }
	static unsigned char pImg[TRAIN_IMG_R * TRAIN_IMG_C] = {0};
	static float chnFtrForTrain[TRAIN_CHN_FTR_DIM] = { .0f };

	if (!remove(ftrPosFileName)) printf("remove %s file success!\n", ftrPosFileName);
	if (!remove(ftrNegFileName)) printf("remove %s file success!\n", ftrNegFileName);
	if (!remove(ftrHardFileName)) printf("remove %s file success!\n", ftrHardFileName);
    //POS
	for (int num = 0; num < POS_SAM_NUM && getline(finPos, ImgName); num++){

		cout << "Processing£º " << ImgName << endl;
		ImgName = posTrainPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		src = src(Rect(16, 16, 64, 128));
		if (src.rows != TRAIN_IMG_R || src.cols != TRAIN_IMG_C)
			resize(src, src, Size(TRAIN_IMG_C, TRAIN_IMG_R));//TODO
		assert(src.rows == TRAIN_IMG_R && src.cols == TRAIN_IMG_C);
		assert(src.rows*src.cols == sizeof(pImg) / sizeof(pImg[0]));

		Mat2ImgPointer(src, pImg);
		constructFeatureIntHist(pImg, TRAIN_IMG_R, TRAIN_IMG_C);
		rect r = { 0, 0, TRAIN_IMG_C, TRAIN_IMG_R };
		computeChnFtr(pImg, TRAIN_IMG_R, TRAIN_IMG_C, r, chnFtrForTrain, TRAIN_CHN_FTR_DIM);

		appendAdaboostData2File(ftrPosFileName, chnFtrForTrain, TRAIN_CHN_FTR_DIM, 1);

	}

	//NEG
	for (int num = 0; num < NEG_SAM_NUM && getline(finNeg, ImgName); num++){

		cout << "Processing£º " << ImgName << endl;
		ImgName = negTrainPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		if (src.rows != TRAIN_IMG_R || src.cols != TRAIN_IMG_C)
			resize(src, src, Size(TRAIN_IMG_C, TRAIN_IMG_R));//TODO
		assert(src.rows == TRAIN_IMG_R&&src.cols == TRAIN_IMG_C);
        assert(src.rows*src.cols == sizeof(pImg) / sizeof(pImg[0]));

		Mat2ImgPointer(src, pImg);
		constructFeatureIntHist(pImg, TRAIN_IMG_R, TRAIN_IMG_C);
		rect r = { 0, 0, TRAIN_IMG_C, TRAIN_IMG_R };
		computeChnFtr(pImg, TRAIN_IMG_R, TRAIN_IMG_C, r, chnFtrForTrain, TRAIN_CHN_FTR_DIM);

		appendAdaboostData2File(ftrNegFileName, chnFtrForTrain, TRAIN_CHN_FTR_DIM, 1);

	}

	//HARD
	for (int num = 0; num < HARD_SAM_NUM && getline(finHard, ImgName); num++){

		cout << "Processing£º " << ImgName << endl;
		ImgName = hardTrainPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		if (src.rows != TRAIN_IMG_R || src.cols != TRAIN_IMG_C)
			resize(src, src, Size(TRAIN_IMG_C, TRAIN_IMG_R));//TODO
		assert(src.rows == TRAIN_IMG_R&&src.cols == TRAIN_IMG_C);
        assert(src.rows*src.cols == sizeof(pImg) / sizeof(pImg[0]));

		Mat2ImgPointer(src, pImg);
		constructFeatureIntHist(pImg, TRAIN_IMG_R, TRAIN_IMG_C);
		rect r = { 0, 0, TRAIN_IMG_C, TRAIN_IMG_R };
		computeChnFtr(pImg, TRAIN_IMG_R, TRAIN_IMG_C, r, chnFtrForTrain, TRAIN_CHN_FTR_DIM);

		appendAdaboostData2File(ftrHardFileName, chnFtrForTrain, TRAIN_CHN_FTR_DIM, 1);

	}

	printf("posNum: %d\n", POS_SAM_NUM);
	printf("negNum+HardNum: %d\n", NEG_SAM_NUM + HARD_SAM_NUM);
	printf("congragulation, train complete!\n");
	system("pause"); exit(0);
}

static int appendAdaboostData2File(const char * filename, const float * arrayData, int dimension, int num){

	FILE *fp = fopen(filename, "a");
	if (fp == NULL) return -1;

	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			fprintf(fp, "%f ", arrayData[dimension * i + j]);
		}
		fprintf(fp, "\n");
	}
	if (ferror(fp) != 0 || fclose(fp) != 0) return -1;
	else return 0;
}

static void checkParamSampleTrain(){

	//Param
	printf("PARAMETER:\n");
	cout << "posTrainTxt: " << posTrainTxt << endl;
	cout << "negTrainTxt: " << negTrainTxt << endl;
	cout << "hardTrainTxt: " << hardTrainTxt << endl;
	printf("TRAIN_IMG_SIZE<rows x cols>: %d x %d\n", TRAIN_IMG_R, TRAIN_IMG_C);
	printf("POS_SAM_NUM: %d\n", POS_SAM_NUM);
	printf("NEG_SAM_NUM: %d\n", NEG_SAM_NUM);
	printf("HAED_SAM_NUM: %d\n", HARD_SAM_NUM);
	printf("FEATURE DIM: %d\n", TRAIN_CHN_FTR_DIM);
	printf("THE NAME OF FILE SAVE POS FEATURE : %s\n", ftrPosFileName);
	printf("THE NAME OF FILE SAVE NEG FEATURE: %s\n", ftrNegFileName);
	printf("THE NAME OF FILE SAVE HARD FEATURE: %s\n\n", ftrHardFileName);

	//Warning
	printf("\nWARNING: %s and %s and %s will be deleted if coninue running\n",
		ftrPosFileName, ftrNegFileName, ftrHardFileName);

	printf("\nDo you want to continue: y/n?\n");
	char ch = 'c';  scanf("%c", &ch);
	if (ch != 'y' && ch != 'Y'){
		exit(1);
	}
}