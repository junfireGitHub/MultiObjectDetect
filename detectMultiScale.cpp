#include"myCLib.h"
#include"setting.h"
#include"postPro.h"
#include"computeFtr.h"
#include"constructFtrIntHist.h"
#include"classify.h"
#include"RFPredict.h"
#include"getHardFromNeg.h"
#include"reduceCandidate.h"
#include"detectMultiScale.h"
#include<iostream>
#include"opencvLib.h"
using namespace cv;


static inline int getZoomNum(int imgR, int imgC, int winR, int winC, float scale){
	assert(scale > 1);
	int zoomNumR = log((float)imgR / winR) / log(scale);
	int zoomNumC = log((float)imgC / winC) / log(scale);
	int zoomNum = zoomNumR > zoomNumC ? zoomNumC : zoomNumR;
	printf("zoomNum:%d\n", zoomNum);
	return zoomNum > 0 ? zoomNum : 0;
}

void calDetectCandidate(int r, int c, int range,
	bool a[MAX_IN_IMG_R >> 3][MAX_IN_IMG_C >> 3],
	Uint8 w[MAX_IN_IMG_R >> 3][MAX_IN_IMG_C >> 3],
	int winSize){
	int sr = r - range >= 0 ? r - range : 0;
	int er = r + range < MAX_IN_IMG_R >> 3 ? r + range : MAX_IN_IMG_R >> 3 - 1;
	int sc = c - range >= 0 ? c - range : 0;
	int ec = c + range < MAX_IN_IMG_C >> 3 ? c + range : MAX_IN_IMG_C >> 3 - 1;
	int i = 0, j = 0;
	for (i = sr; i <= er; ++i){
		for (j = sc; j <= ec; ++j){
			a[i][j] = true;
			w[i][j] = winSize;
		}
	}
}

void detectMultiScale(Mat &src, int srcR, int srcC, DetectOpt detectOpt){
	/*zoom out image if its size is too big*/
	//if (src.rows > MAX_IN_IMG_R || src.cols > MAX_IN_IMG_C){
	//	if (src.rows > src.cols)
	//		cv::resize(src, src, cv::Size(floor(MAX_IN_IMG_R*1.0f / src.rows*src.cols), MAX_IN_IMG_R));
	//	else
	//		cv::resize(src, src, cv::Size(MAX_IN_IMG_C, floor(MAX_IN_IMG_C*1.0f / src.cols*src.rows)));
	//	std::cout <<"image is resized" <<"size: " << src.rows << " x " << src.cols << std::endl;
	//}
	//assert(src.rows <= MAX_IN_IMG_R && src.cols <= MAX_IN_IMG_C);

	int imgR = src.rows;
	int imgC = src.cols;
	int winR = detectOpt.winR;
	int winC = detectOpt.winC;
	int winStrideR = detectOpt.winStrideR;
	int winStrideC = detectOpt.winStrideC;
	float scale = detectOpt.scale;

	Mat srcGray;
	if (src.channels() > 1) cv::cvtColor(src, srcGray, CV_BGR2GRAY);
	else srcGray = src;

	int zoomNum = getZoomNum(imgR, imgC, winR, winC, scale);
	int ftrDim = getFtrDim(winR, winC);
	float *pFtrForDetect = new float[ftrDim];
	int afterZoomR, afterZoomC;

	int trueWinCount = 0; int totalWinCount = 0;
	vector<Bbox> bb;
	vector<Rect> found;
	for (int zi = 0; zi <= zoomNum; zi++){
		float ratio = pow(scale, (float)zi);
		afterZoomR = imgR / ratio;
		afterZoomC = imgC / ratio;

		Mat srcZoom; cv::resize(srcGray, srcZoom, Size(afterZoomC, afterZoomR));
		constructFtrIntHist(srcZoom);

		int winStepNumR = (afterZoomR - winR) / winStrideR + 1; //tip：int div int is int
		int winStepNumC = (afterZoomC - winC) / winStrideC + 1;
		int wr = 0, wc = 0;
		for (wr = 0; wr < winStepNumR; wr++){
			int startR = wr*winStrideR;
			for (wc = 0; wc < winStepNumC; wc++){
				totalWinCount++;
				int startC = wc*winStrideC;

				rect w = { startC, startR, winC, winR };
				int ftrDim = getFtrDim(winR, winC);
				computeFtr(afterZoomR, afterZoomC, w, pFtrForDetect, ftrDim);

				float score = adaPersonOneScale(pFtrForDetect);
				//float score = adaPersonMultiScale(pFtrForDetect, ftrDim, 2);

				if (score <= 0) continue;
				int x = 0, y = 0, width = 0, height = 0;
				y = (startR + 0.5)*imgR / afterZoomR - 0.5;
				x = (startC + 0.5)*imgC / afterZoomC - 0.5;
				height = winR*ratio;  width = winC*ratio;
				Rect r(x, y, width, height);
				found.push_back(r);
				Bbox b = { r, score, PERSON };
				bb.push_back(b);
				trueWinCount++;
			}
		}
	}

	printf("totalWinCount: %d\n", totalWinCount);
	printf("trueWinCount: %d\n", trueWinCount);

	if (trueWinCount > 0){
		vector<Rect> finalPos;
		if (detectOpt.isGetHard)   saveHardPics(src, found);
		else bbNmsMaxMultiClass(src, bb, detectOpt.isPostPro, finalPos);
	}

	delete[] pFtrForDetect;
	pFtrForDetect = NULL;
}

