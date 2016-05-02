#ifndef CONSTRUCT_GRAD_HIST_INT_HIST
#define CONSTRUCT_GRAD_HIST_INT_HIST
#pragma once

void constructFeatureIntHist(const UInt8 *pSrc, int srcR, int srcC);

//extern double gradHistIntArr[MAX_IN_IMG_R + 1][MAX_IN_IMG_C + 1][NUM_ORIENT];
extern float grayImgShrink[MAX_IN_IMG_SIZE / SHRINK / SHRINK];
extern float gradMagShrink[MAX_IN_IMG_SIZE / SHRINK / SHRINK];
extern float gradHistShrink[(MAX_IN_IMG_R / BIN_SIZE)*(MAX_IN_IMG_C / BIN_SIZE)][NUM_ORIENT];

#endif