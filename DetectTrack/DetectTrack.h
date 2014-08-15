#pragma once
#include <iostream>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "facevue.h"
#include "FaceTracker.h"
#include "../FaceContent.h"
using namespace cv;

class DetectTrack
{
public:
	DetectTrack(void);
	~DetectTrack(void);
	//void run();
	vector<FaceContent> DetectFaces(Mat frame);

	FaceVue* faceObj;
	FaceTracker* trackerObj;
	//void showFrame(Mat frame, vector<FaceContent> rects);
	vector<FaceContent> Frame_Process(Mat& frame, Mat& grayFrame);
	IplImage* blank;
private:
	
};

