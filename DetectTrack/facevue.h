#ifndef FACEVUE_H
#define FACEVUE_H

#include <fstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "flandmark_detector.h"
#include "cascadedetect.hpp"

using namespace cv;
using namespace std;

//Struct for Loading the model
typedef struct model {
    double * W;

} Model;

//FaceVue class for recording information of each face which has been deteceted
class FaceVue {
public:

    struct FaceContent2
    {
        int left_eye_x;
        int left_eye_y;

        int right_eye_x;
        int right_eye_y;

        int mouth_x;
        int mouth_y;

        int p1_x;
        int p1_y;
        int p2_x;
        int p2_y;

        int index;
    };

    //Constructor
    FaceVue();

    //Destructors
    ~FaceVue();

    //aligned checking
    bool is_aligned;
    bool is_Face_Found;

    //threshhold for face detection
    double detection_threshold;

    //Load cascade model
    bool load_Detection_Model(const string &filename);

	
    //Load flandmark model
    bool load_Landmark_Model(const string &filename);

    //Return regions of detected faces
    vector<Rect> detect_ALLFacesROI(const IplImage *frame, IplImage* input);

    //Return region of detected face and target_Face is filled
    CvRect detect_FaceROI(const IplImage* frame);

    //Return aligned face
    IplImage* align_Face(const IplImage* frame,const CvRect &faceROI, IplImage* warp_dst);

    //Compute Landmark
    void my_Landmark(IplImage* input,double* value,int *bbox);

    //Object of FaceContent class for recording and processing of each face which has been detected
    FaceContent2 *target_Face;

    double *landmarks;


private:

    int *bbox;

    //Detection model
    CascadeClassifier *detection_Model;

	

    //Alignment model
    FLANDMARK_Model *landmark_Model;

};

#endif //FACEVUE_H
