#ifndef SW_H
#define SW_H

class Data;

class SW {
public:
	SW(const Data& db, const Data& query);
	~SW();
private:
	int* mScore;
	// Culculate the score by Dynamic Programing 
	void DP(const Data& db, const Data& query);
};


#endif