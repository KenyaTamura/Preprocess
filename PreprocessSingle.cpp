#include"PreprocessSingle.h"
#include"Data.h"

#include<algorithm>
#include<iostream>

#include"Writer.h"

using namespace std;

namespace {
	constexpr int Type = 4;
	auto convert = [](char acid) {
		if (acid == 'A') return 0;
		else if (acid == 'C') return 1;
		else if (acid == 'G') return 2;
		else if (acid == 'T') return 3;
		else return 0;
	};
}

PreprocessSingle::PreprocessSingle(const Data& db, const Data& query, const int threshold) {
	start(db, query, threshold, "Single");
}

PreprocessSingle::~PreprocessSingle() {
	if (mRange) {
		delete[] mRange;
	}
}

void PreprocessSingle::process(const Data& db, const Data& query, const int threshold) {
	get_range(db, query, threshold);
}

void PreprocessSingle::get_range(const Data& db, const Data& query, const int threshold) {
	// Get hash, the length is query size
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
		// Minus hash i~i+1 and plus hash i + query.size()-1 ~ i+query.size()
		int dec = convert(db[i]);
		int inc = convert(db[i + psize]);
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

void PreprocessSingle::get_hash(const Data& data, int size, int* hash) const {
	for (int i = 0; i < size; ++i) {
		++hash[convert(data[i])];
	}
}

int PreprocessSingle::get_score(const int* hash1, const int* hash2) const {
	int score = 0;
	for (int i = 0; i < Type; ++i) {
		score += min(hash1[i], hash2[i]);
	}
	if (score == 0) { return 0; }
	else { return score; }
}

