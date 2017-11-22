#include"PreprocessParallel.h"
#include"Data.h"

#include<algorithm>
#include<iostream>

#include"Writer.h"

#include<thread>

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

PreprocessParallel::PreprocessParallel(const Data& db, const Data& query, const int threshold){
	start(db, query, threshold, "Parallel");
}

PreprocessParallel::~PreprocessParallel() {
	if (mRange) {
		delete[] mRange;
	}
}

void PreprocessParallel::process(const Data& db, const Data& query, const int threshold) {
	thread thr[4];
	int blocks[4];
	int buffer[4][10000];
	int start = 0;
	for (int i = 0; i < 4; ++i) {
		int end = start + db.size() / 4 + query.size();
		if (i == 3) {
			end = db.size();
		}
		thr[i] = thread{ &PreprocessParallel::get_range, this,
			ref(db), ref(query), threshold,
			start, end,
			ref(blocks[i]), ref(buffer[i])
		};
		start += db.size() / 4;
	}
	for (int i = 0; i < 4; ++i) {
		thr[i].join();
		mBlock += blocks[i] / 2;
	}
	mRange = new int[mBlock * 2];
	int count = 0;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < blocks[i]; j++) {
			mRange[count++] = buffer[i][j];
		}
	}
}

void PreprocessParallel::get_range(const Data& db, const Data& query, const int threshold, int start, int end, int& block, int* buffer) {
	// Get hash, the length is query size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(db, query.size(), hashT, start);
	get_hash(query, query.size(), hashP, 0);
	int size = end - start - query.size() + 1;
	int psize = query.size();
	block = 0;
	int score = get_score(hashT, hashP);
	size += start;
	for (int i = start; i < size; ++i) {
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
	/*
	mRange = new int[block];
	for (int i = 0; i < block; ++i) {
		mRange[i] = buffer[i];
	}*/
}

void PreprocessParallel::get_hash(const Data& data, int size, int* hash, int start) const {
	for (int i = 0; i < size; ++i) {
		++hash[convert(data[i + start])];
	}
}

int PreprocessParallel::get_score(const int* hash1, const int* hash2) const {
	int score = 0;
	for (int i = 0; i < Type; ++i) {
		score += min(hash1[i], hash2[i]);
	}
	if (score == 0) { return 0; }
	else { return score; }
}

