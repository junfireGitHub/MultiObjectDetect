#include "AdaPersonMultiScale.h"

//void adaPersonMultiScale(const float *data, int num, int dimension, float *result, int id){
//	int i, j;
//	int pos = 0;
//
//	const float *thrsN = &thrs[id][0];
//	const unsigned int *fidsN = &fids[id][0];
//	const float *hsN = &hs[id][0];
//
//	for (i = 0; i < nWeaks; ++i){
//		int datapos = 0;
//		for (j = 0; j < num; ++j){
//			unsigned int k = 0;
//			while (child[k]){
//				if (data[datapos + fidsN[pos + k]] < thrsN[pos + k])
//					k = child[k] - 1;
//				else
//					k = child[k];
//			}
//			result[j] = result[j] + hsN[pos + k];
//			datapos += dimension;
//		}
//		pos += weak_dimension;
//	}
//}

float adaPersonMultiScale(const float *data, int dimension, int id)
{
	int i;
	int pos = 0;
	float result = .0f;

	const float *thrsN = &thrs[id][0];
	const unsigned int *fidsN = &fids[id][0];
	const float *hsN = &hs[id][0];

	for (i = 0; i < nWeaks; ++i){
		unsigned int k = 0;
		while (child[k]){
			if (data[fidsN[pos + k]] < thrsN[pos + k])
				k = child[k] - 1;
			else
				k = child[k];
		}
		result += hsN[pos + k];
		if (result < -1) return  result;
		pos += weak_dimension;
	}
	return  result;
}