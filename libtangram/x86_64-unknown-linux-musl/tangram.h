/** This header file defines the C API for libtangram, the Tangram C library. */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/// `tangram_feature_contribution_type` corresponds to the Tangram feature type.
typedef enum {
  IDENTITY,
  NORMALIZED,
  ONE_HOT_ENCODED,
  BAG_OF_WORDS,
  WORD_EMBEDDING,
} tangram_feature_contribution_entry_type;

/// `tangram_ngram_type` identifies the ngram type.
typedef enum {
  UNIGRAM,
  BIGRAM,
} tangram_ngram_type;

/// A `tangram_task` identifies the task a model performs, one of regression, binary classification, or multiclass classification.
typedef enum {
  REGRESSION,
  BINARY_CLASSIFICATION,
  MULTICLASS_CLASSIFICATION,
} tangram_task;

/// `tangram_bag_of_words_feature_contribution` is an opaque handle to a single tangram bag of words feature contribution.
typedef struct tangram_bag_of_words_feature_contribution tangram_bag_of_words_feature_contribution;

/// A `tangram_binary_classification_predict_output` is an opaque handle to a binary classification predict output returned by `tangram_model_predict`.
typedef struct tangram_binary_classification_predict_output tangram_binary_classification_predict_output;

/// A `tangram_error` value is an opaque handle to an error returned by a libtangram function.
typedef struct tangram_error tangram_error;

/// `tangram_feature_contribution` is an opaque handle to a single tangram feature contribution.
typedef struct tangram_feature_contribution_entry tangram_feature_contribution_entry;

/// `tangram_feature_contributions` is an opaque handle to the feature contributions returned from `tangram_model_predict`.
typedef struct tangram_feature_contributions tangram_feature_contributions;

/// `tangram_identity_feature_contribution` is an opaque handle to a single tangram identity feature contribution.
typedef struct tangram_identity_feature_contribution tangram_identity_feature_contribution;

typedef struct tangram_model tangram_model;

/// A `tangram_multiclass_classification_predict_output` is an opaque handle to a multiclass classfication predict output returned by `tangram_model_predict`.
typedef struct tangram_multiclass_classification_predict_output tangram_multiclass_classification_predict_output;

/// A `tangram_multiclass_classification_predict_output_feature_contributions_iter` value is an iterator over `(class_name, feature_contributions)` pairs.
typedef struct tangram_multiclass_classification_predict_output_feature_contributions_iter tangram_multiclass_classification_predict_output_feature_contributions_iter;

/// A `tangram_multiclass_classification_predict_output_probabilities_iter` value is an iterator over `(class_name, probability)` pairs.
typedef struct tangram_multiclass_classification_predict_output_probabilities_iter tangram_multiclass_classification_predict_output_probabilities_iter;

/// `tangram_ngram` is an opaque handle to an ngram.
typedef struct tangram_ngram tangram_ngram;

/// `tangram_normalized_feature_contribution` is an opaque handle to a single tangram normalized feature contribution.
typedef struct tangram_normalized_feature_contribution tangram_normalized_feature_contribution;

/// `tangram_one_hot_encoded_feature_contribution` is an opaque handle to a single tangram one hot encoded feature contribution.
typedef struct tangram_one_hot_encoded_feature_contribution tangram_one_hot_encoded_feature_contribution;

/// A `tangram_predict_input` is an opaque handle to a predict input to be passed to `tangram_model_predict`.
typedef struct tangram_predict_input tangram_predict_input;

/// A `tangram_predict_input_vec` is an opaque handle to a vec of predict inputs.
typedef struct tangram_predict_input_vec tangram_predict_input_vec;

/// A `tangram_predict_options` value is an opaque handle to predict options to be passed to `tangram_model_predict`.
typedef struct tangram_predict_options tangram_predict_options;

/// A `tangram_predict_output` value is an opaque handle to the output of `tangram_model_predict`.
typedef struct tangram_predict_output tangram_predict_output;

/// A `tangram_predict_output_vec` is an opaque handle to a vec of predict outputs.
typedef struct tangram_predict_output_vec tangram_predict_output_vec;

/// A `tangram_regression_predict_output` value is an opaque handle to a regression predict output returned by `tangram_model_predict`.
typedef struct tangram_regression_predict_output tangram_regression_predict_output;

