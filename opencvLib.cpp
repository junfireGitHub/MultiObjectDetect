#include"opencvLib.h"

void Mat2ImgPointer(const cv::Mat &src, unsigned char *pImg){
	/*该函数将Mat的数据提取出来转化为一个一维的数据，这里的src必须是单通道*/

	assert(src.channels() == 1);
	int row = src.rows;
	int col = src.cols;
	int i, j;

	for (j = 0; j < row; j++){
		for (i = 0; i < col; i++){
			pImg[j*col + i] = src.at<unsigned char>(j, i);
		}
	}
}

cv::Mat ImgPointer2Mat(const unsigned char *pImg, int imgR, int imgC){
	/*将一个一维的数据转化为Mat,Mat的通道数为1*/
	cv::Mat mat(cv::Size(imgC, imgR), CV_8UC1);
	int i, j;
	for (j = 0; j < imgR; j++){
		for (i = 0; i < imgC; i++){
			mat.at<unsigned char>(j, i) = pImg[j*imgC + i];
		}
	}
	return mat;
}