#ifndef OBJECTTRACKER_H
#define OBJECTTRACKER_H


#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "flags.h"
#include "facevue.h"
#include "trackedface.h"
#include "trackingstatus.h"

#include "utils.h"

#include "dbinterface.h"
#include "detectioninformation.h"
#include "detectionevent.h"
#include "coloredrect.h"

#include <algorithm>

//Constants used for face tracking
#define MULTIPLE_DETECT_DELTA 25

#define MAX_DIST_BETWEEN_DETECTS 150//75


class ObjectTracker
{


    public:
        //Default constructer has no persistence
        ObjectTracker();

        //This constructor uses the supplied DB for persistence
        ObjectTracker(DBInterface *database);


        //This should be called every time the frame is updated to
        //keep the tracking smooth
        void tick(Mat& frame);

        //This method updates the tracking with comfirmed face positions
        void updateTracking(std::vector<DetectionEvent> rects);


        //Returns the count of faces found since the creation of the object
        int getFaceCountSoFar(){
            return foundFacesTotal;
        }

        vector<DetectionInformation> getBestDetection_m()
        {
            int oldest = -1;
            vector<DetectionInformation> x;
            for(std::vector<TrackedFace*>::iterator it = trackedFaces.begin(); it != trackedFaces.end(); ++it)
            {

                (*it)->time_app=getCurrentDateTime_c();
                if ((*it)->getTotalTicks() ==15)
                {
                    oldest = (*it)->getTotalTicks();
                    x.push_back((*it)->getDetectionInformation());

                }
            }

            if (oldest == -1){
                x.push_back(DetectionInformation("0","0",false,-1));
                return x;
            }

            return x;
        }



        //Returns a rect for every tracked face
        std::vector<ColoredRect> getFaceRectangles()
        {

            std::vector<ColoredRect> rectangles;


            for(std::vector<TrackedFace*>::iterator it = trackedFaces.begin(); it != trackedFaces.end(); ++it)
            {
                ColoredRect& colorRect((*it)->getCurrentRect());

                rectangles.push_back(colorRect);

            }

            return rectangles;
        }


        //Return a bool for every tracked face, representing if we believe it to be male or female
        std::vector<bool> getGender()
        {
            std::vector<bool> sexes;
            for(std::vector<TrackedFace*>::iterator it = trackedFaces.begin(); it != trackedFaces.end(); ++it)
            {
                sexes.push_back((*it)->getCurrentGender());

            }
            return sexes;
        }


        //Return a bool for every tracked face, representing if we believe it to be male or female
        std::vector<int> getAges()
        {
            std::vector<int> ages;
            for(std::vector<TrackedFace*>::iterator it = trackedFaces.begin(); it != trackedFaces.end(); ++it)
            {
                ages.push_back((*it)->getCurrentAge());

            }
            return ages;
        }

        //Return the current number of tracked faces
        int getTrackedFaceCount(){  return trackedFaces.size(); }
        std::vector<TrackedFace*> trackedFaces;

        std::string getCurrentDateTime_c() {
            time_t     now = time(0);
            struct tm  tstruct;
            char       buf[80];
            tstruct = *localtime(&now);
            // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
            // for more information about date/time format
            strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

            return buf;
        }

    private:
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

        //Vector of tracked faces


        std::vector<int> matchRects(std::vector<DetectionEvent> rects);

        //Persistence unit
        DBInterface* db;

        //Current grayscale frame
        cv::Mat gray;

        //Previous grayscale frame
        cv::Mat gray_prev;

        //Total number of faces tracked during this run
        int foundFacesTotal;

        //Start tracking a new face
        void startTracking(DetectionEvent faceToTrack);

        //Update tracking an existing face
        void updateTracking(int faceToUpdate, DetectionEvent faceToTrack);

};

#endif // FACETRACKER_H