#include "DetectTrack.h"


DetectTrack::DetectTrack(void)
{
    faceObj= new FaceVue();
    faceObj->load_Detection_Model("Models//haarcascade_frontalface_alt2.xml");
	faceObj->load_Landmark_Model("Models//flandmark_model.dat");
    trackerObj= new FaceTracker(faceObj);
    trackerObj->InitializeTracker();
}

DetectTrack::~DetectTrack(void)
{
    cvReleaseImage(&blank);
}

vector<FaceContent> DetectTrack::DetectFaces(Mat frame){
    
    
    IplImage image = frame;
	vector<FaceContent> facesContents;
    blank = cvCreateImage(cvSize(image.width,image.height),IPL_DEPTH_8U,1);
    vector<Rect> faces=faceObj->detect_ALLFacesROI(&image, blank);
	for(int i=0;i<faces.size();i++)
	{
		FaceContent tmp(faces[i]);
		facesContents.push_back(tmp);
	}
    //blank=NULL;
    //Detects probable face locations
	return facesContents;
}

vector<FaceContent> DetectTrack::Frame_Process(Mat& frame,Mat& grayFrame){    
    vector<FaceContent> rects;
	vector<FaceContent> facesinf;
	IplImage greyImage=grayFrame.clone();
    
    
    rects = DetectFaces(frame);
	//Face tracking
    if(rects.size())
    {
        
        //Loop through the rects and make sure that they are all actually faces
        //Remove the false positives
        int k = 0;
        for (int i = 0; i < rects.size(); i++)
        {
            double result = -1;
            int rect[4];
			rect[0] = rects[i].currentrect.x;
            rect[1] = rects[i].currentrect.y;
            rect[2] = rects[i].currentrect.x+ rects[i].currentrect.width;
            rect[3] = rects[i].currentrect.y+ rects[i].currentrect.height;
            
            faceObj->my_Landmark(&greyImage, &result, rect);
            
            //Is the face a true detection
            if (result > faceObj->detection_threshold)
            {
                //Manually construct a face. This should be in face_api
                
                int left_eye_x = (faceObj->landmarks[4]+faceObj->landmarks[12])/2;
                int left_eye_y = (faceObj->landmarks[5]+faceObj->landmarks[13])/2;
                int right_eye_x = (faceObj->landmarks[2]+faceObj->landmarks[10])/2;
                int right_eye_y = (faceObj->landmarks[3]+faceObj->landmarks[11])/2;
                int mouth_x = (faceObj->landmarks[6]+faceObj->landmarks[8])/2;
                int mouth_y = (faceObj->landmarks[7]+faceObj->landmarks[9])/2;
				
				rects[i].setFlandmarks(right_eye_x, right_eye_y, left_eye_x, left_eye_y);
                rects[k++] = rects[i];
                
            }
        }
        //Remove all the false detects
        rects.resize(k);
        
    }
    
    facesinf=trackerObj->Track(frame,grayFrame,rects);
	cvReleaseImage(&blank);
    return facesinf;
}