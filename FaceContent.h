#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;

class FaceContent
{
public:
	FaceContent(void){}
	FaceContent(Rect rect){
		this->currentrect=rect;
	}
	FaceContent(Rect rect, int id, bool detectedORnot);
	~FaceContent(void);


	int faceID;
	Rect currentrect;
	bool detectedORnot;

	int right_eye_x, right_eye_y;
	int left_eye_x, left_eye_y;
	int center_mouth_x, center_mouth_y;


	void setFlandmarks(int _right_eye_x, int _right_eye_y, int _left_eye_x, int _left_eye_y){
		this->right_eye_x=_right_eye_x;
		this->right_eye_y=_right_eye_y;
		this->left_eye_x=_left_eye_x;
		this->left_eye_y=_left_eye_y;
	}
};

