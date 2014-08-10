#pragma once
#include <iostream>
#include <vector>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "FaceObject.h"
#include "TrackedObject.h"
#include "../FaceContent.h"
using namespace std;
using namespace cv;

#define MAX_DIST_BETWEEN_DETECTS 150//75
#define MULTIPLE_DETECT_DELTA 25

class FaceTracker
{
public:
	FaceTracker(FaceVue* faceDobj);
	~FaceTracker(void);
	
	void InitializeTracker();
	vector<FaceContent> Track(Mat& frame, Mat& greyImage, vector<FaceContent> rects);
	void startTracking(FaceContent faceToTrack);
	std::vector<int> matchRects(std::vector<FaceContent> rects);

	vector<TrackedObject*> trackedFaces;

	Mat gray_prev;
	vector<FaceContent> GetFaces();
	int foundFacesTotal;
	FaceVue* facedetectorObj;
private:
	
	int id_counter;
	Mutex mutex_1;

	void removingOverlappedRects(vector<FaceContent> &rects);

	double getDistanceBetweenRectsRect(cv::Rect first,cv::Rect other)
        {

            int thisMiddleX = first.x + ((first.width) /2);
            int thisMiddleY = first.y + ((first.height) /2);

            int otherMiddleX = other.x + ((other.width) /2);
            int otherMiddleY = other.y + ((other.height) /2);

            int deltaX = thisMiddleX - otherMiddleX;
            int deltaY = thisMiddleY - otherMiddleY;



            return sqrt((deltaX * deltaX) + (deltaY * deltaY));
        }
};

