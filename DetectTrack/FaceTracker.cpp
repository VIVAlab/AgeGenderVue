#include "FaceTracker.h"


FaceTracker::FaceTracker(FaceVue* faceDobj)
{

	facedetectorObj=faceDobj;
	foundFacesTotal=0;
	id_counter=0;
}


FaceTracker::~FaceTracker(void)
{
}

void FaceTracker::InitializeTracker(){}

void FaceTracker::removingOverlappedRects(vector<FaceContent> &rects){
	std::vector<FaceContent> uniqueRects;

    //Cycle through the faces found by the face detector,
    //finding the best matches and discarding some false
    //detections
    for (int i = 0; i < rects.size(); i++)
    {
		Rect faceToTrack = rects[i].currentrect;

        //Trying to detect a false detect, where two faces are pratically overlapped
        //If it's the case, skip this face, the other one will be the matched one.
        int delta = MULTIPLE_DETECT_DELTA;
        for (int j = 0; j < i; j++)
        {
			Rect otherFace = rects[j].currentrect;
            delta = getDistanceBetweenRectsRect(faceToTrack, otherFace);

            if (delta < MULTIPLE_DETECT_DELTA)
            {
                //cout<<("Avoided duplicate detection")<<endl;
                goto break_out;
            }
        }

        uniqueRects.push_back(rects[i]);
        break_out: continue;
    }
	rects=uniqueRects;
}

vector<FaceContent> FaceTracker::Track(Mat& frame, Mat& greyImage, vector<FaceContent> rects){
	
	
    //Nothing to do here
	if(rects.size()==0 && trackedFaces.size()==0){
		vector<FaceContent> z;
		return z;
	}
	removingOverlappedRects(rects);
	if (gray_prev.empty())
        greyImage.copyTo(gray_prev);
	
	for (int i = 0; i < trackedFaces.size(); i++)
		trackedFaces[i]->setdetectioninf(false);

	vector<int> bestIndexForEachRect = matchRects(rects);
	for (int i = 0; i < bestIndexForEachRect.size(); i++)
    {
        //Find the index of the closest tracked face to the detected face
        //int index = findExistingTrackedFace(faceToTrack);
        int index = bestIndexForEachRect[i];

        //No matches, start tracking the face
        if (index == -1)
        {
            startTracking(rects[i]);
        }
        //Found the face, mark it so it isn't matched again
        //Also update the information of the tracked face.
        else
        {
			this->trackedFaces[index]->updatePosition(rects[i]);
        }
    }
	for (int i = 0; i < trackedFaces.size(); i++)
    {
		TrackingStatus currentState =this->trackedFaces[i]->Track(greyImage,gray_prev);
		    //LOST means that the face was tracked long enough to not be
        //a false positive, and it has left the camera
        if (currentState == LOST)
        {

            //cout<<("Lost face")<<endl;
			if(trackedFaces.size()>0)
				trackedFaces.erase(trackedFaces.begin()+i);
        }
        //TRACKING means that the face is still being tracked
        else if (currentState == TRACKING)
        {
            continue;
        }
        //FALSE_DETECT means that the face is probably not a face,
        //but noise or some other false detection.
        //Reverse the detection
        else if (currentState == FALSE_DETECT)
        {
			id_counter--;
			//cout<<("FALSE_DETECT")<<endl;
			if(trackedFaces.size()>0)
				trackedFaces.erase(trackedFaces.begin()+i);
            foundFacesTotal--;
			
        }

	}
	greyImage.copyTo(gray_prev);
	return GetFaces();
}

void FaceTracker::startTracking(FaceContent faceToTrack)
{
    //Add the face to the list of tracked faces
	TrackedObject* newFace = new TrackedObject(faceToTrack,facedetectorObj, id_counter++);
	if(id_counter==10000)
		id_counter=0;
	trackedFaces.push_back(newFace);
    foundFacesTotal++;
    return;
}

std::vector<int> FaceTracker::matchRects(vector<FaceContent> rects)
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

			Rect& currentRect = rects[i].currentrect;

            for (int j = 0; j < trackedFaces.size(); j++)
            {
                TrackedObject& currentFace = *trackedFaces[j];


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

vector<FaceContent> FaceTracker::GetFaces(){
	vector<FaceContent> faceinfos;

	for (int i = 0; i < trackedFaces.size(); i++){
		FaceContent x(trackedFaces[i]->getFace().currentrect,trackedFaces[i]->getId(),trackedFaces[i]->getdetectioninf());
		x.setFlandmarks(trackedFaces[i]->getFace().right_eye_x,trackedFaces[i]->getFace().right_eye_y,
			trackedFaces[i]->getFace().left_eye_x, trackedFaces[i]->getFace().left_eye_y);
		//rects.push_back(trackedFaces[i]->getRect());
		faceinfos.push_back(x);
	}

    return faceinfos;
}

