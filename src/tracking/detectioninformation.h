#ifndef DETECTIONINFORMATION_H
#define DETECTIONINFORMATION_H

#include <iostream>

//This is used to store all known information about
//a face that we tracked. This class is the class
//that we send to the persistence unit.
class DetectionInformation
{
public:

    DetectionInformation(std::string start, std::string end, bool gender, int age)
    {
        this->start_time = start;
        this->stop_time = end;
        this->gender = gender;
        this->age = age;
    }

    std::string getStartTime()
    {
        return start_time;
    }

    std::string getStopTime()
    {
        return stop_time;
    }

    bool getGender()
    {
        return gender;
    }

    int getAge()
    {
        return age;
    }


private:
    std::string start_time;
    std::string stop_time;

    bool gender;
    int age;

};

#endif // DETECTIONINFORMATION_H
