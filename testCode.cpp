/*该文件主要用于测试代码*/
#include"setting.h"
#include"myCLib.h"
#include"opencvLib.h"
#include"computeFtr.h"
#include"constructFtrIntHist.h"
#include<time.h>
#include<iostream>
#include<fstream>
using namespace std;
using namespace cv;


void testCode(){


}

/*测试computeFtr(ACF)正确结果保存在computeChnFtrTest.txt中*/
//void testCode(){
//
//	//freopen("computeChnFtrTest.txt", "a", stdout);
//	time_t now = time(NULL);
//	char buf[25];
//	strftime(buf, 24, "%Y-%m-%d %H:%M:%S", localtime(&now));
//	printf("%s\n", buf);
//
//	static float chnFtr[MAX_CHN_FTR_DIM];
//
//	const int IMG_R = 4;
//	const int IMG_C = 4;
//	UInt8 I[IMG_R*IMG_C] = 
//				  { 1, 2, 3, 4, 
//					3, 4, 7, 1, 
//					4, 8, 7, 10,  
//					2, 5, 12, 6 };
//
//	constructFtrIntHist(I, IMG_R, IMG_C);
//	int ftrDim = (IMG_R / SHRINK)*(IMG_C / SHRINK) + (IMG_R / SHRINK)*(IMG_C / SHRINK) +
//		            (IMG_R / BIN_SIZE)*(IMG_C / BIN_SIZE)*NUM_ORIENT;
//	rect r = { 0, 0, 4, 4 };
//	computeFtr(I, IMG_R, IMG_C, r,chnFtr,ftrDim);
//
//	int acc = 0;
//	for (int j = 0; j < NUM_ORIENT+ 2; j++)
//		printf("%f ",chnFtr[j+acc]);
//	printf("\n");
//
//	acc += 8;
//	for (int j = 0; j < NUM_ORIENT + 2; j++)
//		printf("%f ", chnFtr[j+acc]);
//	printf("\n");
//
//	acc += 8;
//	for (int j = 0; j < NUM_ORIENT + 2; j++)
//		printf("%f ", chnFtr[j + acc]);
//	printf("\n");
//
//	acc += 8;
//	for (int j = 0; j < NUM_ORIENT + 2; j++)
//		printf("%f ", chnFtr[j + acc]);
//	printf("\n");	
//	//fclose(stdout);
//}


/*测试constructGradHistIntHist.cpp正确结果保存在constructGradHistIntHistTest.txt中*/
//void testCode(){
//
//	freopen("constructGradHistIntHistTest.txt", "a", stdout);
//
//	const int IMG_R = 4;
//	const int IMG_C = 4;
//	UInt8 I[IMG_R*IMG_C] = { 1, 2, 3, 4,
//					3, 4, 7, 1,
//					4, 8, 7, 10, 
//					2, 5, 12, 6 };
//	float mag[IMG_R*IMG_C] = { .0f };
//	float ori[IMG_R*IMG_C] = { .0f };
//	computeGradient(I, IMG_R, IMG_C, mag, ori);
//	int i, j;
//	for (j = 0; j < IMG_R; j++){
//		for (i = 0; i < IMG_C; i++){
//			printf("%-5f ", mag[j*IMG_C + i]);
//		}
//		printf("\n");
//	}
//	printf("\n");
//	for (j = 0; j < IMG_R; j++){
//		for (i = 0; i < IMG_C; i++){
//			printf("%-5f ", ori[j*IMG_C + i]);
//		}
//		printf("\n");
//	}
//
//	constructGradHistIntHist(ori, mag, IMG_R, IMG_C);
//
//	double H[NUM_ORIENT] = {.0f};
//	
//	int y1[4] = { 0, 0, 2, 2 };
//	int x1[4] = { 0, 2, 0, 2 };
//
//	int y2[4] = { 1, 1, 3, 3 };
//	int x2[4] = { 1, 3, 1, 3 };
//	
//
//	int binSize = 2;
//
//	printf("\nH[k]:\n");
//	for (int i = 0; i < 4; i++){
//		x2[i]++; y2[i]++;
//		for (int k = 0; k < NUM_ORIENT; k++){
//			H[k] = gradHistIntArr[y2[i]][x2[i]][k] + gradHistIntArr[y1[i]][x1[i]][k] -
//				gradHistIntArr[y2[i]][x1[i]][k] - gradHistIntArr[y1[i]][x2[i]][k];
//			printf("%-5f ", H[k]);
//			//assert(H[k] >= 0);
//		}
//		printf("\n");
//	}
//	
//	fclose(stdout);
//}


///*测试computeGraident正确结果保存在computeGradientTtest.txt中*/
//void testCode(){
//
//	freopen("computeGradientTtest.txt", "a", stdout);
//
//	const int IMG_R = 4;
//	const int IMG_C = 4;
//	UInt8 I[IMG_R*IMG_C] = { 1, 2, 3, 4,
//					3, 4, 7, 1,
//					4, 8, 7, 10, 
//					2, 5, 12, 6 };
//	float mag[IMG_R*IMG_C] = { .0f };
//	float ori[IMG_R*IMG_C] = { .0f };
//	computeGradient(I, IMG_R, IMG_C, mag, ori);
//	int i, j;
//	for (j = 0; j < IMG_R; j++){
//		for (i = 0; i < IMG_C; i++){
//			printf("%-5f ", mag[j*IMG_C + i]);
//		}
//		printf("\n");
//	}
//	printf("\n");
//	for (j = 0; j < IMG_R; j++){
//		for (i = 0; i < IMG_C; i++){
//			printf("%-5f ", ori[j*IMG_C + i]);
//		}
//		printf("\n");
//	}
//	/*结果应该为*/
//	fclose(stdout);
//}