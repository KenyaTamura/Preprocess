#ifndef SIMPLESW_H
#define SIMPLESW_H

#include"SWBase.h"
class Data;

class SimpleSW : public SWBase{
public:
	SimpleSW(const Data& db, const Data& query, int threshold);
	~SimpleSW();
private:
	SimpleSW() {};
	// Culculate the score by Dynamic Programing 
	void DP(const Data& db, const Data& query);
};


#endif