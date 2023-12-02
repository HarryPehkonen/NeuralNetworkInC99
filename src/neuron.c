#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neuron.h"

time_t seed = (time_t)0;

// activation functions
nn_value_t
act_sigmoid(nn_value_t x) {
	return (nn_value_t) (1.0 / (1.0 + exp(-x)));
}

nn_value_t
act_tanh(nn_value_t x) {
    return tanh(x);
}

nn_value_t
act_relu(nn_value_t x) {
    return (x > 0) ? x : 0;
}

nn_value_t
act_leaky_relu(nn_value_t x) {
    return (x > 0) ? x : 0.01 * x;
}

// TODO:  Softmax?

// must be in same order as activation_t enums
activation_function_t activations[] = {
	act_sigmoid,
	act_tanh,
	act_relu,
	act_leaky_relu
};

// neuron
neuron_t *
neuron_make(unsigned int n_weights, activation_t activation) {
	neuron_t *neuron = (neuron_t *)malloc(sizeof(neuron));
	if (neuron != NULL) {
		neuron->weights = (nn_value_t *)malloc(n_weights * sizeof(nn_value_t));
	}
	if ((neuron == NULL) || (neuron->weights == NULL)) {
		exit(1);
	}

	neuron->activation_idx = activation;

	// start with a seed, but only the first time
	if (seed == 0) {
		seed = time(NULL);
		if (seed == (time_t)(-1)) {
			exit(2);
		}
		srand(seed);
	}

	neuron->n_weights = n_weights;
	for (int i = 0; i < n_weights; i++) {
		neuron->weights[i] = rand() / (nn_value_t)RAND_MAX;
	}
	neuron->bias = rand() / (nn_value_t)RAND_MAX;

	return neuron;
}

neuron_t *
neuron_free(neuron_t *neuron) {
	free(neuron->weights);
	neuron->weights = NULL;
	free(neuron);
	return NULL;
}

nn_value_t
neuron_calculate_output(neuron_t *neuron, nn_value_t *inputs, int inputs_len) {
	nn_value_t sum = neuron->bias;
	for (int i = 0; i < inputs_len; i++) {
		sum += inputs[i] * neuron->weights[i];
	}
	return activations[neuron->activation_idx](sum);
}

// not multithreaded
char * serialized_neuron = NULL;
size_t serialized_neuron_len = 128;

char *
neuron_serialize(neuron_t *neuron) {

	// keep track of how much we have written so we can append to end
	size_t written = 0;

	// allocate buffer
	if (serialized_neuron == NULL) {
		serialized_neuron = malloc(serialized_neuron_len);
		if (serialized_neuron == NULL) {
			exit(3);
		}
	}

	// write n_weights
	written += snprintf(
		&serialized_neuron[written],
		serialized_neuron_len - written,
		"%04x%c",
		neuron->n_weights,
		NN_DELIMITER
	);

	// write weights
	for (int i = 0; i < neuron->n_weights; ++i) {
		written += snprintf(
			&serialized_neuron[written],
			serialized_neuron_len - written,
			"%1.15lf%c",
			neuron->weights[i],
			NN_DELIMITER
		);
	}

	// write bias
	written += snprintf(
		&serialized_neuron[written],
		serialized_neuron_len - written,
		"%1.15lf%c",
		neuron->bias,
		NN_DELIMITER
	);

	return serialized_neuron;
}

static int
is_delimiter_correct(char delimiter) {
	if (delimiter != NN_DELIMITER) {
		fprintf(stderr, "Expected delimiter, but received %c\n", delimiter);
		return 1 == 0;
	}
	return 1 == 1;
}

neuron_t *
neuron_deserialize(char *txt) {
	unsigned int n_weights = 0;
	size_t read = 0;
	size_t chars_read = 0;
	char delimiter;

	// get the number of weights
	chars_read = sscanf(&txt[read], "%04x", &n_weights) * 4;
	read += chars_read;

	// skip over the delimiter
	// make sure it's correct
	chars_read = sscanf(&txt[read], "%c", &delimiter) * sizeof(char);
	if (!is_delimiter_correct(delimiter)) {
		return NULL;
	}
	read += chars_read;

	// make a new neuron, and start adding values
	neuron_t *neuron = neuron_make(n_weights, SIGMOID);

	// read and populate weights
	for (int i = 0; i < n_weights; ++i) {
		nn_value_t weight;
		chars_read = sscanf(&txt[read], "%lf", &weight) * NN_DOUBLE_LEN;
		read += chars_read;

		// make sure the delimiter is correct
		chars_read = sscanf(&txt[read], "%c", &delimiter) * sizeof(char);
		if (!is_delimiter_correct(delimiter)) {
			return NULL;
		}
		read += chars_read;
		neuron->weights[i] = weight;
	}

	nn_value_t bias;
	chars_read = sscanf(&txt[read], "%lf", &bias) * NN_DOUBLE_LEN;
	read += chars_read;
	
	// make sure the delimiter is correct
	chars_read = sscanf(&txt[read], "%c", &delimiter) * sizeof(char);
	if (!is_delimiter_correct(delimiter)) {
		return NULL;
	}
	read += chars_read;
	neuron->bias = bias;

	return neuron;
}
