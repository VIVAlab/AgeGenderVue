#ifndef DETECTIONEVENT_H
#define DETECTIONEVENT_H

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//Class used to represent a face detection
class DetectionEvent
{
public:
    DetectionEvent(cv::Rect rect, double male, int age);

    cv::Rect& getRect()
    {
        return rect;
    }

    double& getGender()
    {
        return male;
    }

    int& getAge()
    {
        return age;
    }

    static std::vector<DetectionEvent> generate(std::vector<cv::Rect> rects,
                                                std::vector<int> ages,
                                                std::vector<double> genders)
    {
        std::vector<DetectionEvent> events;


        if (rects.size() != ages.size() || rects.size() != genders.size())
        {
            throw "Data is different lengths";
        }

        for (int i = 0; i < rects.size(); i++)
        {
            events.push_back(DetectionEvent(rects[i], genders[i], ages[i]));
        }

        return events;
    }


private:
    cv::Rect rect;
    double male;
    int age;

};

#endif // DETECTIONEVENT_H
