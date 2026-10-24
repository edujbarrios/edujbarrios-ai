# Two-Layer Neural Binary Classifier

## Motivation

A neural network is more than isolated activations and matrix products:
its weights must be fitted by **backpropagating a loss gradient**. This
reference shows a complete, deterministic training loop for the smallest
nonlinear supervised network with two trainable layers: a single ReLU hidden
neuron feeding a sigmoid output neuron. It connects the repository's
dense-forward, sigmoid, binary cross-entropy and backpropagation examples
without a machine-learning library (Goodfellow et al., 2016).

This is explicitly a **one-feature, one-hidden-neuron** educational network,
not an arbitrary-depth or multi-feature trainer.

## Architecture and equations

Given scalar feature `x`, trainable parameters `(w1,b1,w2,b2)`,
and label `y ∈ {0,1}`:

```text
pre_hidden = w1*x + b1
hidden     = max(0, pre_hidden)
logit      = w2*hidden + b2
prediction = sigmoid(logit)

loss = -y*ln(prediction) - (1-y)*ln(1-prediction)

delta_out = prediction - y
dL/dw2 = delta_out * hidden
dL/db2 = delta_out
delta_hidden = delta_out*w2 if pre_hidden > 0 else 0
dL/dw1 = delta_hidden*x
dL/db1 = delta_hidden
```

At **pre_hidden=0**, the ReLU subgradient is **defined as zero**.
The `prediction-y` output delta is the simplified derivative of
sigmoid plus binary cross-entropy, avoiding unstable `ln(0)` calculations.

Full-batch descent accumulates these four gradients across all `n`
training samples, then subtracts `learning_rate * gradient / n`
from each parameter **simultaneously once per epoch**.

## Deterministic initialization

```text
(w1, b1, w2, b2) = (1.0, 0.5, 1.0, 0.0)
```

The initial positive hidden bias enables gradient propagation for the
typical small nonnegative example, without stochastic initialization.
Training processes samples in original order and performs **exactly**
the requested number of epochs.

The API returns the four fitted parameters in the order
`(w1,b1,w2,b2)`; a separate prediction function returns
`sigmoid(w2*ReLU(w1*x+b1)+b2)` for a scalar query.

For `x=[0,1,2,3]`, `y=[0,0,1,1]`, learning rate `0.1`
and `500` epochs, the fitted model yields a low probability at
`x=0` and a high probability at `x=3`.

## Input/output contract

- Training requires **equal-length nonempty vectors** of finite scalar
  features and exactly binary labels (0 or 1).
- The learning rate is finite and **strictly positive**. `epochs` is
  a positive integer; no early stopping or automatic scheduling is used.
- Return four finite parameters, or an error on invalid input and
  **nonfinite intermediates** (hidden preactivation, output logit,
  gradients, accumulation, learning step or updated weights).
- Python and R raise errors; Rust returns `Result<[f64; 4], ...>`.
  C returns `bool` and writes caller-owned `parameters[4]` only if
  training succeeds. C callers must provide vectors of at least `n`
  elements and disjoint input/output buffers.
- Inference uses the **same** stable piecewise sigmoid in every language
  and rejects nonfinite inputs/parameters and nonfinite arithmetic.
- Even with valid inputs, a large learning rate may diverge. No claim of
  convergence, optimal accuracy or generalization is made. ReLU can
  permanently deactivate the hidden neuron on some data.

## Complexity

Training takes **O(epochs × n)** time and O(1) auxiliary memory in
C/Rust/R. Python validates and copies the paired training vectors,
requiring O(n) auxiliary memory. Scalar inference takes O(1) time/space.

## Implementations

| Language | Folder |
| --- | --- |
| C | [two_layer_binary_classifier_c](two_layer_binary_classifier_c/) |
| Python | [two_layer_binary_classifier_python](two_layer_binary_classifier_python/) |
| Rust | [two_layer_binary_classifier_rust](two_layer_binary_classifier_rust/) |
| R | [two_layer_binary_classifier_r](two_layer_binary_classifier_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover exact one-step gradient updates, training separation,
inference probabilities, inactive-ReLU gradients, invalid labels,
empty/mismatched input, nonfinite values, zero epochs and gradient
overflow. CUDA is not claimed without tested GPU training code.

## Limitations

Single real input feature, one ReLU hidden neuron and one sigmoid output.
No vectorized training, multiple hidden units, multi-class learning,
optimizer variants, regularization, normalization, shuffling,
cross-validation, saved model format or autograd framework. A single
hidden neuron cannot model XOR; this is a minimal complete training
example rather than a production neural network.

## References

Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*.
MIT Press. https://www.deeplearningbook.org/
