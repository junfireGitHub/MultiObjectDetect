#ifndef _CIRCLEPERSON_
#define _CIRCLEPERSON_
#include"opencvLib.h"
#include"myCLib.h"

typedef enum ObjectType{
	 CAR, BM, ROAD, PERSON
}ObjectType; 

typedef struct Bbox{
	cv::Rect pos;//x,y,width,height
	float score;
	ObjectType obType;
}Bbox;

void bbNmsMultiClass(cv::Mat& src, cv::vector<Bbox>& bb, bool isPostPro);
void bbNms(cv::Mat& src, cv::vector<cv::Rect> found, bool isPostPro);
void bbNmsMax(cv::Mat& src, cv::vector<cv::Rect> found, cv::vector<float> score, bool isPostPro);

#endif