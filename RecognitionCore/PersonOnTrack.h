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
	PersonOnTrack(int iindex, int iage, int igender)//:age_fr(vector<int>()),gender_fr(vector<int>())
	{
		age_fr=vector<int>();
		gender_fr=vector<int>();

		age=new int[3];
		gender=new int[2];

		for(int i=0;i<3;i++)
			age[i]=0;

		age[iage]=1;
		gender[igender]=1;
		gender[(1-igender)]=0;

		index=iindex;
		status=true;
	
		age_fr.push_back(iage+0);
		gender_fr.push_back(igender+0);
		
	}
	
	~PersonOnTrack(){
		if(age)
			delete[] age;
		if(gender)
			delete[] gender;
	}

	int* age;
	int* gender;
	vector<int> age_fr;
	vector<int> gender_fr;
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
		
		if(gender_fr.size()>=VOTING_CAPACITY){
			gender[gender_fr[0]]--;
			gender_fr.erase(gender_fr.begin());
		}

		if(age_fr.size()>=VOTING_CAPACITY){
		
			age[age_fr[0]]--;
			age_fr.erase(age_fr.begin());
		}
		age[_age]++;
		age_fr.push_back(_age);
		gender[_gender]++;
		gender_fr.push_back(_gender);
		age_sum++;
		gender_sum++;
	}

};

