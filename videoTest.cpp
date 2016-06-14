#include"myCLib.h"
#include"computeFtr.h"
#include"constructFtrIntHist.h"
#include"sampleTest.h"
#include"sampleTrain.h"
#include"detectMultiScale.h"
#include"getHardFromNeg.h"
#include"videoTest.h"

void detect(Mat &img, Mat &out){
	// Convert to gray
	//Mat img2(img, Rect(0, 0, img.cols, img.rows));
	//cv::cvtColor(img2, out, CV_BGR2GRAY);
	//resize(out, out, Size(out.cols / 1.5, out.rows / 1.5));
	//DetectOpt detectOpt = { 128, 64, 8, 8, 1.2, false/*isUseDp*/, true/*isPostPro*/, false/*isGetHard*/ };
	//detectOpt.isUseDp = false; 
	//detectOpt.isGetHard = false;
	////detectMultiClassifier(out, out.rows, out.cols, detectOpt);
	//detectMultiScale(out, out.rows, out.cols, detectOpt);
	out = img(Rect(0, 0, img.cols/2, img.rows));
	const int ROAD = 0, CAR_BM = 1, PERSON = 2;
	DetectOpt detectOpt[3];
	detectOpt[ROAD] = { 32, 32, 32, 32, 1.2, false, false, false };
	detectOpt[CAR_BM] = { 96, 128, 8, 8, 1.2, false, false, false };
	detectOpt[PERSON] = { 128, 64, 8, 8, 1.2, false, false, false };
	detect(out, detectOpt);
}

/*CHECK LIST*/
//1. detect
//2. only_show
//3. path

void videoTest()
{
    #define ONLY_SHOW true  //if false, output video will be saved

	string videoPath = "D:\\MyProject\\MultiObjectDetect-v3.5\\testVideo\\º”¬Àπ‚∆¨\\20160614155834.avi";
	string videoOutPath = "D:\\testVideo\\ydp_test2_result.avi";
    
	VideoProcessor processor;
	processor.setInput(videoPath);// Open video file
	if (ONLY_SHOW){
		// Declare a window to display the video
		processor.displayInput("Input Video");
		processor.displayOutput("Output Video");
		// Play the video at the original frame rate
		processor.setDelay(1000. / processor.getFrameRate());
	}

	processor.setFrameProcessor(detect);// Set the frame processor callback function

	if (!ONLY_SHOW){
		string path = videoOutPath;
		processor.setOutput(path, -1);
	}
	processor.run();// Start the process
	cv::waitKey();

	system("pause"); exit(0);
}
