#include "myCLib.h"

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