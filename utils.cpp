#include "utils.h"


// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
std::string getCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
	tstruct = *localtime(&now);
	int time_id=(tstruct.tm_hour*360+tstruct.tm_min*6)+tstruct.tm_sec/10;
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
	sprintf(buf,"%04d-%02d-%02d-%d",tstruct.tm_year+1900, tstruct.tm_mon+1, tstruct.tm_mday, time_id);
	

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
