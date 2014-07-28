#pragma once

#include <map>
#include <cassert>
#include <iostream>
#include <vector>
#include "../FaceContent.h"
#include "../AGPacket.h"
#include "PersonOnTrack.h"
#include "histogram1d.h"

#define _DEFINE_DEPRECATED_HASH_CLASSES 0

using namespace std;

class AGRecognitionCore
{
public:
	AGRecognitionCore(void);
	~AGRecognitionCore(void);

	vector<AGPacket> Face_Process(Mat frame, vector<FaceContent> faces);
	
private:
	map<int, PersonOnTrack> inFrameFaces;
	LBPHISTOGRAM* Recognizer;

	int GenderPredict(Mat* input);
	int AgePredict(Mat* input);


};

