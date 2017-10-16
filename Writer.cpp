#include"Writer.h"

#include<fstream>

using namespace std;

Writer::Writer() {
	
}

Writer::~Writer(){}

void Writer::writing_score(const char* fname, const int* data, int size) {
	ofstream ofs(fname, ios::trunc);
	if (!ofs) { return; }
	for (int i = 0; i < size; ++i) {
		ofs << data[i] << ',';
	}
}

void Writer::writing_score(const char* fname, const int* data, int size, int interval) {
	ofstream ofs(fname, ios::trunc);
	if (!ofs) { return; }
	for (int i = 0; i < size; i+=interval) {
		ofs << data[i] << ',';
	}
}