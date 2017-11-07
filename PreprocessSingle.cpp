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

PreprocessSingle::PreprocessSingle(const Data& txt, const Data& ptn, const int threshold) {
	process(txt, ptn, threshold, "Single");
}

PreprocessSingle::PreprocessSingle(const Data& txt, const Data& ptn, const char* fname) {
	if (txt.size() < ptn.size()) {
		cout << "Reverse txt and ptn" << endl;
		return;
	}
	cout << "PreprocessSingle start" << endl;
	// The search range of origin
	int* score = new int[txt.size() - ptn.size()];
	// Check the range
	check_score(txt, ptn, score);
	Writer w;
	w.writing_score(fname, score, txt.size() - ptn.size(), txt.size() / 100, 100);
	cout << "PreprocessSingle end" << endl;
}

PreprocessSingle::~PreprocessSingle() {
	if (mRange) {
		delete[] mRange;
	}
}

void PreprocessSingle::check_score(const Data& txt, const Data& ptn, int* range) {
	// Get hash, the length is ptn size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(txt, ptn.size(), hashT);
	get_hash(ptn, ptn.size(), hashP);
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	auto sum = [&](int i) {
		return convert(txt[i]);
	};
	for (int i = 0; i < size; ++i) {
		range[i] = get_score(hashT, hashP);
		// Minus i and plus i + ptn.size()
		--hashT[sum(i)];
		++hashT[sum(i + psize)];
	}
}

void PreprocessSingle::get_range(const Data& txt, const Data& ptn, const int threshold) {
	// Buffer
	int* buffer = new int[txt.size() / ptn.size()];
	// Get hash, the length is ptn size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(txt, ptn.size(), hashT);
	get_hash(ptn, ptn.size(), hashP);
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
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
		// Minus hash i~i+1 and plus hash i + ptn.size()-1 ~ i+ptn.size()
		int dec = convert(txt[i]);
		int inc = convert(txt[i + psize]);
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

