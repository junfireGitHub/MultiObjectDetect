#include"myCLib.h"
#include"constructFtrIntHist.h"
#include"opencvLib.h"

float grayImgShrink[(MAX_IN_IMG_R / SHRINK)* (MAX_IN_IMG_C / SHRINK)] = { .0f };
float gradMagShrink[(MAX_IN_IMG_R / SHRINK)* (MAX_IN_IMG_C / SHRINK)] = { .0f };
float gradHistShrink[(MAX_IN_IMG_R / BIN_SIZE)*(MAX_IN_IMG_C / BIN_SIZE)][NUM_ORIENT] = { .0f };

static double gradHistIntArr[MAX_IN_IMG_R + 1][MAX_IN_IMG_C + 1][NUM_ORIENT] = { .0f };
static double grayIntHist[MAX_IN_IMG_R + 1][MAX_IN_IMG_C + 1] = { .0f };
static double gradMagIntHist[MAX_IN_IMG_R + 1][MAX_IN_IMG_C + 1] = { .0f };
static float gradMag[MAX_IN_IMG_SIZE] = { .0f };
static float gradOri[MAX_IN_IMG_SIZE] = { .0f };


static void computeGradient(const UInt8 *src, int imgR, int imgC, float *pGradMag, float *pOri){

	static float ysub[MAX_IN_IMG_SIZE] = { 0 };
	static float xsub[MAX_IN_IMG_SIZE] = { 0 };

	int y = 0, x = 0;
	int pos = 0;

	/*gradient of x orientation,replication padding*/
	for (y = 0; y < imgR; y++){
		for (x = 1; x < imgC - 1; x++)
		{
			pos = y*imgC + x;
			xsub[pos] = src[pos + 1] - src[pos - 1];
			xsub[pos] /= 2.0;
		}
	}
	for (y = 0; y < imgR; y++)
	{
		pos = y*imgC;
		xsub[pos] = src[pos + 1] - src[pos];
		xsub[pos + imgC - 1] = src[pos + imgC - 1] - src[pos + imgC - 2];
	}

	/*gradient of y orientation£¬replication padding*/
	for (y = 1; y < imgR - 1; y++){
		for (x = 0; x < imgC; x++)
		{
			pos = y*imgC + x;
			ysub[pos] = src[pos + imgC] - src[pos - imgC];
			ysub[pos] /= 2.0;
		}
	}
	for (x = 0; x < imgC; x++)
	{
		pos = imgC*(imgR - 1);
		ysub[x] = src[imgC + x] - src[x];
		ysub[pos + x] = src[pos + x] - src[pos - imgC + x];
	}

	int i = 0;
	int imgRMpyC = imgR*imgC;

	for (i = 0; i < imgRMpyC; ++i)
	{
		pGradMag[i] = sqrt(ysub[i] * ysub[i] + xsub[i] * xsub[i]); //magnitude of gradient
		pOri[i] = atan2(ysub[i], xsub[i]) + 3.14159*(ysub[i] < 0); //angle of gradient 3.14159
	}
}

static void gradQuantize(const float *pO, const float *pM, int row, int col){

	int i = 0, j = 0;
	float alpha = .0f;
	int idx = 0;
	static const float O_MULT = NUM_ORIENT / 3.1416f;
	static const float S = BIN_SIZE*BIN_SIZE;
	float mag = .0f;
	int rpos = 0, pos = 0;
	for (j = 0; j < row; ++j, rpos += col){
		for (i = 0; i < col; ++i){

			pos = rpos + i;
			alpha = pO[pos] * O_MULT;
			idx = (int)alpha;
			alpha = alpha - idx;
			if (idx < 0) idx += NUM_ORIENT;
			else if (idx >= NUM_ORIENT) idx -= NUM_ORIENT;
			mag = pM[pos] / S;
			gradHistIntArr[j + 1][i + 1][idx] = mag * (1 - alpha);
			idx++; if (idx >= NUM_ORIENT) idx = 0;
			gradHistIntArr[j + 1][i + 1][idx] = mag * alpha;
		}
	}
}

static void constructGradHistIntHist(const float *pOri, const float *pMag, int imgR, int imgC){

	int i = 0, j = 0, k = 0;
	memset(gradHistIntArr, 0, sizeof(gradHistIntArr));
	gradQuantize(pOri, pMag, imgR, imgC);

	//each row accumulation
	for (j = 0 + 1; j < imgR + 1; j++){
		for (i = 1 + 1; i < imgC + 1; i++){
			for (k = 0; k < NUM_ORIENT; k++){
				gradHistIntArr[j][i][k] += gradHistIntArr[j][i - 1][k];
			}
		}
	}

	//each cols accumulation
	for (i = 0 + 1; i < imgC + 1; i++){
		for (j = 1 + 1; j < imgR + 1; j++){
			for (k = 0; k < NUM_ORIENT; k++){
				gradHistIntArr[j][i][k] += gradHistIntArr[j - 1][i][k];
			}
		}
	}

}

