#include"opencvLib.h"

void Mat2ImgPointer(const cv::Mat &src, unsigned char *pImg){
	/*�ú�����Mat��������ȡ����ת��Ϊһ��һά�����ݣ������src�����ǵ�ͨ��*/

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
	/*��һ��һά������ת��ΪMat,Mat��ͨ����Ϊ1*/
	cv::Mat mat(cv::Size(imgC, imgR), CV_8UC1);
	int i, j;
	for (j = 0; j < imgR; j++){
		for (i = 0; i < imgC; i++){
			mat.at<unsigned char>(j, i) = pImg[j*imgC + i];
		}
	}
	return mat;
}