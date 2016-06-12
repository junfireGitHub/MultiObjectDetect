#include "postPro.h"
#include "opencvLib.h"
using namespace cv;

static inline bool isMerge(Rect r1, Rect r2);
static vector<Rect> mergeRect(const vector<Rect>& rec, const vector<float> &score);

void bbNmsMax(Mat& src, vector<Rect> found, vector<float> score, bool isPostPro){
	if (!isPostPro){/*no postprocessing, just draw all rectangles*/
		for (int i = 0; i < found.size(); i++){
			Rect r = found[i];
			rectangle(src, r, Scalar(255), 1);
		}
	}
	else{
		vector<Rect> foundFiltered = mergeRect(found, score);
		printf("afterPostProCount: %d\n", foundFiltered.size());
		for (int i = 0; i < foundFiltered.size(); i++){
			Rect r = foundFiltered[i];
			rectangle(src, r, Scalar(255), 1);
		}
	}
}

static vector<Rect> mergeRect(const vector<Rect>& rec, const vector<float> &score){
	assert(rec.size() == score.size());

	vector<Rect> r = rec;

	for (int i = 0; i < rec.size(); ++i){
		if (r[i].x == -1) continue;//maxg
		for (int j = i + 1; j < rec.size(); ++j){
			if (r[j].x == -1) continue;
			if (isMerge(rec[i], rec[j])){
				int pos = score[i]>score[j] ? j : i;
				r[pos].x = -1;
			}
		}
	}

	vector<Rect> ret;
	for (int i = 0; i < r.size(); ++i){
		if (r[i].x != -1)
			ret.push_back(r[i]);
	}
	return ret;
}

static inline bool isMerge(Rect r1, Rect r2){

	int w = 0, h = 0;
	int area1 = r1.width*r1.height;
	int area2 = r2.width*r2.height;

	if (r1.x > r2.x) w = r2.x + r2.width - r1.x;
	else w = r1.x + r1.width - r2.x;
	if (w <= 0) return false;

	if (r1.y > r2.y) h = r2.y + r2.height - r1.y;
	else h = r1.y + r1.height - r2.y;
	if (h <= 0) return false;

	return w*h >= (area1 + area2 - w*h)*0.5;
}