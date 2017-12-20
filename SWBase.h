#ifndef SWBASE_H
#define SWBASE_H

class Data;

class SWBase {
public:
	virtual ~SWBase() {};
	int max_score();
	int max_position();
	int* all_score();
protected:
	SWBase(int threshold);
	SWBase() {};
	void traceback(const Data& db, const Data& query) const;
	int* mScore;
	int mSize;
	int mThreshold;
private:
	void search_max();
	int mMaxPos;
	int mMaxScore;
};

#endif
