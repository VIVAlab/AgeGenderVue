#include "TrackedObject.h"

int TrackedObject::max_count=500;
float TrackedObject::qlevel=0.01;
float TrackedObject::min_dist=3.0;


TrackedObject::TrackedObject(FaceContent& faceRect, FaceVue* faceDobj, int id){
    lastDetections.insert(lastDetections.begin(), faceRect);
    framesNotSpotted = 0;
    spotCount=0;
    notMoved=0;
    
    //averageDelta = 0;
    totalTicks = 0;
    notMoved = 0;
    spotCount = 0;
    
    facedetectorObj=faceDobj;
    stopTrackingTreshold_limit=10;
    stopTrackingTreshold = MIN_FRAME_TO_STOP;
    
    start_time = getCurrentDateTime();
    stop_time = getCurrentDateTime();
    
    updatePointFlag = true;
    
    currentLocation = faceRect;
    updatePosition(faceRect);
    //time_app="0";
	this->id=id;
	setdetectioninf(true);
    
}


TrackedObject::~TrackedObject(void)
{
}

TrackingStatus TrackedObject::Track(Mat& currentFrame, Mat& previousFrame){
    framesNotSpotted++;
    totalTicks++;
    
    
    //Should remove, face detection accurate enough now
    if (notMoved > IMMOBILE_FACE_FOR_FALSE)
    {
        //std::cout<<("Immobile false detect")<<std::endl;
        return FALSE_DETECT;
    }
    
    //Check if we have lost the face.
    //Can add checks here for better results.
    if (framesNotSpotted > stopTrackingTreshold)
    {
        //std::cout<<("Face not spotted for ") << (framesNotSpotted)<<"_"<<stopTrackingTreshold<<std::endl;
        
        
        if(isThereanyFace(currentFrame)){
            stopTrackingTreshold_limit=10;
            framesNotSpotted=0;
        }else{
            
            stopTrackingTreshold_limit--;
            if(stopTrackingTreshold_limit<0){
                if (spotCount < MINIMUM_SPOT_COUNT )
                {             //std::cout<<("Spot count false detect")<<std::endl;
					
                    return FALSE_DETECT;
                }
                return LOST;
			}
        }
        
    }
    
    
    if (addNewPoints(currentFrame))
    {
        detectFeaturePoints(currentFrame);
        
        //Add the new feature points at the end of the maintained list
        points[0].insert(points[0].end(), features.begin(), features.end());
        //std::cout<<"Added " << features.size() << " points"<<std::endl;
    }
    
    //Calculate where the points have moved, or if they have been lost
    cv::calcOpticalFlowPyrLK(
            previousFrame, currentFrame,
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
        if ((points[0].size() - points[1].size())/(double)points[0].size() > 0.5)
        {
            //std::cout<<"Lost " << (points[0].size() - points[1].size())/(double)points[0].size() * 100 << " percent of features."<<std::endl;
            if (spotCount < MINIMUM_SPOT_COUNT)
            {
                //std::cout<<"Spot count false detect"<<std::endl;
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
        averageDeltaX += currentLocation.currentrect.x + (currentLocation.currentrect.width/2) - points[1][i].x;
        averageDeltaY += currentLocation.currentrect.y + (currentLocation.currentrect.height/2) - points[1][i].y;
        
        averageX += points[1][i].x;
        averageY += points[1][i].y;
        
        
    }
    
    
    averageDeltaX /= points[1].size();
    averageDeltaY /= points[1].size();
    
    averageX /= points[1].size();
    averageY /= points[1].size();
    
    
    //here is another option, u can use their min and max! instead of using average its better to use min and max!
    currentLocation.currentrect.x = averageX - (currentLocation.currentrect.width/2);
    currentLocation.currentrect.y = averageY - (currentLocation.currentrect.height/2);
    //std::cout<<"min:"<<minX<<"-"<<minY<<std::endl;
    //int pwoctave=currentLocation.width/2;
    //int phoctave=currentLocation.height/2;
    //currentLocation.x = minX - (pwoctave/2);
    //currentLocation.y = minY - (phoctave/2);
    //(minX - (pwoctave)>0)?(currentLocation.x = minX - (pwoctave)):(currentLocation.x = 0);
    //(minY - (phoctave)>0)?(currentLocation.y = minY - (phoctave)):(currentLocation.y = 0);
    //(maxX + (pwoctave)<currentFrame.cols)?(currentLocation.width = (maxX + (pwoctave)-currentLocation.x)):(currentLocation.width = (currentFrame.cols-currentLocation.x));
    //(maxY + (phoctave)<currentFrame.rows)?(currentLocation.height = (maxY + (phoctave)-currentLocation.y)):(currentLocation.height = (currentFrame.rows-currentLocation.y));
    
    
    //Swap the data for the next tick
    std::swap(points[1], points[0]);
    return TRACKING;
}


//Check that this is a good tracked point.
bool TrackedObject::acceptTrackedPoint(unsigned char status)
{
    return status;// && (abs(points[0][i].x - points[1][i].x) + (abs(points[0][i].y - points[1][i].y)) > MIN_MOVEMENT_FOR_TRACKED_POINT);
}

void TrackedObject::detectFeaturePoints(cv::Mat &gray)
{
    
    features.clear();
    
    if (currentLocation.currentrect.x < 0)
        currentLocation.currentrect.x = 0;
    
    if (currentLocation.currentrect.x + currentLocation.currentrect.width > gray.cols)
        currentLocation.currentrect.width = gray.cols - currentLocation.currentrect.x;
    
    if (currentLocation.currentrect.y < 0)
        currentLocation.currentrect.y = 0;
    
    if (currentLocation.currentrect.y + currentLocation.currentrect.height > gray.rows)
        currentLocation.currentrect.height = gray.rows - currentLocation.currentrect.y;
    
    //Only get points in the center of the face
    double octaveWidth = currentLocation.currentrect.width/2;
    double octaveHeight = currentLocation.currentrect.height/2;
    
    Rect area = currentLocation.currentrect;
    
    area.width = octaveWidth;
    area.height = octaveHeight;
    
    area.x += octaveWidth/2;
    area.y += octaveHeight/2;
    
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


void TrackedObject::initialize(){}

bool TrackedObject::addNewPoints(cv::Mat& gray) {
    
    
    bool result = updatePointFlag && (points[0].size() <= ADD_NEW_POINTS_THRESHOLD);// && ((points[0].size() < max_count) || updatePointFlag);
    
    //It will be not necessary to check that the new feature points are out of the border or not
    if (currentLocation.currentrect.x - currentLocation.currentrect.width/BORDER_SIZE_RATIO < 0
        || currentLocation.currentrect.x + currentLocation.currentrect.width * (BORDER_SIZE_RATIO*2 - 1)/BORDER_SIZE_RATIO > gray.cols
        || currentLocation.currentrect.y - currentLocation.currentrect.height/BORDER_SIZE_RATIO < 0
        || currentLocation.currentrect.y + currentLocation.currentrect.height * (BORDER_SIZE_RATIO*2 - 1)/BORDER_SIZE_RATIO  > gray.rows)
        updatePointFlag = false;
    else
        updatePointFlag = true;
    
    
    return result;
}



void TrackedObject::updatePosition(FaceContent newPosition){
    
    lastDetections.insert(lastDetections.begin(), newPosition);
    
    
    stop_time = getCurrentDateTime();
    stopTrackingTreshold += TRACK_FRAME_DURATION_INCREMENT;
    if (stopTrackingTreshold > MAX_FRAME_TO_STOP)
        stopTrackingTreshold = MAX_FRAME_TO_STOP;
    
    framesNotSpotted = 0;
    spotCount++;
    
	if (currentLocation.currentrect.x == newPosition.currentrect.x && currentLocation.currentrect.y == newPosition.currentrect.y)
    {
        notMoved++;
    }
    else
    {
        notMoved = 0;
    }
    currentLocation=newPosition;
    
    updatePointFlag = true;
	setdetectioninf(true);
    
    //Remove all of the points not in the detected area,
    //since we know they do not belong to the face
    int k = 0;
    for (int i = 0; i < points[0].size(); i++)
    {
        
        double quarterWidth = newPosition.currentrect.width/4;//Changed from 1/4 to 1/8
        double quarterHeight = newPosition.currentrect.height/4;
        
        if (!(points[0][i].x < newPosition.currentrect.x + quarterWidth || points[0][i].x > (newPosition.currentrect.x + newPosition.currentrect.width - quarterWidth) ||
              points[0][i].y < newPosition.currentrect.y + quarterHeight || points[0][i].y > (newPosition.currentrect.y + newPosition.currentrect.height - quarterHeight)))
        {
            points[0][k] = points[0][i];
            k++;
        }
    }
    
    points[0].resize(k);
    
}

FaceContent TrackedObject::getFace(){
    return this->currentLocation;
}
int TrackedObject::getId(){
	return this->id;
}
bool TrackedObject::getdetectioninf(){
	return this->detectedORnot;
}
void TrackedObject::setdetectioninf(bool dtctornot){
	this->detectedORnot=dtctornot;
}


double TrackedObject::getDistanceFromRect(Rect other)
{
    int thisMiddleX = this->currentLocation.currentrect.x + ((this->currentLocation.currentrect.width - this->currentLocation.currentrect.x) /2);
    int thisMiddleY = this->currentLocation.currentrect.y + ((this->currentLocation.currentrect.height - this->currentLocation.currentrect.y) /2);
    
    int otherMiddleX = other.x + ((other.width - other.x) /2);
    int otherMiddleY = other.y + ((other.height - other.y) /2);
    
    int deltaX = thisMiddleX - otherMiddleX;
    int deltaY = thisMiddleY - otherMiddleY;
    
    return sqrt((deltaX * deltaX) + (deltaY * deltaY));
}