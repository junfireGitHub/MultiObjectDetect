#ifndef _ADABOOST_PERDICT_H
#define _ADABOOST_PERDICT_H

void adaPredictPerson(const float *data, int num, int dimension, float *result, int id);
float adaPredictPersonEarlyExit(const float *data, int dimension, int id);

void adaPredictCar(const float *data, int num, int dimension, float *result);
void adaPredictBm(const float *data, int num, int dimension, float *result);

float adaboostPredictCar(const float *data);
float adaboostPredictBm(const float *data);

#endif