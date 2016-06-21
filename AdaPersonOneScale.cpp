#include"AdaPersonOneScale.h"

float adaPersonOneScale(const float *data){
	int i;
	int pos = 0;
	float result = .0f;
	for (i = 0; i < nWeaks; ++i){
		unsigned int k = 0;
		while (child[k]){
			k = child[k] - (data[fids[pos + k]] < thrs[pos + k]);
		}
		result += hs[pos + k];
		if (result < -1) return  result;
		//if (result > 10 - (i >> 7)) return  result;
		pos += weak_dimension;
	}
	return  result;
}