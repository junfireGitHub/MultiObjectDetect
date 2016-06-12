#include"AdaPara_car_vs_all.h"
#include "classify.h"

float adaboostPredictCar(const float *data)
{
	int i;
	int pos = 0;
	float result = .0f;
	for (i = 0; i < nWeaks; ++i)
	{
		unsigned int k = 0;
		while (car_child[k])
		{
			k = car_child[k] - (data[car_fids[pos + k]] < car_thrs[pos + k]);
		}
		result += car_hs[pos + k];
		if (result < -1) return  result;
		//if (result > 10 - (i >> 7)) return  result;
		pos += weak_dimension;
	}
	return  result;
}
