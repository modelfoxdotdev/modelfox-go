/*
This header file defines the C API for libtangram, the Tangram C library, which is used by the libraries for each programming language to make predictions using a model trained with the Tangram CLI. While the APIs for the language libraries are stable, this API is unstable and subject to change. All functions return a status code which will be zero on success or non-zero on error.
*/

// `tangram_model` is an opaque handle to a model loaded by libtangram.
typedef void tangram_model;

/// Retrieve the version of libtangram that is in use. On success, a pointer to the C string with the version will be written to `version_ptr`. You must call `tangram_string_free` when you are done with it.
int tangram_version(
	char** version_ptr
);

/// Load a model from the bytes pointed to by `model_data` with length `model_data_len`. On success, a pointer to the loaded model will be written to `model_ptr`. You must call `tangram_model_free` when you are done with it.
int tangram_model_load(
	const void* model_data,
	size_t model_data_len,
	tangram_model** model_ptr
);

/// Retrieve the id of the model. On success, a pointer to the model id as a C string will be written to `id_ptr`. You must call `tangram_string_free` when you are done with it.
int tangram_model_id(
	tangram_model* model,
	char** id_ptr
);

/// Make a prediction! `model` should point to a model loaded with `tangram_model_load`. `input` should be a C string of a json serialized PredictInput. On success, a pointer to the output as a json serialized C string will be written to `output_ptr`. You must call `tangram_string_free` when you are done with it.
int tangram_model_predict(
	tangram_model* model,
	const char* input,
	const char* options,
	char** output_ptr
);

/// Free the C string created by libtangram pointed to by `string`.
int tangram_string_free(
	const char* string
);

/// Free the model pointed to by `model`.
int tangram_model_free(
	tangram_model* model
);
