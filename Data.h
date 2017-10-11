#ifndef DATA_H
#define DATA_H

class Data {
public:
	Data(const char* fname);
	~Data();
	char operator[](int n) const;
private:
	int mSize;
	char* mData;
};


#endif