/// `tangram_word_embedding_feature_contribution` is an opaque handle to a single tangram word embedding feature contribution.
typedef struct tangram_word_embedding_feature_contribution tangram_word_embedding_feature_contribution;

/// A `tangram_string_view` value provides the pointer and length of a UTF-8 encoded string.
typedef struct {
  /// The pointer to the UTF-8 encoded bytes.
  const char *ptr;
  /// The number bytes in the string.
  size_t len;
} tangram_string_view;

/// Retrieve the version of libtangram that is in use. On success, a string view of the version will be written to `version_ptr`.
void tangram_version(tangram_string_view *version_ptr);

/// Delete an error.
void tangram_error_delete(tangram_error *error);

/// Retrieve an error message as a string view. The string view will be valid until `error` is deleted by calling `tangram_error_delete`.
void tangram_error_get_message(tangram_error *error,
                               tangram_string_view *message_ptr);

/// Load a model from the file at `path`. On success, a pointer to the loaded model will be written to `model_ptr`. You must call `tangram_model_delete` when you are done with it.
tangram_error *tangram_model_from_path(const char *path,
                                       const tangram_model **model_ptr);

/// Load a model from the bytes pointed to by `model_data` with length `model_data_len`. On success, a pointer to the loaded model will be written to `model_ptr`. You must call `tangram_model_delete` when you are done with it.
tangram_error *tangram_model_from_bytes(const void *model_bytes,
                                        size_t model_bytes_len,
                                        const tangram_model **model_ptr);

/// Delete a model.
void tangram_model_delete(tangram_model *model);

/// Retrieve the id of a model. On success, a pointer to the model id as a tangram string view will be written to `id_ptr`.
void tangram_model_get_id(const tangram_model *model,
                          tangram_string_view *id_ptr);

/// Retrieve the task of the model. On success, the task will be written to `task_ptr`.
void tangram_model_get_task(const tangram_model *model, tangram_task *task_ptr);

/// Create a new predict input. You must add it to a `tangram_predict_input_vec` or call `tangram_predict_input_delete` when you are done with it.
void tangram_predict_input_new(const tangram_predict_input **predict_input_ptr);

/// Delete a predict input.
void tangram_predict_input_delete(tangram_predict_input *predict_input);

/// Set the value of column `column_name` to the string `value`.
tangram_error *tangram_predict_input_set_value_string(tangram_predict_input *predict_input,
                                                      const char *column_name,
                                                      const char *value);

/// Set the value of column `column_name` to the number `value`.
tangram_error *tangram_predict_input_set_value_number(tangram_predict_input *predict_input,
                                                      const char *column_name,
                                                      double value);

/// Create a new predict input vec.
void tangram_predict_input_vec_new(const tangram_predict_input_vec **predict_input_vec);

/// Delete a predict input vec.
void tangram_predict_input_vec_delete(tangram_predict_input_vec *predict_input_vec);

/// Add a predict input to the predict input vec.
void tangram_predict_input_vec_push(tangram_predict_input_vec *predict_input_vec,
                                    tangram_predict_input *predict_input);

/// Create a new `tangram_predict_options` value. You must call `tangram_predict_options_delete` when you are done with it.
void tangram_predict_options_new(const tangram_predict_options **predict_options_ptr);

/// Delete a `tangram_predict_options` value.
void tangram_predict_options_delete(tangram_predict_options *predict_options);

/// Set the classification threshold.
void tangram_predict_options_set_threshold(tangram_predict_options *predict_options,
                                           float threshold);

/// Enable or disable computing feature contributions.
void tangram_predict_options_set_compute_feature_contributions(tangram_predict_options *predict_options,
                                                               bool compute_feature_contributions);

/// Make a prediction! `model` should point to a model loaded with `tangram_model_load`. `input` should be a `tangram_predict_input` value and options should be a `tangram_predict_options` value. On success, a pointer to a `tangram_predict_output` output will be written to `output_ptr`. You must call `tangram_predict_output_delete` when you are done with it.
tangram_error *tangram_model_predict(const tangram_model *model,
                                     const tangram_predict_input_vec *input,
                                     const tangram_predict_options *options,
                                     const tangram_predict_output_vec **output_ptr);

/// Delete a predict output.
void tangram_predict_output_delete(tangram_predict_output *predict_output);

