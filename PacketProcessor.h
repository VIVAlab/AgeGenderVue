#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include <queue>
#include <string>
#include <map>
#include "AGPacket.h"
#include "FaceContent.h"
#include "DetectTrack\DetectTrack.h"
#include "RecognitionCore\AGRecognitionCore.h"
#include <cassert>
#include <time.h>
#include "persistence\sqlitedb.h"
#include <thread>
#include "Firefly\videocapturefirefly.h"
#include <memory>

using namespace std;
#define _DEFINE_DEPRECATED_HASH_CLASSES 0
#define STACK_OVERFLOW_SIZE	10

class last_result{
public:
	last_result(void){}
	last_result(int id){
		faceID=id;
		gender=-1;
		age=-1;
	}
	
	~last_result(void){}
	int faceID;
	int gender;
	int age;

};

class Item{
public:
	Item(void){}
	~Item(void){}
	vector<FaceContent> rects;
	Mat frame;
	int	last_age;
	int last_gender;
};
class Item_AG{
public:
	Item_AG(void){}
	~Item_AG(void){}
	vector<AGPacket> rects_inf;
};

class PacketProcessor
{
public:
    PacketProcessor(void);
    ~PacketProcessor(void);
    bool flag;
private:
	clock_t init, final;
    string text_FT,text_FA, text_FS, text_MT, text_MA, text_MS;
	string currentGenderAge;
    queue<vector<AGPacket>> queue_inf;
    VideoCapture cap;
    int camera_type;
    VideoCaptureFirefly *firefly_camera;
    Mat frame;
    Mat grayFrame;
	queue<Item*> queue_face_AGEGENDER;
	SqliteDB* database;
    DetectTrack DT_obj;	
    AGRecognitionCore AG_obj;
    
    mutex mutex_1, mutex_2, mutex_3, mutex_4;
	Item* item;
    Item* item2;
	Item_AG* item_ag;
	Item_AG* item_ag2;
	bool init_time;
	map<int, int> DBB;
	map<int, last_result*> last_results;	

public:
	void Update_facesINframe(vector<FaceContent>& tmp){
		int indx, id;
		vector<int> toerase;
		auto it= last_results.begin();
		
		while(it!= last_results.end()){
			indx=-1;
			for(int j=0;j<tmp.size();j++){
				if(tmp[j].faceID==it->second->faceID){
					indx=1;
					break;
				}
			}
			if(indx==-1)
			{
				toerase.push_back(it->first);
				
			}
			it++;
		}

	for(int i=0;i<toerase.size();i++){
		delete last_results[toerase[i]];
		last_results.erase(toerase[i]);
	}


		for(int i=0;i<tmp.size();i++){
			if(last_results.find(tmp[i].faceID)==last_results.end()){
				last_result* a= new last_result(tmp[i].faceID);
				last_results[tmp[i].faceID]=a;
			}
		}
	}
	void Update_facesINframe(vector<AGPacket>& tmp){
		
		for(int i=0;i<tmp.size();i++){
			if(last_results.find(tmp[i].faceID)!=last_results.end()){
				last_results.find(tmp[i].faceID)->second->age=tmp[i].ageCatg;
				last_results.find(tmp[i].faceID)->second->gender=tmp[i].gender;
			}
		}
	}

    void putItem() {
        
		//cap>>frame;
        Mat img;
        if(camera_type == FIREFLY)
            firefly_camera->read(img);
       else if(camera_type == WEBCAM)
            cap.read(img);

       //input image is grayscale
       if (img.type() == CV_8UC1)  
            cvtColor(img, frame, CV_GRAY2RGB);
       else
            frame = img;

		flip(frame,frame,1);
		cvtColor(frame,grayFrame,CV_RGB2GRAY);
		item->frame=frame.clone();
		Mat gg=grayFrame.clone();
		item->rects=DT_obj.Frame_Process(item->frame,gg);
		if(item->rects.size()==0)
			goto SHOW;
		mutex_2.lock();
		Update_facesINframe(item->rects);
		mutex_2.unlock();
		mutex_1.lock();
		queue_face_AGEGENDER.push(item);
        mutex_1.unlock();
		
		SHOW: showFrame(item->rects);
		
    }
    

	void DB_Buffer_Pushback(vector<AGPacket> rects){
		for(int i=0;i<rects.size();i++){
			DBB[rects[i].faceID]=(rects[i].gender+1)*10+rects[i].ageCatg;
		}
	}

