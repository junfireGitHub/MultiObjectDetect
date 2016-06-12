#include"setting.h"
#include"myCLib.h"
#include"detectMultiScale.h"
#include<iostream>
#include<fstream>
#include"opencvLib.h"
using namespace std;
using namespace cv;

static string negForHardPath = "D:\\trainPics\\BIKE_AND_EBIKE\\negForHard\\";
static string negForHardNameTxt = negForHardPath + "negForHard.txt";
static int hardIdStartNum = 1;
char saveHardPath[256] = "D:\\trainPics\\BIKE_AND_EBIKE\\hard_96x128\\";

#define HARD_IMG_ROWS (96u)
#define HARD_IMG_COLS (128u)
#define WIN_STEP_R (16u)
#define WIN_STEP_C (16u)
#define SCALE (1.5f)

static void checkParaGetNegFromHard();

void getHardFromNeg(){

	checkParaGetNegFromHard();
	ifstream fileNeg(negForHardNameTxt); 
	if (!fileNeg) { cout<<"fileNeg don't exist,potion:getHardFromNeg"<<endl; system("pause"); exit(1); }
	string imgName;
//	unsigned char *pImg;
	for (int num = 0; getline(fileNeg, imgName); num++){
		cout << "Processing： " << imgName << endl;
		imgName = negForHardPath + imgName;
		Mat src = imread(imgName, 0); assert(src.data != NULL);
		DetectOpt detectOpt = {HARD_IMG_ROWS, HARD_IMG_COLS, WIN_STEP_R, WIN_STEP_C, SCALE, 
			false/*isUseDp*/, false/*isPostPro*/, true/*isGetHard*/ };
		detectMultiScale(src, src.rows, src.cols, detectOpt);
	}
	cout << "congragulation, program run successfully" << endl;
	system("pause"); exit(0);
}

void saveHardPics(Mat &src, vector<Rect> found){
	for (int i = 0; i < found.size(); i++){
		char saveName[256]; char hardPicName[100];
		Rect r = found[i];
		Mat hardExampleImg = src(r);//从原图上截取矩形框大小的图片  
		resize(hardExampleImg, hardExampleImg, Size(HARD_IMG_COLS, HARD_IMG_ROWS));//将剪裁出来的图片缩放为64*128大小 
		//sprintf(saveName, "D:\\trainPics\\hardForChnsFtr\\192x96\\hard%d.jpg", hardIdStartNum++);//生成hard example图片的文件名 
		sprintf(hardPicName, "hard%d.jpg", hardIdStartNum++);//生成hard example图片的文件名  
		strcpy(saveName, saveHardPath);
		strcat(saveName,hardPicName);
		assert(strlen(saveName) < 256);
		imwrite(saveName, hardExampleImg);//保存文件
	}
}

static void checkParaGetNegFromHard(){

	//param
	printf("geHardFromNeg() PARAMETER\n");
	printf("hard pics size<rows x cols> :  %d x %d\n", HARD_IMG_ROWS, HARD_IMG_COLS);
	cout << "negForHardNameTxt: " << negForHardNameTxt << endl;
	printf("hard pics index start from : %d\n", hardIdStartNum);	
	printf("path of hard pics saved:  %s\n", saveHardPath);
	printf("detectMultiScale(winR winC winStepR winStepC scale):(%d %d %d %d %f)\n", HARD_IMG_ROWS, HARD_IMG_COLS, WIN_STEP_R,WIN_STEP_C,SCALE);
	//printf("!!!!!!!!!!!!is the zi in detectMultiScale correct? !!!!!!!!!!!!!\n");

	//Warning
	printf("\nWARNING: Before continue running,please make sure there is no pics named hardx.jpg in %s  , otherwise they'll be overwritted!\n", saveHardPath);

	printf("\nDo you want to continue: y/n?");
	char ch = 'c';
	while (cin >> ch){
		if (tolower(ch) == 'n')   exit(1);
		else if (tolower(ch) == 'y') break;
		else continue;
	}
}