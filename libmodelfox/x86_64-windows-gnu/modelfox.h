/** This header file defines the C API for libmodelfox, the ModelFox C library. */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/// `modelfox_feature_contribution_type` corresponds to the ModelFox feature type.
typedef enum {
  IDENTITY,
  NORMALIZED,
  ONE_HOT_ENCODED,
  BAG_OF_WORDS,
  BAG_OF_WORDS_COSINE_SIMILARITY,
  WORD_EMBEDDING,
} modelfox_feature_contribution_entry_type;

/// `modelfox_ngram_type` identifies the ngram type.
typedef enum {
  UNIGRAM,
  BIGRAM,
} modelfox_ngram_type;

/// A `modelfox_task` identifies the task a model performs, one of regression, binary classification, or multiclass classification.
typedef enum {
  REGRESSION,
  BINARY_CLASSIFICATION,
  MULTICLASS_CLASSIFICATION,
} modelfox_task;

/// `modelfox_bag_of_words_cosine_similarity_feature_contribution` is an opaque handle to a single modelfox bag of words cosine similarity feature contribution.
typedef struct modelfox_bag_of_words_cosine_similarity_feature_contribution modelfox_bag_of_words_cosine_similarity_feature_contribution;

/// `modelfox_bag_of_words_feature_contribution` is an opaque handle to a single modelfox bag of words feature contribution.
typedef struct modelfox_bag_of_words_feature_contribution modelfox_bag_of_words_feature_contribution;

/// A `modelfox_binary_classification_predict_output` is an opaque handle to a binary classification predict output returned by `modelfox_model_predict`.
typedef struct modelfox_binary_classification_predict_output modelfox_binary_classification_predict_output;

/// A `modelfox_error` value is an opaque handle to an error returned by a libmodelfox function.
typedef struct modelfox_error modelfox_error;

/// `modelfox_feature_contribution` is an opaque handle to a single modelfox feature contribution.
typedef struct modelfox_feature_contribution_entry modelfox_feature_contribution_entry;

/// `modelfox_feature_contributions` is an opaque handle to the feature contributions returned from `modelfox_model_predict`.
typedef struct modelfox_feature_contributions modelfox_feature_contributions;

/// `modelfox_identity_feature_contribution` is an opaque handle to a single modelfox identity feature contribution.
typedef struct modelfox_identity_feature_contribution modelfox_identity_feature_contribution;

typedef struct modelfox_model modelfox_model;

/// A `modelfox_multiclass_classification_predict_output` is an opaque handle to a multiclass classfication predict output returned by `modelfox_model_predict`.
typedef struct modelfox_multiclass_classification_predict_output modelfox_multiclass_classification_predict_output;

/// A `modelfox_multiclass_classification_predict_output_feature_contributions_iter` value is an iterator over `(class_name, feature_contributions)` pairs.
typedef struct modelfox_multiclass_classification_predict_output_feature_contributions_iter modelfox_multiclass_classification_predict_output_feature_contributions_iter;

/// A `modelfox_multiclass_classification_predict_output_probabilities_iter` value is an iterator over `(class_name, probability)` pairs.
typedef struct modelfox_multiclass_classification_predict_output_probabilities_iter modelfox_multiclass_classification_predict_output_probabilities_iter;

/// `modelfox_ngram` is an opaque handle to an ngram.
typedef struct modelfox_ngram modelfox_ngram;

/// `modelfox_normalized_feature_contribution` is an opaque handle to a single modelfox normalized feature contribution.
typedef struct modelfox_normalized_feature_contribution modelfox_normalized_feature_contribution;

/// `modelfox_one_hot_encoded_feature_contribution` is an opaque handle to a single modelfox one hot encoded feature contribution.
typedef struct modelfox_one_hot_encoded_feature_contribution modelfox_one_hot_encoded_feature_contribution;

/// A `modelfox_predict_input` is an opaque handle to a predict input to be passed to `modelfox_model_predict`.
typedef struct modelfox_predict_input modelfox_predict_input;

