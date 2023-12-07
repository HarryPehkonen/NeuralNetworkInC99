#ifndef NEURON_H
#define NEURON_H

typedef struct {
	size_t n_inputs;
	size_t n_populated;

	nn_value_t** inputs;
	nn_value_t* weights;
	nn_value_t bias;
	nn_value_t output;
} neuron_t;

typedef nn_value_t(*activation_function_t)(nn_value_t);

neuron_t* neuron_make(unsigned int n_inputs);
neuron_t* neuron_free(neuron_t*);

nn_value_t act_sigmoid(nn_value_t);
nn_value_t act_tanh(nn_value_t);
nn_value_t act_relu(nn_value_t);
nn_value_t act_leaky_relu(nn_value_t);

// int neuron_connect(neuron_t* dest, neuron_t* src);
void neuron_activate(neuron_t*, activation_function_t act);
int neuron_serialize(FILE* fp, neuron_t* neuron);
neuron_t* neuron_deserialize(FILE*);

#endif