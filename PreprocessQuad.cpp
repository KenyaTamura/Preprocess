#include"PreprocessQuad.h"
#include"Data.h"

#include<iostream>
#include<algorithm>

#include"Writer.h"

using namespace std;

namespace {
	constexpr int Quad = 3;
	constexpr int Type = 256;
	auto convert = [](char acid) {
		if (acid == 'A') return 0;
		else if (acid == 'C') return 1;
		else if (acid == 'G') return 2;
		else if (acid == 'T') return 3;
		else return 0;
	};
}

PreprocessQuad::PreprocessQuad(const Data& db, const Data& query, const int threshold) {
	start(db, query, threshold, "Quad");
}

PreprocessQuad::PreprocessQuad(const Data& db, const Data& query, const char* fname) {
	if (db.size() < query.size()) {
		cout << "Reverse db and query" << endl;
		return;
	}
	cout << "PreprocessQuad start" << endl;
	// The search range of origin
	int* score = new int[db.size() - query.size()];
	// Check the range
	check_score(db, query, score);
	Writer w;
	w.writing_score(fname, score, db.size() - query.size(), db.size() / 100, 100);
	cout << "PreprocessQuad end" << endl;
}

PreprocessQuad::~PreprocessQuad() {
	if (mRange) {
		delete[] mRange;
	}
}

void PreprocessQuad::check_score(const Data& db, const Data& query, int* range) {
	// Get hash, the length is query size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(db, query.size(), hashT);
	get_hash(query, query.size(), hashP);
	int size = db.size() - query.size();
	int psize = query.size();
	auto sum = [&](int i) {
		return (convert(db[i]) << 6) + (convert(db[i + 1]) << 4) + (convert(db[i + 2]) << 2) + convert(db[i + 3]);
	};
	for (int i = 0; i < size; ++i) {
		range[i] = get_score(hashT, hashP);
		// Minus i and plus i + query.size()
		--hashT[sum(i)];
		++hashT[sum(i + psize - Quad)];
	}
}

void PreprocessQuad::get_range(const Data& db, const Data& query, const int threshold) {
	// Buffer
	int* buffer = new int[db.size() / query.size()];
	// Get hash, the length is query size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(db, query.size(), hashT);
	get_hash(query, query.size(), hashP);
	int size = db.size() - query.size();
	int psize = query.size();
	int block = 0;
	int score = get_score(hashT, hashP);
	for (int i = 0; i < size; ++i) {
		if (score >= threshold) {
			if (block % 2 == 0) {
				buffer[block++] = i;
				buffer[block] = i + 1;
			}
			else {
				buffer[block] = i;
			}
		}
		else {
			if (block % 2 != 0 && i - buffer[block] > psize) {
				++block;
			}
		}
		// Minus hash i~i+3 and plus hash i + query.size()-3 ~ i+query.size()
		int dec = (convert(db[i]) << 6) + (convert(db[i + 1]) << 4) + (convert(db[i + 2]) << 2) + convert(db[i + 3]);
		int offset = i + psize;
		int inc = (convert(db[offset - 3]) << 6) + (convert(db[offset - 2]) << 4) + (convert(db[offset - 1]) << 2) + convert(db[offset]);
		if (hashT[dec] <= hashP[dec]) {
			--score;
		}
		--hashT[dec];
		if (hashT[inc] < hashP[inc]) {
			++score;
		}
		++hashT[inc];
	}
	if (block % 2 == 1) {
		++block;
	}
	mBlock = block / 2;
	mRange = new int[block];
	for (int i = 0; i < block; ++i) {
		mRange[i] = buffer[i];
	}
	delete[] buffer;
}

int PreprocessQuad::get_score(const int* hash1, const int* hash2) const{
	int score = 0;
	for (int i = 0; i < Type; ++i) {
		score += min(hash1[i], hash2[i]);
	}
	if (score == 0) { return 0; }
	else { return score + Quad; }
}

void PreprocessQuad::get_hash(const Data& data, int size, int* hash) const{
	for (int i = 0; i < size - Quad; ++i) {
		++hash[(convert(data[i]) << 6) + (convert(data[i + 1]) << 4) + (convert(data[i + 2]) << 2) + convert(data[i + 3])];
	}
	/*for (int i = 0; i < Type; ++i) {
		cout << static_cast<int>(hash[i]) << endl;
	}*/
}
