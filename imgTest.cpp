#include"imgTest.h"
#include"myCLib.h"
#include"detectMultiScale.h"
#include"reduceCandidate.h"
#include"opencvLib.h"
#include<iostream>
#include<fstream>
using namespace cv;
using namespace std;

void imgTest(){

	string testPicsPath = "D:\\testPics\\FileRecv\\"; 
	string testPicsFilePath = testPicsPath + "test.txt";

	/*string testPicsPath = "D:\\testPics\\my_video_pics\\";
	string testPicsFilePath = testPicsPath + "my_video_test.txt";*/

	/*string testPicsPath = "D:\\testPics\\INRIA_pos_huge_test\\";
	string testPicsFilePath = testPicsPath + "INRIA_pos_huge_test.txt";*/

	string testPicsPathP = "D:\\testPics\\FileRecv\\";
	string testPicsFilePathP = testPicsPathP + "disparityTest.txt";

	Mat src;  string imgName; string imgNameP;
	ifstream fileTest(testPicsFilePath); if (!fileTest){ printf("file fileTest is NULL");system("pause");}
	ifstream fileTestP(testPicsFilePathP); if (!fileTestP){ printf("file fileTestP is NULL"); system("pause"); }

	while (getline(fileTest, imgName) && !imgName.empty() && 
		   getline(fileTestP, imgNameP) && !imgNameP.empty() ){

		string winName = "imgName";//TODO
		cout << "testing:  " << imgName << endl;
		imgName = testPicsPath + imgName;
		src = imread(imgName, 0); assert(src.data != NULL);
		cout << "size: " << src.rows << " x " << src.cols << endl;
		//double duration = static_cast<double>(getTickCount());
		//resize(src, src, Size(src.cols / 1.0f / src.rows * 240, 240));//TODO
		//resize(src, src, Size(320, 240));
		//cout << "size: " << src.rows << " x " << src.cols << endl;

		string winNameP = "imgNameP"; 
		imgNameP = testPicsPathP + imgNameP;
		Mat srcP = imread(imgNameP, 0); assert(srcP.data != NULL);
		
		processDisparity(srcP);
		
		//namedWindow(winNameP); imshow(winNameP, srcP); waitKey();

		//double duration_detect = static_cast<double>(getTickCount());

		//detectMultiScale(src, src.rows, src.cols, 192, 96, 12, 12, 1.2,false);

		detectMultiClassifier(src, src.rows, src.cols, 128, 64, 8, 8, 1.2);
		

		//duration_detect = static_cast<double>(getTickCount()) - duration_detect;
		//printf("detect duration time(not accurate): %fms\n", 1000.0 * duration_detect / getTickFrequency());

		namedWindow(winName); imshow(winName, src); waitKey(); cout << endl;
	}
	fileTest.close();
	system("pause");
}