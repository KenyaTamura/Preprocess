#include"PreprocessBase.h"
#include"Data.h"
#include<iostream>
#include<algorithm>

using namespace std;

void PreprocessBase::start(const Data& db, const Data& query, const int threshold, const char* id) {
	if (db.size() < query.size()) {
		cout << "Reverse db and query" << endl;
		return;
	}
	cout << "Preprocess" << id << " start" << endl;
	// Check the range
	process(db, query, threshold);
	cout << "Block = " << mBlock << endl;
	int newrange = 0;
	for (int i = 0; i < mBlock; ++i) {
		newrange += mRange[i * 2 + 1] - mRange[i * 2] + 1 + query.size();
	}
	mPercent = 100 * (double)(newrange) / (double)(db.size());
	cout << "New length is " << mPercent << "%" << endl;
	cout << "Preprocess" << id << " end" << endl;

}

int PreprocessBase::get(int i) const {
	if (i < mBlock * 2) {
		return mRange[i];
	}
	else {
		std::cerr << "Out of bounds\n";
		exit(1);
	}
	return -1;
}

int* PreprocessBase::getAll() const {
	return mRange;
}

int PreprocessBase::block() const {
	return mBlock;
}

double PreprocessBase::get_percent() const {
	return mPercent;
}

void PreprocessBase::get_hash(const Data& data, int size, int* hash, int start) const {
	for (int i = 0; i < size - mAcid + 1; ++i) {
		++hash[convert2(data, i + start)];
	}
}

int PreprocessBase::get_score(const int* hash1, const int* hash2) const {
	int score = 0;
	int hash_num = static_cast<int>(pow(4, mAcid));
	for (int i = 0; i < hash_num; ++i) {
		score += min(hash1[i], hash2[i]);
	}
	if (score == 0) { return 0; }
	else { return score + mAcid - 1; }
}