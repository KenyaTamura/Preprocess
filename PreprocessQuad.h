#ifndef PREPROCESSQUAD_H
#define PREPROCESSQUAD_H

#include"PreprocessBase.h"

class Data;

class PreprocessQuad : public PreprocessBase{
public:
	PreprocessQuad(const Data& db, const Data& query, const int threshold);
	PreprocessQuad(const Data& db, const Data& query, const char* fname);	// research score
	~PreprocessQuad();
private:
	// Main process
	void get_range(const Data& db, const Data& query, const int threshold);
	// Get hash
	void get_hash(const Data& data, int length, int* hash) const;
	// Compare hash
	int get_score(const int* hash1, const int* hash2) const;
	// Check score
	void check_score(const Data& db, const Data& query, int* range);
};

#endif
