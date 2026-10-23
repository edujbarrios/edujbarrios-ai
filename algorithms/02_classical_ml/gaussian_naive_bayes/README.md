# Binary Gaussian Naive Bayes

## Why it belongs in the AI hub

Gaussian Naive Bayes is a supervised probabilistic classifier. It models
each feature using a class-conditional Gaussian, assumes features are
conditionally independent given the label, and uses Bayes' rule to classify
a query (Murphy, 2012). The simplifying independence assumption makes it
a useful interpretable baseline before more complex supervised models.

## Algorithm and mathematical definition

For `n` training samples of `d` numeric features and labels `0` or `1`:

1. Compute each class's sample count `n_c` and prior `P(c)=n_c/n`.
2. Compute mean `mu[c,j]` and **population** variance
   `variance[c,j] = sum((x[i,j]-mu[c,j])²)/n_c` for each feature and class.
3. Regularize zero/small variances with a fixed
   `variance[c,j] = max(variance[c,j], 1e-9)`.
4. For a query `q`, calculate two class scores:

```text
log_score[c] = ln(n_c/n) - 0.5 * sum(
    ln(2*pi*variance[c,j]) + (q[j] - mu[c,j])² / variance[c,j]
    for j=0..d-1
)
probability_1 = sigmoid(log_score[1] - log_score[0])
predicted_label = 1 if log_score[1] > log_score[0] else 0
```

The sigmoid is evaluated with a sign-aware branch. On **equal log scores**
the predicted class is **0**, including a probability of `0.5`.
This is one fitting-and-prediction operation, rather than a persistent
model object with incremental updates.

Example:

```text
samples = [[0,0], [0,1], [5,5], [5,6]]
labels  = [0, 0, 1, 1]
query   = [5, 5]
result  = label 1, probability_1 very close to 1
```

## Input and output contract

- Dense, nonempty, rectangular real matrix `n × d` with `n>=2`, `d>=1`.
- Exactly `n` binary labels, **both classes must be represented**,
  and one finite real query vector of dimension `d`.
- Return `(predicted_label, probability_1)`, with probability in `[0,1]`.
  Python/R raise errors; Rust returns a `Result`; C returns `bool`.
- C and Rust use flat row-major samples; C requires caller-allocated buffers,
  output pointers and correct buffer lengths. Output values are unchanged on
  validation or numeric errors. R uses a base numeric matrix and returns a
  named list containing the label and probability.
- NaN, infinity, shape mismatches, absent classes, invalid labels,
  non-finite intermediate sums, underflowed variance floor or overflowing
  Gaussian likelihood calculations are errors. Very large coordinates may
  be rejected even if their theoretical classification is well-defined.
- The variance floor is deliberately **fixed** (not automatically adapted
  to feature magnitude). Scale features appropriately before using this
  educational reference. The method does not calibrate probabilities.

## Complexity

For one fit and query: **O(nd)** time and **O(d)** auxiliary space for
means and variances of two classes; Python also copies the input matrix
(O(nd) auxiliary space). The output uses constant memory.

## Implementations

| Language | Directory |
| --- | --- |
| C | [gaussian_naive_bayes_c](gaussian_naive_bayes_c/) |
| Python | [gaussian_naive_bayes_python](gaussian_naive_bayes_python/) |
| Rust | [gaussian_naive_bayes_rust](gaussian_naive_bayes_rust/) |
| R | [gaussian_naive_bayes_r](gaussian_naive_bayes_r/) |

## Tests

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` from the repository root.
Tests cover separable multivariate classes, class-zero predictions,
variance flooring, exact score ties, single-class training rejection,
invalid shapes, bad labels, and non-finite values.

## Limitations

Only two classes, finite dense real features, a fixed variance floor, one
prediction per call and no persistent trained model. The naive conditional
independence assumption may be unrealistic; output posterior probabilities
are not necessarily calibrated. No GPU implementation or performance claim
is included.

## References

Murphy, K. P. (2012). *Machine learning: A probabilistic perspective*.
MIT Press.
