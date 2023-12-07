#include <math.h> // make vs2022 happy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests.h"
#include "../src/defs.h"
#include "../src/neuron.h"

// ANSI color escape sequences
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

static int test_neuron_initialization(void)
{
    size_t n_inputs = 5;
    int result = 0;
    neuron_t* neuron = neuron_make(n_inputs);
    if (neuron == NULL) {
        result |= 1;
    } else if (neuron->weights == NULL) {
        result |= 2;
    }
    neuron_free(neuron);

    return result;
}

static int test_neuron_serialize(void)
{
    int result = 0;
    unsigned int n_inputs = 4;
    char* filename = "test_neuron_serialize.txt";
    activation_idx_t activation_idx = RELU;

    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        return __LINE__;
    }
    neuron_t* neuron = neuron_make(n_inputs);

    // for consistency, set all random values to something known
    for (int i = 0; i < n_inputs; ++i) {
        neuron->weights[i] = 1;
    }
    neuron->bias = 0;

    if (neuron_serialize(fp, neuron)) {
        return __LINE__;
    }
    fclose(fp);

    // read it back in
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        return __LINE__;
    }
    neuron_t* deserialized_neuron = neuron_deserialize(fp);
    fclose(fp);

    if (deserialized_neuron == NULL) {
        return __LINE__;
    }

    // make sure n_weights is the same
    if (deserialized_neuron->n_inputs != neuron->n_inputs) {
        result |= 1;
    }

    // make sure weights are the same
    for (int i = 0; i < deserialized_neuron->n_inputs; ++i) {
        if (fabs(neuron->weights[i] - deserialized_neuron->weights[i]) > NN_EPSILON) {
            printf("compared %1.15lf to %1.15lf\n", neuron->weights[i],
                deserialized_neuron->weights[i]);
            result |= 2;
        }
    }

    // make sure bias is the same
    if (fabs(deserialized_neuron->bias - neuron->bias) > NN_EPSILON) {
        result |= 4;
    }

    // wait until the end to clean up.  if we fail, we can inspect the file
    if (result == 0) {
        if (remove(filename)) {
            return __LINE__;
        }
    }

    neuron_free(neuron);
    neuron_free(deserialized_neuron);
    return result;
}

// static int test_neuron_output(void) {
// 	int result = 0;
// 	unsigned int n_inputs = 5;
// 	nn_value_t input1[5] = { -1.0, -0.5, 0, 0.5, 1.0 };
// 	neuron_t* neuron = neuron_make(n_inputs, SIGMOID);

// 	// set all weights to 1
// 	for (int i = 0; i < n_inputs; ++i) {
// 		neuron->weights[i] = 1;
// 	}
// 	neuron->bias = 0;
// 	nn_value_t output = neuron_activate(neuron, input1, n_inputs);
// 	if (fabs(output - 0.5) > NN_EPSILON) {
// 		result |= 1;
// 	}

// 	return result;
// }

static test_t tests[] = {
    { "Neuron Initialization Test", test_neuron_initialization },
    { "Neuron Serialization Test", test_neuron_serialize },
    //	{"Neuron Calculation Test", test_neuron_output},
};

void test()
{
    printf("Starting tests...\n");
    int success_count = 0;
    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        printf("Test %zu - %s: ", i + 1, tests[i].description);
        int result = tests[i].test_func();
        if (result == 0) {
            printf(ANSI_COLOR_GREEN "Success" ANSI_COLOR_RESET "\n");
            success_count++;
        } else {
            printf(ANSI_COLOR_RED "Failure (%u)" ANSI_COLOR_RESET "\n", result);
        }
    }
    printf("Tests completed. %d/%zu tests passed.\n", success_count,
        sizeof(tests) / sizeof(tests[0]));
}

int main(int argc, char* argv[])
{
    test();
    return 0;
}
