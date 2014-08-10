#ifndef VIDEOCAPTUREFIREFLY_H
#define VIDEOCAPTUREFIREFLY_H

#include <iostream>
#include "FlyCapture2.h"
#include "opencv2/core/core.hpp"
#include "opencv/cv.h"

using namespace std;
using namespace FlyCapture2;
using namespace cv;

enum CameraType {FIREFLY, WEBCAM, OTHER};

// This class allows to capture video from firefly cameras and transform the frames into
// opencv format.
//
// To use this class in your opencv project, include the following library file.
// FlyCapture2d_v100.lib
//
// Here is how the class can be used.
//
//      VideoCaptureFirefly cap;
//      cap.open(0);
//      if( !cap.is_firefly_camera_present() )
//          return -1;
//
//      for(;;)
//      {
//          Mat frame;
//          cap.read(frame);
//          imshow("video", frame);
//          if(waitKey(1) >= 0) break;
//       }
class VideoCaptureFirefly
{

private:
	Error error;
	BusManager busMgr;
	unsigned int numCameras;
	PGRGuid guid;
	Camera cam;
	Image rawImage;
	bool camera_initialized;

public:
        VideoCaptureFirefly(void)
        {
            camera_initialized = false;
        }
        ~VideoCaptureFirefly()
        {
            // Stop capturing images
			error = cam.StopCapture();

            if (error != PGRERROR_OK)
            {
                error.PrintErrorTrace();
                return;
            }

            // Disconnect the camera
            error = cam.Disconnect();
            if (error != PGRERROR_OK)
           {
               error.PrintErrorTrace();
               return;
            }
        }

        bool open(int camera_number)
        {
            //camera_initialized = false;
			error = busMgr.GetNumOfCameras(&numCameras);
            if (error != PGRERROR_OK)
            {
                error.PrintErrorTrace();
                return false;
            }
            cout << "Number of firefly cameras detected: " <<  numCameras << endl;

            if (numCameras < 1)
            {
                cout << "ERROR: No firefly cameras detected" <<  endl;
                return false;
            }

            error = busMgr.GetCameraFromIndex(camera_number, &guid);

            if (error != PGRERROR_OK)
            {
                error.PrintErrorTrace();
                return false;
            }

            // Connect to a firefly camera
            error = cam.Connect(&guid);
            if (error != PGRERROR_OK)
            {
                error.PrintErrorTrace();
                return false;
            }

            // Start capturing images
            error = cam.StartCapture();
            if (error != PGRERROR_OK)
            {
                error.PrintErrorTrace();
                return false;
            }
            camera_initialized = true;
            return camera_initialized;
        }

        
        bool is_firefly_camera_present()
        {
            return camera_initialized;
        }

        int read(Mat & frame)
        {
            // Retrieve an image
			error = cam.RetrieveBuffer( &rawImage );
            if (error != PGRERROR_OK)
            {
                error.PrintErrorTrace();
                return -1;
            }

	       // create a opencv image
	      frame =  Mat( rawImage.GetRows(), rawImage.GetCols(), CV_8UC1, rawImage.GetData());
	    return 0;
	}
};

#endif // VIDEOCAPTUREFIREFLY_H