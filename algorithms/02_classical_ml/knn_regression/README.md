# K-Nearest Neighbors Regression

## Why it belongs in AI

**K-Nearest Neighbors (k-NN) regression** predicts a continuous
outcome using nearby labeled examples rather than fitting explicit
coefficients. The prediction is the **arithmetic mean of the target
values belonging to the k closest training samples**. It is a useful
baseline for nonparametric supervised learning and complements this
catalogue's k-NN classification algorithm (Hastie et al., 2009).

## Algorithm

Given n labeled training vectors `x_i∈R^d`, finite real regression
targets `y_i`, a query vector `q∈R^d`, and `1<=k<=n`:

```text
distance[i] = sqrt(sum_j (x_i[j] - q[j])^2)
neighbors = first k indices sorted by (distance[i], i)
prediction = (1/k) * sum_{i in neighbors} y_i
```

Exact distance ties choose the **smallest original zero-based row
index**. Repeated input vectors are separate training observations;
their targets are included independently if selected.

A **hypotenuse recurrence** `hypot(current,difference)` computes
Euclidean distance without unnecessary square overflow. Target
contributions are added as `y_i/k` instead of summing large y
values before division, preventing some avoidable target-sum overflow.

### Example

```text
query=[0,0]
train=[[0,0],[2,0],[0,2],[5,5]]
targets=[2,4,6,10]

k=1 -> 2        # nearest index 0
k=2 -> 3        # indices 0 and 1, tie with index 2
k=3 -> 4        # indices 0, 1 and 2
k=4 -> 5.5      # all four observations
```

## Shared contract

- Nonempty, rectangular training matrix `X[n,d]` with
  **1<=n<=4096**, **1<=d<=64**, finite real coordinates.
- A query vector with exactly d finite real coordinates and an
  aligned target vector of n finite real numbers. Negative targets,
  zero vectors and duplicate observations are valid.
- `k` must be an integer in `[1,n]`.
- Returns a **finite predicted real number**. All distances and
  target values are checked even if their rows would not rank in
  top-k. Reject nonfinite coordinate differences, distance norms,
  target contributions or accumulated prediction values.
- C uses **flat row-major** training vectors `X[n*d]`,
  `targets[n]`, `query[d]` and caller-owned `double* prediction`.
  Input and output memory must not overlap; the output is unchanged
  on error. Rust takes a row-major flat dataset and returns
  `Result<f64,...>`. Python uses nested arrays and returns float;
  R accepts a numeric matrix and returns a scalar numeric value.
- Python rejects bool inputs (including k), NaN and infinity.
  R requires integer-valued k and rejects missing/invalid numerics.
  The implementation uses finite double arithmetic, not arbitrary
  precision.
- This algorithm is **unweighted nearest-neighbor regression**.
  It does not choose neighbors based on the numeric target values.

## Complexity

Calculate n distances in **O(n*d)**, then sort them in
**O(n log n)** time. Uses **O(n)** additional storage (Python also
copies the candidate rows during validation).

## Implementations

| Language | Directory |
| --- | --- |
| C | [knn_regression_c](knn_regression_c/) |
| Python | [knn_regression_python](knn_regression_python/) |
| Rust | [knn_regression_rust](knn_regression_rust/) |
| R | [knn_regression_r](knn_regression_r/) |

Tests from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover tie-breaking, different k values, negative targets,
duplicate observations, singleton training sets, out-of-range k,
nonfinite inputs, distance overflow and invalid shapes.

## Limitations

No distance weights, feature normalization, training-derived scaling,
approximate vector index, cross-validation, missing data handling,
streaming updates or GPU benchmarks. k-NN can be sensitive to
feature scale, irrelevant dimensions and choice of k. Normalize
inputs using training-fitted statistics to avoid data leakage.

## References

Hastie, T., Tibshirani, R., & Friedman, J. (2009). *The elements of
statistical learning: Data mining, inference, and prediction*
(2nd ed.). Springer.
https://hastie.su.domains/ElemStatLearn/
