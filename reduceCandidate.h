#ifndef REDUCE_CANDIDATE_H_
#define REDUCE_CANDIDATE_H_

#include"opencvLib.h"

void processDisparity(const cv::Mat &src);
bool judgeCandidate(int startY, int startX, int winR, int winC);

#endif