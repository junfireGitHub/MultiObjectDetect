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

vector<Bbox> bb;

void detect(Mat& src, DetectOpt detectOpt[]){
	//init
	bb.clear();

	//road
	//std::cout << "detect road" << std::endl;
	//detectRoad(src, src.rows, src.cols,
	//	detectOpt[0].winR, detectOpt[0].winC, detectOpt[0].winStrideR, detectOpt[0].winStrideC);
	//std::cout << std::endl;

	////car and bike&elecBike
	//std::cout << "detect car and bm" << std::endl;
	//detectMultiScale(src, src.rows, src.cols, detectOpt[1]);
	//std::cout << std::endl;

	//person
	std::cout << "detect person" << std::endl;
	detectMultiClassifier(src, src.rows, src.cols, detectOpt[2]);
	std::cout << std::endl;

	//postprocessing	
	if (bb.size() > 1){
		//bbNmsMultiClass(src, bb, true);
		bbNmsMaxMultiClass(src, bb, true);
	}
}

void detectRoad(Mat &src, int imgR, int imgC,
	int winR, int winC, int winStrideR, int winStrideC)
{
	int ftrDim = getFtrDim(winR, winC);
	float *chnFtrForDetect = new float[ftrDim];

	int trueCount = 0; int totalWinCount = 0;
	//vector<Rect> found;

	int chnFtrDim = getFtrDim(winR, winC);;

	Mat srcGray;
	if (src.channels() > 1) cv::cvtColor(src, srcGray, CV_BGR2GRAY);
	else srcGray = src;
	constructFtrIntHist(srcGray);

	int pixelXOffset = imgC % winC;
	int pixelYOffset = imgR % winR;
	int numXOffset = 0;
	int numYOffset = 2;

	int winStepNumR = (imgR - winR) / winStrideR + 1; //tip£ºint div int is int
	int winStepNumC = (imgC - winC) / winStrideC + 1;

	int wr = 0, wc = 0;
	for (wr = numYOffset; wr < winStepNumR; wr++){
		int startR = wr*winStrideR + pixelYOffset;
		for (wc = numXOffset; wc < winStepNumC; wc++){
			int startC = wc*winStrideC + pixelXOffset;

			rect r = { startC, startR, winC, winR };
			computeFtr(imgR, imgC, r, chnFtrForDetect, chnFtrDim);
			//float score = .0f; adaboostPredictRoad(chnFtrForDetect, 1, chnFtrDim, &score);
			float score = adaboostPredictRoad2(chnFtrForDetect);

			if (score > 0){
				Rect rect(startC, startR, winC, winR); //found.push_back(rect);
				Bbox b = { rect, score, ROAD };
				bb.push_back(b);
				trueCount++;
			}
			totalWinCount++;
		}
	}
	printf("totalWinCount: %d\n", totalWinCount);
	printf("trueCount: %d\n", trueCount);
	delete[] chnFtrForDetect;
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
	//vector<Bbox> bb;
	//vector<Rect> found;
	//vector<float> fScore;
	//zoomNum = 0;//CHANGED
	for (int zi = 0; zi <= zoomNum; zi++){
		float ratio = pow(scale, (float)zi);
		afterZoomR = imgR / ratio;
		afterZoomC = imgC / ratio;
		
		Mat srcZoom; cv::resize(srcGray, srcZoom, Size(afterZoomC, afterZoomR));
		constructFtrIntHist(srcZoom);

		int winStepNumR = (afterZoomR - winR) / winStrideR + 1; //tip£ºint div int is int
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

				//float score = .0f; adaPredictPerson(pFtrForDetect, 1,ftrDim, &score, 1);
				float carScore = adaboostPredictCar(pFtrForDetect);
				float bmScore =  adaboostPredictBm(pFtrForDetect);
				ObjectType obj = carScore > bmScore ? CAR : BM;
				float score = carScore > bmScore ? carScore : bmScore;

				if (score <= 0) continue;
				int x = 0, y = 0, width = 0, height = 0;
				y = (startR + 0.5)*imgR / afterZoomR - 0.5;
				x = (startC + 0.5)*imgC / afterZoomC - 0.5;
				height = winR*ratio;  width = winC*ratio;
				Rect r(x, y, width, height); 
				//found.push_back(r); fScore.push_back(score);
				Bbox b = { r, score, obj };
				bb.push_back(b);
				trueWinCount++;		 		
				/*double duration_detect = static_cast<double>(getTickCount());
				int hs = rfPredict(pFtrForDetect);
				duration_detect = static_cast<double>(getTickCount()) - duration_detect;
				printf("predict once duration time: %fms\n", 1000.0 * duration_detect / getTickFrequency());			
				assert(hs <= 4);
				if (hs<4){
					trueWinCount++;
					int x, y, width, height;
					x = (startC + 0.5)*imgC / afterZoomC - 0.5;
					y = (startR + 0.5)*imgR / afterZoomR - 0.5;
					width = winC*ratio; height = winR*ratio;  

					Rect rect(x, y, width, height);
					ObjectType classNum[] = { CAR, BIKE, ELECBIKE, ROAD };
					ObjectType obType = classNum[hs];
					Bbox bbTmp = { rect, 0, obType };
					bb.push_back(bbTmp);
				}*/
			}
		}
	}

	printf("totalWinCount: %d\n", totalWinCount);
	printf("trueWinCount: %d\n", trueWinCount);

	//if (trueWinCount > 0){
	//	if (detectOpt.isGetHard)  saveHardPics(srcGray, found);
	//    else bbNmsMultiClass(src, bb, detectOpt.isPostPro);
	//	//bbNms(src, found, detectOpt.isPostPro);
	//}
	delete[] pFtrForDetect;
}

