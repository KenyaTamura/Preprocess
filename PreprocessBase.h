#ifndef PREPROCESSBASE_H
#define PREPROCESSBASE_H

class Data;

class PreprocessBase{
public:
	PreprocessBase() : mRange{ nullptr }, mBlock{ 0 }, mTNum{ 0 }, mSize{ 0 } {};
	virtual ~PreprocessBase() {};
	int get(int i) const;
	int* getAll() const;
	int block() const;
	double get_percent() const;
	int get_size() const;
protected:
	// 0,13,25,50 = 0~13 and 25~50
	int* mRange;
	// The number of range blocks
	int mBlock;
	// The percent of reduce
	double mPercent;
	// The max size of blocks
	int mSize;
	// The number of threads
	int mTNum;
	// First call
	void start(const Data& db, const Data& query, const int threshold, const char* id);
	// Main process
	virtual void process(const Data& db, const Data& query, const int threshold) {};
};


#endif
