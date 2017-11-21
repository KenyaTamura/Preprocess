#ifndef PREPROCESSSINGLE_H
#define PREPROCESSSINGLE_H

#include"PreprocessBase.h"

class Data;

class PreprocessSingle : public PreprocessBase{
public:
	PreprocessSingle(const Data& txt, const Data& ptn, const int threshold);
	~PreprocessSingle();
private:
	// Main process
	/// txt = long sequence, ptn = short sequence, threshold = Border of OK
	void get_range(const Data& txt, const Data& ptn, const int threshold, int start, int end);
	// Get hash
	void get_hash(const Data& data, int length, int* hash, int start) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
};


#endif