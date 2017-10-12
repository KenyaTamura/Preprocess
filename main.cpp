#include<iostream>

#include"Preprocess.h"

using namespace std;

void arg_branch(int argc, char* argv[]) {
	auto cmp = [&](int i, const char* str) -> bool {return (strcmp(argv[i], str) == 0); };
	for (int i = 1; i < argc; ++i) {
		if (cmp(i,"-convert")) {
		}
	}
}

int main(int argc, char* argv[]) {
	arg_branch(argc, argv);
}