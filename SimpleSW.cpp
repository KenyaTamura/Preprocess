#include"SimpleSW.h"
#include"Data.h"
#include"Writer.h"
#include"Cost.h"

#include<algorithm>
#include<iostream>

using namespace std;

SimpleSW::SimpleSW(const Data& db, const Data& query, int threshold) : SWBase{ threshold } {
	cout << "SimpleSW start" << endl;
	DP(db, query);
	cout << "Max score is " << max_score() << ", max position is " << max_position() << endl;
	cout << "SimpleSW end" << endl;
	
}

SimpleSW::~SimpleSW() {
	if (mScore) {
		delete[] mScore;
	}
}

void SimpleSW::DP(const Data& db, const Data& query) {
	mScore = new int[db.size()]{ 0 };
	mSize = db.size();
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
			FScore[p] = max({ FBeg, FExt, 0 });
			EScore = max({ EBeg, EExt, 0 });
			prevScore = max({ FScore[p],EScore,score });
			if (mScore[t] < prevScore) {
				mScore[t] = prevScore;
			}
		}
	}
	delete[] FScore;
	delete[] MainScore;
}
