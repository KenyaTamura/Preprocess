#include"Preprocess.h"

#include<algorithm>
#include<iostream>

#include"Timer.h"
#include"Writer.h"

using namespace std;

Preprocess::Preprocess(const Data& txt, const Data& ptn, const int threshold) : mRange{ nullptr }, mBlock{ 0 } {
	if (txt.size() < ptn.size()) {
		cout << "Reverse txt and ptn" << endl;
		return;
	}
	cout << "Preprocess process start" << endl;
	// The search range of origin
//	list<int> range_list;
//	get_range(txt, ptn, threshold, range_list);
// Shape the original range
//	shape(range, ptn.size());
	
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

Preprocess::Preprocess(const Data& txt, const Data& ptn, const char* fname) : mRange{ nullptr }, mBlock{ 0 } {
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
	w.writing_score(fname, score, txt.size() - ptn.size(), txt.size()/100, 100);
	cout << "Preprocess process end" << endl;
}

Preprocess::~Preprocess() {
	if (mRange) {
		delete[] mRange;
	}
}

int Preprocess::get(int i) const {
	if (i < mBlock * 2) {
		return mRange[i];
	}
	else {
		cerr << "Out of bounds\n";
		exit(1);
	}
	return -1;
}

int* Preprocess::getAll() const {
	return mRange;
}

int Preprocess::block() const {
	return mBlock;
}

void Preprocess::get_range(const Data& txt, const Data& ptn, const int threshold, list<int>& range) {
	// Get hash, the length is ptn size
	Hash hashT = get_hash(txt, ptn.size());
	Hash hashP = get_hash(ptn, ptn.size());
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	int j = 0;
	for (int i = 0; i < size; ++i) {
		if (get_score(hashT, hashP) >= threshold) {
			range.push_back(i);
		}
		// Minus i and plus i + ptn.size()
		minus_hash(hashT, txt[i]);
		plus_hash(hashT, txt[i + psize]);
	}
}

void Preprocess::get_range(const Data& txt, const Data& ptn, const int threshold) {
	// Get hash, the length is ptn size
	Hash hashT = get_hash(txt, ptn.size());
	Hash hashP = get_hash(ptn, ptn.size());
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	int block = 0;
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
		minus_hash(hashT, txt[i]);
		plus_hash(hashT, txt[i + psize]);
	}
	if (block % 2 == 1) {
		++block;
	}
	mBlock = block / 2;
}

void Preprocess::check_score(const Data& txt, const Data& ptn, int* score) {
	// Get hash, the length is ptn size
	Hash hashT = get_hash(txt, ptn.size());
	Hash hashP = get_hash(ptn, ptn.size());
	int size = txt.size() - ptn.size();
	int psize = ptn.size();
	int j = 0;
	for (int i = 0; i < size; ++i) {
		score[i] = get_score(hashT, hashP);
		// Minus i and plus i + ptn.size()
		minus_hash(hashT, txt[i]);
		plus_hash(hashT, txt[i + psize]);
	}
}

Preprocess::Hash Preprocess::get_hash(const Data& data, int size) const {
	Hash h;
	for (int i = 0; i < size; ++i) {
		plus_hash(h, data[i]);
	}
	return h;
}

int Preprocess::get_score(const Hash& hash1, const Hash& hash2) const {
	int score = 0;
	score += std::min(hash1.ADE, hash2.ADE);
	score += std::min(hash1.CYT, hash2.CYT);
	score += std::min(hash1.GUA, hash2.GUA);
	score += std::min(hash1.TYM, hash2.TYM);
	return score;
}

void Preprocess::shape(list<int>& origin, const int interval) {
	// Combine some block
	list<int>::iterator it = origin.begin();
	if (it == origin.end()) { return; }
	int prev = *it;	// Previous value of iterator
	++it;
	while (it != origin.end()) {
		// The distance over interval
		if (*it - prev > interval) {
			origin.insert(it, prev);
			prev = *it;
			++mBlock;
			++it;
		}
		// Combine
		else {
			prev = *it;
			it = origin.erase(it);
		}
	}
	origin.insert(it, prev);
	++mBlock;
	// Copy to mRange
	mRange = new int[mBlock * 2];
	it = origin.begin();
	for (int i = 0; it != origin.end(); ++it) {
		mRange[i] = *it;
		++i;
	}
}

void Preprocess::plus_hash(Preprocess::Hash& h, char acid) const {
	if (acid == 'A') { ++h.ADE; }
	else if (acid == 'C') { ++h.CYT; }
	else if (acid == 'G') { ++h.GUA; }
	else { ++h.TYM; }
}

void Preprocess::minus_hash(Preprocess::Hash& h, char acid) const {
	if (acid == 'A') { --h.ADE; }
	else if (acid == 'C') { --h.CYT; }
	else if (acid == 'G') { --h.GUA; }
	else { --h.TYM; }
}