    void processItem() {
		
		vector<AGPacket> rects_inf;
        item2 = NULL;
		int size=0;
		mutex_1.lock();
		size=queue_face_AGEGENDER.size();
		mutex_1.unlock();
		if (size==0) {
			//mutex_1.unlock(); // If missing -> Candidate for a deadlock!
			return;
        } else {
			
			mutex_1.lock();
			if(queue_face_AGEGENDER.size()>STACK_OVERFLOW_SIZE)
			{
				for(int i=0;i<STACK_OVERFLOW_SIZE-1;i++)
					queue_face_AGEGENDER.pop();
			}
			item2 = queue_face_AGEGENDER.front();
			queue_face_AGEGENDER.pop();
			mutex_1.unlock();
			
			string currentTime=getCurrentDateTime();
			rects_inf= AG_obj.Face_Process(item2->frame,item2->rects);
			for(int i=0;i<rects_inf.size();i++)
				rects_inf[i].currentTime=currentTime;

				mutex_4.lock();
				DB_Buffer_Pushback(rects_inf);
				mutex_4.unlock();
		
			if(rects_inf.size()!=0){
			mutex_2.lock();
				Update_facesINframe(rects_inf);
			mutex_2.unlock();
			}
				
			
        }
        
    }
    
	int id_finder(int id, vector<AGPacket> tmp){
        for(int i=0;i<tmp.size();i++)
            if(tmp[i].faceID==id)
                return i;
        return -1;
    }
    
	void Write_DB(string _time){
		mutex_4.lock();
		map<int,int> tmp2=DBB;
		DBB.clear();
		mutex_4.unlock();
		
		int ft=0,mt=0,fa=0,ma=0,fs=0,ms=0;

		for (std::map<int,int>::iterator it=tmp2.begin(); it!=tmp2.end(); ++it)
			switch (it->second)
		{
			case 10:
				ft++;
				break;
			case 11:
				fa++;
				break;
			case 12:
				fs++;
				break;
			case 20:
				mt++;
				break;
			case 21:
				ma++;
				break;
			case 22:
				ms++;
			default:
				break;
		}

		this->database->writeINdb(_time,ft,fa,fs,mt,ma,ms);
		
	
	}

	void Timer_DB(){
		(init_time==true) && (init=clock()) &&  ( init_time=false);
		
		if(abs(clock()-init-10000)<100){
			string curr=getCurrentDateTime();
			Write_DB(curr);
			init=clock();
		}
	}
	
	void showFrame(vector<FaceContent> & face_rects) {
		
		int _age,_gender;
		if(face_rects.size()!=0)
        {
		    for (int i = 0; i < face_rects.size(); i++){

				Point p1 = Point(face_rects[i].currentrect.x, face_rects[i].currentrect.y);
                Point p2 = Point(face_rects[i].currentrect.x + face_rects[i].currentrect.width, face_rects[i].currentrect.y + face_rects[i].currentrect.height);
                rectangle(frame, p1, p2, Scalar(0,255,0), 3, 8, 0);
				
				if(last_results[face_rects[i].faceID]->age==-1)
					continue;
				_age=last_results[face_rects[i].faceID]->age;
				_gender=last_results[face_rects[i].faceID]->gender;
				    
                    Point p11 = cvPoint(face_rects[i].currentrect.x, face_rects[i].currentrect.y + face_rects[i].currentrect.height+40);
                    
                    Point bbul = Point(face_rects[i].currentrect.x, face_rects[i].currentrect.y + face_rects[i].currentrect.height);
                    Point bblr = Point(face_rects[i].currentrect.x + face_rects[i].currentrect.width, face_rects[i].currentrect.y + face_rects[i].currentrect.height+40);;
                    
                    rectangle(frame, bbul, bblr, cv::Scalar(0, 0, 0), -1);
                    double size = face_rects[i].currentrect.width/100.0;
                    
                    if (size < 0.5)     size = 0.5;
					
					if(_gender>0.5){
						if (_age < 0.5)
							currentGenderAge=text_MT+"";
						else if (_age < 1.5)
							currentGenderAge=text_MA+"";
						else if (_age > 1.5)
							currentGenderAge=text_MS+"";	
					}
                    else{
						if (_age < 0.5)
							currentGenderAge=text_FT+"";
						else if (_age < 1.5)
							currentGenderAge=text_FA+"";
						else if (_age > 1.5)
							currentGenderAge=text_FS+"";
					}
                    
                    putBoldedText(frame, currentGenderAge.c_str(), p11, size);
			}                
		}
		imshow("Window",frame);
		if(waitKey(1)=='q')
			flag=false;
    }
    
};