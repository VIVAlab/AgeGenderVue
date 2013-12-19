#include "detectionevent.h"

//Initialize the detection event
DetectionEvent::DetectionEvent(cv::Rect rect, double male, int age) : rect(rect), male(male), age(age)
{
}

