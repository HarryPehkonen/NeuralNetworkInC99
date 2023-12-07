#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "activation.h"
#include "defs.h"
#include "neuron.h"

time_t seed = (time_t)0;

// must be in same order as activation_t enums
activation_function_t activations[] = {
    act_sigmoid,
    act_tanh,
    act_relu,
    act_leaky_relu
    // softmax?
};

// once a neuron is created, it cannot be resized.
neuron_t*
neuron_make(unsigned int n_inputs)
{
    neuron_t* neuron = (neuron_t*)malloc(sizeof(neuron_t));
    if (neuron == NULL) {
        exit(__LINE__);
    }
    neuron->inputs = (nn_value_t**)malloc(n_inputs * sizeof(nn_value_t*));
    if (neuron->inputs == NULL) {
        free(neuron);
        exit(__LINE__);
    }
    neuron->weights = (nn_value_t*)malloc(n_inputs * sizeof(nn_value_t));
    if (neuron->weights == NULL) {
        free(neuron->inputs);
        free(neuron);
        exit(__LINE__);
    }

    // start with a seed, but only the first time
    if (seed == 0) {
        seed = time(NULL);
        if (seed == (time_t)(-1)) {
            exit(__LINE__);
        }
        srand(seed);
    }

    neuron->n_inputs = n_inputs;
    for (int i = 0; i < n_inputs; i++) {

        // initialize inputs to NULL
        neuron->inputs[i] = NULL;

        // initialize weights to random values between 0 and 1
        neuron->weights[i] = rand() / (nn_value_t)RAND_MAX;
    }
    neuron->bias = rand() / (nn_value_t)RAND_MAX;

    return neuron;
}

neuron_t*
neuron_free(neuron_t* neuron)
{

    if (neuron == NULL) {
        return NULL;
    }
    free(neuron->weights);
    neuron->weights = NULL;

    free(neuron->inputs);
    neuron->inputs = NULL;

    free(neuron);
    return NULL;
}

void neuron_activate(neuron_t* neuron, activation_function_t activation_function)
{
    nn_value_t weighted_sum = neuron->bias;
    for (int i = 0; i < neuron->n_populated; i++) {
        weighted_sum += *neuron->inputs[i] * neuron->weights[i];
    }
    neuron->output = activation_function(weighted_sum);
}

int neuron_serialize(FILE* fh, neuron_t* neuron)
{

    // result from each write
    int result = 0;
    char separator = '#';

    // write the size of the neuron structure first
    size_t serialized_neuron_len = sizeof(neuron_t);
    if (fwrite(&serialized_neuron_len, sizeof(serialized_neuron_len), 1, fh) != 1) {
        return -1;
    }

    if (fwrite(&separator, sizeof(separator), 1, fh) != 1) {
        return -1;
    }

    // write the number of inputs
    if (fwrite(&neuron->n_inputs, sizeof(neuron->n_inputs), 1, fh) != 1) {
        return -1;
    }

    if (fwrite(&separator, sizeof(separator), 1, fh) != 1) {
        return -1;
    }

    // write the neuron structure.  this should copy over any
    // variables that are not pointers.
    if (fwrite(neuron, serialized_neuron_len, 1, fh) != 1) {
        return -1;
    }

    if (fwrite(&separator, sizeof(separator), 1, fh) != 1) {
        return -1;
    }

    // weights
    if (fwrite(neuron->weights, sizeof(nn_value_t), neuron->n_inputs, fh) != neuron->n_inputs) {
        return -1;
    }

    // connections (output of previous neuron to input of this neuron)
    // are not serialized.  they are set at runtime.

    return 0;
}

neuron_t*
neuron_deserialize(FILE* fp)
{
    int result = 0;
    char separator_expected = '#';
    char separator_actual = 0;
    neuron_t* neuron = NULL;
    neuron_t* dummy = NULL;

    // read the size of the neuron structure first
    size_t serialized_neuron_len = 0;
    if (fread(&serialized_neuron_len, sizeof(serialized_neuron_len), 1, fp) != 1) {
        return NULL;
    }

    // read the separator
    if (fread(&separator_actual, sizeof(separator_actual), 1, fp) != 1) {
        return NULL;
    }
    if (separator_actual != separator_expected) {
        return NULL;
    }

    // read the number of inputs
    size_t n_inputs = 0;
    if (fread(&n_inputs, sizeof(n_inputs), 1, fp) != 1) {
        return NULL;
    }

    // read the separator
    if (fread(&separator_actual, sizeof(separator_actual), 1, fp) != 1) {
        return NULL;
    }
    if (separator_actual != separator_expected) {
        return NULL;
    }

    // Make the neuron structure
    neuron = neuron_make(n_inputs);
    if (neuron == NULL) {
        return NULL;
    }

    // Read the neuron structure into a dummy structure -- we don't
    // want to lose pointers without freeing them.
    dummy = (neuron_t*)malloc(sizeof(neuron_t));
    if (dummy == NULL) {
        return NULL;
    }
    if (fread(dummy, sizeof(neuron_t), 1, fp) != 1) {
        return NULL;
    }

    // read the separator
    if (fread(&separator_actual, sizeof(separator_actual), 1, fp) != 1) {
        return NULL;
    }

    // copy the dummy structure into the real structure
    neuron->bias = dummy->bias;

    // done with the dummy structure
    free(dummy);
    dummy = NULL;

    // weights
    size_t readcount = fread(neuron->weights, sizeof(nn_value_t), n_inputs, fp);
    if (readcount != n_inputs) {
        return NULL;
    }

    return neuron;
}
