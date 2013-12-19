#include "objecttracker.h"

//Construct the facetracker with no persistence unit
ObjectTracker::ObjectTracker()  : db(NULL)
{
    foundFacesTotal = 0;
}

//Construct the facetracker with a persistence unit
ObjectTracker::ObjectTracker(DBInterface* database)  : db(database)
{
    foundFacesTotal = 0;
}


//This method will start tracking the face define by the rect.
//The male argument is true if the face is thought to be male,
//false if female.
void ObjectTracker::startTracking(DetectionEvent faceToTrack)
{
    //Add the face to the list of tracked faces
    TrackedFace* newFace = new TrackedFace(faceToTrack);
    trackedFaces.push_back(newFace);
    foundFacesTotal++;
    return;
}


//This function should be called once per frame to update the tracked points and
//update the internal data strucutres with the frame data
void ObjectTracker::tick(Mat& frame)
{
    //Convert to grayscale
    cv::cvtColor(frame, gray, CV_BGR2GRAY);



    //Nothing to do here
    if (trackedFaces.size() == 0)
        return;




    //For the first frame, both previous and current frame are the same.
    if (gray_prev.empty())
        gray.copyTo(gray_prev);


    //Update the tracked points of all currently tracked faces
    std::vector<TrackedFace*>::iterator it = trackedFaces.begin();
    while (it != trackedFaces.end()) {

        TrackingStatus currentState = (*it)->tick(gray, gray_prev);

        //LOST means that the face was tracked long enough to not be
        //a false positive, and it has left the camera
        if (currentState == LOST)
        {

            print_info("Lost face");
            if (this->db != NULL)
            {
                print_info("Saving face data");
                this->db->writeDetection((*it)->getDetectionInformation());
            }


            delete(*it);
            trackedFaces.erase(it);
        }
        //TRACKING means that the face is still being tracked
        else if (currentState == TRACKING)
        {
            ++it;
        }
        //FALSE_DETECT means that the face is probably not a face,
        //but noise or some other false detection.
        //Reverse the detection
        else if (currentState == FALSE_DETECT)
        {
            delete(*it);
            trackedFaces.erase(it);
            foundFacesTotal--;
        }

    }
#if VIVA_DEBUG
    for (int i = 0; i < trackedFaces.size(); i++)
    {
        std::vector<cv::Point2f> points = trackedFaces[i]->getTrackedPoints();
        for (int j = 0; j < points.size(); j++)
        {
            cv::circle(frame, points[j], 2, trackedFaces[i]->getCurrentRect().getColor());
        }

    }
#endif



    cv::swap(gray_prev, gray);
    return;
}






//The goal of this function is to minimize the space between each rect
//The algorithm we use here is greedy.
//We start by finding the two closest points.
//We add them to the bestIndexForEach rect vector.
//Then we iterate.
std::vector<int> ObjectTracker::matchRects(std::vector<DetectionEvent> rects)
{
    std::vector<int> bestIndexForEachRect;

    if (rects.size() == 0)
        return bestIndexForEachRect;

    //Initialize everything with no match
    for (int i = 0; i < rects.size(); i++)
        bestIndexForEachRect.push_back(-1);

    //This can be made more efficient
    //but only if needed. Benchmarks + profiling should be done beforehand.
    for (int x = 0; x < rects.size(); x++)
    {
        int bestI = -1;
        int bestJ = -1;
        double bestDistance = 999999;

        for (int i = 0; i < rects.size(); i++)
        {

            cv::Rect& currentRect = rects[i].getRect();

            for (int j = 0; j < trackedFaces.size(); j++)
            {
                TrackedFace& currentFace = *trackedFaces[j];


                double dist = currentFace.getDistanceFromRect(currentRect);
                if (dist < bestDistance)
                {
                    if (bestIndexForEachRect[i] != -1)
                    {
                        continue;
                    }

                    for (int y = 0; y < bestIndexForEachRect.size(); y++)
                    {
                        if (bestIndexForEachRect[y] == j)
                        {
                            goto break_out;
                        }
                    }


                    bestDistance = dist;
                    bestI = i;
                    bestJ = j;
                }
                break_out: continue;
            }
        }

        if (bestDistance > MAX_DIST_BETWEEN_DETECTS)
        {
            break;
        }

        bestIndexForEachRect[bestI] = bestJ;
    }


    return bestIndexForEachRect;
}


void ObjectTracker::updateTracking(std::vector<DetectionEvent> rects)
{
    std::vector<DetectionEvent> uniqueRects;

    //Cycle through the faces found by the face detector,
    //finding the best matches and discarding some false
    //detections
    for (int i = 0; i < rects.size(); i++)
    {
        CvRect faceToTrack = rects[i].getRect();

        //Trying to detect a false detect, where two faces are pratically overlapped
        //If it's the case, skip this face, the other one will be the matched one.
        int delta = MULTIPLE_DETECT_DELTA;
        for (int j = 0; j < i; j++)
        {
            CvRect otherFace = rects[j].getRect();
            delta = getDistanceBetweenRectsRect(faceToTrack, otherFace);

            if (delta < MULTIPLE_DETECT_DELTA)
            {
                print_info("Avoided duplicate detection");
                goto break_out;
            }
        }

        uniqueRects.push_back(rects[i]);
        break_out: continue;
    }


    vector<int> bestIndexForEachRect = matchRects(uniqueRects);
    for (int i = 0; i < bestIndexForEachRect.size(); i++)
    {
        //Find the index of the closest tracked face to the detected face
        //int index = findExistingTrackedFace(faceToTrack);
        int index = bestIndexForEachRect[i];

        //No matches, start tracking the face
        if (index == -1)
        {
            startTracking(uniqueRects[i]);
        }
        //Found the face, mark it so it isn't matched again
        //Also update the information of the tracked face.
        else
        {
            this->trackedFaces[index]->update(uniqueRects[i]);
        }
    }

}


