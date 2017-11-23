#include"PreprocessTriple.h"
#include"Data.h"

#include<iostream>
#include<algorithm>
#include<thread>
#include<vector>

#include"Writer.h"

using namespace std;

namespace {
	constexpr int Type = 64;
	constexpr int Triple = 2;
	auto convert = [](char acid) {
		if (acid == 'A') return 0;
		else if (acid == 'C') return 1;
		else if (acid == 'G') return 2;
		else if (acid == 'T') return 3;
		else return 0;
	};
}

PreprocessTriple::PreprocessTriple(const Data& db, const Data& query, const int threshold, const int thread_num) {
	mTNum = thread_num;
	start(db, query, threshold, "Triple");
}

PreprocessTriple::~PreprocessTriple() {
	if (mRange) {
		delete[] mRange;
	}
}

void PreprocessTriple::process(const Data& db, const Data& query, const int threshold) {
	cout << "Thread number is " << mTNum << endl;
	vector<thread> thr(mTNum);
	vector<int> blocks(mTNum);
	vector<int*> buffer(mTNum);
	int start = 0;
	for (int i = 0; i < mTNum; ++i) {
		// buffer size = (search length) / (base length)
		buffer[i] = new int[db.size() / mTNum / query.size()];
		int end = start + db.size() / mTNum + query.size();
		if (i == mTNum - 1) {
			end = db.size();
		}
		thr[i] = thread{ &PreprocessTriple::get_range, this,
			ref(db), ref(query), threshold,
			start, end,
			ref(blocks[i]), ref(buffer[i])
		};
		start += db.size() / mTNum;
	}
	for (int i = 0; i < mTNum; ++i) {
		thr[i].join();
		mBlock += blocks[i] / 2;
	}
	mRange = new int[mBlock * 2];
	int count = 0;
	for (int i = 0; i < mTNum; ++i) {
		for (int j = 0; j < blocks[i]; j++) {
			mRange[count++] = buffer[i][j];
		}
		delete[] buffer[i];
	}
}

void PreprocessTriple::get_range(const Data& db, const Data& query, const int threshold, int start, int end, int& block, int* buffer) {
	// Get hash, the length is query size
	int hashD[Type]{ 0 };
	int hashQ[Type]{ 0 };
	get_hash(db, query.size(), hashD, start);
	get_hash(query, query.size(), hashQ, 0);
	// applied size
	int size = end - query.size();
	int qsize = query.size();
	block = 0;
	int score = get_score(hashD, hashQ);
	for (int i = start; i < size; ++i) {
		if (score >= threshold) {
			// if block start
			if (block % 2 == 0) {
				buffer[block++] = i;	// start point
				buffer[block] = i + 1;	// end point(provisional)
			}
			// if block already started
			else {
				buffer[block] = i;
			}
		}
		else {
			// if delete interval over query size 
			if (block % 2 == 1 && i - buffer[block] > qsize) {
				++block;
			}
		}
		// Minus hash i~i+2 and plus hash i+query.size()-2 ~ i+query.size()
		int dec = (convert(db[i]) << 4) + (convert(db[i + 1]) << 2) + convert(db[i + 2]);
		int offset = i + qsize;
		int inc = (convert(db[offset - 2]) << 4) + (convert(db[offset - 1]) << 2) + convert(db[offset]);
		if (hashD[dec] <= hashQ[dec]) {
			--score;
		}
		--hashD[dec];
		if (hashD[inc] < hashQ[inc]) {
			++score;
		}
		++hashD[inc];
	}
	if (block % 2 == 1) {
		++block;
	}
}

void PreprocessTriple::get_hash(const Data& data, int size, int* hash, int start) const {
	for (int i = 0; i < size - Triple; ++i) {
		++hash[(convert(data[i + start]) << 4) + (convert(data[i + start + 1]) << 2) + convert(data[i + start + 2])];
	}
}

int PreprocessTriple::get_score(const int* hash1, const int* hash2) const {
	int score = 0;
	for (int i = 0; i < Type; ++i) {
		score += min(hash1[i], hash2[i]);
	}
	if (score == 0) { return 0; }
	else { return score + Triple; }
}

