#include"myCLib.h"
#include"constructFtrIntHist.h"
#include"computeFtr.h"


void getColorFeature(const float *src, int srcC, rect r, float *pColorFtr){
	int i = 0, j = 0;
	int kd = 0;
	int ks = r.y*srcC + r.x;

	for (j = 0; j <= r.height - 1; ++j){
		memcpy(pColorFtr + kd, src + ks, sizeof(src[0])*r.width);
		kd += r.width;
		ks += srcC;
	}

}

void getGradMagFeature(const float *pMag, int srcC, rect r, float *pMagFtr){
	int i = 0, j = 0;
	int kd = 0;
	int ks = r.y*srcC + r.x;

	for (j = 0; j <= r.height - 1; ++j){
		memcpy(pMagFtr + kd, pMag + ks, sizeof(pMag[0])*r.width);
		kd += r.width;
		ks += srcC;
	}

}

static void getGradHistFeature(const float pSrc[][NUM_ORIENT], int srcC, rect r, float *pFtr){
	int i = 0, j = 0;
	int kd = 0;
	int ks = r.y*srcC + r.x;
	const int KD_PLUS = r.width*NUM_ORIENT;

	for (j = 0; j <= r.height - 1; ++j){
		memcpy(pFtr + kd, &pSrc[ks][0], sizeof(pFtr[0])*KD_PLUS);
		kd += KD_PLUS;
		ks += srcC;
	}
}

int getFtrDim(int rows, int cols){
	return  (rows / SHRINK)*(cols / SHRINK) +  (rows / SHRINK)*(cols / SHRINK) + 
			(rows / BIN_SIZE)* (cols / BIN_SIZE)*NUM_ORIENT;
}

//from constructFtrIntHist.cpp
extern float grayImgShrink[MAX_IN_IMG_SIZE / SHRINK / SHRINK];
extern float gradMagShrink[MAX_IN_IMG_SIZE / SHRINK / SHRINK];
extern float gradHistShrink[(MAX_IN_IMG_R / BIN_SIZE)*(MAX_IN_IMG_C / BIN_SIZE)][NUM_ORIENT];

void computeFtr(int srcR, int srcC, rect r, float *pChnFtr, int ftrDim){

	assert(r.x + r.width <= srcC && r.y + r.height <= srcR);
	rect rShrk = { r.x / SHRINK, r.y / SHRINK, r.width / SHRINK, r.height / SHRINK };
	int shrkC = srcC / SHRINK;

	int gradHistFtrLen = (r.width / BIN_SIZE) * (r.height / BIN_SIZE) * NUM_ORIENT;
	int gradMagFtrLen = rShrk.width *rShrk.height;
	int colorFtrLen = rShrk.width *rShrk.height;

	assert(colorFtrLen + gradMagFtrLen + gradHistFtrLen == ftrDim);

	getGradHistFeature(gradHistShrink, shrkC, rShrk, pChnFtr);
	getGradMagFeature(gradMagShrink, shrkC, rShrk, pChnFtr + gradHistFtrLen);
	getColorFeature(grayImgShrink, shrkC, rShrk, pChnFtr + gradHistFtrLen + gradMagFtrLen);
}