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

PreprocessParallel::PreprocessParallel(const Data& txt, const Data& ptn, const int threshold) : mRange{ nullptr }, mBlock{ 0 } {
	if (txt.size() < ptn.size()) {
		cout << "Reverse txt and ptn" << endl;
		return;
	}
	cout << "Preprocess Parallel start" << endl;
	// Check the range
	thread thread1{ &PreprocessParallel::get_range, this, ref(txt), ref(ptn), threshold, (txt.size() / 2) + 1, txt.size() -1};
	thread thread2{ &PreprocessParallel::get_range, this, ref(txt), ref(ptn), threshold, 0, (txt.size() / 2) };
	thread1.join();
	thread2.join();
	cout << "Block = " << mBlock << endl;
	mRange = new int[mBlock];
	int newrange = ptn.size();
	for (int i = 0; i < mBlock; ++i) {
		newrange += mRange[i * 2 + 1] - mRange[i * 2] + 1;
	}
	mPercent = 100 * (double)(newrange) / (double)(txt.size());
	cout << "New length is " << mPercent << "%" << endl;
	cout << "Preprocess Parallel end" << endl;
}

PreprocessParallel::~PreprocessParallel() {
	if (mRange) {
		delete[] mRange;
	}
}


int PreprocessParallel::get(int i) const {
	if (i < mBlock * 2) {
		return mRange[i];
	}
	else {
		std::cerr << "Out of bounds\n";
		exit(1);
	}
	return -1;
}

int* PreprocessParallel::getAll() const {
	return mRange;
}

int PreprocessParallel::block() const {
	return mBlock;
}

double PreprocessParallel::get_percent() const {
	return mPercent;
}


void PreprocessParallel::get_range(const Data& txt, const Data& ptn, const int threshold, int start, int end) {
	// Get hash, the length is ptn size
	int hashT[Type]{ 0 };
	int hashP[Type]{ 0 };
	get_hash(txt, ptn.size(), hashT, start);
	get_hash(ptn, ptn.size(), hashP, 0);
	int size = end - start - ptn.size() + 1;
	int psize = ptn.size();
	int block = 0;
	int score = get_score(hashT, hashP);
	// Buffer
	int* buffer = new int[size / psize];
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
	mBlock += block / 2;
	/*
	mRange = new int[block];
	for (int i = 0; i < block; ++i) {
		mRange[i] = buffer[i];
	}*/
	delete[] buffer;
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

