#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "activation.h"
#include "neuron.h"
#include "layer.h"

layer_t*
layer_make(int n_inputs) {
	layer_t* layer = NULL;
	layer = (layer_t*)malloc(sizeof(layer_t));
	if (layer == NULL) {
		return NULL;
	}
	layer->n_inputs = n_inputs;
	layer->n_populated = 0;

	layer->neurons = (neuron_t**)malloc(sizeof(neuron_t*) * n_inputs);
	if (layer->neurons == NULL) {
		free(layer);
		return NULL;
	}

	return layer;
}

// add a neuron to the layer.  return the number of neurons populated
int
layer_add_neuron(layer_t* layer, neuron_t* neuron) {

	// safety check
	if (layer->n_populated >= layer->n_inputs) {
		return -1;
	}
	layer->neurons[layer->n_populated] = neuron;
	layer->n_populated++;
	return layer->n_populated;
}

// assume same number of inputs as populated neurons.
// since this is an input layer, each neuron has a single input.
void
layer_activate(layer_t* layer, activation_function_t act) {
	for (int i = 0; i < layer->n_populated; i++) {
		neuron_activate(layer->neurons[i], act);
	}
}

void
layer_free(layer_t* layer) {
	for (int i = layer->n_populated - 1; i >= 0; i--) {
		neuron_free(layer->neurons[i]);
		layer->neurons[i] = NULL;
	}
	layer->n_populated = 0;
	free(layer->neurons);
	layer->neurons = NULL;

	free(layer);
	layer = NULL; // for some reason
}
