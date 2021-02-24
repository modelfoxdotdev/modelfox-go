package tangram

// #cgo linux CFLAGS: -I${SRCDIR}/libtangram/x86_64-unknown-linux-gnu
// #cgo linux LDFLAGS: -L${SRCDIR}/libtangram/x86_64-unknown-linux-gnu -ltangram -ldl -lm
// #cgo darwin CFLAGS: -I${SRCDIR}/libtangram/x86_64-apple-darwin
// #cgo darwin LDFLAGS: -L${SRCDIR}/libtangram/x86_64-apple-darwin -ltangram
// #cgo windows CFLAGS: -I${SRCDIR}/libtangram/x86_64-pc-windows-gnu
// #cgo windows LDFLAGS: -L${SRCDIR}/libtangram/x86_64-pc-windows-gnu -ltangram -luserenv -lws2_32
// #include "tangram.h"
import "C"

import (
	"bytes"
	"encoding/json"
	"errors"
	"io/ioutil"
	"log"
	"net/http"
	"unsafe"
)

// A Model represents a model loaded from a .tangram file.
type Model struct {
	modelPtr *C.tangram_model
	options  *ModelOptions
	logQueue []event
}

// ModelOptions is the second argument to the load function. It is used to configure the URL where the tangram reporting and monitoring web app is running.
type ModelOptions struct {
	TangramURL string
}

// PredictOptions is the second argument to the predict function. It is used to configure the threshold for classification tasks.
type PredictOptions struct {
	Threshold float32 `json:"threshold"`
}

// Input is the input to a model for making predictions.
type Input map[string]interface{}

// Output is the output of calling Predict
type Output struct {
	Value         float32            `json:"value"`
	ClassName     string             `json:"className"`
	Probabilities map[string]float32 `json:"probabilities"`
}

// LogPredictionOptions should be passed to the LogPrediction function.
type LogPredictionOptions struct {
	Identifier string
	Input      Input
	Options    PredictOptions
	Output     Output
}

// LogTrueValueOptions should be passed to the LogTrueValue function.
type LogTrueValueOptions struct {
	Identifier string
	TrueValue  interface{}
}

type event map[string]interface{}

// LoadModel loads a model from a byte slice.
func LoadModel(modelData []byte, options *ModelOptions) (*Model, error) {
	cModelData := C.CBytes(modelData)
	cModelDataLen := C.size_t(len(modelData))
	var modelPtr *C.tangram_model
	result := C.tangram_model_load(cModelData, cModelDataLen, (*unsafe.Pointer)(unsafe.Pointer(&modelPtr)))
	if result != 0 {
		return nil, errors.New("tangram error")
	}
	logQueue := []event{}
	model := Model{
		modelPtr,
		options,
		logQueue,
	}
	return &model, nil
}

// LoadModelFromFile loads a model from the specified path.
func LoadModelFromFile(path string, options *ModelOptions) (*Model, error) {
	modelData, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}
	return LoadModel(modelData, options)
}

// Destroy frees up the memory used by the model. You should call this with defer after loading your model.
func (m Model) Destroy() {
	result := C.tangram_model_free(unsafe.Pointer(m.modelPtr))
	if result != 0 {
		log.Fatal("tangram error")
	}
}

// ID is the unique identifier for this model.
func (m Model) ID() string {
	var cID *C.char
	result := C.tangram_model_id(unsafe.Pointer(m.modelPtr), &cID)
	if result != 0 {
		log.Fatal("tangram error")
	}
	id := C.GoString(cID)
	C.tangram_string_free(cID)
	return id
}

// PredictOne makes a prediction a single input.
func (m Model) PredictOne(input Input, options *PredictOptions) Output {
	return m.Predict([]Input{input}, options)[0]
}

// Predict makes predictions with a slice of inputs.
func (m Model) Predict(input []Input, options *PredictOptions) []Output {
	inputStr, err := json.Marshal(input)
	if err != nil {
		log.Fatal(err)
	}
	cInputStr := C.CString(string(inputStr))
	var cOptionsStr *C.char
	if options != nil {
		optionsStr, err := json.Marshal(options)
		if err != nil {
			log.Fatal(err)
		}
		cOptionsStr = C.CString(string(optionsStr))
	}
	var cOutputStr *C.char
	result := C.tangram_model_predict(unsafe.Pointer(m.modelPtr), cInputStr, cOptionsStr, &cOutputStr)
	if result != 0 {
		log.Fatal("tangram error")
	}
	outputStr := C.GoString(cOutputStr)
	result = C.tangram_string_free(cOutputStr)
	if result != 0 {
		log.Fatal("tangram error")
	}
	var output []Output
	json.Unmarshal([]byte(outputStr), &output)
	return output
}

// LogPrediction immediately sends a prediction monitoring event.
func (m Model) LogPrediction(options LogPredictionOptions) error {
	return m.logEvent(m.predictionEvent(options))
}

// LogTrueValue immediately sends a true value monitoring event.
func (m Model) LogTrueValue(options LogTrueValueOptions) error {
	return m.logEvent(m.trueValueEvent(options))
}

// EnqueueLogPrediction queues a prediction monitoring event to be sent later.
func (m Model) EnqueueLogPrediction(options LogPredictionOptions) {
	m.logQueue = append(m.logQueue, m.predictionEvent(options))
}

// EnqueueLogTrueValue queues a true value monitoring event to be sent later.
func (m Model) EnqueueLogTrueValue(options LogTrueValueOptions) {
	m.logQueue = append(m.logQueue, m.trueValueEvent(options))
}

// FlushLogQueue sends a network request to the Tangram API with any queued monitoring events.
func (m Model) FlushLogQueue() error {
	err := m.logEvents(m.logQueue)
	if err != nil {
		return err
	}
	m.logQueue = []event{}
	return nil
}

func (m Model) logEvent(e event) error {
	return m.logEvents([]event{e})
}

func (m Model) logEvents(events []event) error {
	body, err := json.Marshal(events)
	if err != nil {
		return err
	}
	request, err := http.NewRequest(
		"POST",
		m.options.TangramURL+"/track",
		bytes.NewReader(body),
	)
	if err != nil {
		return err
	}
	request.Header.Set("Content-Type", "application/json")
	response, err := http.DefaultClient.Do(request)
	if err != nil {
		return err
	}
	defer response.Body.Close()
	if response.StatusCode < 200 || response.StatusCode > 299 {
		body, err := ioutil.ReadAll(response.Body)
		if err != nil {
			return err
		}
		return errors.New(string(body))
	}
	return nil
}

func (m Model) predictionEvent(options LogPredictionOptions) event {
	var output map[string]interface{}
	if options.Output.ClassName != "" {
		output = map[string]interface{}{
			"className": options.Output.ClassName,
		}
	} else {
		output = map[string]interface{}{
			"value": options.Output.Value,
		}
	}
	return event{
		"type":       "prediction",
		"modelId":    m.ID(),
		"identifier": options.Identifier,
		"options":    options.Options,
		"input":      options.Input,
		"output":     output,
	}
}

func (m Model) trueValueEvent(options LogTrueValueOptions) event {
	return event{
		"type":       "true_value",
		"modelId":    m.ID(),
		"identifier": options.Identifier,
		"trueValue":  options.TrueValue,
	}
}
