#pragma oncee
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define	VOTING_CAPACITY	15

class PersonOnTrack
{
public:

	PersonOnTrack(){
		//age_front.push_back(1);
	}
	PersonOnTrack(int iindex, int iage, int igender)//:age_front(vector<int>(1,0))
	{

		for(int i=0;i<3;i++)
			age[i]=0;
		cout<<"G1"<<endl;
		age[iage]=1;
		gender[igender]=1;
		gender[(1-igender)]=0;
		cout<<"G2"<<endl;
		index=iindex;
		cout<<"G3"<<endl;
		status=true;
		age_sum=1;
		gender_sum=1;
		cout<<"G3.1"<<endl;
		age_front.clear();
		gender_front.clear();

		//vector<int> hh ;
		//hh.push_back(1);

		//cout<<hh.size()<<endl;
		cout<<age_front.size()<<endl;
        cout<<iage<<endl;
		age_front.push_back(iage);
		cout<<"G3.2"<<endl;
		//gender_front.push_back(igender);
		cout<<"G4"<<endl;
		int a  = 1;
		cout<<"G5"<<endl;
	}
	
	~PersonOnTrack(){}

	int age[3];
	int gender[2];
	vector<int> age_front;
	vector<int> gender_front;
	int age_sum;
	int gender_sum;
	int index;
	bool status;
	
	
	
	bool get_status(){
		return status;
	}
	void set_status(bool _status){
		status=_status;
	}


	int getAge(){
		int m = age[0];
		int id=0;
        (m < age[1]) && (m = age[1]) && (id = 1); //these are not conditional statements.
        (m < age[2]) && (id = 2); //these are just boolean expressions.
		return id;
	}

	bool getGender(){
		int m = gender[0];
		bool sex=false;
        (m < gender[1]) && (sex=true); //these are not conditional statements.
        return sex;
	}

	void updateAgeGender(int _gender, int _age){
		
		if(gender_front.size()>=VOTING_CAPACITY){
			cout<<"size "<<gender_front.size()<<endl;
			cout<<" addad "<<gender_front[0]<<endl;
			gender[gender_front[0]]--;
			gender_front.erase(gender_front.begin());
		}

		if(age_front.size()>=VOTING_CAPACITY){
		
			age[age_front[0]]--;
			age_front.erase(age_front.begin());
		}
		age[_age]++;
		age_front.push_back(_age);
		gender[_gender]++;
		gender_front.push_back(_gender);
		age_sum++;
		gender_sum++;
	}

};

