#include "layer_input.h"
#include "neuron.h"


layer_input_t *
layer_input_make(int n_inputs) {
	/*
	typedef struct {
		int n_inputs;
		int n_populated;
		neuron_t *neurons;
		nn_value_t *results;
	} layer_input_t;
	*/
	layer_input_t *layer_input = NULL;
	layer_input = (layer_input_t *)malloc(sizeof(layer_input_t));
	if (layer_input == NULL) {
		return NULL;
	}
	layer_input->n_inputs = n_inputs;
	layer_input->n_populated = 0;

	layer_input->neurons = (neuron_t *)malloc(sizeof(neuron_t) * n_inputs);
	if (layer_input->neurons == NULL) {
		free(layer_input);
		return NULL;
	}
	layer_input->results = (nn_value_t *)malloc(sizeof(nn_value_t) * n_inputs);
	 if (layer_input->results == NULL) {
		free(layer_input->neurons);
		free(layer_input);
		return NULL;
	}

	return layer_input;
}

int
layer_input_add(layer_input_t *layer_input, neuron_t *neuron) {
	layer_input->neurons[layer_input->n_populated] = neuron;]
	layer_input->n_populated++;
	return layer_input->n_populated;
}

// assume same number of inputs as populated neurons
nn_value_t *
layer_input_run(layer_input_t *layer_input, nn_input_t *inputs[]) {
	for (int i = 0; i < layer_input->n_populated; i++) {
		layer_input->results[i] = neuron_run(layer_input->neurons[i], inputs[i], len);
	}
	return layer_input->results;
}

void
layer_input_free(layer_input_t *layer_input) {
	for (int i = layer_input->n_populated - 1; i >= 0; i--) {
		neuron_free(layer_input->neurons[i]);
		layer_input->neurons[i] = NULL;
	}
	layer_input->n_populated = 0;
	free(layer_input->neurons);
	layer_input->neurons = NULL;

	free(layer_input->result);
	layer_input->result = NULL;

	free(layer_input);
	layer_input = NULL; // for some reason
}
