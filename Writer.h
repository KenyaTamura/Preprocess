#ifndef WRITER_H
#define WRITER_H

class Writer {
public:
	Writer();
	~Writer();
	void writing_score(const char* fname, const int* data, int size);
	void writing_score(const char* fname, const int* data, int size, int interval);
private:
};


#endif