/// A `modelfox_predict_input_vec` is an opaque handle to a vec of predict inputs.
typedef struct modelfox_predict_input_vec modelfox_predict_input_vec;

/// A `modelfox_predict_options` value is an opaque handle to predict options to be passed to `modelfox_model_predict`.
typedef struct modelfox_predict_options modelfox_predict_options;

/// A `modelfox_predict_output` value is an opaque handle to the output of `modelfox_model_predict`.
typedef struct modelfox_predict_output modelfox_predict_output;

/// A `modelfox_predict_output_vec` is an opaque handle to a vec of predict outputs.
typedef struct modelfox_predict_output_vec modelfox_predict_output_vec;

/// A `modelfox_regression_predict_output` value is an opaque handle to a regression predict output returned by `modelfox_model_predict`.
typedef struct modelfox_regression_predict_output modelfox_regression_predict_output;

/// `modelfox_word_embedding_feature_contribution` is an opaque handle to a single modelfox word embedding feature contribution.
typedef struct modelfox_word_embedding_feature_contribution modelfox_word_embedding_feature_contribution;

/// A `modelfox_string_view` value provides the pointer and length of a UTF-8 encoded string.
typedef struct {
  /// The pointer to the UTF-8 encoded bytes.
  const char *ptr;
  /// The number bytes in the string.
  size_t len;
} modelfox_string_view;

/// Retrieve the version of libmodelfox that is in use. On success, a string view of the version will be written to `version_ptr`.
void modelfox_version(modelfox_string_view *version_ptr);

/// Delete an error.
void modelfox_error_delete(modelfox_error *error);

/// Retrieve an error message as a string view. The string view will be valid until `error` is deleted by calling `modelfox_error_delete`.
void modelfox_error_get_message(modelfox_error *error,
                                modelfox_string_view *message_ptr);

/// Load a model from the file at `path`. On success, a pointer to the loaded model will be written to `model_ptr`. You must call `modelfox_model_delete` when you are done with it.
modelfox_error *modelfox_model_from_path(const char *path,
                                         const modelfox_model **model_ptr);

/// Load a model from the bytes pointed to by `model_data` with length `model_data_len`. On success, a pointer to the loaded model will be written to `model_ptr`. You must call `modelfox_model_delete` when you are done with it.
modelfox_error *modelfox_model_from_bytes(const void *model_bytes,
                                          size_t model_bytes_len,
                                          const modelfox_model **model_ptr);

/// Delete a model.
void modelfox_model_delete(modelfox_model *model);

/// Retrieve the id of a model. On success, a pointer to the model id as a modelfox string view will be written to `id_ptr`.
void modelfox_model_get_id(const modelfox_model *model,
                           modelfox_string_view *id_ptr);

/// Retrieve the task of the model. On success, the task will be written to `task_ptr`.
void modelfox_model_get_task(const modelfox_model *model, modelfox_task *task_ptr);

/// Create a new predict input. You must add it to a `modelfox_predict_input_vec` or call `modelfox_predict_input_delete` when you are done with it.
void modelfox_predict_input_new(const modelfox_predict_input **predict_input_ptr);

/// Delete a predict input.
void modelfox_predict_input_delete(modelfox_predict_input *predict_input);

/// Set the value of column `column_name` to the string `value`.
modelfox_error *modelfox_predict_input_set_value_string(modelfox_predict_input *predict_input,
                                                        const char *column_name,
                                                        const char *value);

/// Set the value of column `column_name` to the number `value`.
modelfox_error *modelfox_predict_input_set_value_number(modelfox_predict_input *predict_input,
                                                        const char *column_name,
                                                        double value);

/// Create a new predict input vec.
void modelfox_predict_input_vec_new(const modelfox_predict_input_vec **predict_input_vec);

/// Delete a predict input vec.
void modelfox_predict_input_vec_delete(modelfox_predict_input_vec *predict_input_vec);

/// Add a predict input to the predict input vec.
void modelfox_predict_input_vec_push(modelfox_predict_input_vec *predict_input_vec,
                                     modelfox_predict_input *predict_input);

