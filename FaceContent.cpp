#include "FaceContent.h"


FaceContent::FaceContent(Rect rect, int id, bool detectedORnot)
{
	this->currentrect=rect;
	this->detectedORnot=detectedORnot;
	this->faceID=id;
}


FaceContent::~FaceContent(void)
{
}
