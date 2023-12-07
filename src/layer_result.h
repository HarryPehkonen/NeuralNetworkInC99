#ifndef LAYER_RESULT_H
#define LAYER_RESULT_H

//#include "neuron.h"

typedef struct {
	nn_value_t* values;
	int length;
	int n_populated;
} layer_result_t;

layer_result_t* layer_result_make(int length);
int layer_result_add(layer_result_t*, nn_value_t);

#endif