#ifndef PREPROCESSDOUBLE_H
#define PREPROCESSDOUBLE_H

#include"PreprocessBase.h"

class Data;

class PreprocessDouble : public PreprocessBase {
public:
	PreprocessDouble(const Data& txt, const Data& ptn, const int threshold);
	~PreprocessDouble();
private:
	// Main process
	void get_range(const Data& txt, const Data& ptn, const int threshold);
	// Get hash
	void get_hash(const Data& data, int length, char* hash) const;
	// Compare hash
	int get_score(const char* hash1, const char* hash2) const;
	// Check score
	void check_score(const Data& txt, const Data& ptn, int* range);
};


#endif
