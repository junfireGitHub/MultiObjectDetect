#ifndef CONSTRUCT_GRAD_HIST_INT_HIST
#define CONSTRUCT_GRAD_HIST_INT_HIST
#pragma once

#include"opencvLib.h"

#define SHRINK (4u)
#define BIN_SIZE (4u)
#define NUM_ORIENT (6u)

//void constructFtrIntHist(const UInt8 *pSrc, int srcR, int srcC);
void constructFtrIntHist(const cv::Mat& mat);

//extern float grayImgShrink[MAX_IN_IMG_SIZE / SHRINK / SHRINK];
//extern float gradMagShrink[MAX_IN_IMG_SIZE / SHRINK / SHRINK];
//extern float gradHistShrink[(MAX_IN_IMG_R / BIN_SIZE)*(MAX_IN_IMG_C / BIN_SIZE)][NUM_ORIENT];

#endif