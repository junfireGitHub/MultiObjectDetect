#include"myCLib.h"
#include"constructFeatureIntHist.h"
#include"computeChnFtr.h"


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

//inline void getBlockGradHist(rect r, float ftr[NUM_ORIENT]){
//
//	int x1 = r.x;
//	int y1 = r.y;
//	int x2 = x1 + r.width;
//	int y2 = y1 + r.height;
//	int k = 0;
//
//	for (k = 0; k < NUM_ORIENT; k++){
//		ftr[k] = gradHistIntArr[y2][x2][k] + gradHistIntArr[y1][x1][k] -
//			gradHistIntArr[y2][x1][k] - gradHistIntArr[y1][x2][k];
//	}
//}
//
//void getGradHistFeature(rect r, float *pH){
//
//	float arrH[NUM_ORIENT] = { .0f };
//
//	const int NEG_BIN_R = r.height / BIN_SIZE;
//	const int NEG_BIN_C = r.width / BIN_SIZE;
//
//	int i = 0, j = 0;
//	int x1 = r.x, y1 = r.y;
//	int ofs = 0;
//	rect rp = { 0, 0, 0, 0 };
//	for (j = 0; j < NEG_BIN_R; ++j){
//
//		x1 = r.x;
//		for (i = 0; i < NEG_BIN_C; ++i){
//
//			rp = { x1, y1, BIN_SIZE, BIN_SIZE };
//			getBlockGradHist(rp, arrH);
//			memcpy(pH + ofs, arrH, sizeof(arrH));
//			ofs += NUM_ORIENT;
//			x1 += BIN_SIZE;
//		}
//		y1 += BIN_SIZE;
//	}
//}

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


//void computeChnFtr(const UInt8 *pSrc, int srcR, int srcC, rect r, float *pChnFtr){
//
//	rect rShrk = { r.x / SHRINK, r.y / SHRINK, r.width / SHRINK, r.height / SHRINK };
//	int shrkC = srcC / SHRINK;	
//	
//	int gradHistFtrLen = (r.width / BIN_SIZE) * (r.height / BIN_SIZE) * NUM_ORIENT;
//	int gradMagFtrLen = rShrk.width *rShrk.height;
//	int colorFtrLen = rShrk.width *rShrk.height;
//
//
//	assert(colorFtrLen + gradMagFtrLen + gradHistFtrLen <= MAX_CHN_FTR_DIM);
//	assert(rShrk.x + rShrk.width <= MAX_IN_IMG_C / SHRINK);
//	assert(rShrk.y + rShrk.height <= MAX_IN_IMG_R / SHRINK);
//
//	getGradHistFeature(gradHistShrink, shrkC, rShrk, pChnFtr);
//	getGradMagFeature(gradMagShrink, shrkC, rShrk, pChnFtr + gradHistFtrLen);
//	getColorFeature(grayImgShrink, shrkC, rShrk, pChnFtr + gradHistFtrLen + gradMagFtrLen);
//}

void computeChnFtr(const UInt8 *pSrc, int srcR, int srcC, rect r, float *pChnFtr, int ftrDim){

	rect rShrk = { r.x / SHRINK, r.y / SHRINK, r.width / SHRINK, r.height / SHRINK };
	int shrkC = srcC / SHRINK;

	int gradHistFtrLen = (r.width / BIN_SIZE) * (r.height / BIN_SIZE) * NUM_ORIENT;
	int gradMagFtrLen = rShrk.width *rShrk.height;
	int colorFtrLen = rShrk.width *rShrk.height;

	assert(ftrDim <= MAX_CHN_FTR_DIM);
	assert(colorFtrLen + gradMagFtrLen + gradHistFtrLen == ftrDim);
	assert(rShrk.x + rShrk.width <= MAX_IN_IMG_C / SHRINK);
	assert(rShrk.y + rShrk.height <= MAX_IN_IMG_R / SHRINK);

	getGradHistFeature(gradHistShrink, shrkC, rShrk, pChnFtr);
	getGradMagFeature(gradMagShrink, shrkC, rShrk, pChnFtr + gradHistFtrLen);
	getColorFeature(grayImgShrink, shrkC, rShrk, pChnFtr + gradHistFtrLen + gradMagFtrLen);
}