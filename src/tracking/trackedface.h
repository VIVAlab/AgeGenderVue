#ifndef TRACKEDFACE_H
#define TRACKEDFACE_H


#include <iostream>
#include <vector>
#include <queue>
#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

#include "flags.h"
#include "trackingstatus.h"
#include "utils.h"
#include "coloredrect.h"
#include "detectioninformation.h"
#include "detectionevent.h"

//Heuristics for false detections
#define TOO_BIG_DELTA 500
#define TOO_SMALL_DELTA 5

#define MINIMUM_SPOT_COUNT 3
#define IMMOBILE_FACE_FOR_FALSE 2

#define SPREAD_OUT_TICKS_FOR_FALSE 3
#define STOP_CHECKING_FOR_FALSE 5

//The duration to track after the last seen
//face is dependent on how long the face has been
//tracked. This is to minimize the time spent tracking
//false positives without discarding true detections
#define MIN_FRAME_TO_STOP 25
#define MAX_FRAME_TO_STOP 75
#define TRACK_FRAME_DURATION_INCREMENT 0

#define ADD_NEW_POINTS_THRESHOLD 100
#define MIN_MOVEMENT_FOR_TRACKED_POINT -1

//This constant defines the fraction of the size of the face
//to use as a border to not add new points, allowing
//us to detect when the face leaves without generating
//new tracking points
//ex : 4 means use 1/4 of the width of the detected face
#define BORDER_SIZE_RATIO   4

//1 box for every possible age
#define AGE_BOX_COUNT 4

#define EVENT_QUEUE_SIZE 10


class TrackedFace
{
public:
    //Construct the trackedface with the detection event, the list of all currently tracked points that
    //it could use for tracking and the status of each of these tracked points.
    TrackedFace(DetectionEvent& event);


    //Force update the position of the face with a confirmed detection
    void update(DetectionEvent& event);

    //Count the reference points in the given rect.
    int getPointsInRectCount();

    //Maintain the point tracking and detect false detections with some simple heuristics.
    //Should be called once per frame
    TrackingStatus tick(cv::Mat &gray, cv::Mat &gray_prev);

    //Return the current estimated location
    ColoredRect& getCurrentRect()
    {
        if (getCurrentGender())
        {
            currentLocation.setColor(cv::Scalar(255, 0, 0));
        }
        else
        {
            currentLocation.setColor(cv::Scalar(255, 0, 239));
        }

        return currentLocation;
    }

    double getDistanceFromRect(cv::Rect other)
    {
        int thisMiddleX = this->currentLocation.x + ((this->currentLocation.width - this->currentLocation.x) /2);
        int thisMiddleY = this->currentLocation.y + ((this->currentLocation.height - this->currentLocation.y) /2);

        int otherMiddleX = other.x + ((other.width - other.x) /2);
        int otherMiddleY = other.y + ((other.height - other.y) /2);

        int deltaX = thisMiddleX - otherMiddleX;
        int deltaY = thisMiddleY - otherMiddleY;

        return sqrt((deltaX * deltaX) + (deltaY * deltaY));
    }

    //Return the current best guess of the gender
    bool getCurrentGender()
    {
        while (lastDetections.size() > 15)
            lastDetections.pop_back();

        int maleVoteCount = 0;
        int femaleVoteCount = 0;

        for (int i = 0; i < lastDetections.size(); i++)
        {
            if (lastDetections[i].getGender() > 0)
                maleVoteCount++;
            else
                femaleVoteCount++;
        }



        return maleVotes > femaleVotes;
    }

    //Return the current best guess of the gender
    int getCurrentAge()
    {
        int bestAge = 0;

        for (int i = 0; i < AGE_BOX_COUNT; i++)
        {
            if (ageCounts[bestAge] < ageCounts[i])
                bestAge = i;

        }

        return bestAge;
    }

    //Return the information collected about this detection.
    //This includes first sighting, last sighting, gender and age
    DetectionInformation getDetectionInformation()
    {
        int bestAge = 0;

        for (int i = 0; i < AGE_BOX_COUNT; i++)
        {
            if (ageCounts[bestAge] < ageCounts[i])
                bestAge = i;
        }
        DetectionInformation info(start_time, stop_time, getCurrentGender(), bestAge);
        return info;
    }

    int getTotalTicks()
    {
        return totalTicks;
    }

    std::vector<cv::Point2f> getTrackedPoints()
    {
        return points[0];
    }

    std::string time_app;

private:

    bool updatePointFlag;

   // std::queue<DetectionEvent> recentEvents;

    static const int max_count = 500;
    static const float qlevel = 0.01;
    static const float min_dist = 3.;

    void detectFeaturePoints(cv::Mat& gray);

    //Check used to verify if adding points would be appropriate
    bool addNewPoints(cv::Mat& gray) {
        bool result = updatePointFlag && (points[0].size() <= ADD_NEW_POINTS_THRESHOLD);// && ((points[0].size() < max_count) || updatePointFlag);


        if (currentLocation.x - currentLocation.width/BORDER_SIZE_RATIO < 0
                || currentLocation.x + currentLocation.width * (BORDER_SIZE_RATIO*2 - 1)/BORDER_SIZE_RATIO > gray.cols
                || currentLocation.y - currentLocation.height/BORDER_SIZE_RATIO < 0
                || currentLocation.y + currentLocation.height * (BORDER_SIZE_RATIO*2 - 1)/BORDER_SIZE_RATIO  > gray.rows)
            updatePointFlag = false;
        else
            updatePointFlag = true;


        return result;
    }


    //Check that this is a good tracked point.
    bool acceptTrackedPoint(unsigned char status)
    {
        return status;// && (abs(points[0][i].x - points[1][i].x) + (abs(points[0][i].y - points[1][i].y)) > MIN_MOVEMENT_FOR_TRACKED_POINT);
    }


    //features is a list of good features to track
    std::vector<cv::Point2f> features;

    std::vector<unsigned char> status;
    std::vector<float> err;

    //points[0] is the previous set of points
    //points[1] is the new set of points
    std::vector<cv::Point2f> points[2];


    //Number of frames since a confirmed sighting
    int framesNotSpotted;

    //Frames without a spotting before losing the face
    int stopTrackingTreshold;

    //The current position of the tracked face
    ColoredRect currentLocation;

    //Number of frames that the face has not moved.
    //Will filter out certain false detections
    int notMoved;

    //Number of times that the face has been spotted.
    //Used in certain heuristics.
    int spotCount;

    //Number of votes for both genders
    int maleVotes;
    int femaleVotes;


    //Average movement of the face, used as a heuristic
    double averageDelta;

    //Total number of ticks that the face has been tracked
    int totalTicks;

    //Strings denoting the first and last spotting
    std::string start_time;
    std::string stop_time;

    //Age boxes
    int ageCounts[AGE_BOX_COUNT];


    std::vector<DetectionEvent> lastDetections;




};

#endif // TRACKEDFACE_H
