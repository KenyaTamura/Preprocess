#ifndef PREPROCESSPARALLEL_H
#define PREPROCESSPARALLEL_H

class Data;

#include"PreprocessBase.h"

class PreprocessParallel : public PreprocessBase {
public:
	PreprocessParallel(const Data& db, const Data& query, const int threshold);
	~PreprocessParallel();
private:
	// Main process
	void process(const Data& txt, const Data& ptn, const int threshold);
	/// db = long sequence, query = short sequence, threshold = Border of OK, start/end = start/end point at each thread, block/buf = result
	void get_range(const Data& db, const Data& query, const int threshold, int start, int end, int& block, int* buf);
	// Get hash
	void get_hash(const Data& data, int length, int* hash, int start) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
};


#endif
