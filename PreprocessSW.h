#ifndef PREPROCESSSW_H
#define PREPROCESSSW_H

class Preprocess;
class PreprocessQuad;
class Data;

class PreprocessSW {
public:
	PreprocessSW(const Data& db, const Data& query, const Preprocess& pre, int threshold);
	~PreprocessSW();
private:
	void DP(const Data& db, const Data& query);
};



#endif
