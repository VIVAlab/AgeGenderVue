#include "PacketProcessor.h"
#include "persistence\sqlitedb.h"

PacketProcessor::PacketProcessor(void)
{
	text_FT="Female Teen";
	text_FA="Female Adult";
	text_FS="Female Senior";
	text_MT="Male Teen";
	text_MA="Male Adult";
	text_MS="Male Senior";
	item=new Item();
	item_ag=new Item_AG();

    firefly_camera = new VideoCaptureFirefly();
    //find and open a firefly camera
    camera_type = (firefly_camera->open(0) ? FIREFLY : WEBCAM);
	    
    if (camera_type == WEBCAM) // if firefly camera is not available, then
        cap.open(0);           // open the default webcam

    if(camera_type == WEBCAM && !cap.isOpened()) // web camera is not present
    {   
        camera_type = OTHER;
        cap.release();
    }

    Mat img;
	flag=true;
    if(camera_type == FIREFLY)
        firefly_camera->read(img);
    else if(camera_type == WEBCAM)
        cap.read(img);
    
     //input image is grayscale
     if (img.type() == CV_8UC1)  
        cvtColor(img, frame, CV_GRAY2RGB);
     else
        frame = img;
 
    init_time=true;
	string t=getCurrentDateTime();
	database = new SqliteDB(t);
}


PacketProcessor::~PacketProcessor(void)
{
    if(camera_type == FIREFLY)
       delete firefly_camera;
	flag=false;
}
