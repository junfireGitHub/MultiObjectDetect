#ifndef _ADABOOST_PERDICT_H
#define _ADABOOST_PERDICT_H

void adaboostPredict(const float *data, int num, int dimension, float *result, int id);

float adaboostPredict3(const float *data, int dimension, int id);

#endif