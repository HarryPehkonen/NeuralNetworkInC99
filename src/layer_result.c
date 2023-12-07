#include <stdlib.h>
#include "defs.h"
#include "layer_result.h"

layer_result_t*
layer_result_make(int length) {
	layer_result_t* layer_result = NULL;
	layer_result = (layer_result_t*)malloc(sizeof(layer_result_t));
	if (layer_result == NULL) {
		return NULL;
	}
	layer_result->length = length;
	layer_result->n_populated = 0;

	layer_result->values = (nn_value_t*)malloc(sizeof(nn_value_t) * length);
	if (layer_result->values == NULL) {
		free(layer_result);
		return NULL;
	}
	return layer_result;
}

int
layer_result_add(layer_result_t* layer_result, nn_value_t value) {

	// safety check
	if (layer_result->n_populated >= layer_result->length) {
		return -1;
	}
	layer_result->values[layer_result->n_populated] = value;
	layer_result->n_populated++;
	return layer_result->n_populated;
}