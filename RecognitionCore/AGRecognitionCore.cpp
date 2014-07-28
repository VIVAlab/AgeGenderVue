#include "AGRecognitionCore.h"


AGRecognitionCore::AGRecognitionCore(void)
{
    Recognizer=new LBPHISTOGRAM();

}


AGRecognitionCore::~AGRecognitionCore(void)
{
}

int AGRecognitionCore::GenderPredict(Mat* input){
	return Recognizer->gender_predictor(input);
}
int AGRecognitionCore::AgePredict(Mat* input){
    return Recognizer->age_predictor(input);
}

vector<AGPacket> AGRecognitionCore::Face_Process(Mat frame, vector<FaceContent> faces){
    vector<AGPacket> output;
	Mat imtemp;
	int age_tmp, gender_tmp;
	
    if(!(inFrameFaces.size()==0 && faces.size()==0))
    {
        for (std::map<int, PersonOnTrack>::iterator it=inFrameFaces.begin(); it!=inFrameFaces.end(); ++it)
            it->second.set_status(false);
        
        for(vector<FaceContent>::iterator it = faces.begin(); it != faces.end(); it++) {
            /* std::cout << *it; ... */
            std::map<int, PersonOnTrack>::iterator i = inFrameFaces.find(it->faceID);
			
				if (i != inFrameFaces.end()){
					if(it->detectedORnot){
						cout<<"B1"<<endl;
						imtemp=Recognizer->rotate(&frame,it->right_eye_x,it->right_eye_y,it->left_eye_x,it->left_eye_y);
						age_tmp = AgePredict(&imtemp);
						gender_tmp = GenderPredict(&imtemp);
						cout<<"B2"<<endl;
						i->second.updateAgeGender(gender_tmp,age_tmp);
					
					}
					cout<<"B3"<<endl;
					i->second.set_status(true);
					AGPacket tmp; 
					tmp.set(i->second.index,i->second.getGender(),i->second.getAge());
					cout<<"CB4"<<endl;
					output.push_back(tmp);
				}else{
					cout<<"C1"<<endl;
					imtemp=Recognizer->rotate(&frame,it->right_eye_x,it->right_eye_y,it->left_eye_x,it->left_eye_y);
					age_tmp = AgePredict(&imtemp);
					gender_tmp = GenderPredict(&imtemp);
					cout<<"C2"<<endl;
					cout<<it->faceID<<endl;
					//PersonOnTrack* temp=new PersonOnTrack(it->faceID,gender_tmp,age_tmp);
					PersonOnTrack temp(it->faceID,gender_tmp,age_tmp);
					cout<<"C3"<<endl;

					//memcpy(inFrameFaces[it->faceID],temp,sizeof(PersonOnTrack));

					inFrameFaces[it->faceID]=temp;

					cout<<"C4"<<endl;
					AGPacket tmp; 
					tmp.set(it->faceID,temp.getGender(),temp.getAge());

					
					

					cout<<"C5"<<endl;
					output.push_back(tmp);
				}
        }
    }
    cout<<"D1"<<endl;
    std::map<int, PersonOnTrack>::iterator itr = inFrameFaces.begin();
    while (itr != inFrameFaces.end()) {
        if(!itr->second.get_status()) {
            std::map<int, PersonOnTrack>::iterator toErase = itr;
            ++itr;
            inFrameFaces.erase(toErase);
        } else {
            ++itr;
        }
    }
	cout<<"D2"<<endl;
    return output;
    
}