static inline void getBlockGradHist(int x1, int y1, int x2, int y2, float ftr[NUM_ORIENT]){

	int k = 0;

	for (k = 0; k < NUM_ORIENT; k++){
		ftr[k] = gradHistIntArr[y2][x2][k] + gradHistIntArr[y1][x1][k] -
			gradHistIntArr[y2][x1][k] - gradHistIntArr[y1][x2][k];
	}
}

static void computeGradHistShrink(int srcR, int srcC, int binSize, float gradHistShrink[][NUM_ORIENT]){

	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	int pos = 0;
	for (y1 = 0; y1 + binSize <= srcR; y1 += binSize){// y1 + binSize - 1 <= srcR - 1
		for (x1 = 0; x1 + binSize <= srcC; x1 += binSize){

			x2 = x1 + binSize;
			y2 = y1 + binSize;
			getBlockGradHist(x1, y1, x2, y2, gradHistShrink[pos]);
			pos++;
		}
	}
	assert(pos == (srcR / binSize)*(srcC / binSize));
}

static void constructGradMagIntHist(const float *pChn, int row, int col){

	int i = 0, j = 0;
	memset(gradMagIntHist, 0, sizeof(gradMagIntHist));
	//each row accumulation
	int pos = 0;
	for (j = 0 + 1; j < row + 1; j++){
		for (i = 1; i < col + 1; i++){

			gradMagIntHist[j][i] = pChn[pos + i - 1] + gradMagIntHist[j][i - 1];
		}
		pos += col;
	}

	//each cols accumulation
	for (i = 0 + 1; i < col + 1; i++){
		for (j = 1 + 1; j < row + 1; j++){

			gradMagIntHist[j][i] += gradMagIntHist[j - 1][i];

		}
	}
}

static void constructColorIntHist(const UInt8 *pChn, int row, int col){

	int i = 0, j = 0;
	memset(grayIntHist, 0, sizeof(grayIntHist));
	//each row accumulation
	int pos = 0;
	for (j = 0 + 1; j < row + 1; j++){
		for (i = 1; i < col + 1; i++){

			grayIntHist[j][i] = pChn[pos + i - 1] + grayIntHist[j][i - 1];
		}
		pos += col;
	}

	//each cols accumulation
	for (i = 0 + 1; i < col + 1; i++){
		for (j = 1 + 1; j < row + 1; j++){

			grayIntHist[j][i] += grayIntHist[j - 1][i];

		}
	}
}

static void computeChnShrink(double intHist[][MAX_IN_IMG_C + 1], int srcR, int srcC, int shrink, float chnShrink[]){

	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	int pos = 0;
	for (y1 = 0; y1 + shrink <= srcR; y1 += shrink){// y1 + binSize - 1 <= srcR - 1
		for (x1 = 0; x1 + shrink <= srcC; x1 += shrink){

			x2 = x1 + shrink;
			y2 = y1 + shrink;
			chnShrink[pos] = intHist[y2][x2] + intHist[y1][x1] - intHist[y1][x2] - intHist[y2][x1];
			chnShrink[pos] /= shrink*shrink;
			pos++;
		}
	}
	assert(pos == (srcR / shrink)*(srcC / shrink));
}

//ACF 
void constructFtrIntHist(const cv::Mat& src){
	//cv::Mat src = mat.clone();
	//if (src.rows > MAX_IN_IMG_R || src.cols > MAX_IN_IMG_C){
	//	if (src.rows > src.cols)
	//		cv::resize(src, src, cv::Size(floor(MAX_IN_IMG_R / src.rows*src.cols), MAX_IN_IMG_R));
	//	else
	//		cv::resize(src, src, cv::Size(MAX_IN_IMG_C, floor(MAX_IN_IMG_C/src.cols*src.rows)));
	//}
	//assert(src.rows <= MAX_IN_IMG_R && src.cols <= MAX_IN_IMG_C);

	int srcR = src.rows;
	int srcC = src.cols;
	unsigned char *pSrc = new unsigned char[srcR*srcC];
	Mat2ImgPointer(src, pSrc);

	computeGradient(pSrc, srcR, srcC, gradMag, gradOri);

	constructGradHistIntHist(gradOri, gradMag, srcR, srcC);
	computeGradHistShrink(srcR, srcC, BIN_SIZE, gradHistShrink);

	constructGradMagIntHist(gradMag, srcR, srcC);
	computeChnShrink(gradMagIntHist, srcR, srcC, SHRINK, gradMagShrink);

	constructColorIntHist(pSrc, srcR, srcC);
	computeChnShrink(grayIntHist, srcR, srcC, SHRINK, grayImgShrink);

	delete[] pSrc;
}


//void filter(UInt8 *pImg, int imgR, int imgC){
//	int i = 0, j = 0;
//	for (j = 0; j < imgR; ++j){
//		for (i = 1; i < imgC - 1; ++i){
//			int pos = j*imgC + i;
//			pImg[pos] = (pImg[pos - 1] + pImg[pos] * 2 + pImg[pos + 1]) >> 2;
//		}
//	}
//}





