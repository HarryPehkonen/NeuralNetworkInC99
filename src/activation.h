#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "defs.h"

nn_value_t sigmoid(nn_value_t);
nn_value_t tanh(nn_value_t);
nn_value_t relu(nn_value_t);
nn_value_t leaky_relu(nn_value_t);

#endif