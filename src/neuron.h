#define NN_EPSILON 0.000000000000001

// length of double when serializing/deserializing
#define NN_DOUBLE_LEN 17
#define NN_DELIMITER '\t'

typedef double nn_value_t;

typedef struct {
	unsigned int n_weights;
	nn_value_t *weights;
	nn_value_t bias;
} neuron_t;

neuron_t *neuron_make(unsigned int n_weights);
neuron_t *neuron_free(neuron_t *);
nn_value_t sigmoid(nn_value_t);
nn_value_t neuron_calculate_output(neuron_t *, nn_value_t *, int);
char *neuron_serialize(neuron_t *);
neuron_t *neuron_deserialize(char *);

