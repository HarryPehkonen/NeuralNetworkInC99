#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neuron.h"
#include "tests.h"

// ANSI color escape sequences
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static int test_neuron_initialization(void) {
	unsigned int n_weights = 5;
	int result = 0;
	neuron_t *neuron = neuron_make(n_weights);
	if (neuron == NULL) {
		result |= 1;
	}
	else if (neuron->weights == NULL) {
		result |= 2;
	}
	neuron_free(neuron);

	return result;
}

static int test_neuron_serialize(void) {
	int result = 0;
	unsigned int n_weights = 4;
	neuron_t *neuron = neuron_make(n_weights);
	char *serialized = neuron_serialize(neuron);
	neuron_t *deserialized_neuron = neuron_deserialize(serialized);

	if (deserialized_neuron == NULL) {
		return 0xFFFF;
	}

	// make sure n_weights is the same
	if (deserialized_neuron->n_weights != neuron->n_weights) {
		result |= 1;
	}

	// make sure weights are the same
	for (int i = 0; i < deserialized_neuron->n_weights; ++i) {
		if (abs(neuron->weights[i] - deserialized_neuron->weights[i]) > NN_EPSILON) {
			result |= 2;
		}
	}

	if (abs(deserialized_neuron->bias - neuron->bias) > NN_EPSILON) {
		result |= 4;
	}

	neuron_free(neuron);
	neuron_free(deserialized_neuron);
	return result;
}

static test_t tests[] = {
	{"Neuron Initialization Test", test_neuron_initialization},
	{"Neuron Serialization Test", test_neuron_serialize},
};

void
test() {
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
	printf("Tests completed. %d/%zu tests passed.\n", success_count, sizeof(tests) / sizeof(tests[0]));
}

int
main(int argc, char *argv[]) {
	test();
	return 0;
}
