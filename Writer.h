#ifndef WRITER_H
#define WRITER_H

class Writer {
public:
	Writer();
	~Writer();
	void writing_score(const char* fname, const int* data, int size);
	void writing_score(const char* fname, const int* data, int size, int interval, int timer = 3);
	void writing(const char* fname, int i);
	void writing(const char* fname, double d);
private:
};


#endif
