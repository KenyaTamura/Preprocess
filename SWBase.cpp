#include"SWBase.h"

#include"Data.h"
#include"Cost.h"

#include<algorithm>
#include<string>
#include<iostream>

SWBase::SWBase(int threshold) :
	mScore{ nullptr },
	mThreshold{ threshold },
	mSize{ 0 },
	mMaxScore{ -1 },
	mMaxPos{ -1 } {}

int SWBase::max_score() {
	if (mMaxScore == -1) {
		search_max();
	}
	return mMaxScore;
}

int SWBase::max_position() {
	if (mMaxScore == -1) {
		search_max();
	}
	return mMaxPos;
}

int* SWBase::all_score() {
	if (!mScore) {
		return mScore;
	}
	else {
		return nullptr;
	}
}

void SWBase::search_max() {
	for (int i = 0; i < mSize; ++i) {
		if (mScore[i] >= mThreshold && mScore[i] > mMaxScore) {
			mMaxScore = mScore[i];
			mMaxPos = i;
		}
	}
	if (mMaxScore == -1) {
		mMaxScore = 0;
		mMaxPos = 0;
	}
}

void SWBase::traceback(const Data& db, const Data& query) const {
	if (mMaxScore == 0) { return; }
	int len = query.size() + query.size() / 2;
	if (len > db.size()) { len = db.size(); }
	if (len > mMaxPos) { len = 0; }
	enum Direction {
		DIA,
		UP,
		LEFT,
		ZERO
	};
	Direction* dir = new Direction[len * query.size()]{ ZERO };
	int psize = query.size();
	int* FScore = new int[psize] {0};
	int* MainScore = new int[psize + 1]{ 0 };
	int EScore = 0;
	int score = 0;
	int FBeg = 0;
	int FExt = 0;
	int EBeg = 0;
	int EExt = 0;
	int match = cost.Match;
	int miss = cost.Miss;
	int ext = cost.Extend;
	int beg = cost.Begin;
	// SW
	int dir_point = psize * len - 1;
	int dir_t = 0;
	for (int t = mMaxPos - len; t <= mMaxPos; ++t) {
		int EScore = 0;
		int score = 0;
		int prevScore = 0;
		int FBeg = 0;
		int FExt = 0;
		int EBeg = 0;
		int EExt = 0;
		char db_char = db[t];
		for (int p = 0; p < psize; ++p) {
			// Match or miss match
			if (db_char == query[p]) {
				score = MainScore[p] + match;
			}
			else {
				score = MainScore[p] + miss;
			}
			// Gap begin
			FBeg = MainScore[p + 1] + beg;
			EBeg = prevScore + beg;
			// Gap extend
			FExt = FScore[p] + ext;
			EExt = EScore + ext;
			// set value
			MainScore[p] = prevScore;
			FScore[p] = std::max({ FBeg, FExt, 0 });
			EScore = std::max({ EBeg, EExt, 0 });
			prevScore = std::max({ FScore[p],EScore,score });
			// Set direction
			int d = p*len + dir_t;
			if (prevScore == 0) {
				dir[d] = Direction::ZERO;
			}
			else if (FScore[p] > EScore && FScore[p] > score) {
				dir[d] = Direction::LEFT;
			}
			else if (EScore > score) {
				dir[d] = Direction::UP;
			}
			else {
				dir[d] = Direction::DIA;
			}
			if (prevScore == mMaxScore) {
				dir_point = d;
			}
		}
		++dir_t;
	}
	// Traceback
	int t = mMaxPos;
	std::string s;
	while (dir[dir_point] != Direction::ZERO) {
		switch (dir[dir_point]) {
		case Direction::DIA:
			dir_point -= (len + 1);
			s += db[t--];
			break;
		case Direction::LEFT:
			dir_point -= 1;
			s += '/';
			t--;
			break;
		case Direction::UP:
			dir_point -= len;
			s += '+';
			break;
		default:
			break;
		}
		if (dir_point < 0) { break; }
	}
	std::reverse(std::begin(s), std::end(s));
	std::cout << "Orignal array\n" << query.data() << std::endl
		<< "Result array\n"<< s << std::endl;
}
