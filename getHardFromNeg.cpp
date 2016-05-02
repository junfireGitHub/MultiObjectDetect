#include<iostream>
#include<fstream>
#include"opencvLib.h"
using namespace std;
using namespace cv;

#include"setting.h"
#include"myCLib.h"
#include"detectMultiScale.h"

static void checkParaGetNegFromHard();

static string negForHardPath = "D:\\trainPics\\negPicsForHardExamples\\";
static string negForHardNameTxt = negForHardPath + "negForHard.txt";
static int hardExampleCount = 1;
char saveHardPath[256] = "D:\\trainPics\\hardForChnsFtr\\192x96_acf\\";

//#define HARD_IMG_ROWS (TRAIN_IMG_R)
//#define HARD_IMG_COLS (TRAIN_IMG_C)

#define HARD_IMG_ROWS (128u)
#define HARD_IMG_COLS (64u)
#define WIN_STEP_R (32u)
#define WIN_STEP_C (16u)
#define SCALE (1.3f)

void getHardFromNeg(){

	checkParaGetNegFromHard();
	ifstream fileNeg(negForHardNameTxt);
	string imgName;
	unsigned char *pImg;
	for (int num = 0; getline(fileNeg, imgName); num++){
		cout << "Processing： " << imgName << endl;
		imgName = negForHardPath + imgName;
		Mat src = imread(imgName, 0); assert(src.data != NULL);
		detectMultiScale(src, src.rows, src.cols, 
			HARD_IMG_ROWS, HARD_IMG_COLS, WIN_STEP_R, WIN_STEP_C, SCALE,true);
	}

	system("pause"); exit(0);
}

void saveHardPics(Mat &src, vector<Rect> found){
	for (int i = 0; i < found.size(); i++){
		char saveName[256]; char hardPicName[100];
		Rect r = found[i];
		Mat hardExampleImg = src(r);//从原图上截取矩形框大小的图片  
		resize(hardExampleImg, hardExampleImg, Size(HARD_IMG_COLS, HARD_IMG_ROWS));//将剪裁出来的图片缩放为64*128大小 
		//sprintf(saveName, "D:\\trainPics\\hardForChnsFtr\\192x96\\hard%d.jpg", hardExampleCount++);//生成hard example图片的文件名 
		sprintf(hardPicName, "hard%d.jpg", hardExampleCount++);//生成hard example图片的文件名  
		strcpy(saveName, saveHardPath);
		strcat(saveName,hardPicName);
		assert(strlen(saveName) < 256);
		imwrite(saveName, hardExampleImg);//保存文件
	}
}

static void checkParaGetNegFromHard(){

	//param
	printf("PARAMETER\n");
	cout << "negForHardNameTxt: " << negForHardNameTxt << endl;
	printf("hard pics index start from : %d\n", hardExampleCount);
	printf("hard pics size<rows x cols> :  %d x %d\n", HARD_IMG_ROWS, HARD_IMG_COLS);
	printf("path of hard pics saved:  %s\n", saveHardPath);
	printf("detectMultiScale(winR winC winStepR winStepC scale):(%d %d %d %d %f)\n", HARD_IMG_ROWS, HARD_IMG_COLS, WIN_STEP_R,WIN_STEP_C,SCALE);

	//Warning
	printf("\nWARNING: Before continue running,please make sure there is no pics named hardx.jpg in %s  , otherwise they'll be overwritted!\n", saveHardPath);

	printf("\nDo you want to continue: y/n?");
	char ch = 'c';  scanf("%c", &ch);
	if (ch != 'y' && ch != 'Y'){
		exit(1);
	}
}