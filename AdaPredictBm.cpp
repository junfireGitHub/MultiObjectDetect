#include"AdaPara_bm_vs_all.h"
#include "classify.h"

float adaboostPredictBm(const float *data)
{
	int i;
	int pos = 0;
	float result = .0f;
	for (i = 0; i < nWeaks; ++i)
	{
		unsigned int k = 0;
		while (bm_child[k])
		{
			k = bm_child[k] - (data[bm_fids[pos + k]] < bm_thrs[pos + k]);
		}
		result += bm_hs[pos + k];
		if (result < -1) return  result;
		//if (result > 10 - (i >> 7)) return  result;
		pos += weak_dimension;
	}
	return  result;
}
