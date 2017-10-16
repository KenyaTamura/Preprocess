#include"SW.h"
#include"Data.h"

#include<algorithm>
#include<iostream>


SW::SW(const Data& db, const Data& query) : mScore{ nullptr } {
	mScore = new int[db.size()]{ 0 };
	DP(db, query);
}

SW::~SW() {
	if (mScore) {
		delete[] mScore;
	}
}

void SW::DP(const Data& db, const Data& query) {
	int tsize = db.size();
	int psize = query.size();
	int* FScore = new int[psize] {0};
	int* MainScore = new int[psize + 1] {0};
	int EScore = 0;
	int prevScore = 0;
	int score = 0;
	int FBeg = 0;
	int FExt = 0;
	int EBeg = 0;
	int EExt = 0;
	for (int t = 0; t < tsize; ++t) {
		char db_char = db[t];
		for (int p = 0; p < psize; ++p) {
			// Match or miss match
			if (db_char == query[p]) {
				score = MainScore[p] + 1;
			}
			else {
				score = MainScore[p] - 3;
			}
			// Gap begin
			FBeg = MainScore[p + 1] - 5;
			EBeg = prevScore - 5;
			// Gap extend
			FExt = FScore[p] - 3;
			EExt = EScore - 3;
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

