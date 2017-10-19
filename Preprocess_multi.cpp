#include"Preprocess_multi.h"
#include"Data.h"

#include<iostream>
#include<algorithm>

#include"Writer.h"

using namespace std;

Preprocess_multi::Preprocess_multi(const Data& txt, const Data& ptn, const int threshold) : mRange{ nullptr }, mBlock{ 0 } {
	if (txt.size() < ptn.size()) {
		cout << "Reverse txt and ptn" << endl;
		return;
	}
	cout << "Preprocess process start" << endl;
	mRange = new int[txt.size() / ptn.size()];
	// Check the range
	get_range(txt, ptn, threshold);
	cout << "Block = " << mBlock << endl;
	int newrange = ptn.size();
	for (int i = 0; i < mBlock; ++i) {
		newrange += mRange[i * 2 + 1] - mRange[i * 2] + 1;
	}
	cout << "New length is " << 100 * (double)(newrange) / (double)(txt.size()) << "%" << endl;
	cout << "Preprocess process end" << endl;
}

Preprocess_multi::Preprocess_multi(const Data& txt, const Data& ptn, const char* fname) : mRange{ nullptr }, mBlock{ 0 } {
	if (txt.size() < ptn.size()) {
		cout << "Reverse txt and ptn" << endl;
		return;
	}
	cout << "Preprocess process start" << endl;
	// The search range of origin
	int* score = new int[txt.size() - ptn.size()];
	// Check the range
	check_score(txt, ptn, score);
	Writer w;
	w.writing_score(fname, score, txt.size() - ptn.size(), txt.size() / 100, 100);
	cout << "Preprocess process end" << endl;
}


Preprocess_multi::~Preprocess_multi() {
	if (mRange) {
		delete[] mRange;
	}
}

void Preprocess_multi::check_score(const Data& txt, const Data& ptn, int* range) {
	// Get hash, the length is ptn size
	char hashT[256]{ 0 };
	char hashP[256]{ 0 };
	get_hash(txt, ptn.size(), hashT);
	get_hash(ptn, ptn.size(), hashP);
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	auto convert = [](char acid) {
		if (acid == 'A') return 0;
		else if (acid == 'C') return 1;
		else if (acid == 'G') return 2;
		else if (acid == 'T') return 3;
		else return 0;
	};
	auto sum = [&](int i) {
		return convert(txt[i]) * 64 + convert(txt[i + 1]) * 16 + convert(txt[i + 2]) * 4 + convert(txt[i + 3]);
	};
	for (int i = 0; i < size; ++i) {
		range[i] = get_score(hashT, hashP);
		// Minus i and plus i + ptn.size()
		--hashT[sum(i)];
		++hashT[sum(i + psize - 3)];
	}
}

void Preprocess_multi::get_range(const Data& txt, const Data& ptn, const int threshold) {
	// Get hash, the length is ptn size
	char hashT[256]{ 0 };
	char hashP[256]{ 0 };
	get_hash(txt, ptn.size(), hashT);
	get_hash(ptn, ptn.size(), hashP);
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	int block = 0;
	auto convert = [](char acid) {
		if (acid == 'A') return 0;
		else if (acid == 'C') return 1;
		else if (acid == 'G') return 2;
		else if (acid == 'T') return 3;
		else return 0;
	};
	auto sum = [&](int i) {
		return convert(txt[i]) * 64 + convert(txt[i + 1]) * 16 + convert(txt[i + 2]) * 4 + convert(txt[i + 3]);
	};
	for (int i = 0; i < size; ++i) {
		if (get_score(hashT, hashP) >= threshold) {
			if (block % 2 == 0) {
				mRange[block++] = i;
				mRange[block] = i + 1;
			}
			else {
				mRange[block] = i;
			}
		}
		else {
			if (block % 2 != 0 && i - mRange[block] > psize) {
				++block;
			}
		}
		// Minus i and plus i + ptn.size()
		--hashT[sum(i)];
		++hashT[sum(i + psize - 3)];
	}
	if (block % 2 == 1) {
		++block;
	}
	mBlock = block / 2;
}

int Preprocess_multi::get_score(const char* hash1, const char* hash2) const{
	int score = 0;
	for (int i = 0; i < 256; ++i) {
		score += min(hash1[i], hash2[i]);
	}
	return score + 3;
}

void Preprocess_multi::get_hash(const Data& data, int size, char* hash) const{
	auto convert = [](char acid) { 
		if (acid == 'A') return 0;
		else if (acid == 'C') return 1;
		else if (acid == 'G') return 2;
		else if (acid == 'T') return 3;
		else return 0;
	};
	auto sum = [&](int i) {
		return convert(data[i]) * 64 + convert(data[i + 1]) * 16 + convert(data[i + 2]) * 4 + convert(data[i + 3]);
	};
	for (int i = 0; i < size - 3; i++) {
		++hash[sum(i)];
	}
}
