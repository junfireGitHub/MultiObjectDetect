#include"imgTest.h"
#include"myCLib.h"
#include"detectMultiScale.h"
#include"reduceCandidate.h"
#include"opencvLib.h"
#include<iostream>
#include<fstream>
using namespace cv;
using namespace std;

void imgTestDp(){ /*加入视差后在图片上进行检测*/

	string testPicsPath = "D:\\testPics\\FileRecv\\"; 
	string testPicsFilePath = testPicsPath + "test.txt";

	string testPicsPathP = "D:\\testPics\\FileRecv\\";
	string testPicsFilePathP = testPicsPathP + "disparityTest.txt";

	Mat src;  string imgName, imgNameP;
	ifstream fileTest(testPicsFilePath); if (!fileTest){ printf("file fileTest is NULL");system("pause");}
	ifstream fileTestP(testPicsFilePathP); if (!fileTestP){ printf("file fileTestP is NULL"); system("pause"); }

	while (getline(fileTest, imgName) && !imgName.empty() && 
		   getline(fileTestP, imgNameP) && !imgNameP.empty() ){
		
		cout << "testing:  " << imgName << endl;
		imgName = testPicsPath + imgName;
		src = imread(imgName); assert(src.data != NULL);
		cout << "size: " << src.rows << " x " << src.cols << endl;

		imgNameP = testPicsPathP + imgNameP;
		Mat srcP = imread(imgNameP, 0); assert(srcP.data != NULL);
		
		processDisparity(srcP);
		
		//namedWindow(winNameP); imshow(winNameP, srcP); waitKey();
		//double duration_detect = static_cast<double>(getTickCount());
		//detectMultiScale(src, src.rows, src.cols, 192, 96, 12, 12, 1.2,false);

		DetectOpt detectOpt = { 128, 64, 8, 8, 1.2, true/*isUseDp*/, true/*isPostPro*/,false/*isGetHard*/ };
		detectMultiClassifier(src, src.rows, src.cols, detectOpt);
		
		//duration_detect = static_cast<double>(getTickCount()) - duration_detect;
		//printf("detect duration time(not accurate): %fms\n", 1000.0 * duration_detect / getTickFrequency());
		string winName = "imgName";
		namedWindow(winName); imshow(winName, src); waitKey(); cout << endl;
	}
	fileTest.close();
	system("pause");  exit(0);
}

void imgTest(){/*在图片上进行检测*/
	string testPicsPath, testPicsFilePath;
	//testPicsPath = "D:\\MyProject\\MultiObjectDetect-v3.5\\testFromOurVideo\\";
	//testPicsFilePath = testPicsPath + "test.txt";
	//testPicsPath = "D:\\MyProject\\MultiObjectDetect - v3.5\\testPics\\";
	//testPicsFilePath = testPicsPath + "test.txt";

	//testPicsPath = "D:\\MyProject\\MultiObjectDetect - v3.5\\bike_test\\";
    //testPicsFilePath = testPicsPath + "test.txt";

	testPicsPath = "D:\\testPics\\FileRecv\\";
	testPicsFilePath = testPicsPath + "test.txt";
	//testPicsPath = "D:\\testPics\\my_video_pics\\";
	//testPicsFilePath = testPicsPath + "my_video_test.txt";
	//testPicsPath = "D:\\testPics\\INRIA_pos_huge_test\\";
	//testPicsFilePath = testPicsPath + "INRIA_pos_huge_test.txt";
	
	ifstream fileTest(testPicsFilePath); if (!fileTest){ printf("file fileTest is NULL"); system("pause"); exit(1);}

	Mat src;  string imgName;
	while (getline(fileTest, imgName) && !imgName.empty() ){	
		cout << "testing:  " << imgName << endl;
		imgName = testPicsPath + imgName;
		src = imread(imgName); assert(src.data != NULL);
		//resize(src, src, Size(320, 240));
		cout << "size: " << src.rows << " x " << src.cols << endl;
		
		double duration_detect = static_cast<double>(getTickCount());

		DetectOpt detectOpt = {96, 128, 8, 8, 1.2, false/*isUseDp*/, false/*isPostPro*/, false/*isGetHard*/};
		detectMultiScale(src, src.rows, src.cols, detectOpt);

		duration_detect = static_cast<double>(getTickCount()) - duration_detect;
		printf("detect duration time(not accurate): %fms\n", 1000.0 * duration_detect / getTickFrequency());

		string winName = "imgName";
		namedWindow(winName); imshow(winName, src); waitKey(); cout << endl;
	}
	fileTest.close();
	system("pause"); exit(0);
}