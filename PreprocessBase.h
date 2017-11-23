#ifndef PREPROCESSBASE_H
#define PREPROCESSBASE_H

class Data;

class PreprocessBase{
public:
	PreprocessBase() : mRange{ nullptr }, mBlock{ 0 } {};
	virtual ~PreprocessBase() {};
	int get(int i) const;
	int* getAll() const;
	int block() const;
	double get_percent() const;
protected:
	// 0,13,25,50 = 0~13 and 25~50
	int* mRange;
	// The number of range blocks
	int mBlock;
	// The percent of reduce
	double mPercent;
	// The number of mix acid
	int mAcid;
	// First call
	void start(const Data& db, const Data& query, const int threshold, const char* id);
	// Main process
	virtual void process(const Data& db, const Data& query, const int threshold) {};
	// Get hash
	void get_hash(const Data& data, int length, int* hash, int start) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
	// Set acid get hash
	virtual int convert2(const Data& data, int i) const { return -1; };
};


#endif
