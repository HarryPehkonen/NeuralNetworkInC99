typedef struct {
	int n_inputs;
	int n_populated;
	neuron_t *neurons[];
	nn_value_t *results[];
} layer_input_t;

layer_input_t *layer_input_make(int);
int layer_input_add(layer_input_t *, neuron_t *);
nn_value_t *layer_input_run(layer_input_t *, nn_input_t *inputs[]);
void layer_input_free(layer_input_t *);
