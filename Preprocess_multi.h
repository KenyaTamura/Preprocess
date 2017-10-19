#ifndef PREPROCESS_MULTI_H
#define PREPROCESS_MULTI_H

class Data;

class Preprocess_multi{
public:
	Preprocess_multi(const Data& txt, const Data& ptn, const int threshold);
	Preprocess_multi(const Data& txt, const Data& ptn, const char* fname);	// research score
	~Preprocess_multi();
private:
	// 0,13,25,50 = 0~13 and 25~50
	int* mRange;
	// The number of range blocks
	int mBlock;
	// Main process
	/// txt = long sequence, ptn = short sequence, threshold = Border of OK
	void get_range(const Data& txt, const Data& ptn, const int threshold);
	// Get hash
	void get_hash(const Data& data, int length, char* hash) const;
	// Compare hash
	int get_score(const char* hash1, const char* hash2) const;
	// Check score
	void check_score(const Data& txt, const Data& ptn, int* range);
	// Length of hash array
	int hash_len;
};



#endif
