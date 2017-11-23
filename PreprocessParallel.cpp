#include"PreprocessParallel.h"
#include"Data.h"

#include<algorithm>
#include<iostream>
#include<thread>
#include<vector>

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
	int Num = 8;
}

PreprocessParallel::PreprocessParallel(const Data& db, const Data& query, const int threshold, const int thread_num){
	Num = thread_num;
	start(db, query, threshold, "Parallel");
}

PreprocessParallel::~PreprocessParallel() {
	if (mRange) {
		delete[] mRange;
	}
}

void PreprocessParallel::process(const Data& db, const Data& query, const int threshold) {
	cout << "Thread number is " << Num << endl;
	vector<thread> thr(Num);
	vector<int> blocks(Num);
	vector<int*> buffer(Num);
	int start = 0;
	for (int i = 0; i < Num; ++i) {
		// buffer size = (search length) / (base length)
		buffer[i] = new int[db.size() / Num / query.size()];
		int end = start + db.size() / Num + query.size();
		if (i == Num - 1) {
			end = db.size();
		}
		thr[i] = thread{ &PreprocessParallel::get_range, this,
			ref(db), ref(query), threshold,
			start, end,
			ref(blocks[i]), ref(buffer[i])
		};
		start += db.size() / Num;
	}
	for (int i = 0; i < Num; ++i) {
		thr[i].join();
		mBlock += blocks[i] / 2;
	}
	mRange = new int[mBlock * 2];
	int count = 0;
	for (int i = 0; i < Num; ++i) {
		for (int j = 0; j < blocks[i]; j++) {
			mRange[count++] = buffer[i][j];
		}
		delete[] buffer[i];
	}
}

void PreprocessParallel::get_range(const Data& db, const Data& query, const int threshold, int start, int end, int& block, int* buffer) {
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
		// Minus hash i~i+1 and plus hash i + query.size()-1 ~ i+query.size()
		int dec = convert(db[i]);
		int inc = convert(db[i + qsize]);
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

