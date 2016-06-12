#include"reduceCandidate.h"
#include"opencvLib.h"
#include"myCLib.h"
using namespace cv;

static int intHist[MAX_IN_IMG_R + 1][MAX_IN_IMG_C + 1] = { 0 };
static void constructIntHist(const UInt8 *pImg, int row, int col, int threshold);

void processDisparity(const Mat &mat){

	cv::Mat src = mat.clone();
	if (src.rows > MAX_IN_IMG_R || src.cols > MAX_IN_IMG_C){
		if (src.rows > src.cols)
			cv::resize(src, src, cv::Size(floor(MAX_IN_IMG_R / src.rows*src.cols), MAX_IN_IMG_R));
		else
			cv::resize(src, src, cv::Size(MAX_IN_IMG_C, floor(MAX_IN_IMG_C / src.cols*src.rows)));
	}
	assert(src.rows <= MAX_IN_IMG_R && src.cols <= MAX_IN_IMG_C);

	const int THRESHOLD = 16;
	const int ROW = src.rows;
	const int COL = src.cols;
	
	static UInt8 pImg[MAX_IN_IMG_SIZE] = { 0 };
	Mat2ImgPointer(src, pImg);

	//double duration = static_cast<double>(getTickCount());
	constructIntHist(pImg, ROW, COL,THRESHOLD);
	//duration = static_cast<double>(getTickCount()) - duration;
	//printf("processDisparity duration time: %fms\n", 1000.0 * duration / getTickFrequency());

}

bool judgeCandidate(int startY , int startX, int winR, int winC){

	const int THRESHOLD = winR*winC / 6;

	int x1 = startX + winC / 3;
	int x2 = startX + winC / 3 *2;
	int y1 = startY;
	int y2 = startY + winR - 1;

	int sum = intHist[y2][x2] + intHist[y1][x1] - intHist[y2][x1] - intHist[y1][x2];
	return sum > THRESHOLD;

}

static void constructIntHist(const UInt8 *pImg,  int row, int col, int threshold){

	int i = 0, j = 0;
	memset(intHist,0,sizeof(intHist));

	//each row accumulation
	int watchVar = 0;
	int pos = 0;
	for (j = 0 + 1; j < row + 1; j++){
		for (i = 1; i < col + 1; i++){

			intHist[j][i] = (pImg[pos + i - 1]> threshold )*1 + intHist[j][i - 1];
			watchVar = intHist[j][i];
			//assert(watchVar == 0);
		}
		pos += col;
	}

	//each cols accumulation
	for (i = 0 + 1; i < col + 1; i++){
		for (j = 1 + 1; j < row + 1; j++){

			intHist[j][i] += intHist[j - 1][i];

		}
	}
}