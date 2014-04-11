#ifndef FACETRACKER_H
#define FACETRACKER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "detectioninformation.h"
#include "flags.h"

#include "flags.h"
#include "facevue.h"
#include "faceRecognition.h"
#include "histogram1d.h"

#include "objecttracker.h"

#include "sqlitedb.h"


class FaceTracker
{
public:
    FaceTracker();

    void init(bool enableDB, bool enableDrawing);
    cv::Mat track(cv::Mat frame);

    vector<TrackedFace*> getAllDetection(){
        return objectTracker->trackedFaces;
    }

    vector<DetectionInformation> getBestDetection_m(){


        vector<DetectionInformation> bestv = objectTracker->getBestDetection_m();

        return bestv;
    }

    ObjectTracker* objectTracker;


private:

    int spotCount;
    std::string bestSpotTime;

    LBPHISTOGRAM* TEMP;

    bool lastGender;
    int lastAge;

    cv::Point facecountLocation;

    //Text for displaying the sex of the identified face
    char text_male[1024];
    char text_female[1024];
    FaceVue* face_obj;



    Mat grayFrame;//alternate video frame
    Mat warp_dst;
    IplImage* blank;

    int height, width, channels;

    bool drawing;


    SqliteDB* database;

};

#endif // FACETRACKER_H
