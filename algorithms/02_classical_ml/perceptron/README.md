# Online Binary Perceptron Learning

## Why it belongs in the AI hub

The perceptron is a foundational supervised linear classifier. It makes
**mistake-driven updates** to a weight vector and bias while traversing
labeled observations sequentially. Its training rule connects linear
classification, neural-network history, and online learning
(Rosenblatt, 1958).

This example uses multiple numeric features but only **two classes**. It
implements the original online update rule rather than logistic regression
or gradient descent on a probabilistic loss.

## Mathematical definition

For row-major training features `X[n,d]`, binary labels `y[i]`
in `{0,1}`, positive learning rate `eta`, and a positive integer
number of epochs:

```text
Initialize weight[d] = zeros, bias = 0.

For each epoch and each row i in original input order:
    score = dot(weight, X[i]) + bias
    predicted = 1 if score > 0 else 0
    error = y[i] - predicted   # -1, 0, or +1
    if error != 0:
        weight[j] += eta * error * X[i,j]   for every feature j
        bias += eta * error

Return (weight, bias).
```

The tie convention is explicit: **score exactly zero predicts 0**.
No random initialization or shuffling is used. Inference applies this
same strict-positive decision rule.

For `X=[[-2,-1],[-1,-1],[1,1],[2,1]]` and
`y=[0,0,1,1]`, one epoch with `eta=1` yields
`weight=[1,1]` and `bias=1`, separating the training data.

## Shared contract

- A nonempty `n × d` matrix, where both `n >= 1` and `d >= 1`,
  containing finite real values. Exactly `n` integer binary labels,
  with either one or both classes permitted.
- `learning_rate` must be finite and **strictly positive**;
  `epochs` must be a positive integer.
- The output consists of `d` finite weights and one finite bias.
  Training performs **exactly** the specified number of epochs.
- Malformed dimensions, non-binary labels, NaN/Inf and overflowing
  scores or updates are errors. Nonseparable data may cause continuous
  mistakes; there is **no convergence guarantee** in that setting.
- C accepts a flat row-major `X[n*d]`, `uint8_t labels[n]`,
  `weights_out[d]` and `bias_out` buffers. Output remains unchanged
  on failure; no input/output aliasing is supported. Rust accepts a
  flat row-major matrix and returns `Result<(Vec<f64>, f64), ...>`.
  Python takes nested rows and returns `(weights, bias)`. R accepts
  a base numeric matrix and returns `list(weights,bias)`.
- Prediction is a separate helper taking trained parameters and a
  finite feature row, returning an integer 0 or 1. It does not refit.
- Floating-point summation follows feature order. This is a **reference
  algorithm**, not a benchmark.

## Complexity

Training runs in **O(epochs × n × d)** time and requires O(d) auxiliary
memory for weights (plus O(n*d) Python validation copies).
Inference takes O(d) time and O(1) extra memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [perceptron_c](perceptron_c/) |
| Python | [perceptron_python](perceptron_python/) |
| Rust | [perceptron_rust](perceptron_rust/) |
| R | [perceptron_r](perceptron_r/) |

## Examples and tests

```text
train([[-2,-1],[-1,-1],[1,1],[2,1]], [0,0,1,1], eta=1, epochs=1)
    -> weights=[1,1], bias=1
predict([2,1]) -> 1
predict([-2,-1]) -> 0
train([[0,0]], [1], eta=1, epochs=1) -> weights=[0,0], bias=1
```

From the repository root run:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the one-pass update, score-zero decision, bias-only update,
multiple epochs, nonfinite inputs, invalid hyperparameters, mismatched
shapes, invalid binary labels and overflow.

## Limitations

Linear binary classification only. No kernel trick, multiclass schemes,
pocket algorithm, averaged perceptron, sample weighting, regularization
or automatic convergence detection. The perceptron convergence theorem
requires linearly separable data under suitable assumptions. Data order
matters; do not interpret output as a calibrated probability. CUDA is not
claimed without a device-tested parallel algorithm.

## References

Rosenblatt, F. (1958). The perceptron: A probabilistic model for
information storage and organization in the brain. *Psychological
Review, 65*(6), 386–408. https://doi.org/10.1037/h0042519
