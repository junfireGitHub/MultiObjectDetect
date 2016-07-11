//#include"myCLib.h"
//#include"setting.h"
//#include"postPro.h"
//#include"computeFtr.h"
//#include"constructFtrIntHist.h"
//#include"classify.h"
//#include"RFPredict.h"
//#include"getHardFromNeg.h"
//#include"reduceCandidate.h"
//#include"detectMultiScale.h"
//#include<iostream>
//#include"opencvLib.h"
//using namespace cv;
//
//
//void detectOneScale(Mat &src, int row, int col, DetectOpt detectOpt, vector<Bbox> result){
//	int srcR = src.rows;
//	int srcC = src.cols;
//
//	int maxFtrDim = getFtrDim(MAX_IN_IMG_R, MAX_IN_IMG_C);
//	float *pFtrForDetect = new float[maxFtrDim];
//	int trueWinCount = 0, totalWinCount = 0, computedWinCount = 0;
//	vector<Bbox> bb;
//
//	const float OPT[3][4] = {
//		{ 128, 64, 6, 6},
//		{ 144, 72, 7, 7},
//		{ 152, 76, 8, 8},
//	};
//
//	for (int zi = 0; zi < 3; zi++){
//		int newWinR = OPT[zi][0];
//		int newWinC = OPT[zi][1];
//		int newWinStepR = OPT[zi][2];
//		int newWinStepC = OPT[zi][3];
//		float ratio = detectOpt.scale;   
//
//		int imgR = srcR / ratio;
//		int imgC = srcC / ratio;
//
//		int startR = 0;
//		while (startR + newWinR <= imgR){
//			int startC = 0;
//			while (startC + newWinC <= imgC){
//				totalWinCount++;
//
//				bool likely = true;
//				if (detectOpt.isUseDp) {
//					int x = (startC + 0.5)*ratio - 0.5;
//					int y = (startR + 0.5)*ratio - 0.5;
//					int c = newWinC*ratio;
//					int r = newWinR*ratio;
//					likely = judgeCandidate(y, x, r, c);
//				}
//
//				if (likely){
//					computedWinCount++;
//					rect r = { startC, startR, newWinC, newWinR };
//					int ftrDim = getFtrDim(newWinR, newWinC);
//					computeFtr(imgR, imgC, r, pFtrForDetect, ftrDim);
//
//					float score = adaPersonMultiScale(pFtrForDetect, ftrDim, zi % 3);//here
//					if (score > 0) {
//						trueWinCount++;
//						Rect rec((startC + 0.5)*ratio - 0.5, (startR + 0.5)*ratio - 0.5, newWinC*ratio, newWinR*ratio);
//						ObjectType obType = PERSON;
//						Bbox bbTmp = { rec, score, obType };
//						bb.push_back(bbTmp);
//					}
//				}
//				startC += newWinStepC;
//			}
//			startR += newWinStepR;
//		}
//	}
//	printf("totalWinCount: %d\n", totalWinCount);
//	printf("computedWinCount: %d\n", computedWinCount);
//	printf("trueWinCount: %d\n", trueWinCount);
//
//	if (trueWinCount > 0){
//		bbNmsMaxMultiClass(src, bb, detectOpt.isPostPro); // postProcessing
//	}
//	delete[] pFtrForDetect; pFtrForDetect = NULL;
//}
//
//void detectPersonInVideo(Mat &src, int row, int col, DetectOpt detectOpt){
//	Mat srcGray;
//	if (src.channels() > 1) cv::cvtColor(src, srcGray, CV_BGR2GRAY);
//	else srcGray = src;
//
//	static int frameCount = 0;
//	const int PER_FRAME = 10;
//	vector<Bbox> bbScaleOne, bbScaleTwo;
//
//	float ratio = 1.0f;
//	constructFtrIntHist(srcGray);
//	if (frameCount % PER_FRAME == 0){
//		DetectOpt detectPara = { 128,64, 8, 8, 1.2, false/*isUseDp*/, false/*isPostPro*/, false/*isGetHard*/ };
//		detectPara.scale = ratio;
//		detectOneScale(srcGray,srcGray.rows,srcGray.cols,detectPara, bbScaleOne);
//	}
//	else{
//		for (int i = 0; i < bbScaleOne.size(); ++i){
//			//detectOneScale();
//		}
//	}
//
//	ratio = 1.331;
//	resize(srcGray, srcGray, Size(row / ratio, col / ratio));
//	constructFtrIntHist(srcGray);
//
//	if (frameCount % PER_FRAME == 0){
//		DetectOpt detectPara = {128, 64, 8, 8, 1.2, false/*isUseDp*/, false/*isPostPro*/, false/*isGetHard*/ };
//		detectPara.scale = ratio;
//		detectOneScale(srcGray, srcGray.rows, srcGray.cols, detectPara, bbScaleTwo);
//	}
//}
//
//static bool isDetect(vector<Rect> bb, Rect candidate){
//	Rect bbC = candidate;
//	for (int j = 0; j < bb.size(); j++){
//		if (bbC.x >= bb[j].x && bbC.y >= bb[j].y &&
//			bbC.x + bbC.width <= bb[j].x + bb[j].width  &&
//			bbC.y + bbC.height <= bb[j].y + bb[j].height)
//			return true;
//	}
//	return false;
//}