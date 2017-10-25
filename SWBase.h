#ifndef SWBASE_H
#define SWBASE_H

class SWBase {
public:
	virtual ~SWBase() {};
	int max_score();
	int max_position();
	int* all_score();
protected:
	SWBase(int threshold);
	SWBase() {};
	int* mScore;
	int mSize;
	int mThreshold;
private:
	void search_max();
	int mMaxPos;
	int mMaxScore;
};

#endif
