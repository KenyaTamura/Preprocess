#include<iostream>

#include"Preprocess.h"
#include"FileConverter.h"
#include"SW.h";
#include"Timer.h"

using namespace std;

namespace {
	Data* db = nullptr;
	Data* q = nullptr;
	Preprocess* pre = nullptr;
	int threshold = 400;
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
	}
}

int main(int argc, char* argv[]) {
	arg_branch(argc, argv);
	Timer t;
	if (db != nullptr && q != nullptr) {
//		pre = new Preprocess(*db, *q, threshold);
		SW sw{ *db,*q };
	}
	cout << t.get_millsec() << endl;
	if (db) {
		delete db;
	}
	if (q) {
		delete q;
	}
}