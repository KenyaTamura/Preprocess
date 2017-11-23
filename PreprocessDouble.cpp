#include"PreprocessDouble.h"
#include"Data.h"

#include<iostream>
#include<algorithm>

#include"Writer.h"

using namespace std;

namespace {
	constexpr int Double = 1;
	constexpr int Type = 16;
	auto convert = [](char acid) {
		if (acid == 'A') return 0;
		else if (acid == 'C') return 1;
		else if (acid == 'G') return 2;
		else if (acid == 'T') return 3;
		else return 0;
	};
}

PreprocessDouble::PreprocessDouble(const Data& db, const Data& query, const int threshold) {
	start(db, query, threshold, "Double");
}

void PreprocessDouble::process(const Data& db, const Data& query, const int threshold) {
	get_range(db, query, threshold);
}

PreprocessDouble::~PreprocessDouble() {
	if (mRange) {
		delete[] mRange;
	}
}

void PreprocessDouble::check_score(const Data& db, const Data& query, int* range) {
	// Get hash, the length is query size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(db, query.size(), hashT);
	get_hash(query, query.size(), hashP);
	int size = db.size() - query.size();
	int psize = query.size();
	auto sum = [&](int i) {
		return (convert(db[i]) << 2) + convert(db[i + 1]);
	};
	for (int i = 0; i < size; ++i) {
		range[i] = get_score(hashT, hashP);
		// Minus i and plus i + query.size()
		--hashT[sum(i)];
		++hashT[sum(i + psize - Double)];
	}
}

void PreprocessDouble::get_range(const Data& db, const Data& query, const int threshold) {
	// Buffer
	int* buffer = new int[db.size() / query.size()];
	// Get hash, the length is query size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(db, query.size(), hashT);
	get_hash(query, query.size(), hashP);
	for (int i = 0; i < Type; ++i) {
		cout << hashT[i] << " ";
	}
	cout << endl;
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
		int dec = (convert(db[i]) << 2) + convert(db[i + 1]);
		int offset = i + psize;
		int inc = (convert(db[offset - 1]) << 2) + convert(db[offset]);
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

int PreprocessDouble::get_score(const int* hash1, const int* hash2) const {
	int score = 0;
	for (int i = 0; i < Type; ++i) {
		score += min(hash1[i], hash2[i]);
	}
	if (score == 0) { return 0; }
	else { return score + Double; }
}

void PreprocessDouble::get_hash(const Data& data, int size, int* hash) const {
	for (int i = 0; i < size - Double; ++i) {
		++hash[(convert(data[i]) << 2) + convert(data[i + 1])];
	}
	/*for (int i = 0; i < Type; ++i) {
	cout << static_cast<int>(hash[i]) << endl;
	}*/
}
