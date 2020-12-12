#ifndef __LOG_REG__
#define __LOG_REG__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

float innerProduct(float* x_i, float* w, unsigned int r);
float sigmoid(float t);
float hypothesis(float* x_i, float* w, float bias, unsigned int r);
bool* predict( float** x_test, float* y_test, float* w, float bias, unsigned int m, unsigned int r );
void gradient_descent(float** x, float* y, float* w, float* bias, unsigned int m, unsigned int r, float eta, float epsilon);

#endif