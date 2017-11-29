#include"PreprocessBase.h"
#include"Data.h"
#include<iostream>
#include<thread>

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
		int size = mRange[i * 2 + 1] - mRange[i * 2] + 1 + query.size();
		newrange += size;
		if (mSize < size) { mSize = size; }
	}
	mPercent = 100 * (double)(newrange) / (double)(db.size());
	cout << "Max size is " << mSize << endl;
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

int PreprocessBase::get_size() const {
	return mSize;
}