#include"myCLib.h"
#include"setting.h"
#include"sampleTest.h"
#include"sampleTrain.h"
#include"detectMultiScale.h"
#include"getHardFromNeg.h"
#include"videoTest.h"
#include"reduceCandidate.h"
#include"imgTest.h"
#include"testCode.h"

#include"opencvLib.h"
#include<iostream>
#include<fstream>
using namespace std;
using namespace cv;

int main(){

	//sampleTest();
	//sampleTrain();
	
	//getHardFromNeg(); /*detectMultiScale have  many scales*/
	
	//videoTest();

	imgTest();
	 

	system("pause"); return 0;
}