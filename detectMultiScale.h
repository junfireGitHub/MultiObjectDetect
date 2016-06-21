#ifndef DETECT_MULTI_SCALE_H_
#define DETECT_MULTI_SCALE_H_

#include"opencvLib.h"
using namespace cv;

typedef struct DetectOpt{
	int winR;
	int winC;
	int winStrideR;
	int winStrideC;
	float scale;
	bool isUseDp;  // �Ƿ�ʹ���Ӳ�
	bool isPostPro;// �Ƿ����
	bool isGetHard;
}DetectOpt;

void detectMultiScale(Mat &src, int imgR, int imgC, DetectOpt detectOpt);
void detectMultiClassifier(Mat &src, int imgR, int imgC, DetectOpt detectOpt);

#endif