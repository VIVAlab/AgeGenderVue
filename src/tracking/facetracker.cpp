#include "facetracker.h"

FaceTracker::FaceTracker()
{
    drawing = false;
    lastAge = 0;
    lastGender = false;
    spotCount = 0;
}


void FaceTracker::init(bool enableDB, bool enableDrawing)
{
    //Face detection does not return coordinates for multiple faces
    TEMP = new LBPHISTOGRAM();


    snprintf(text_male,sizeof(text_male),"Male");

    snprintf(text_female,sizeof(text_female),"Female");

    //Load the face detection models
    face_obj = new FaceVue();
    face_obj->load_Detection_Model("Models//lbpcascade_frontalface.xml");
    face_obj->load_Landmark_Model("Models//flandmark_model.dat");


    drawing = enableDrawing;
    if (enableDB)
    {
        //Load the database or create a new one if it doesn't exist
        database = new SqliteDB("Database//results.db");

        //Initialize the facetrracker with a database so that it saves the results
        //within. If you initialize it without one, it won't save results.
        objectTracker = new ObjectTracker(database);
    }
    else
    {
       objectTracker = new ObjectTracker();
    }


     blank = NULL;

     facecountLocation = cv::Point(10, 100);
}


cv::Mat FaceTracker::track(cv::Mat frame)
{



    //Do some conversions for various different functions
    cvtColor(frame,grayFrame,CV_RGB2GRAY);

    IplImage image = frame;
    IplImage greyImage = grayFrame;

    //Rects to store all detected faces
    std::vector<Rect> rects;

    //Free the blank image used the previous loop
    //If it's already empty it null-ops
    cvReleaseImage(&blank);

    //Create an empty image to send to the face detector
    blank = cvCreateImage(cvSize(image.width,image.height),IPL_DEPTH_8U,1);
    //Detects probable face locations
    rects = face_obj->detect_ALLFacesROI(&image, blank);
    //Face tracking
    if(rects.size())
    {
        //The ages are currently stubbed
        std::vector<int > stubbedAgeVector;
        std::vector<double> genderVector;

        //Loop through the rects and make sure that they are all actually faces
        //Remove the false positives
        int k = 0;
        for (int i = 0; i < rects.size(); i++)
        {
            double result = -1;
            int rect[4];
            rect[0] = rects[i].x;
            rect[1] = rects[i].y;
            rect[2] = rects[i].x+ rects[i].width;
            rect[3] = rects[i].y+ rects[i].height;

            face_obj->my_Landmark(&greyImage, &result, rect);

            //Is the face a true detection
            if (result > face_obj->detection_threshold)
            {


                //Manually construct a face. This should be in face_api
                FaceVue::FaceContent currentFace;
                currentFace.left_eye_x = (face_obj->landmarks[4]+face_obj->landmarks[12])/2;
                currentFace.left_eye_y = (face_obj->landmarks[5]+face_obj->landmarks[13])/2;
                currentFace.right_eye_x = (face_obj->landmarks[2]+face_obj->landmarks[10])/2;
                currentFace.right_eye_y = (face_obj->landmarks[3]+face_obj->landmarks[11])/2;
                currentFace.mouth_x = (face_obj->landmarks[6]+face_obj->landmarks[8])/2;
                currentFace.mouth_y = (face_obj->landmarks[7]+face_obj->landmarks[9])/2;

                //Align the face
                int genderVal=0;
                int ageVal=0;

                warp_dst=TEMP->rotate(
                            &grayFrame,
                            currentFace.right_eye_x,
                            currentFace.right_eye_y,
                            currentFace.left_eye_x,
                            currentFace.left_eye_y
                            );

                //Verify the gender of the face
                if(TEMP->is_rotated)
                {
                    genderVal=TEMP->gender_predictor(&warp_dst);
                    ageVal=TEMP->age_predictor(&warp_dst);

                }
                else
                {
                    continue;
                }

                //In-place technique to remove false detects
                rects[k++] = rects[i];


                //Push back a stubbed age value
                stubbedAgeVector.push_back(ageVal);



                //Push back the detected gender
                genderVector.push_back(genderVal);
            }
        }
        //Remove all the false detects
        rects.resize(k);

        //With all the true detections, generate a detection Event and pass it to
        //the faceTracker to update the positions and information of all tracked
        //faces
        objectTracker->updateTracking(DetectionEvent::generate(rects, stubbedAgeVector, genderVector));
    }
    //Update the tracking points of the face tracker
    objectTracker->tick(frame);
    if (drawing)
    {
    //Get the rectangles of tracked faces, as well as their genders
    std::vector<ColoredRect> rectangles = objectTracker->getFaceRectangles();
    std::vector<bool> males = objectTracker->getGender();
    std::vector<int> ages = objectTracker->getAges();


    //Draw each tracked rectangle
    for (int i = 0; i < rectangles.size(); i++)
    {
        CvPoint p1 = cvPoint(rectangles[i].x, rectangles[i].y);
        CvPoint p2 = cvPoint(rectangles[i].x + rectangles[i].width, rectangles[i].y + rectangles[i].height);


        height = image.height;
        width = image.width;
        channels = image.nChannels;

        rectangle(frame, p1, p2, rectangles[i].getColor(), 3, 8, 0);
        CvPoint p11 = cvPoint(rectangles[i].x, rectangles[i].y + rectangles[i].height+20);
        CvPoint p22 = cvPoint(rectangles[i].x + rectangles[i].width/2, rectangles[i].y + rectangles[i].height+20);

        CvPoint bbul = cvPoint(rectangles[i].x, rectangles[i].y + rectangles[i].height);
        CvPoint bblr = cvPoint(rectangles[i].x + rectangles[i].width, rectangles[i].y + rectangles[i].height+40);;

        rectangle(frame, bbul, bblr, cv::Scalar(0, 0, 0), -1);

        double size = rectangles[i].width/100.0;

        if (size < 0.5) size = 0.5;

        if(males[i])
        {
            putBoldedText(frame, text_male, p11, size);
        }
        else
        {
            putBoldedText(frame, text_female, p11, size);
        }

        char currentAge[100];

        if (ages[i] < 1.5)
        {
            snprintf(currentAge,sizeof(currentAge),"Teen");
        }

        else if (ages[i] < 2.5)
        {
            snprintf(currentAge,sizeof(currentAge),"Adult");
        }

        else if (ages[i] > 2.5)
        {
            snprintf(currentAge,sizeof(currentAge),"Senior");
        }




        putBoldedText(frame, currentAge, p22, size);
    }
    std::stringstream ss;
    ss << objectTracker->getFaceCountSoFar();

    }


    return frame;


}
