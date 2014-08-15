#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "DetectTrack\DetectTrack.h"
#include <vector>
#include "FaceContent.h"
#include "RecognitionCore\AGRecognitionCore.h"
#include <thread>
#include "PacketProcessor.h"


using namespace cv;
PacketProcessor a;
void produce(){
	while(a.flag){
		a.putItem();
	}
}
void consume(){
	while(a.flag)
		a.processItem();
}

void writer(){
	while(a.flag)
		a.Timer_DB();
}


int main(){
	thread aa(produce);
	thread bb(consume);
	thread cc(writer);
	aa.join();
	bb.join();
	cc.join();
	
	return 1;
}

