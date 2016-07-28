#include "postPro.h"
#include "opencvLib.h"
#include "getHardFromNeg.h"
using namespace cv;

static inline bool isMerge(Rect r1, Rect r2);
static vector<Rect> mergeRect(const vector<Rect>& rec, const vector<float> &score);

typedef struct FrameRect{
	Rect r;
	int count;//统计该位置已经出现多少次
}FrameRect;

static bool isNearPos(const Rect& r1, const Rect& r2, int VAL){
	return abs(r1.x - r2.x) <= VAL &&
		abs(r1.y - r2.y) <= VAL &&
		abs(r1.x + r1.width - r2.x - r2.width) <= VAL &&
		abs(r1.y + r1.height - r2.y - r2.height) <= VAL;
}

static vector<Rect> useInterframeInfo(vector<FrameRect>& lastFramePos, vector<Rect> thisFramePos){
	/*利用帧间位置的比较来调整框的位置*/
	vector<FrameRect> newLastFramePos; //用于更新 lastFramePos
	vector<Rect> retPos = thisFramePos;
	for (int i = 0; i < thisFramePos.size(); ++i){
		newLastFramePos.push_back({ thisFramePos[i], 1 });
	}

	// if first frame or no person	
	if (lastFramePos.size() == 0){	
		lastFramePos = newLastFramePos;
		return retPos;
	}
	
	//两帧之间是否有位置相差很小，可以近似认为是相同位置的框
	const int MAX_COUNT = 4;
	for (int i = 0; i < thisFramePos.size(); ++i){
		for (int j = 0; j < lastFramePos.size(); ++j){
			if (isNearPos(thisFramePos[i], lastFramePos[j].r, 16)){
				retPos[i] = lastFramePos[j].r;
				newLastFramePos[i] = lastFramePos[j];
				newLastFramePos[i].count++;
				if (newLastFramePos[i].count > MAX_COUNT)
					newLastFramePos[i].count = MAX_COUNT;
			}
		}
	}

	//是否保留，对于已经出现多次的框，假如这帧中没有出现而且该框周围没有框，则认为是漏检了
	for (int j = 0; j < lastFramePos.size(); ++j){
		if (lastFramePos[j].count == MAX_COUNT){
			int i = 0;
			for (i = 0; i < thisFramePos.size(); ++i){
				if (isNearPos(thisFramePos[i], lastFramePos[j].r, 24))
					break;
			}
			if (i != 0 && i == thisFramePos.size()){
				retPos.push_back(lastFramePos[j].r);
				lastFramePos[j].count--;
				newLastFramePos.push_back(lastFramePos[j]);
			}
		}
	}

	lastFramePos = newLastFramePos;
	return retPos;
}

void bbNmsMaxMultiClass(Mat& src, vector<Bbox>& bb, bool isPostPro, vector<Rect> &foundFiltered){
	if (!isPostPro){/*no postprocessing, just draw all rectangles*/
		for (int i = 0; i < bb.size(); i++){
			Rect r = bb[i].pos;
			switch (bb[i].obType){			
			case PERSON: rectangle(src, r, Scalar(255), 2); break;
				//case CAR:    rectangle(src, r, Scalar(0, 255, 0), 1); break;//green
				//case BM:   rectangle(src, r, Scalar(0, 0, 255), 1); break;//red
				//case ELECBIKE: rectangle(src, r, Scalar(255, 0, 0), 2); break;//blue
				//case ROAD:   rectangle(src, r, Scalar(0, 255, 255), 1); break;//gray
			}
		}
	}
	else{
		const int NUM_OBJ = 2;
		ObjectType obType[NUM_OBJ] = { ROAD, PERSON };
		for (int i = 0; i < NUM_OBJ; i++){
			vector<Rect> found;
			vector<float> score;
			for (int j = 0; j < bb.size(); j++){
				if (bb[j].obType == obType[i]){
					found.push_back(bb[j].pos);
					score.push_back(bb[j].score);
				}
			}
			if (obType[i] == PERSON){
				foundFiltered = mergeRect(found, score); //merge rects
				//static vector<FrameRect> lastFramePos;
				//foundFiltered = useInterframeInfo(lastFramePos, foundFiltered);

				for (int k = 0; k < foundFiltered.size(); k++){
					Rect r = foundFiltered[k];
					switch (obType[i]){					  
					case PERSON: rectangle(src, r, Scalar(255), 2); break;
						//saveHardPics(src, foundFiltered); break;
						//case CAR:    rectangle(src, r, Scalar(0, 255, 0), 2); break;//green
						//case BM:     rectangle(src, r, Scalar(0, 0, 255), 2); break;//red
						//case ELECBIKE: rectangle(src, r, Scalar(255, 0, 0), 2); break;
						//case ROAD:   rectangle(src, r, Scalar(128, 128, 128), 1); break;
					}
				}
			}
			//else{//ROAD,no postprocessing
			//	for (int j = 0; j < bb.size(); j++){
			//		if (bb[j].obType == ROAD)
			//			rectangle(src, bb[j].pos, Scalar(0, 255, 255), 1);//gray
			//	}
			//}
		}
	}
}

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

	if (rec.size() == 0){
		vector<Rect> ret;
		return ret;
	}
	vector<Rect> r = rec;
	vector<int> pc(rec.size(),0);

	for (int i = 0; i < rec.size(); ++i){
		if (r[i].x == -1) continue;//maxg
		for (int j = i + 1; j < rec.size(); ++j){
			if (r[j].x == -1) continue;
			if (isMerge(rec[i], rec[j])){
				int pos = score[i]>=score[j]+5 ? j : i;
				r[pos].x = -1;
				pos = score[i]>=score[j]+5 ? i : j;
				pc[pos]++;
			}
		}
	}

	vector<Rect> ret;
	for (int i = 0; i < r.size(); ++i){
		if (r[i].x != -1 /*&& (pc[i] > 0 || score[i]>10)*/)
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