#include"setting.h"
#include"myCLib.h"
#include"computeFtr.h"
#include"constructFtrIntHist.h"
#include<iostream>
#include<fstream>
#include"opencvLib.h"
using namespace std;
using namespace cv;


#define TRAIN_IMG_R (96u)
#define TRAIN_IMG_C  (128u)
#define POS_SAM_NUM  0   
#define NEG_SAM_NUM  0
#define HARD_SAM_NUM 21848

const string posTrainPath = "D:\\trainPics\\pos\\";
const string negTrainPath = "D:\\trainPics\\neg\\";
const string hardTrainPath = "D:\\trainPics\\BIKE_AND_EBIKE\\hard_96x128\\";
const string posTrainTxt = posTrainPath + "pos.txt";
const string negTrainTxt = negTrainPath + "neg.txt";
const string hardTrainTxt = hardTrainPath + "hard.txt";

const char ftrPosFileName[256] = "ftrPos192x96_acf_2416p_5000n.txt";
const char ftrNegFileName[256] = "ftrNeg192x96_acf_2416p_5000n.txt";
const char ftrHardFileName[256] = "ftrHard_96x128_acf_bm_21848h.txt";


static int appendAdaboostData2File(const char * filename, const float * arrayData, int dimension, int num);
static void checkParamSampleTrain();

void sampleTrain(){

	checkParamSampleTrain();
	string ImgName;
	ifstream finPos(posTrainTxt); if (!finPos && POS_SAM_NUM > 0){ printf("file finPos is NULL"); system("pause"); exit(1); }
	ifstream finNeg(negTrainTxt);  if (!finNeg && NEG_SAM_NUM > 0){ printf("file finNegt is NULL"); system("pause"); exit(1); }
	ifstream finHard(hardTrainTxt); if (!finHard && HARD_SAM_NUM > 0){ printf("file finHard is NULL"); system("pause"); exit(1); }
	//static unsigned char pImg[TRAIN_IMG_R * TRAIN_IMG_C] = {0};
	const int TRAIN_CHN_FTR_DIM = getFtrDim(TRAIN_IMG_R, TRAIN_IMG_C);
	static float *pFtrForTrain = new float[TRAIN_CHN_FTR_DIM];

	if (!remove(ftrPosFileName)) printf("remove %s file success!\n", ftrPosFileName);
	if (!remove(ftrNegFileName)) printf("remove %s file success!\n", ftrNegFileName);
	if (!remove(ftrHardFileName)) printf("remove %s file success!\n", ftrHardFileName);
    //POS
	for (int num = 0; num < POS_SAM_NUM && getline(finPos, ImgName); num++){
		cout << "Processing£º " << ImgName << endl;
		ImgName = posTrainPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		//src = src(Rect(16, 16, 64, 128));
		if (src.rows != TRAIN_IMG_R || src.cols != TRAIN_IMG_C)
			resize(src, src, Size(TRAIN_IMG_C, TRAIN_IMG_R));//TODO
		assert(src.rows == TRAIN_IMG_R && src.cols == TRAIN_IMG_C);
		//assert(src.rows*src.cols == sizeof(pImg) / sizeof(pImg[0]));

		//Mat2ImgPointer(src, pImg);
		//constructFtrIntHist(pImg, TRAIN_IMG_R, TRAIN_IMG_C);
		constructFtrIntHist(src);
		rect r = { 0, 0, TRAIN_IMG_C, TRAIN_IMG_R };
		computeFtr(TRAIN_IMG_R, TRAIN_IMG_C, r, pFtrForTrain, TRAIN_CHN_FTR_DIM);

		appendAdaboostData2File(ftrPosFileName, pFtrForTrain, TRAIN_CHN_FTR_DIM, 1);
	}

	//NEG
	for (int num = 0; num < NEG_SAM_NUM && getline(finNeg, ImgName); num++){
		cout << "Processing£º " << ImgName << endl;
		ImgName = negTrainPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		if (src.rows != TRAIN_IMG_R || src.cols != TRAIN_IMG_C)
			resize(src, src, Size(TRAIN_IMG_C, TRAIN_IMG_R));//TODO
		assert(src.rows == TRAIN_IMG_R&&src.cols == TRAIN_IMG_C);
        //assert(src.rows*src.cols == sizeof(pImg) / sizeof(pImg[0]));

		//Mat2ImgPointer(src, pImg);
		//constructFtrIntHist(pImg, TRAIN_IMG_R, TRAIN_IMG_C);
		constructFtrIntHist(src);
		rect r = { 0, 0, TRAIN_IMG_C, TRAIN_IMG_R };
		computeFtr(TRAIN_IMG_R, TRAIN_IMG_C, r, pFtrForTrain, TRAIN_CHN_FTR_DIM);

		appendAdaboostData2File(ftrNegFileName, pFtrForTrain, TRAIN_CHN_FTR_DIM, 1);

	}

	//HARD
	for (int num = 0; num < HARD_SAM_NUM && getline(finHard, ImgName); num++){
		cout << "Processing£º " << ImgName << endl;
		ImgName = hardTrainPath + ImgName;
		Mat src = imread(ImgName, 0); assert(src.data != NULL);
		if (src.rows != TRAIN_IMG_R || src.cols != TRAIN_IMG_C)
			resize(src, src, Size(TRAIN_IMG_C, TRAIN_IMG_R));//TODO
		assert(src.rows == TRAIN_IMG_R&&src.cols == TRAIN_IMG_C);
        //assert(src.rows*src.cols == sizeof(pImg) / sizeof(pImg[0]));

		//Mat2ImgPointer(src, pImg);
		constructFtrIntHist(src);
		//constructFtrIntHist(pImg, TRAIN_IMG_R, TRAIN_IMG_C);
		rect r = { 0, 0, TRAIN_IMG_C, TRAIN_IMG_R };
		computeFtr(TRAIN_IMG_R, TRAIN_IMG_C, r, pFtrForTrain, TRAIN_CHN_FTR_DIM);

		appendAdaboostData2File(ftrHardFileName, pFtrForTrain, TRAIN_CHN_FTR_DIM, 1);

	}
	delete[] pFtrForTrain;

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
	printf("sampleTrain() PARAMETER:\n");
	printf("TRAIN_IMG_SIZE<rows x cols>: %d x %d\n", TRAIN_IMG_R, TRAIN_IMG_C);
	printf("FEATURE DIM: %d\n", getFtrDim(TRAIN_IMG_R, TRAIN_IMG_C));
	printf("POS_SAM_NUM: %d\n", POS_SAM_NUM);
	printf("NEG_SAM_NUM: %d\n", NEG_SAM_NUM);
	printf("HAED_SAM_NUM: %d\n", HARD_SAM_NUM);
	cout << "posTrainTxt: " << posTrainTxt << endl;
	cout << "negTrainTxt: " << negTrainTxt << endl;
	cout << "hardTrainTxt: " << hardTrainTxt << endl;
	
	printf("THE NAME OF FILE SAVE POS FEATURE : %s\n", ftrPosFileName);
	printf("THE NAME OF FILE SAVE NEG FEATURE: %s\n", ftrNegFileName);
	printf("THE NAME OF FILE SAVE HARD FEATURE: %s\n\n", ftrHardFileName);

	//Warning
	printf("\nWARNING: %s and %s and %s will be deleted if coninue running\n",
		ftrPosFileName, ftrNegFileName, ftrHardFileName);

	printf("\nDo you want to continue: y/n?\n");
	char ch = 'c';  
	while (cin >> ch){
		if (tolower(ch) == 'n')   exit(1);
		else if (tolower(ch) == 'y') break;
		else continue;
	}
}