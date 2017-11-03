#ifndef DATA_H
#define DATA_H

class Data {
public:
	Data(const char* fname);
	~Data();
	char operator[](int n) const;
	int size() const;
	char* data() const;
private:
	Data();
	int mSize;
	char* mData;
};


#endif
