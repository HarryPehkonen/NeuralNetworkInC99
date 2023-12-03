#define NN_EPSILON 0.00000001

// length of double when serializing/deserializing
#define NN_DOUBLE_LEN 17
#define NN_DELIMITER '\t'

typedef double nn_value_t;

typedef enum {
	SIGMOID,
	TANH,
	RELU,
	LEAKY_RELU
} activation_t;

typedef struct {
	unsigned int n_weights;

	// it's easier to de/serialize integers than function pointers
	activation_t activation_idx;
	nn_value_t *weights;
	nn_value_t bias;
} neuron_t;

typedef nn_value_t (*activation_function_t)(nn_value_t);

neuron_t *neuron_make(unsigned int n_weights, activation_t);
neuron_t *neuron_free(neuron_t *);
nn_value_t sigmoid(nn_value_t);
nn_value_t neuron_run(neuron_t *, nn_value_t *inputs[], int inputs_len);
char *neuron_serialize(neuron_t *);
neuron_t *neuron_deserialize(char *);

