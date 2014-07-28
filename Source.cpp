#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "DetectTrack\DetectTrack.h"
#include <vector>
#include "FaceContent.h"
#include "RecognitionCore\AGRecognitionCore.h"
#include <thread>
#include "PacketProcessor.h"

/*
using namespace cv;

DetectTrack DT_obj;	
AGRecognitionCore AG_obj;
				
void showFrame(Mat& frame, vector<FaceContent> rects, vector<AGPacket> rects_inf){
    //Draw each tracked rectangle
    for (int i = 0; i < rects.size(); i++)
    {

		string currentGender,currentAge;

		Point p1 = Point(rects[i].currentrect.x, rects[i].currentrect.y);
        Point p2 = Point(rects[i].currentrect.x + rects[i].currentrect.width, rects[i].currentrect.y + rects[i].currentrect.height);
        
		Point p11 = cvPoint(rects[i].currentrect.x, rects[i].currentrect.y + rects[i].currentrect.height+40);
		
		Point bbul = Point(rects[i].currentrect.x, rects[i].currentrect.y + rects[i].currentrect.height);
        Point bblr = Point(rects[i].currentrect.x + rects[i].currentrect.width, rects[i].currentrect.y + rects[i].currentrect.height+40);;

            rectangle(frame, bbul, bblr, cv::Scalar(0, 0, 0), -1);
		    double size = rects[i].currentrect.width/100.0;

            if (size < 0.5)     size = 0.5;
			if(rects_inf[i].gender)
				currentGender="Male ";
            else
				currentGender="Female ";

			if (rects_inf[i].ageCatg < .5)
				currentAge="Teen";
            else if (rects_inf[i].ageCatg < 1.5)
				currentAge="Adult";
            else if (rects_inf[i].ageCatg > 1.5)
				currentAge="Senior";
				
			currentGender.append(currentAge);
			putBoldedText(frame, currentGender.c_str(), p11, size);
			
			rectangle(frame, p1, p2, Scalar(0,255,0), 3, 8, 0);

    }
    
    imshow("Window",frame);
    waitKey(12);
}


void DetectFaces(vector<FaceContent>& rects, Mat frame, Mat grayFrame){
	rects=DT_obj.Frame_Process(frame,grayFrame);
}

void AgeGenderRecognition(vector<AGPacket> rects_inf,vector<FaceContent>& rects, Mat frame){
		rects_inf= AG_obj.Face_Process(frame,rects);
}


void run(){
    
    VideoCapture cap;
    Mat frame;
    Mat grayFrame;
    
    cap.open(0);
    cap>>frame;
    vector<FaceContent> rects;
	vector<AGPacket> rects_inf;

    while(true){
        
        cap>>frame;
        flip(frame,frame,1);
		Mat grayFrame;
		cvtColor(frame,grayFrame,CV_RGB2GRAY);
		rects=DT_obj.Frame_Process(frame,grayFrame);
		for(int i=0;i<rects.size();i++)
			cout<<"r"<<i<<":"<<rects[i].faceID<<"-";
		cout<<endl;
		rects_inf= AG_obj.Face_Process(frame,rects);
        showFrame(frame,rects,rects_inf);
        cvReleaseImage(&DT_obj.blank);
    }
}



void main(){
	run();
	
}
*/
PacketProcessor a;
void produce(){
	while(true)
 		a.putItem();
}
void consume(){
	while(true)
		a.processItem();
}

void writer(){
	while(true)
		a.Timer_DB();
}


int main(){
	thread aa(produce);
	thread bb(consume);
	//thread cc(writer);
	char p;
	cin.get(p);
	aa.join();
	bb.join();
	//cc.join();
	//bb.join();

	//cc.join();
	return 1;
}