//MULTI CLASSIFIER
void detectMultiClassifier(Mat &src, int row, int col, DetectOpt detectOpt){
	//if (src.rows > MAX_IN_IMG_R || src.cols > MAX_IN_IMG_C){
	//	if (src.rows > src.cols)
	//		cv::resize(src, src, cv::Size(floor(MAX_IN_IMG_R*1.0f / src.rows*src.cols), MAX_IN_IMG_R));
	//	else
	//		cv::resize(src, src, cv::Size(MAX_IN_IMG_C, floor(MAX_IN_IMG_C*1.0f / src.cols*src.rows)));
	//	std::cout <<"image is resized," <<"size: " << src.rows << " x " << src.cols << std::endl;
	//}
	//assert(src.rows <= MAX_IN_IMG_R && src.cols <= MAX_IN_IMG_C);

	int srcR = src.rows;
	int srcC = src.cols;
	int winR = detectOpt.winR;
	int winC = detectOpt.winC;
	int winStrideR = detectOpt.winStrideR;
	int winStrideC = detectOpt.winStrideC;
	float scale = detectOpt.scale;

	int maxFtrDim = getFtrDim(MAX_IN_IMG_R, MAX_IN_IMG_C);
	float *pFtrForDetect = new float[maxFtrDim];

	Mat srcGray;
	if (src.channels() > 1) cv::cvtColor(src, srcGray, CV_BGR2GRAY);
	else srcGray = src;
	constructFtrIntHist(srcGray);

	int trueWinCount = 0, totalWinCount = 0, zi = 0;
	vector<Bbox> bb;
	int computedWinCount = 0;

	const float OPT[6][5] = { //here
		{ 128, 64, 8, 8, 1 },
		{ 144, 72, 8, 8, 1 },
		{ 152, 76, 8, 8, 1 },

		{ 128, 64, 8, 8, 1.331 },
		{ 144, 72, 8, 8, 1.331 },
		{ 152, 76, 8, 8, 1.331 },
	};

	const int SIZE_R = MAX_IN_IMG_R >> 3;
	const int SIZE_C = MAX_IN_IMG_C >> 3;
	static bool isDetect[SIZE_R][SIZE_C] = { false };/*根据这个决定是否需要检测*/
	bool isDetectNew[SIZE_R][SIZE_C] = { false }; /*用于暂时存放新的需要检测区域*/
	Uint8 detectZoneWinSize[SIZE_R][SIZE_C] = { 0 };
	const int FRAME_INTERVAL = 5;/*每隔多少帧需要重新检测一次*/
	const int RANGE = 3; /*一个窗口周围多大范围内设置为需要检测，范围大小为(2*RANGE+1)*(2*RANGE+1)*/
	static int frameCount = 0;/*统计帧数*/
	//if (frameCount % FRAME_INTERVAL == 0){
	//	memset(isDetect, 1, sizeof(isDetect));
	//	memset(detectZoneWinSize, 1, sizeof(detectZoneWinSize));
	//}
	memset(isDetectNew, 0, sizeof(isDetectNew));

	for (zi = 0; zi < 6; zi++){
		int newWinR = OPT[zi][0];
		int newWinC = OPT[zi][1];
		int newWinStepR = OPT[zi][2];
		int newWinStepC = OPT[zi][3];
		float ratio = OPT[zi][4];     //float ratio = pow(scale, (float)zi);

		if (zi == 3) {
			resize(srcGray, srcGray, Size(srcC / ratio, srcR / ratio));
			constructFtrIntHist(srcGray);
		}
		int imgR = srcR / ratio;
		int imgC = srcC / ratio;//here

		int startR = 0;
		while (startR + newWinR <= imgR){
			int startC = 0;
			while (startC + newWinC <= imgC){
				totalWinCount++;

				bool likely = true;
				if (detectOpt.isUseDp) {
					int x = (startC + 0.5)*ratio - 0.5;
					int y = (startR + 0.5)*ratio - 0.5;
					int c = newWinC*ratio;
					int r = newWinR*ratio;
					likely = judgeCandidate(y, x, r, c);
				}
				unsigned int oriR = (startR + 0.5)*ratio - 0.5;
				unsigned int oriC = (startC + 0.5)*ratio - 0.5;
				oriR >>= 3;  oriC >>= 3;
				if (likely && (frameCount % FRAME_INTERVAL == 0 ||
					isDetect[oriR][oriC] && abs(detectZoneWinSize[oriR][oriC] - zi) <= 1))
				{
					computedWinCount++;
					rect r = { startC, startR, newWinC, newWinR };
					int ftrDim = getFtrDim(newWinR, newWinC);
					computeFtr(imgR, imgC, r, pFtrForDetect, ftrDim);

					float score = adaPersonMultiScale(pFtrForDetect, ftrDim, zi % 3);//here
					if (score > 0) {
						trueWinCount++;
						Rect rec((startC + 0.5)*ratio - 0.5,
							(startR + 0.5)*ratio - 0.5,
							newWinC*ratio, newWinR*ratio);
						ObjectType obType = PERSON;
						Bbox bbTmp = { rec, score, obType };
						bb.push_back(bbTmp);
					}
				}
				startC += newWinStepC;
			}
			startR += newWinStepR;
		}
	}

	printf("totalWinCount: %d\n", totalWinCount);
	printf("computedWinCount: %d\n", computedWinCount);
	printf("trueWinCount: %d\n", trueWinCount);

	if (trueWinCount > 0){
		cv::vector<Rect> finalPos;
		bbNmsMaxMultiClass(src, bb, detectOpt.isPostPro,finalPos); // postProcessing and show 
		/*更新需要检测区域和对应区域的窗口大小*/
		Uint8 ah[50];
		ah[64-64] = 0; ah[72-64] = 1;
		ah[76-64] = 2; ah[85-64] = 3;
		ah[95-64] = 4; ah[101-64] = 5;
		for (int i = 0; i < finalPos.size(); ++i){
			Rect r = finalPos[i];
			int zi = ah[r.width - 64];
			printf("r.width:%d\n", r.width);
			calDetectCandidate(r.y >> 3, r.x >> 3, RANGE, isDetectNew, detectZoneWinSize, zi);
		}
	}
	frameCount++;
	memcpy(isDetect, isDetectNew, sizeof(isDetectNew));

	delete[] pFtrForDetect; pFtrForDetect = NULL;
}