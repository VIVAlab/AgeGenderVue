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


    DetectionInformation getBestDetection()
    {
        DetectionInformation best = objectTracker->getBestDetection();



        if (bestSpotTime != best.getStartTime())
        {
            bestSpotTime = best.getStartTime();
            spotCount = 0;
        }
        else
        {
            if ((lastAge != best.getAge() || lastGender != best.getGender()) && spotCount < 15)
            {
                lastAge = best.getAge();
                lastGender = best.getGender();
                spotCount = 0;

            }
            spotCount++;
        }



        if (spotCount == 15)
        {
            return best;
        }

        return DetectionInformation("0", "0", false, -1);
    }



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

    ObjectTracker* objectTracker;

    Mat grayFrame;//alternate video frame
    Mat warp_dst;
    IplImage* blank;

     int height, width, channels;

    bool drawing;


    SqliteDB* database;

};

#endif // FACETRACKER_H
