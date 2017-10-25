#include"SW.h"
#include"Data.h"
#include"Writer.h"
#include"Cost.h"

#include<algorithm>
#include<iostream>


SW::SW(const Data& db, const Data& query) : mScore{ nullptr } {
	DP(db, query);
}

SW::~SW() {
	if (mScore) {
		delete[] mScore;
	}
}

void SW::DP(const Data& db, const Data& query) {
	mScore = new int[db.size()]{ 0 };
	int tsize = db.size();
	int psize = query.size();
	int* FScore = new int[psize] {0};
	int* MainScore = new int[psize + 1] {0};
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
	for (int t = 0; t < tsize; ++t) {
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
			if (mScore[t] < prevScore) {
				mScore[t] = prevScore;
			}
		}
	}
	delete[] FScore;
	delete[] MainScore;
}

int* SW::score() {
	return mScore;
}