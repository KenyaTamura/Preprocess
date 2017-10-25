#ifndef PREPROCESSSW_H
#define PREPROCESSSW_H

class PreprocessBase;
class Data;

class PreprocessSW {
public:
	PreprocessSW(const Data& db, const Data& query, const PreprocessBase& pre, int threshold);
	~PreprocessSW();
	int max_score();
	int max_position();
	int* all_score();
private:
	PreprocessSW() {};
	void DP(const Data& db, const Data& query, int start, int end);
	int* mScore;
	int mSize;
	int mThreshold;
	int mMaxPos;
	int mMaxScore;
	void search_max();
};



#endif
