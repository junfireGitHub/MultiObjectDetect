#ifndef DETECT_MULTI_SCALE_H_
#define DETECT_MULTI_SCALE_H_

#include"opencvLib.h"
using namespace cv;

void detectMultiScale(Mat &src, int imgR, int imgC,
	int winR, int winC, int winStrideR, int winStrideC, float scale, bool isGetHard);

void detectMultiClassifier(Mat &src, int imgR, int imgC,
	int winR, int winC, int winStrideR, int winStrideC, float scale);

//void detectMultiScaleNoResize(Mat &src, int imgR, int imgC,
//	int winR, int winC, int winStrideR, int winStrideC, float scale);
//void detectMultiScaleNoTxt(Mat &src, int imgR, int imgC,
//	int winR, int winC, int winStrideR, int winStrideC, float scale);
//void detectMultiClassifiers(Mat &src, int imgR, int imgC,
//	int winR, int winC, int winStrideR, int winStrideC, float scale);
#endif