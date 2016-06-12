#ifndef COMPUTE_CHNS_FTR_H_
#define COMPUTE_CHNS_FTR_H_

void computeFtr(int srcR, int srcC, rect r, float *pChnFtr, int ftrDim);
int getFtrDim(int rows, int cols);

#endif