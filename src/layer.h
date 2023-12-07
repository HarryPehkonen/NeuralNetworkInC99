#ifndef layer_H
#define layer_H

typedef struct {
	int n_inputs;
	int n_populated;
	neuron_t** neurons;
} layer_t;

layer_t* layer_make(int);
int layer_add_neuron(layer_t*, neuron_t*);
void layer_activate(layer_t*, activation_function_t);
void layer_free(layer_t*);

#endif