/// Delete a predict output vec.
void tangram_predict_output_vec_delete(tangram_predict_output_vec *predict_output_vec);

/// Retrieve the len of the output vec.
void tangram_predict_output_vec_len(const tangram_predict_output_vec *predict_output_vec,
                                    size_t *len_ptr);

/// Get the predict output at `index`.
void tangram_predict_output_vec_get_at_index(tangram_predict_output_vec *predict_output_vec,
                                             size_t index,
                                             const tangram_predict_output **predict_output_ptr);

/// Cast the predict output as `tangram_regression_predict_output`. If this predict output is not for regression, null will be written to `regression_predict_output_ptr`.
void tangram_predict_output_as_regression(const tangram_predict_output *predict_output,
                                          const tangram_regression_predict_output **predict_output_ptr);

/// Cast the predict output as `tangram_binary_classification_predict_output`. If this predict output is not for binary classification, null will be written to `binary_classification_predict_output_ptr`.
void tangram_predict_output_as_binary_classification(const tangram_predict_output *predict_output,
                                                     const tangram_binary_classification_predict_output **predict_output_ptr);

/// Cast the predict output as `tangram_multiclass_classification_predict_output`. If this predict output is not for multiclass classification, null will be written to `multiclass_classification_predict_output_ptr`.
void tangram_predict_output_as_multiclass_classification(const tangram_predict_output *predict_output,
                                                         const tangram_multiclass_classification_predict_output **predict_output_ptr);

/// Retrieve the value from a regression predict output.
void tangram_regression_predict_output_get_value(const tangram_regression_predict_output *predict_output,
                                                 float *output_value);

/// Retrieve the feature contributions from a regression predict output. If feature contributions were not computed for this prediction, null will be written to `feature_contributions_ptr`.
void tangram_regression_predict_output_get_feature_contributions(const tangram_regression_predict_output *predict_output,
                                                                 const tangram_feature_contributions **feature_contributions_ptr);

/// Retrieve the class name from a binary classification predict output.
void tangram_binary_classification_predict_output_get_class_name(const tangram_binary_classification_predict_output *predict_output,
                                                                 tangram_string_view *class_name_ptr);

/// Retrieve the probability from a binary classification predict output.
void tangram_binary_classification_predict_output_get_probability(const tangram_binary_classification_predict_output *predict_output,
                                                                  float *probability);

/// Retrieve the feature contributions from a binary classification predict output.
void tangram_binary_classification_predict_output_get_feature_contributions(const tangram_binary_classification_predict_output *predict_output,
                                                                            const tangram_feature_contributions **feature_contributions_ptr);

/// Retrieve the class name from a multiclass classification predict output. If feature contributions were not computed for this prediction, null will be written to `feature_contributions_ptr`.
void tangram_multiclass_classification_predict_output_get_class_name(const tangram_multiclass_classification_predict_output *predict_output,
                                                                     tangram_string_view *class_name_ptr);

/// Retrieve the probability from a multiclass classification predict output.
void tangram_multiclass_classification_predict_output_get_probability(const tangram_multiclass_classification_predict_output *predict_output,
                                                                      float *probability_ptr);

/// Retrieve the number of classes from a multiclass classification predict output.
void tangram_multiclass_classification_predict_output_get_probabilities_len(const tangram_multiclass_classification_predict_output *predict_output,
                                                                            size_t *len_ptr);

/// Delete a multiclass classification predict output probabilities iterator.
void tangram_multiclass_classification_predict_output_probabilities_iter_delete(tangram_multiclass_classification_predict_output_probabilities_iter *probabilities_iter);

/// Get an iterator over the probabilities for a multiclass classification predict output. You must call `tangram_multiclass_classification_predict_output_probabilities_iter_delete` when you are done with it.
void tangram_multiclass_classification_predict_output_get_probabilities_iter(const tangram_multiclass_classification_predict_output *predict_output,
                                                                             const tangram_multiclass_classification_predict_output_probabilities_iter **probabilities_iter_ptr);

/// Retrieve the next `(class_name, probability)` pair from the probabilties iterator. This function returns `true` if `class_name_ptr` and `probability_ptr` have been successfully set, or `false` if the iterator has reached its end.
bool tangram_multiclass_classification_predict_output_probabilities_iter_next(tangram_multiclass_classification_predict_output_probabilities_iter *probabilities_iter,
                                                                              tangram_string_view *class_name_ptr,
                                                                              float *probability_ptr);

