#ifndef PREPROCESSMERGE_H
#define PREPROCESSMERGE_H

class PreprocessBase;
class Data;

class PreprocessMerge {
public:
	PreprocessMerge(const Data& db, const Data& q, const int threshold, const int thread, PreprocessBase* pre);
	~PreprocessMerge();
private:
	PreprocessMerge() {};
	enum Mode {
		Single = 0,
		Double,
		Triple,
		Quad
	};
	Mode mode_select(const Data& q, const int threshold) const;
};




#endif