/// Create a new `modelfox_predict_options` value. You must call `modelfox_predict_options_delete` when you are done with it.
void modelfox_predict_options_new(const modelfox_predict_options **predict_options_ptr);

/// Delete a `modelfox_predict_options` value.
void modelfox_predict_options_delete(modelfox_predict_options *predict_options);

/// Set the classification threshold.
void modelfox_predict_options_set_threshold(modelfox_predict_options *predict_options,
                                            float threshold);

/// Enable or disable computing feature contributions.
void modelfox_predict_options_set_compute_feature_contributions(modelfox_predict_options *predict_options,
                                                                bool compute_feature_contributions);

/// Make a prediction! `model` should point to a model loaded with `modelfox_model_load`. `input` should be a `modelfox_predict_input` value and options should be a `modelfox_predict_options` value. On success, a pointer to a `modelfox_predict_output` output will be written to `output_ptr`. You must call `modelfox_predict_output_delete` when you are done with it.
modelfox_error *modelfox_model_predict(const modelfox_model *model,
                                       const modelfox_predict_input_vec *input,
                                       const modelfox_predict_options *options,
                                       const modelfox_predict_output_vec **output_ptr);

/// Delete a predict output.
void modelfox_predict_output_delete(modelfox_predict_output *predict_output);

/// Delete a predict output vec.
void modelfox_predict_output_vec_delete(modelfox_predict_output_vec *predict_output_vec);

/// Retrieve the len of the output vec.
void modelfox_predict_output_vec_len(const modelfox_predict_output_vec *predict_output_vec,
                                     size_t *len_ptr);

/// Get the predict output at `index`.
void modelfox_predict_output_vec_get_at_index(modelfox_predict_output_vec *predict_output_vec,
                                              size_t index,
                                              const modelfox_predict_output **predict_output_ptr);

/// Cast the predict output as `modelfox_regression_predict_output`. If this predict output is not for regression, null will be written to `regression_predict_output_ptr`.
void modelfox_predict_output_as_regression(const modelfox_predict_output *predict_output,
                                           const modelfox_regression_predict_output **predict_output_ptr);

/// Cast the predict output as `modelfox_binary_classification_predict_output`. If this predict output is not for binary classification, null will be written to `binary_classification_predict_output_ptr`.
void modelfox_predict_output_as_binary_classification(const modelfox_predict_output *predict_output,
                                                      const modelfox_binary_classification_predict_output **predict_output_ptr);

/// Cast the predict output as `modelfox_multiclass_classification_predict_output`. If this predict output is not for multiclass classification, null will be written to `multiclass_classification_predict_output_ptr`.
void modelfox_predict_output_as_multiclass_classification(const modelfox_predict_output *predict_output,
                                                          const modelfox_multiclass_classification_predict_output **predict_output_ptr);

/// Retrieve the value from a regression predict output.
void modelfox_regression_predict_output_get_value(const modelfox_regression_predict_output *predict_output,
                                                  float *output_value);

/// Retrieve the feature contributions from a regression predict output. If feature contributions were not computed for this prediction, null will be written to `feature_contributions_ptr`.
void modelfox_regression_predict_output_get_feature_contributions(const modelfox_regression_predict_output *predict_output,
                                                                  const modelfox_feature_contributions **feature_contributions_ptr);

/// Retrieve the class name from a binary classification predict output.
void modelfox_binary_classification_predict_output_get_class_name(const modelfox_binary_classification_predict_output *predict_output,
                                                                  modelfox_string_view *class_name_ptr);

/// Retrieve the probability from a binary classification predict output.
void modelfox_binary_classification_predict_output_get_probability(const modelfox_binary_classification_predict_output *predict_output,
                                                                   float *probability);

/// Retrieve the feature contributions from a binary classification predict output.
void modelfox_binary_classification_predict_output_get_feature_contributions(const modelfox_binary_classification_predict_output *predict_output,
                                                                             const modelfox_feature_contributions **feature_contributions_ptr);

