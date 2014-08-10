#include "utils.h"


// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
std::string getCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
	tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}



void putBoldedText(cv::Mat frame, std::string text, cv::Point pos, double size)
{

    cv::Point pos1 = cvPoint(pos.x, pos.y + 1);
    cv::Point pos2 = cvPoint(pos.x + 1, pos.y + 1);
    cv::Point pos3 = cvPoint(pos.x + 1, pos.y);

    cv::putText(frame,text.c_str(),pos,CV_FONT_HERSHEY_PLAIN, size, CV_RGB(255,255,255));
    cv::putText(frame,text.c_str(),pos1,CV_FONT_HERSHEY_PLAIN, size, CV_RGB(255,255,255));
    cv::putText(frame,text.c_str(),pos2,CV_FONT_HERSHEY_PLAIN, size, CV_RGB(255,255,255));
    cv::putText(frame,text.c_str(),pos3,CV_FONT_HERSHEY_PLAIN, size, CV_RGB(255,255,255));


}
