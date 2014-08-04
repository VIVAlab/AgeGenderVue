#include "AGRecognitionCore.h"


AGRecognitionCore::AGRecognitionCore(void)
{
    Recognizer=new LBPHISTOGRAM();

}


AGRecognitionCore::~AGRecognitionCore(void)
{
}

int AGRecognitionCore::GenderPredict(Mat* input){
	int x=Recognizer->gender_predictor(input);
	return x;
}
int AGRecognitionCore::AgePredict(Mat* input){
	int x=Recognizer->age_predictor(input);
    return x;
}

vector<AGPacket> AGRecognitionCore::Face_Process(Mat frame, vector<FaceContent> faces){
    vector<AGPacket> output;
	Mat imtemp;
	int age_tmp, gender_tmp;
	for (std::map<int, PersonOnTrack*>::iterator it=inFrameFaces.begin(); it!=inFrameFaces.end(); ++it)
		it->second->set_status(false);
      
    if(!(inFrameFaces.size()==0 && faces.size()==0))
    {
        
        for(vector<FaceContent>::iterator it = faces.begin(); it != faces.end(); it++) {
            /* std::cout << *it; ... */
            std::map<int, PersonOnTrack*>::iterator i = inFrameFaces.find(it->faceID);
			
				if (i != inFrameFaces.end()){
					if(it->detectedORnot){
						imtemp=Recognizer->rotate(&frame,it->right_eye_x,it->right_eye_y,it->left_eye_x,it->left_eye_y);
						age_tmp = AgePredict(&imtemp);
						gender_tmp = GenderPredict(&imtemp);
						i->second->updateAgeGender(gender_tmp,age_tmp);
					
					}
					i->second->set_status(true);
					AGPacket tmp; 
					tmp.set(i->second->index,i->second->getGender(),i->second->getAge());
					output.push_back(tmp);
				}else{
					imtemp=Recognizer->rotate(&frame,it->right_eye_x,it->right_eye_y,it->left_eye_x,it->left_eye_y);
					age_tmp = AgePredict(&imtemp);
					gender_tmp = GenderPredict(&imtemp);
					
					PersonOnTrack* temp= new PersonOnTrack(it->faceID,age_tmp,gender_tmp);
					
					//memcpy(inFrameFaces[it->faceID],temp,sizeof(PersonOnTrack));
					temp->set_status(true);
					inFrameFaces[it->faceID]=temp;

					AGPacket tmp; 
					tmp.set(it->faceID,temp->getGender(),temp->getAge());

					output.push_back(tmp);
				}
        }
    }
    /*
	std::map<int, PersonOnTrack>::iterator itr = inFrameFaces.begin();
    while (itr != inFrameFaces.end()) {
        if(!itr->second.get_status()) {
            std::map<int, PersonOnTrack>::iterator toErase = itr;
            ++itr;
            inFrameFaces.erase(toErase);
        } else {
            ++itr;
        }
    }*/
	vector<int> toerase;
	auto itr= inFrameFaces.begin();
	while(itr!=inFrameFaces.end()){
		if(!itr->second->get_status()) {
			toerase.push_back(itr->first);
            //itr=inFrameFaces.erase(itr);
        }
		itr++;
	}
	for(int i=0;i<toerase.size();i++){
		delete inFrameFaces[toerase[i]];
		inFrameFaces.erase(toerase[i]);
	}

	return output;
    
}