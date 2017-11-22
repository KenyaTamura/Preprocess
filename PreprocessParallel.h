#ifndef PREPROCESSPARALLEL_H
#define PREPROCESSPARALLEL_H

class Data;

class PreprocessParallel {
public:
	PreprocessParallel(const Data& txt, const Data& ptn, const int threshold);
	~PreprocessParallel();
	int get(int i) const;
	int* getAll() const;
	int block() const;
	double get_percent() const;
private:
	// 0,13,25,50 = 0~13 and 25~50
	int* mRange;
	// The number of range blocks
	int mBlock;
	// The percent of reduce
	double mPercent;
	// Main process
	/// txt = long sequence, ptn = short sequence, threshold = Border of OK, start/end = start/end point at each thread, block/buf = result
	void get_range(const Data& txt, const Data& ptn, const int threshold, int start, int end, int& block, int* buf);
	// Get hash
	void get_hash(const Data& data, int length, int* hash, int start) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
};


#endif
