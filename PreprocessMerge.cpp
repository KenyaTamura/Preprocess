#include"PreprocessMerge.h"
#include"PreprocessBase.h"
#include"PreprocessSingle.h"
#include"PreprocessDouble.h"
#include"PreprocessTriple.h"
#include"PreprocessQuad.h"
#include"Data.h"

PreprocessMerge::PreprocessMerge(const Data& db, const Data& q, const int threshold, const int thread, PreprocessBase* pre) {
	Mode m = mode_select(q, threshold);
	switch (m) {
	case PreprocessMerge::Single:
		pre = new PreprocessSingle(db, q, threshold, thread); break;
	case PreprocessMerge::Double:
		pre = new PreprocessDouble(db, q, threshold, thread); break;
	case PreprocessMerge::Triple:
		pre = new PreprocessTriple(db, q, threshold, thread); break;
	case PreprocessMerge::Quad:
		pre = new PreprocessQuad(db, q, threshold, thread); break;
	}
}

PreprocessMerge::~PreprocessMerge() {

}

PreprocessMerge::Mode PreprocessMerge::mode_select(const Data& query, const int threshold) const {
	int percent = (100 * threshold) / query.size();
	if (query.size() < 150 && percent > 85) {
		return Mode::Double;
	}
	else if ((query.size() < 250 && percent > 60) || (query.size() < 450 && percent > 85)) {
		return Mode::Triple;
	}
	else {
		return Mode::Quad;
	}
}