/// Retrieve the class name from a multiclass classification predict output. If feature contributions were not computed for this prediction, null will be written to `feature_contributions_ptr`.
void modelfox_multiclass_classification_predict_output_get_class_name(const modelfox_multiclass_classification_predict_output *predict_output,
                                                                      modelfox_string_view *class_name_ptr);

/// Retrieve the probability from a multiclass classification predict output.
void modelfox_multiclass_classification_predict_output_get_probability(const modelfox_multiclass_classification_predict_output *predict_output,
                                                                       float *probability_ptr);

/// Retrieve the number of classes from a multiclass classification predict output.
void modelfox_multiclass_classification_predict_output_get_probabilities_len(const modelfox_multiclass_classification_predict_output *predict_output,
                                                                             size_t *len_ptr);

/// Delete a multiclass classification predict output probabilities iterator.
void modelfox_multiclass_classification_predict_output_probabilities_iter_delete(modelfox_multiclass_classification_predict_output_probabilities_iter *probabilities_iter);

/// Get an iterator over the probabilities for a multiclass classification predict output. You must call `modelfox_multiclass_classification_predict_output_probabilities_iter_delete` when you are done with it.
void modelfox_multiclass_classification_predict_output_get_probabilities_iter(const modelfox_multiclass_classification_predict_output *predict_output,
                                                                              const modelfox_multiclass_classification_predict_output_probabilities_iter **probabilities_iter_ptr);

/// Retrieve the next `(class_name, probability)` pair from the probabilties iterator. This function returns `true` if `class_name_ptr` and `probability_ptr` have been successfully set, or `false` if the iterator has reached its end.
bool modelfox_multiclass_classification_predict_output_probabilities_iter_next(modelfox_multiclass_classification_predict_output_probabilities_iter *probabilities_iter,
                                                                               modelfox_string_view *class_name_ptr,
                                                                               float *probability_ptr);

/// Delete a multiclass classification predict output feature contributions iterator.
void modelfox_multiclass_classification_predict_output_feature_contributions_iter_delete(modelfox_multiclass_classification_predict_output_feature_contributions_iter *feature_contributions_iter);

/// Retrieve the feature contributions from a multiclass classification predict output. If feature contributions were not computed for this prediction, null will be written to `feature_contributions_iter_ptr`.
void modelfox_multiclass_classification_predict_output_get_feature_contributions_iter(const modelfox_multiclass_classification_predict_output *predict_output,
                                                                                      const modelfox_multiclass_classification_predict_output_feature_contributions_iter **feature_contributions_iter_ptr);

/// Retrieve the next `(class_name, feature_contributions)` pair from the feature contributions iterator. This function returns `true` if `class_name_ptr` and `feature_contributions_ptr` have been successfully set, or `false` if the iterator has reached its end.
bool modelfox_multiclass_classification_predict_output_feature_contributions_iter_next(modelfox_multiclass_classification_predict_output_feature_contributions_iter *feature_contributions_iter,
                                                                                       modelfox_string_view *class_name_ptr,
                                                                                       const modelfox_feature_contributions **feature_contributions_ptr);

/// Retrieve the baseline value from feature contributions.
void modelfox_feature_contributions_get_baseline_value(const modelfox_feature_contributions *feature_contributions,
                                                       float *baseline_value_ptr);

/// Retrieve the output value from feature contributions.
void modelfox_feature_contributions_get_output_value(const modelfox_feature_contributions *feature_contributions,
                                                     float *output_value_ptr);

/// Retrieve the len of the feature contributions.
void modelfox_feature_contributions_get_entries_len(const modelfox_feature_contributions *feature_contributions,
                                                    size_t *len_ptr);

/// Retrieve the feature contribution at `index`.
void modelfox_feature_contributions_get_entry_at_index(const modelfox_feature_contributions *feature_contributions,
                                                       size_t index,
                                                       const modelfox_feature_contribution_entry **feature_contribution_entry_ptr);

/// Retrieve the type of the feature contribution entry.
void modelfox_feature_contribution_entry_get_type(const modelfox_feature_contribution_entry *feature_contribution_entry,
                                                  modelfox_feature_contribution_entry_type *type_ptr);

