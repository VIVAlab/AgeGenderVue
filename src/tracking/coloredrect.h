#ifndef COLOREDRECT_H
#define COLOREDRECT_H

#include <iostream>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>



//This class is simply a rect that has a color,
//usefull when you want to have a persistently
//colored rectangle (normally for debugging)
class ColoredRect : public cv::Rect
{

public:

    ColoredRect() : cv::Rect()
    {
    }

    ColoredRect(cv::Rect rect) : cv::Rect(rect.x, rect.y, rect.width, rect.height)
    {
        randomizeColor();
    }

    ColoredRect(const ColoredRect& rect) : cv::Rect(rect.x, rect.y, rect.width, rect.height)
    {
        this->color = rect.color;
    }


    void updatePos(cv::Rect rect)
    {
        this->x= rect.x;
        this->y = rect.y;
        this->width = rect.width;
        this->height = rect.height;
    }

    cv::Scalar getColor()
    {
        return color;
    }

    void setColor(cv::Scalar newColor)
    {
        this->color = newColor;
    }


private:

    void randomizeColor()
    {

        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;

        color = cv::Scalar(r, g ,b);
    }

    cv::Scalar color;

};

#endif // COLOREDRECT_H