/// Delete a multiclass classification predict output feature contributions iterator.
void tangram_multiclass_classification_predict_output_feature_contributions_iter_delete(tangram_multiclass_classification_predict_output_feature_contributions_iter *feature_contributions_iter);

/// Retrieve the feature contributions from a multiclass classification predict output. If feature contributions were not computed for this prediction, null will be written to `feature_contributions_iter_ptr`.
void tangram_multiclass_classification_predict_output_get_feature_contributions_iter(const tangram_multiclass_classification_predict_output *predict_output,
                                                                                     const tangram_multiclass_classification_predict_output_feature_contributions_iter **feature_contributions_iter_ptr);

/// Retrieve the next `(class_name, feature_contributions)` pair from the feature contributions iterator. This function returns `true` if `class_name_ptr` and `feature_contributions_ptr` have been successfully set, or `false` if the iterator has reached its end.
bool tangram_multiclass_classification_predict_output_feature_contributions_iter_next(tangram_multiclass_classification_predict_output_feature_contributions_iter *feature_contributions_iter,
                                                                                      tangram_string_view *class_name_ptr,
                                                                                      const tangram_feature_contributions **feature_contributions_ptr);

/// Retrieve the baseline value from feature contributions.
void tangram_feature_contributions_get_baseline_value(const tangram_feature_contributions *feature_contributions,
                                                      float *baseline_value_ptr);

/// Retrieve the output value from feature contributions.
void tangram_feature_contributions_get_output_value(const tangram_feature_contributions *feature_contributions,
                                                    float *output_value_ptr);

/// Retrieve the len of the feature contributions.
void tangram_feature_contributions_get_entries_len(const tangram_feature_contributions *feature_contributions,
                                                   size_t *len_ptr);

/// Retrieve the feature contribution at `index`.
void tangram_feature_contributions_get_entry_at_index(const tangram_feature_contributions *feature_contributions,
                                                      size_t index,
                                                      const tangram_feature_contribution_entry **feature_contribution_entry_ptr);

/// Retrieve the type of the feature contribution entry.
void tangram_feature_contribution_entry_get_type(const tangram_feature_contribution_entry *feature_contribution_entry,
                                                 tangram_feature_contribution_entry_type *type_ptr);

/// Cast the feature contribution entry as `tangram_identity_feature_contribution`. If this feature contribution is not an identity feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void tangram_feature_contribution_entry_as_identity(const tangram_feature_contribution_entry *feature_contribution_entry,
                                                    const tangram_identity_feature_contribution **feature_contribution_ptr);

/// Cast the feature contribution entry as `tangram_normalized_feature_contribution`. If this feature contribution is not a normalized feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void tangram_feature_contribution_entry_as_normalized(const tangram_feature_contribution_entry *feature_contribution_entry,
                                                      const tangram_normalized_feature_contribution **feature_contribution_ptr);

/// Cast the feature contribution entry as `tangram_one_hot_encoded_feature_contribution`. If this feature contribution is not a one hot encoded feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void tangram_feature_contribution_entry_as_one_hot_encoded(const tangram_feature_contribution_entry *feature_contribution_entry,
                                                           const tangram_one_hot_encoded_feature_contribution **feature_contribution_ptr);

/// Cast the feature contribution entry as `tangram_bag_of_words_feature_contribution`. If this feature contribution is not a bag of words feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void tangram_feature_contribution_entry_as_bag_of_words(const tangram_feature_contribution_entry *feature_contribution_entry,
                                                        const tangram_bag_of_words_feature_contribution **feature_contribution_ptr);

/// Cast the feature contribution entry as `tangram_word_embedding_feature_contribution`. If this feature contribution is not a word embedding feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void tangram_feature_contribution_entry_as_word_embedding(const tangram_feature_contribution_entry *feature_contribution_entry,
                                                          const tangram_word_embedding_feature_contribution **feature_contribution_ptr);

/// Retrieve the column name.
void tangram_identity_feature_contribution_get_column_name(const tangram_identity_feature_contribution *feature_contribution,
                                                           tangram_string_view *column_name_ptr);

