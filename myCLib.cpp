#include "myCLib.h"

void printRect(rect rec)
{
	printf("x:%d\n", rec.x);
	printf("y:%d\n", rec.y);
	printf("height:%d\n", rec.height);
	printf("width:%d\n\n", rec.width);
}


///*缩放图像函数，该函数对最后检测结果有一定影响*/
///*有一种方法可以加快该函数的计算，暂时不采用*/
void imResizeUInt8(
	const UInt8 *img, 
	uint imgR, uint imgC,
	uint afterZoomR, uint afterZoomC, 
	float *afterZoomImg)
{
	uint r, c;
	float posR, posC;
	float y, x;
	uint posRInt, posCInt;
	uint indLu, indLd, indRu, indRd;
	float ratioR = imgR / (float)afterZoomR;
	float ratioC = imgC / (float)afterZoomC;  //CHANGE HERE TODO         
	for (r = 0; r < afterZoomR; r++){
		for (c = 0; c < afterZoomC; c++)
		{
			posR = (r + 0.5)*ratioR - 0.5; //加减0.5        //CHANGE HERE TODO 
			posC = (c + 0.5)*ratioC - 0.5;
			posRInt = (uint)posR;  posCInt = (uint)posC;
			y = posR - posRInt;   x = posC - posCInt;
			indLu = posRInt*imgC + posCInt;  indRu = indLu + 1;
			indLd = indLu + imgC;  indRd = indLd + 1;
			//assert(img[indLu] * (1 - x)*(1 - y) +
			//	img[indLd] * y*(1 - x) + img[indRu] * (1 - y)*x + img[indRd] * x*y <= 256);
			afterZoomImg[c + r*afterZoomC] = img[indLu] * (1 - x)*(1 - y) +
				img[indLd] * y*(1 - x) + img[indRu] * (1 - y)*x + img[indRd] * x*y;
		}
	}
}

void imResizeFloat(
	const float *img,
	uint imgR, uint imgC,
	uint afterZoomR, uint afterZoomC,
	float *afterZoomImg)
{
	uint r, c;
	float posR, posC;
	float y, x;
	uint posRInt, posCInt;
	uint indLu, indLd, indRu, indRd;
	float ratioR = imgR / (float)afterZoomR;
	float ratioC = imgC / (float)afterZoomC;  //CHANGE HERE TODO         
	for (r = 0; r < afterZoomR; r++){
		for (c = 0; c < afterZoomC; c++)
		{
			posR = (r + 0.5)*ratioR - 0.5; //加减0.5        //CHANGE HERE TODO 
			posC = (c + 0.5)*ratioC - 0.5;
			posRInt = (uint)posR;  posCInt = (uint)posC;
			y = posR - posRInt;   x = posC - posCInt;
			indLu = posRInt*imgC + posCInt;  indRu = indLu + 1;
			indLd = indLu + imgC;  indRd = indLd + 1;
			//assert(img[indLu] * (1 - x)*(1 - y) +
			//	img[indLd] * y*(1 - x) + img[indRu] * (1 - y)*x + img[indRd] * x*y <= 256);
			afterZoomImg[c + r*afterZoomC] = img[indLu] * (1 - x)*(1 - y) +
				img[indLd] * y*(1 - x) + img[indRu] * (1 - y)*x + img[indRd] * x*y;
		}
	}
}

void outputImg(const UInt8 *img, uint imgR, uint imgC){
	int i, j;
	for (j = 0; j < imgR; j++){
		for (i = 0; i < imgC; i++){
			printf("%-4d ", img[j*imgC + i]);
		}
		printf("\n");
	}
}


void drawRect(UInt8 *img, int imgR, int imgC, rect r){
	int i, j, index;
	int colorVal = 255;
	int thickness = 1;

	for (j = r.y; j < r.y + r.height; j++){
		index = j*imgC + r.x;
		img[index] = colorVal;
		img[index + r.width - thickness] = colorVal;

	}

	//set two rows to coloVal
	memset(&img[r.y*imgC + r.x], colorVal, r.width);
	memset(&img[(r.y + r.height - thickness)*imgC + r.x], colorVal, r.width);
}