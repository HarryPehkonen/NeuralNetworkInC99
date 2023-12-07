#include "defs.h"

// activation functions
nn_value_t
act_sigmoid(nn_value_t x) {
	return (nn_value_t)(1.0 / (1.0 + exp(-x)));
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