/// Cast the feature contribution entry as `modelfox_identity_feature_contribution`. If this feature contribution is not an identity feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void modelfox_feature_contribution_entry_as_identity(const modelfox_feature_contribution_entry *feature_contribution_entry,
                                                     const modelfox_identity_feature_contribution **feature_contribution_ptr);

/// Cast the feature contribution entry as `modelfox_normalized_feature_contribution`. If this feature contribution is not a normalized feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void modelfox_feature_contribution_entry_as_normalized(const modelfox_feature_contribution_entry *feature_contribution_entry,
                                                       const modelfox_normalized_feature_contribution **feature_contribution_ptr);

/// Cast the feature contribution entry as `modelfox_one_hot_encoded_feature_contribution`. If this feature contribution is not a one hot encoded feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void modelfox_feature_contribution_entry_as_one_hot_encoded(const modelfox_feature_contribution_entry *feature_contribution_entry,
                                                            const modelfox_one_hot_encoded_feature_contribution **feature_contribution_ptr);

/// Cast the feature contribution entry as `modelfox_bag_of_words_feature_contribution`. If this feature contribution is not a bag of words feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void modelfox_feature_contribution_entry_as_bag_of_words(const modelfox_feature_contribution_entry *feature_contribution_entry,
                                                         const modelfox_bag_of_words_feature_contribution **feature_contribution_ptr);

/// Cast the feature contribution entry as `modelfox_bag_of_words_cosine_similarity_feature_contribution`. If this feature contribution is not a bag of words cosine similarity feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void modelfox_feature_contribution_entry_as_bag_of_words_cosine_similarity(const modelfox_feature_contribution_entry *feature_contribution_entry,
                                                                           const modelfox_bag_of_words_cosine_similarity_feature_contribution **feature_contribution_ptr);

/// Cast the feature contribution entry as `modelfox_word_embedding_feature_contribution`. If this feature contribution is not a word embedding feature contribution, null will be written to `feature_contribution_ouput_ptr`.
void modelfox_feature_contribution_entry_as_word_embedding(const modelfox_feature_contribution_entry *feature_contribution_entry,
                                                           const modelfox_word_embedding_feature_contribution **feature_contribution_ptr);

/// Retrieve the column name.
void modelfox_identity_feature_contribution_get_column_name(const modelfox_identity_feature_contribution *feature_contribution,
                                                            modelfox_string_view *column_name_ptr);

/// Retrieve the feature contribution value.
void modelfox_identity_feature_contribution_get_feature_contribution_value(const modelfox_identity_feature_contribution *feature_contribution,
                                                                           float *feature_contribution_value);

/// Retrieve the feature value.
void modelfox_identity_feature_contribution_get_feature_value(const modelfox_identity_feature_contribution *feature_contribution,
                                                              float *feature_value);

/// Retrieve the column name.
void modelfox_normalized_feature_contribution_get_column_name(const modelfox_normalized_feature_contribution *feature_contribution,
                                                              modelfox_string_view *column_name_ptr);

/// Retrieve the feature value.
void modelfox_normalized_feature_contribution_get_feature_value(const modelfox_normalized_feature_contribution *feature_contribution,
                                                                float *feature_value);

/// Retrieve the feature contribution value.
void modelfox_normalized_feature_contribution_get_feature_contribution_value(const modelfox_normalized_feature_contribution *feature_contribution,
                                                                             float *feature_contribution_value);

/// Retrieve the column name.
void modelfox_one_hot_encoded_feature_contribution_get_column_name(const modelfox_one_hot_encoded_feature_contribution *feature_contribution,
                                                                   modelfox_string_view *column_name_ptr);

/// Retrieve the variant.
void modelfox_one_hot_encoded_feature_contribution_get_variant(const modelfox_one_hot_encoded_feature_contribution *feature_contribution,
                                                               modelfox_string_view *variant_ptr);

