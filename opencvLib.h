#ifndef OPEN_CV_LIB_H_
#define OPEN_CV_LIB_H_

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/objdetect/objdetect.hpp>  
#include <opencv2/ml/ml.hpp> 

void Mat2ImgPointer(const cv::Mat &src, unsigned char *pImg);
cv::Mat ImgPointer2Mat(const unsigned char *pImg, int imgR, int imgC);

#endif