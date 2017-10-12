#include<iostream>

#include"Preprocess.h"
#include"FileConverter.h"

using namespace std;

void arg_branch(int argc, char* argv[]) {
	auto cmp = [&](int i, const char* str) -> bool {return (strcmp(argv[i], str) == 0); };
	for (int i = 1; i < argc; ++i) {
		if (cmp(i,"-convert")) {
			if (i + 2 < argc) { FileConverter f(argv[i+1],argv[i+2]); }
			i += 2;
		}
		if (cmp(i, "-db")) {
			if (++i < argc) { Data d(argv[i]); }
		}
	}
}

int main(int argc, char* argv[]) {
	arg_branch(argc, argv);
}