/// Retrieve the feature value.
void modelfox_one_hot_encoded_feature_contribution_get_feature_value(const modelfox_one_hot_encoded_feature_contribution *feature_contribution,
                                                                     bool *feature_value);

/// Retrieve the feature contribution value.
void modelfox_one_hot_encoded_feature_contribution_get_feature_contribution_value(const modelfox_one_hot_encoded_feature_contribution *feature_contribution,
                                                                                  float *feature_contribution_value);

/// Retrieve the column name.
void modelfox_bag_of_words_feature_contribution_get_column_name(const modelfox_bag_of_words_feature_contribution *feature_contribution,
                                                                modelfox_string_view *column_name_ptr);

/// Retrieve the ngram.
void modelfox_bag_of_words_feature_contribution_get_ngram(const modelfox_bag_of_words_feature_contribution *feature_contribution,
                                                          const modelfox_ngram **ngram_ptr);

/// Retrieve the ngram type.
void modelfox_ngram_get_type(const modelfox_ngram *ngram, modelfox_ngram_type *ngram_type);

/// Retrieve the unigram token.
void modelfox_unigram_get_token(const modelfox_ngram *ngram, modelfox_string_view *token_ptr);

/// Retrieve the bigram token a.
void modelfox_bigram_get_token_a(const modelfox_ngram *ngram, modelfox_string_view *token_ptr);

/// Retrieve the bigram token b.
void modelfox_bigram_get_token_b(const modelfox_ngram *ngram, modelfox_string_view *token_ptr);

/// Retrieve the feature value.
void modelfox_bag_of_words_feature_contribution_get_feature_value(const modelfox_bag_of_words_feature_contribution *feature_contribution,
                                                                  float *feature_value);

/// Retrieve the feature contribution value.
void modelfox_bag_of_words_feature_contribution_get_feature_contribution_value(const modelfox_bag_of_words_feature_contribution *feature_contribution,
                                                                               float *feature_contribution_value);

/// Retrieve the column name a.
void modelfox_bag_of_words_cosine_similarity_feature_contribution_get_column_name_a(const modelfox_bag_of_words_cosine_similarity_feature_contribution *feature_contribution,
                                                                                    modelfox_string_view *column_name_ptr);

/// Retrieve the column name b.
void modelfox_bag_of_words_cosine_similarity_feature_contribution_get_column_name_b(const modelfox_bag_of_words_cosine_similarity_feature_contribution *feature_contribution,
                                                                                    modelfox_string_view *column_name_ptr);

/// Retrieve the feature value.
void modelfox_bag_of_words_cosine_similarity_feature_contribution_get_feature_value(const modelfox_bag_of_words_cosine_similarity_feature_contribution *feature_contribution,
                                                                                    float *feature_value);

/// Retrieve the feature contribution value.
void modelfox_bag_of_words_cosine_similarity_feature_contribution_get_feature_contribution_value(const modelfox_bag_of_words_cosine_similarity_feature_contribution *feature_contribution,
                                                                                                 float *feature_contribution_value);

/// Retrieve the column name.
void modelfox_word_embedding_feature_contribution_get_column_name(const modelfox_word_embedding_feature_contribution *feature_contribution,
                                                                  modelfox_string_view *column_name_ptr);

/// Retrieve the value index.
void modelfox_word_embedding_feature_contribution_get_value_index(const modelfox_word_embedding_feature_contribution *feature_contribution,
                                                                  size_t *value_index);

/// Retrieve the feature contribution value.
void modelfox_word_embedding_feature_contribution_get_feature_contribution_value(const modelfox_word_embedding_feature_contribution *feature_contribution,
                                                                                 float *feature_contribution_value);

/// This function exposes the allocator used by libmodelfox. It is used by the wasm build of libmodelfox because WebAssembly does not include its own allocator.
void *modelfox_alloc(size_t size,
                     size_t align);

/// This function exposes the allocator used by libmodelfox. It is used by the wasm build of libmodelfox because WebAssembly does not include its own allocator.
void modelfox_dealloc(void *ptr,
                      size_t size,
                      size_t align);
