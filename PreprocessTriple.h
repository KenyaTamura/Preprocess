#ifndef PREPROCESSTRIPLE_H
#define PREPROCESSTRIPLE_H

#include"PreprocessBase.h"

class Data;

class PreprocessTriple : public PreprocessBase {
public:
	PreprocessTriple(const Data& txt, const Data& ptn, const int threshold);
	~PreprocessTriple();
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
