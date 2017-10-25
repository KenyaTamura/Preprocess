#include<iostream>
#include<string>

#include"Data.h"
#include"Preprocess.h"
#include"PreprocessQuad.h"
#include"SW.h"
#include"PreprocessSW.h"
#include"FileConverter.h"
#include"Timer.h"
#include"Writer.h"

using namespace std;

namespace {
	Data* db = nullptr;
	Data* q = nullptr;
	int threshold = 0;
	string ofname;
	string ofname_pre;
}

void arg_branch(int argc, char* argv[]) {
	auto cmp = [&](int i, const char* str) -> bool {return (strcmp(argv[i], str) == 0); };
	for (int i = 1; i < argc; ++i) {
		if (cmp(i,"-convert")) {
			if (i + 2 < argc) { FileConverter f(argv[i+1],argv[i+2]); }
			i += 2;
		}
		if (cmp(i, "-db")) {
			if (++i < argc && !db) { db = new Data(argv[i]); }
		}
		if (cmp(i, "-q")) {
			if (++i < argc && !q) { q = new Data(argv[i]); }
		}
		if (cmp(i, "-t")) {
			if (++i < argc) { threshold = atoi(argv[i]); }
		}
		if (cmp(i, "-o")) {
			if (++i < argc) { ofname = argv[i]; }
		}
		if (cmp(i, "-predebug")) {
			if (++i < argc) { ofname_pre = argv[i]; }
		}
	}
}

int main(int argc, char* argv[]) {
	arg_branch(argc, argv);
	Timer t;
	if (db != nullptr && q != nullptr) {
		if (ofname_pre.empty()) {
			t.start();
			SW sw{ *db,*q,threshold };
//			PreprocessSW(*db, *q, Preprocess(*db, *q, threshold), threshold);
			cout << t.get_millsec() << endl;
			t.start();
			PreprocessSW(*db, *q, PreprocessQuad(*db, *q, threshold), threshold);
			cout << t.get_millsec() << endl;
		}
		else {
			Preprocess(*db, *q, ofname_pre.c_str());
			PreprocessQuad(*db, *q, ofname_pre.c_str());
		}
		if (!ofname.empty()) {
			SW sw{ *db,*q,threshold };
			Writer w;
			w.writing_score(ofname.c_str(), sw.all_score(), db->size(), db->size() / 100);
		}
	}
	cout << t.get_millsec() << endl;
	if (db) {
		delete db;
	}
	if (q) {
		delete q;
	}
}