/// Retrieve the feature contribution value.
void tangram_identity_feature_contribution_get_feature_contribution_value(const tangram_identity_feature_contribution *feature_contribution,
                                                                          float *feature_contribution_value);

/// Retrieve the feature value.
void tangram_identity_feature_contribution_get_feature_value(const tangram_identity_feature_contribution *feature_contribution,
                                                             float *feature_value);

/// Retrieve the column name.
void tangram_normalized_feature_contribution_get_column_name(const tangram_normalized_feature_contribution *feature_contribution,
                                                             tangram_string_view *column_name_ptr);

/// Retrieve the feature value.
void tangram_normalized_feature_contribution_get_feature_value(const tangram_normalized_feature_contribution *feature_contribution,
                                                               float *feature_value);

/// Retrieve the feature contribution value.
void tangram_normalized_feature_contribution_get_feature_contribution_value(const tangram_normalized_feature_contribution *feature_contribution,
                                                                            float *feature_contribution_value);

/// Retrieve the column name.
void tangram_one_hot_encoded_feature_contribution_get_column_name(const tangram_one_hot_encoded_feature_contribution *feature_contribution,
                                                                  tangram_string_view *column_name_ptr);

/// Retrieve the variant.
void tangram_one_hot_encoded_feature_contribution_get_variant(const tangram_one_hot_encoded_feature_contribution *feature_contribution,
                                                              tangram_string_view *variant_ptr);

/// Retrieve the feature value.
void tangram_one_hot_encoded_feature_contribution_get_feature_value(const tangram_one_hot_encoded_feature_contribution *feature_contribution,
                                                                    bool *feature_value);

/// Retrieve the feature contribution value.
void tangram_one_hot_encoded_feature_contribution_get_feature_contribution_value(const tangram_one_hot_encoded_feature_contribution *feature_contribution,
                                                                                 float *feature_contribution_value);

/// Retrieve the column name.
void tangram_bag_of_words_feature_contribution_get_column_name(const tangram_bag_of_words_feature_contribution *feature_contribution,
                                                               tangram_string_view *column_name_ptr);

/// Retrieve the ngram.
void tangram_bag_of_words_feature_contribution_get_ngram(const tangram_bag_of_words_feature_contribution *feature_contribution,
                                                         const tangram_ngram **ngram_ptr);

/// Retrieve the ngram type.
void tangram_ngram_get_type(const tangram_ngram *ngram, tangram_ngram_type *ngram_type);

/// Retrieve the unigram token.
void tangram_unigram_get_token(const tangram_ngram *ngram, tangram_string_view *token_ptr);

/// Retrieve the bigram token a.
void tangram_bigram_get_token_a(const tangram_ngram *ngram, tangram_string_view *token_ptr);

/// Retrieve the bigram token b.
void tangram_bigram_get_token_b(const tangram_ngram *ngram, tangram_string_view *token_ptr);

/// Retrieve the feature value.
void tangram_bag_of_words_feature_contribution_get_feature_value(const tangram_bag_of_words_feature_contribution *feature_contribution,
                                                                 bool *feature_value);

/// Retrieve the feature contribution value.
void tangram_bag_of_words_feature_contribution_get_feature_contribution_value(const tangram_bag_of_words_feature_contribution *feature_contribution,
                                                                              float *feature_contribution_value);

/// Retrieve the column name.
void tangram_word_embedding_feature_contribution_get_column_name(const tangram_word_embedding_feature_contribution *feature_contribution,
                                                                 tangram_string_view *column_name_ptr);

/// Retrieve the value index.
void tangram_word_embedding_feature_contribution_get_value_index(const tangram_word_embedding_feature_contribution *feature_contribution,
                                                                 size_t *value_index);

/// Retrieve the feature contribution value.
void tangram_word_embedding_feature_contribution_get_feature_contribution_value(const tangram_word_embedding_feature_contribution *feature_contribution,
                                                                                float *feature_contribution_value);

/// This function exposes the allocator used by libtangram. It is used by the wasm build of libtangram because WebAssembly does not include its own allocator.
void *tangram_alloc(size_t size,
                    size_t align);

/// This function exposes the allocator used by libtangram. It is used by the wasm build of libtangram because WebAssembly does not include its own allocator.
void tangram_dealloc(void *ptr,
                     size_t size,
                     size_t align);
