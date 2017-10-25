#ifndef SW_H
#define SW_H

#include"SWBase.h"
class Data;

class SW : public SWBase{
public:
	SW(const Data& db, const Data& query, int threshold);
	~SW();
private:
	SW() {};
	// Culculate the score by Dynamic Programing 
	void DP(const Data& db, const Data& query);
};


#endif