//MULTI CLASSIFIER
void detectMultiClassifier(Mat &src, int srcR, int srcC, DetectOpt detectOpt){
	//if (src.rows > MAX_IN_IMG_R || src.cols > MAX_IN_IMG_C){
	//	if (src.rows > src.cols)
	//		cv::resize(src, src, cv::Size(floor(MAX_IN_IMG_R*1.0f / src.rows*src.cols), MAX_IN_IMG_R));
	//	else
	//		cv::resize(src, src, cv::Size(MAX_IN_IMG_C, floor(MAX_IN_IMG_C*1.0f / src.cols*src.rows)));
	//	std::cout <<"image is resized," <<"size: " << src.rows << " x " << src.cols << std::endl;
	//}
	//assert(src.rows <= MAX_IN_IMG_R && src.cols <= MAX_IN_IMG_C);

	int imgR = src.rows;
	int imgC = src.cols;
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
	//vector<Bbox> bb;  //vector<float> fScore;
	int computedWinCount = 0;

	const int SCALE_NUM = 5;
	const float SCALE[SCALE_NUM] = { 0.5, 0.75, 1, 1.25, 1.5 };

	for (zi = 1; zi < SCALE_NUM; zi++){
		float ratio = SCALE[zi];     //float ratio = pow(scale, (float)zi);
		int newWinR = winR*ratio;
		int newWinC = winC*ratio;
		int newWinStepR = winStrideR * ratio;
		int newWinStepC = winStrideC * ratio;

		int startR = 0;
		while (startR + newWinR <= imgR){
			int startC = 0;
			while (startC + newWinC <= imgC){
				totalWinCount++;

				bool likely = true;
				if (detectOpt.isUseDp) { likely = judgeCandidate(startR, startC, newWinR, newWinC); }
				if (likely){
					computedWinCount++;
					rect r = { startC, startR, newWinC, newWinR };
					int ftrDim = getFtrDim(newWinR, newWinC);
					computeFtr(imgR, imgC, r, pFtrForDetect, ftrDim);

					//float score = .0f; adaPredictPerson(pFtrForDetect, 1, ftrDim, &score, zi);
					float score = adaPredictPersonEarlyExit(pFtrForDetect, ftrDim, zi);
					if (score > 0) {
						trueWinCount++;
						Rect rec(startC, startR, newWinC, newWinR);
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

	//if (trueWinCount > 0){
	//	bbNmsMultiClass(src, bb, detectOpt.isPostPro); // postProcessing
	//	//bbNmsMax(src, found, fScore, detectOpt.isPostPro);	
	//}
	delete[] pFtrForDetect;
}