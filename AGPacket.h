#pragma once
#include <string>
#include <iostream>

using namespace std;
struct AGPacket
{
public:
	AGPacket(void){}
	~AGPacket(void){}
	void set(int _faceID, bool _gender, int _ageCatg){
		this->faceID=_faceID;
		this->gender=_gender;
		this->ageCatg=_ageCatg;
	}
	
	int faceID;
	bool gender;
	int ageCatg;
	string currentTime;
};

