#pragma once

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\video\video.hpp>

#include "../FaceContent.h"
#include "facevue.h"

#include "../utils.h"

using namespace cv;

typedef enum
   {
      LOST, TRACKING, FALSE_DETECT
   } TrackingStatus;

#define TRACK_FRAME_DURATION_INCREMENT 0
#define MAX_FRAME_TO_STOP 75
#define IMMOBILE_FACE_FOR_FALSE 2
#define MINIMUM_SPOT_COUNT 3
#define ADD_NEW_POINTS_THRESHOLD 100
//This constant defines the fraction of the size of the face
//to use as a border to not add new points, allowing
//us to detect when the face leaves without generating
//new tracking points
//ex : 4 means use 1/4 of the width of the detected face
#define BORDER_SIZE_RATIO   4
#define MIN_FRAME_TO_STOP 15

class TrackedObject
{
public:
	TrackedObject(void);
	TrackedObject(FaceContent& faceRect, FaceVue* faceDobj, int id);
	~TrackedObject(void);

	
	TrackingStatus Track(Mat& currentFrame, Mat& previousFrame);
	void initialize();
	void updatePosition(FaceContent newPosition);
	FaceContent getFace();
	double getDistanceFromRect(cv::Rect other);
	FaceVue* facedetectorObj;
	int getId();
	bool getdetectioninf();
	void setdetectioninf(bool dtctornot);
	bool detectedORnot;

	int right_eye_x, right_eye_y;
	int left_eye_x, left_eye_y;
	int center_mouth_x, center_mouth_y;


private:
	int id;
	bool isThereanyFace(Mat& frame){
		Rect temp;
		
		(currentLocation.currentrect.x-currentLocation.currentrect.width/4>0)? temp.x=currentLocation.currentrect.x-currentLocation.currentrect.width/4:temp.x=0;
		(currentLocation.currentrect.y-currentLocation.currentrect.height/4>0)? temp.y=currentLocation.currentrect.y-currentLocation.currentrect.height/4:temp.y=0;

		(temp.x+6*currentLocation.currentrect.width/4<frame.cols)? temp.width=6*currentLocation.currentrect.width/4:temp.width=frame.cols-(temp.x);
		(temp.y+6*currentLocation.currentrect.height/4<frame.rows)? temp.height=6*currentLocation.currentrect.height/4:temp.height=frame.rows-(temp.y);
		if(temp.width==0 || temp.height==0)
			return false;
		
		Mat temp2=frame(temp);
		IplImage image = temp2;
		
		blank = cvCreateImage(cvSize(image.width,image.height),IPL_DEPTH_8U,1);
		
		vector<Rect> faces=facedetectorObj->detect_ALLFacesROI(&image,blank);
		int faces_size=-1;
		for(int i=0;i<faces.size();i++){
			if(!(faces[i].width<0.5*currentLocation.currentrect.width))
			{
				faces_size=1;
				break;
			}
			if(!(faces[i].height<0.5*currentLocation.currentrect.height))
			{
				faces_size=1;
				break;
			}
		}
		if(faces_size==-1)
			return false;


		if(faces.size()<=0)
			return false;
		else
			return true;
	}

	IplImage *blank;
	bool addNewPoints(cv::Mat& gray) ;
	
	std::vector<cv::Point2f> features;
	void detectFeaturePoints(cv::Mat &gray);
	bool acceptTrackedPoint(unsigned char status);

	Rect rect;
	FaceContent currentLocation;

	vector<FaceContent> lastDetections;
	vector<cv::Point2f> points[2];

	bool updatePointFlag;

	static int max_count;
	static float qlevel;
    static float min_dist;

	std::vector<unsigned char> status;
    std::vector<float> err;

	//Frames without a spotting before losing the face
    int stopTrackingTreshold;
	int stopTrackingTreshold_limit;
	int	framesNotSpotted;
    int spotCount;
	int notMoved;
	
	//Total number of ticks that the face has been tracked
    int totalTicks;

	string stop_time;
	string start_time;
	
};

