#include "trackedface.h"


//Initializes the position of the face as well as the
//currently tracked points in the face rect
TrackedFace::TrackedFace(DetectionEvent& event)
{
    averageDelta = 0;
    totalTicks = 0;
    notMoved = 0;
    spotCount = 0;

    for (int i = 0; i < AGE_BOX_COUNT; i++)
        ageCounts[i] = 0;

    maleVotes = 0;
    femaleVotes = 0;

    stopTrackingTreshold = MIN_FRAME_TO_STOP;

    start_time = getCurrentDateTime();
    stop_time = getCurrentDateTime();

    updatePointFlag = true;

    currentLocation = ColoredRect(event.getRect());
    update(event);


}


//Recenter the face with a recognized face
void TrackedFace::update(DetectionEvent& event)
{

    lastDetections.insert(lastDetections.begin(), event);



    cv::Rect currentPos = event.getRect();

    stop_time = getCurrentDateTime();

    stopTrackingTreshold += TRACK_FRAME_DURATION_INCREMENT;
    if (stopTrackingTreshold > MAX_FRAME_TO_STOP)
        stopTrackingTreshold = MAX_FRAME_TO_STOP;

    framesNotSpotted = 0;
    spotCount++;

    if (currentLocation.x == currentPos.x && currentLocation.y == currentPos.y)
    {
        notMoved++;
    }
    else
    {
        notMoved = 0;
    }

    currentLocation.updatePos(currentPos);

    if (event.getGender() > 0)
        maleVotes++;
    else if (event.getGender() < 0)
        femaleVotes++;
    else
    {
        maleVotes++;
        femaleVotes++;
    }


    ageCounts[event.getAge()]++;


    //recentEvents.push(event);
    //if (recentEvents.size() >= EVENT_QUEUE_SIZE)
    //    recentEvents.pop();


    updatePointFlag = true;


    //Remove all of the points not in the detected area,
    //since we know they do not belong to the face
    int k = 0;
    for (int i = 0; i < points[0].size(); i++)
    {

        double quarterWidth = currentPos.width/4;
        double quarterHeight = currentPos.height/4;

        if (!(points[0][i].x < currentPos.x + quarterWidth || points[0][i].x > currentPos.x + (quarterWidth*3) ||
              points[0][i].y < currentPos.y + quarterHeight || points[0][i].y > currentPos.y + (quarterHeight *3)))
        {
            points[0][k] = points[0][i];
            k++;
        }
    }

    points[0].resize(k);


}

//Finds new points to track
void TrackedFace::detectFeaturePoints(cv::Mat &gray)
{

    features.clear();

    cv::Rect area = currentLocation;

    if (area.x < 0)
        area.x = 0;

    if (area.x + area.width > gray.cols)
        area.x = gray.rows - area.width;

    if (area.y < 0)
        area.y = 0;

    if (area.y + area.height > gray.rows)
        area.y = gray.rows - area.height;

    //Only get points in the center of the face
    double halfWidth = area.width/2;
    double halfHeight = area.height/2;

    area.width = halfWidth;
    area.height = halfHeight;

    area.x += halfWidth/2;
    area.y += halfHeight/2;

    cv::Mat areaToSearch(gray, area);


    cv::goodFeaturesToTrack(areaToSearch,
                            features,
                            max_count,
                            qlevel,
                            min_dist);


    for (int i = 0; i < features.size(); i++)
    {
        features[i].x += area.x;
        features[i].y += area.y;
    }

}

TrackingStatus TrackedFace::tick(cv::Mat& gray, cv::Mat& gray_prev)
{
    framesNotSpotted++;
    totalTicks++;


    //Should remove, face detection accurate enough now
    if (notMoved > IMMOBILE_FACE_FOR_FALSE)
    {
        print_info("Immobile false detect");
        return FALSE_DETECT;
    }

    //Check if we have lost the face.
    //Can add checks here for better results.
     if (framesNotSpotted > stopTrackingTreshold)
     {

        print_info("Face not spotted for " << framesNotSpotted);

         if (spotCount < MINIMUM_SPOT_COUNT)
         {
             print_info("Spot count false detect");
             return FALSE_DETECT;
         }


         return LOST;
     }


     if (addNewPoints(gray))
     {
         detectFeaturePoints(gray);

         //Add the new feature points at the end of the maintained list
         points[0].insert(points[0].end(), features.begin(), features.end());
         print_info("Added " << features.size() << " points");
     }

     //Calculate where the points have moved, or if they have been lost
     cv::calcOpticalFlowPyrLK(
                 gray_prev, gray,
                 points[0],
                 points[1],
                 status,
                 err);


    {
         //Update the arrays used to track points.
         int k=0;
         for (int i = 0; i < points[1].size(); i++)
         {
             if (acceptTrackedPoint(status[i]))
             {
                 points[1][k] = points[1][i];
                 k++;
             }

         }

         //Resize the arrays appropriately
         points[1].resize(k);
    }

    {
          if ((points[0].size() - points[1].size())/(double)points[0].size() > 0.4)
          {
              print_info("Lost " << (points[0].size() - points[1].size())/(double)points[0].size() * 100 << " percent of features.");
              if (spotCount < MINIMUM_SPOT_COUNT)
              {
                  print_info("Spot count false detect");
                  return FALSE_DETECT;
              }
              return LOST;
          }
    }









     //Here we calculate the average movement of the face so far, then discard it
     //as a false positive if the numbers are too big for human movement
     double averageDeltaX = 0;
     double averageDeltaY = 0;

     double averageX = 0;
     double averageY = 0;



     for (int i = 0; i < points[1].size(); i++)
     {
         averageDeltaX += currentLocation.x + (currentLocation.width/2) - points[1][i].x;
         averageDeltaY += currentLocation.y + (currentLocation.height/2) - points[1][i].y;

         averageX += points[1][i].x;
         averageY += points[1][i].y;
     }


     averageDeltaX /= points[1].size();
     averageDeltaY /= points[1].size();

     averageX /= points[1].size();
     averageY /= points[1].size();



    currentLocation.x = averageX - (currentLocation.width/2);
    currentLocation.y = averageY - (currentLocation.height/2);


     //Swap the data for the next tick
     std::swap(points[1], points[0]);
     return TRACKING;
}

int TrackedFace::getPointsInRectCount()
{
    return points[0].size();
}
