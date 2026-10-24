# Column-wise Z-Score Standardization

## Why it belongs in AI

Machine-learning algorithms that operate on distances, dot products,
or gradient-based optimization can be sensitive to numerical feature
scales. Z-score **standardization** transforms each feature column
using its training-data mean and standard deviation. Feature scaling
is part of practical statistical learning and preprocessing
(Bishop, 2006). This reference computes and returns the statistics
rather than automatically scaling separate test data.

## Mathematics

For an `n × d` data matrix `X`, calculate each feature's population
mean and variance:

```text
mu[j]    = sum_i X[i,j] / n
var[j]   = sum_i (X[i,j] - mu[j])^2 / n
sigma[j] = sqrt(var[j])

Z[i,j] = (X[i,j] - mu[j]) / sigma[j]     if sigma[j] > 0
Z[i,j] = 0                              if sigma[j] == 0
```

Mean and variance use **Welford's one-pass recurrence** to avoid the
catastrophic cancellation of computing `E[X²]-E[X]²`.
This algorithm uses the **population normalization by n**, not the
sample variance normalization by n−1.

For constant columns (`sigma=0`), all standardized entries are
defined to be zero. This avoids division by zero and makes the
output useful in downstream numerical workflows.

## Example

```text
X = [[1,10],
     [2,10],
     [3,10]]

means = [2,10]
stds  = [sqrt(2/3),0]
Z     = [[-sqrt(3/2),0],
         [0,0],
         [sqrt(3/2),0]]
```

A single observation is valid: all standard deviations are zero and
the standardized row is all zeros.

## Shared contract

- Accept an `n × d` finite-real matrix with **n>=1, d>=1**.
- Return three objects: means of length d; standard deviations of
  length d; standardized matrix of shape n×d in original row order.
- Reject inconsistent dimensions, empty matrices, NaN/infinity and
  overflowing mean/covariance/normalization intermediate values.
- C and Rust consume flat row-major input. C writes means[d],
  standard deviations[d], and output[n*d] into caller-owned disjoint
  buffers **only after full validation and computation**. Rust returns
  `Result<(Vec<f64>,Vec<f64>,Vec<f64>),...>`.
  Python returns `(means, stds, rows)`; R returns a named list with a
  native matrix in `standardized`.
- Floating-point results are approximate. Extremely tiny variances
  can underflow to zero; extremely large differences can overflow
  even if all original inputs are finite, and are reported as errors.
- The caller should **fit on training data only**, then reuse those
  statistics to transform validation/test data without leakage.
  This reference does not implement the held-out transform itself.

## Complexity

O(n*d) time. Output occupies O(n*d) space. C uses O(n*d+d)
scratch to preserve outputs on errors; Rust/Python/R also allocate
the standardized matrix and O(d) moments.

## Implementations

| Language | Folder |
| --- | --- |
| C | [z_score_standardization_c](z_score_standardization_c/) |
| Python | [z_score_standardization_python](z_score_standardization_python/) |
| Rust | [z_score_standardization_rust](z_score_standardization_rust/) |
| R | [z_score_standardization_r](z_score_standardization_r/) |

## Tests

From repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover a mixed variable/constant matrix, singleton data, offset
means, NaN/infinity, irregular rows, and finite-input arithmetic
overflow. Comparisons allow small floating-point tolerance.

## Limitations

No robust scaling, clipping, sample weighting, sparse matrices,
missing-data imputation, incremental state storage, cross-validation
pipeline, or optimizations for thousands of dimensions. Even Welford
updates can overflow or lose precision on adversarial finite data.
No CUDA speedup is asserted without device tests and benchmarks.

## References

Bishop, C. M. (2006). *Pattern recognition and machine learning*.
Springer.
