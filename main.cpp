#include<iostream>
#include<string>
#include<string.h>

#include"Data.h"
#include"PreprocessSingle.h"
#include"PreprocessDouble.h"
#include"PreprocessTriple.h"
#include"PreprocessQuad.h"
#include"PreprocessBase.h"
#include"SimpleSW.h"
#include"PreprocessSW.h"
#include"SWBase.h"
#include"FileConverter.h"
#include"Timer.h"
#include"Writer.h"

using namespace std;

#include<thread>

namespace {
	Data* db = nullptr;
	Data* q = nullptr;
	int threshold = 0;
	string ofname;
	string type = "quad";
	bool cmp_flag = false;
	bool sw_flag = false;
	bool percent_flag = false;
	PreprocessBase* pre = nullptr;
}

void arg_branch(int argc, char* argv[]) {
	auto cmp = [&](int i, const char* str) -> bool {return (strcmp(argv[i], str) == 0); };
	for (int i = 1; i < argc; ++i) {
		if (cmp(i, "-convert")) {
			if (i + 2 < argc) { FileConverter f(argv[i + 1], argv[i + 2]); }
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
		if (cmp(i, "-type")) {
			if (++i < argc) { type = argv[i]; }
		}
		if (cmp(i, "-cmp")) {
			cmp_flag = true;
		}
		if (cmp(i, "-o")) {
			if (++i < argc) { ofname = argv[i]; }
		}
		if (cmp(i, "-percent")) {
			if (++i < argc) { ofname = argv[i]; percent_flag = true; }
		}
		if (cmp(i, "-sw")) {
			sw_flag = true;
		}
	}
}

void mode_select() {
	auto type_check = [&](const char* str) -> bool {return (strcmp(type.c_str(), str) == 0); };
	if (db != nullptr && q != nullptr && db->data() != nullptr && q->data() != nullptr) {
		// Compare execution time at preprocess
		if (cmp_flag) {
			Timer t;
			PreprocessSingle(*db, *q, threshold);
			cout << t.get_millsec() << endl;
			t.start();
			PreprocessDouble(*db, *q, threshold);
			cout << t.get_millsec() << endl;
			t.start();
			PreprocessTriple(*db, *q, threshold);
			cout << t.get_millsec() << endl;
			t.start();
			PreprocessQuad(*db, *q, threshold);
			cout << t.get_millsec() << endl;
	//		t.start();
	//		SimpleSW(*db, *q, threshold);
	//		cout << t.get_millsec() << endl;
		}
		// Preprocess
		else {
			if (type_check("simple")) {
				SimpleSW sw(*db, *q, threshold);
				return;
			}		
			if (type_check("single")) {
				pre = new PreprocessSingle(*db, *q, threshold);
			}
			else if (type_check("double")) {
				pre = new PreprocessDouble(*db, *q, threshold);
			}
			else if (type_check("triple")) {
				pre = new PreprocessTriple(*db, *q, threshold);
			}
			else {
				pre = new PreprocessQuad(*db, *q, threshold);
			}
			if (sw_flag) {
				PreprocessSW(*db, *q, *pre, threshold);
			}
		}
	}
	else {
		cout << "File error" << endl;
	}
}
int main(int argc, char* argv[]) {
	arg_branch(argc, argv);
	Timer t;
	PreprocessSingle pres{ *db, *q, threshold };
	cout << t.get_millsec() << endl;
	/*
	mode_select();
	cout << t.get_millsec() << endl;
	if (!ofname.empty()) {
		Writer w;
		if (percent_flag) {
			w.writing(ofname.c_str(), pre->get_percent());
		}
		else {
			w.writing(ofname.c_str(), t.get_millsec());
		}
	}*/
	if (db) {
		delete db;
	}
	if (q) {
		delete q;
	}
	if (pre) {
		delete pre;
	}
}