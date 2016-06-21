#include "postPro.h"
#include "opencvLib.h"
using namespace cv;

static bool similarRect(const Rect& r1, const Rect& r2, float weight);
static int getRoot(int x, int *father);
static void merge(int x, int y, int *father);
static vector<Rect> mergeRect(const vector<Rect>& _vec, int groupThreshold, float weight);

void bbNmsMultiClass(Mat& src, vector<Bbox>& bb, bool isPostPro){
	if (!isPostPro){/*no postprocessing, just draw all rectangles*/
		for (int i = 0; i < bb.size(); i++){
			Rect r = bb[i].pos;
			switch (bb[i].obType){
			//case CAR:    rectangle(src, r, Scalar(0, 255, 0), 1); break;//green
			//case BM:   rectangle(src, r, Scalar(0, 0, 255), 1); break;//red
			case PERSON: rectangle(src, r, Scalar(255), 2); break;//blue
			//case ELECBIKE: rectangle(src, r, Scalar(255, 0, 0), 2); break;//blue
			//case ROAD:   rectangle(src, r, Scalar(0, 255, 255), 1); break;//gray
			}
		}
	}
	else{
		const int NUM_OBJ = 4;
		ObjectType obType[NUM_OBJ] = { CAR, BM, ROAD, PERSON };
		for (int i = 0; i < NUM_OBJ; i++){
			vector<Rect> found;
			found.clear();
			for (int j = 0; j < bb.size(); j++){
				if (bb[j].obType == obType[i])
					found.push_back(bb[j].pos);
			}
			if (obType[i] != ROAD){
				vector<Rect> foundFiltered = mergeRect(found, 2, 0.2); //merge rects
				for (int k = 0; k < foundFiltered.size(); k++){
					Rect r = foundFiltered[k];
					switch (obType[i]){
					//case CAR:    rectangle(src, r, Scalar(0, 255, 0), 2); break;//green
					//case BM:     rectangle(src, r, Scalar(0, 0, 255), 2); break;//red
					case PERSON: rectangle(src, r, Scalar(255), 2); break;//blue
					//case ELECBIKE: rectangle(src, r, Scalar(255, 0, 0), 2); break;
					//case ROAD:   rectangle(src, r, Scalar(128, 128, 128), 1); break;
					}
				}
			}
			else{//ROAD,no postprocessing
				for (int j = 0; j < bb.size(); j++){
					if (bb[j].obType == ROAD)
						rectangle(src, bb[j].pos, Scalar(0, 255, 255), 1);//gray
				}
			}
		}
	}
}

void bbNms(Mat& src, vector<Rect> found, bool isPostPro){
	if (!isPostPro){/*no postprocessing,just draw all rectangles*/
		for (int i = 0; i < found.size(); i++){
			Rect r = found[i];
			rectangle(src, r, Scalar(255), 1);
		}
	}else{
		vector<Rect> foundFiltered = mergeRect(found, 2, 0.2);
		printf("afterPostProCount: %d\n", foundFiltered.size());

		//Mat srcTemp = src.clone();
		for (int i = 0; i < foundFiltered.size(); i++){
			Rect r = foundFiltered[i];
			rectangle(src, r, Scalar(255), 1);
		}
	}
}

static vector<Rect> mergeRect(const vector<Rect>& _vec, int groupThreshold, float weight){
	if (_vec.empty()){ vector<Rect>ret; return ret; }
	int i, j, N = (int)_vec.size();
	const Rect* vec = &_vec[0];
	vector<Rect> rectFinalRes;

	int *father = (int*)calloc(N, sizeof(int));

	// init
	for (i = 0; i < N; ++i)
		father[i] = i;

	//if similar, merge.
	for (i = 0; i < N; ++i){
		for (j = i + 1; j < N; j++){
			if (similarRect(vec[i], vec[j], weight))
				merge(i, j, father);
		}
	}

	//一共多少个类
	int nclass = 0;
	int *className = (int *)calloc(N, sizeof(int));
	for (int i = 0; i < N; i++){
		if (father[i] == i){
			className[nclass++] = i;
		}
	}

	Rect *vecRst;
	int *rectCount;
	vecRst = (Rect*)calloc(nclass, sizeof(Rect));
	rectCount = (int*)calloc(nclass, sizeof(int));

	int k = 0;

	/*将属于相同的类的值加起来并求均值，舍弃少于groupThreshold的类*/
	for (i = 0; i < nclass; i++){
		rectCount[k] = 0;
		memset(&vecRst[k], 0, sizeof(Rect));
		for (j = 0; j < N; j++){
			if (getRoot(j, father) == className[i]){
				rectCount[k]++;
				vecRst[k].x += vec[j].x;
				vecRst[k].y += vec[j].y;
				vecRst[k].width += vec[j].width;
				vecRst[k].height += vec[j].height;
			}
		}
		if (rectCount[k] >= groupThreshold){
			vecRst[k].x /= rectCount[k];
			vecRst[k].y /= rectCount[k];
			vecRst[k].width /= rectCount[k];
			vecRst[k].height /= rectCount[k];
			k++;
		}
	}

	/*将嵌在大矩形框内部的小矩形框过滤掉,最后剩下的矩形框即为聚类的结果*/
	for (i = 0; i < k; i++){
		for (j = 0; j < k; j++){
			if (i != j && vecRst[i].x >= vecRst[j].x && vecRst[i].y >= vecRst[j].y &&
				vecRst[i].x + vecRst[i].width <= vecRst[j].x + vecRst[j].width  &&
				vecRst[i].y + vecRst[i].height <= vecRst[j].y + vecRst[j].height)
				break;
		}
		if (j == k)
			rectFinalRes.push_back(vecRst[i]);
	}

	free(father);
	free(vecRst);
	free(rectCount);
	return rectFinalRes;
}

static bool similarRect(const Rect& r1, const Rect& r2, float weight)
/*该函数判断两个矩形框是否相似，即是否可以划归为同一类*/
{
	// delta为最小长宽的eps*0.5倍
	double eps = weight;
	double delta = eps*(std::min(r1.width, r2.width) + std::min(r1.height, r2.height))*0.5;
	// 如果矩形的四个顶点的位置差别都小于delta，则表示相似的矩形
	return std::abs(r1.x - r2.x) <= delta &&
		std::abs(r1.y - r2.y) <= delta &&
		std::abs(r1.x + r1.width - r2.x - r2.width) <= delta &&
		std::abs(r1.y + r1.height - r2.y - r2.height) <= delta;
}

static int getRoot(int x, int *father){
	//return father[x]==x? x:getRoot(father[x]);
	/*并查集的一个优化叫做【路径压缩】，是在并查集执行查询时对经过的点进行【扁平化】的方法。
	使得下次查询时不需要经过中间节点，代码如下*/
	if (father[x] != x) father[x] = getRoot(father[x], father);
	return father[x];
}

static void  merge(int x, int y, int *father){
	int rx = getRoot(x, father);
	int ry = getRoot(y, father);
	if (rx != ry)
		father[ry] = x;
}