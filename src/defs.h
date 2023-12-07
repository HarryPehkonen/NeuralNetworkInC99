#ifndef DEFS_H
#define DEFS_H

#define NN_EPSILON 0.00000001

typedef double nn_input_t;
typedef double nn_value_t;

typedef enum {
	SIGMOID,
	TANH,
	RELU,
	LEAKY_RELU
} activation_idx_t;


#endif // !DEFS_H

