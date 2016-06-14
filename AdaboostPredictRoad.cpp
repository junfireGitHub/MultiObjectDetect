#include "AdaModParaRoad.h"

void adaboostPredictRoad(const float *data, int num, int dimension, float *result)
{
	int i, j;
	for (i = 0; i < nWeaks; ++i)
	{
		for (j = 0; j < num; ++j)
		{
			unsigned int k = 0;
			while (child[i*weak_dimension + k])
			{
				if (data[j*dimension + fids[i*weak_dimension + k]] < thrs[i*weak_dimension + k])
					k = child[i*weak_dimension + k] - 1;
				else
					k = child[i*weak_dimension + k];
			}
			result[j] = result[j] + hs[i*weak_dimension + k];
		}
	}
}

float adaboostPredictRoad2(const float *data)
{
	int i;
	int pos = 0;
	float result = .0f;
	for (i = 0; i < nWeaks; ++i)
	{
		unsigned int k = 0;
		while (child[k])
		{
			k = child[k] - (data[fids[pos + k]] < thrs[pos + k]);
		}
		result += hs[pos + k];
		/*if (result < -1) return  result;
		if (result > 10 - (i >> 7)) return  result;*/
		pos += weak_dimension;
	}
	return  result;
}
