#include"myCLib.h"
#include"setting.h"
#include"detectMultiScale.h"
#include"circlePerson.h"
#include"computeChnFtr.h"
#include"constructFeatureIntHist.h"
#include"AdaboostPredict.h"
#include"getHardFromNeg.h"
#include"reduceCandidate.h"

#include"opencvLib.h"
using namespace cv;

static inline int getZoomNum(int imgR, int imgC, int winR, int winC, float scale){

	assert(scale > 1);

	int zoomNumR = log((float)imgR / winR) / log(scale);
	int zoomNumC = log((float)imgC / winC) / log(scale);
	int zoomNum = zoomNumR > zoomNumC ? zoomNumC : zoomNumR;
	printf("zoomNum:%d\n", zoomNum);
	return zoomNum;
}

void detectMultiScale(Mat &src, int imgR, int imgC, 
	int winR, int winC, int winStrideR, int winStrideC,float scale, bool isGetHard){

	int zoomNum = getZoomNum(imgR, imgC, winR, winC, scale);

	static float chnFtrForDetect[MAX_CHN_FTR_DIM] = {.0f};

	int afterZoomR, afterZoomC;
	static UInt8 afterZoomImg[MAX_IN_IMG_SIZE];

	int trueCount = 0;
	int totalWinCount = 0;

	vector<Rect> found;

	int chnFtrDim = (winR / SHRINK)*(winC / SHRINK) +
					(winR / SHRINK)*(winC / SHRINK) +
					(winR / BIN_SIZE)*(winC / BIN_SIZE)*NUM_ORIENT;

	int zi = 0;	
	for (zi = 0; zi <= zoomNum; zi++){

		float ratio = pow(scale, (float)zi);
		afterZoomR = imgR / ratio;
		afterZoomC = imgC / ratio;

		Mat srcZoom;
		cv::resize(src, srcZoom, Size(afterZoomC, afterZoomR));
		Mat2ImgPointer(srcZoom, afterZoomImg);
		//double duration_all = static_cast<double>(getTickCount());
		
		//double duration = static_cast<double>(getTickCount());		
		constructFeatureIntHist(afterZoomImg, afterZoomR, afterZoomC);
		//duration = static_cast<double>(getTickCount()) - duration;
       // printf("construct integral image time: %fms\n", 1000.0 * duration / getTickFrequency());
		
		int winStepNumR = (afterZoomR - winR) / winStrideR + 1; //tip£ºint div int is int
		int winStepNumC = (afterZoomC - winC) / winStrideC + 1;
		int wr = 0, wc = 0;
		for (wr = 0; wr < winStepNumR; wr++){
			int startR = wr*winStrideR;
			for (wc = 0; wc < winStepNumC; wc++){

				int startC = wc*winStrideC;

				rect r = { startC, startR, winC, winR};
				computeChnFtr(afterZoomImg, afterZoomR, afterZoomC, r, chnFtrForDetect, chnFtrDim);
				float score = .0f; adaboostPredict(chnFtrForDetect, 1, chnFtrDim, &score,4);
				
				totalWinCount++;
				int x = 0, y = 0, width = 0, height = 0;
				if (score>0){ 
					y = (startR + 0.5)*imgR / afterZoomR - 0.5;
					x = (startC + 0.5)*imgC / afterZoomC - 0.5;
					height = winR*ratio;  width = winC*ratio;
					Rect rect(x, y, width, height); found.push_back(rect);
					trueCount++;
				}
			}
		}
		//duration_all = static_cast<double>(getTickCount()) - duration_all;
		//printf("total duration time: %fms\n", 1000.0 * duration_all / getTickFrequency());
	}	

	printf("totalWinCount: %d\n", totalWinCount);
	printf("trueCount: %d\n", trueCount); 

	if (trueCount > 0){
		if (!isGetHard)  { circlePerson(src, found, false); } // postProcessing and circle persons
		else             { saveHardPics(src, found); }
	}
}

//MULTI CLASSIFIER

void detectMultiClassifier(Mat &src, int imgR, int imgC,
	int winR, int winC, int winStrideR, int winStrideC, float scale){

	static float chnFtrForDetect[MAX_CHN_FTR_DIM] = {.0f};

	static UInt8 pImg[MAX_IN_IMG_SIZE];
	Mat2ImgPointer(src, pImg);

	double duration_all = static_cast<double>(getTickCount());

	double duration_intHist = static_cast<double>(getTickCount());

	constructFeatureIntHist(pImg, imgR, imgC);

	duration_intHist = static_cast<double>(getTickCount()) - duration_intHist;
	printf("construct intHist duration time: %fms\n", 1000.0 * duration_intHist / getTickFrequency());


	double duration_compFtr = static_cast<double>(getTickCount());

	int trueWinCount = 0, totalWinCount = 0, zi = 0;
	vector<Rect> found;
	int computedWinCount = 0;

	const int SCALE_NUM = 5;
	const float SCALE[SCALE_NUM] = { 0.5, 0.75, 1, 1.25, 1.5 };

	for (zi = 1; zi < SCALE_NUM; zi++){

		float ratio = SCALE[zi];     //float ratio = pow(scale, (float)zi);
		int newWinR = winR*ratio;
		int newWinC = winC*ratio;
		int newWinStepR = winStrideR * ratio;//TODO float newWinStepR
		int newWinStepC = winStrideC * ratio;
		//if (newWinStepR < 8) newWinStepR = 8;
		//if (newWinStepC < 8) newWinStepC = 8;

		int chnFtrDim = (newWinR / SHRINK)*(newWinC / SHRINK) +
			            (newWinR / SHRINK)*(newWinC / SHRINK) +
						(newWinR / BIN_SIZE)*(newWinC / BIN_SIZE)*NUM_ORIENT;
		int startR = 0;
		while (startR + newWinR <= imgR){
			int startC = 0;
			while (startC + newWinC <= imgC){

				totalWinCount++;

				bool likely = false;
				likely = judgeCandidate(startR, startC, newWinR, newWinC);
				if (likely){
					computedWinCount++;
					rect r = { startC, startR, newWinC, newWinR };
					computeChnFtr(pImg, imgR, imgC, r, chnFtrForDetect, chnFtrDim);
					float score = .0f; adaboostPredict(chnFtrForDetect, 1, chnFtrDim, &score, zi);
					//float score = adaboostPredict3(chnFtrForDetect, chnFtrDim, zi);
					if (score > 0) { Rect r(startC, startR, newWinC, newWinR); found.push_back(r); trueWinCount++; }
				}
				startC += newWinStepC;
			}
			startR += newWinStepR;
		}
	}
	duration_compFtr = static_cast<double>(getTickCount()) - duration_compFtr;
	printf("compute ftr duration time: %fms\n", 1000.0 * duration_compFtr / getTickFrequency());

	duration_all = static_cast<double>(getTickCount()) - duration_all;
	printf("detect total duration time: %fms\n", 1000.0 * duration_all / getTickFrequency());

	printf("totalWinCount: %d\n", totalWinCount); 
	printf("computedWinCount: %d\n", computedWinCount);
	printf("trueWinCount: %d\n", trueWinCount);	

	if (trueWinCount > 0)  circlePerson(src, found, false); // postProcessing and circle persons

	

}