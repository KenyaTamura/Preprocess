#ifndef PREPROCESSSINGLE_H
#define PREPROCESSSINGLE_H

#include"PreprocessBase.h"

class Data;

class PreprocessSingle : public PreprocessBase{
public:
	PreprocessSingle(const Data& txt, const Data& ptn, const int threshold);
	PreprocessSingle(const Data& txt, const Data& ptn, const char* fname);	// research score
	~PreprocessSingle();
private:
	// Main process
	/// txt = long sequence, ptn = short sequence, threshold = Border of OK
	void get_range(const Data& txt, const Data& ptn, const int threshold);
	// Get hash
	void get_hash(const Data& data, int length, int* hash) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
	// Check score
	void check_score(const Data& txt, const Data& ptn, int* range);
};


#endif