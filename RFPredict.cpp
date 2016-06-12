#include "RFPara.h"

int rfPredict(const float *data)
{
	int i, j;
	float ps[nClass] = { .0f };
	for (i = 0; i < nTrees; ++i)
	{
		unsigned int k = 0;
		while (child[i][k])
		{
			if (data[fids[i][k]] < thrs[i][k])
				k = child[i][k] - 1;
			else
				k = child[i][k];
		}
		for (j = 0; j < nClass; ++j)
		{
			ps[j] += distr[i][k*nClass + j];
		}
	}
	float max = ps[0]; int result = 0;
	for (j = 1; j < nClass; ++j)
	{
		if (ps[j]>max)
		{
			max = ps[j];
			result = j;
		}
	}
	return result;
}
