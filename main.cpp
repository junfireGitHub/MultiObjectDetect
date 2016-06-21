#include"myCLib.h"
#include"setting.h"
#include"sampleTest.h"
#include"sampleTrain.h"
#include"detectMultiScale.h"
#include"getHardFromNeg.h"
#include"videoTest.h"
#include"reduceCandidate.h"
#include"imgTest.h"
#include"opencvLib.h"
#include<iostream>
#include<fstream>
using namespace std;
using namespace cv;

int main(){
	cout << "1 : imgTest()" << endl;
	cout << "2 : imgTestDp()" << endl;
	cout << "3 : videoTest()" << endl;
	cout << "4 : sampleTrain()" << endl;
	cout << "5 : getHardFromNeg()" << endl;
	cout << "6 : sampleTest()" << endl;
	cout << "7 : testCode()" << endl;
	cout << endl;
	cout << "please input right number to run corresponding program" << endl;

	int num; cin >> num;
	switch (num){
		case 1: imgTest(); break;
		case 2: imgTestDp(); break;
		case 3: videoTest(); break;
		case 4: sampleTrain(); break;
		case 5: getHardFromNeg(); break;
		case 6: sampleTest(); break;
		default: exit(-1);
	}
	 
	system("pause"); return 0;
}

