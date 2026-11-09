# Inverse-Distance-Weighted k-NN Regression

## Classical ML motivation

Weighted k-nearest-neighbors regression predicts a continuous target from nearby labeled training observations, giving **closer neighbors greater influence**. It extends the catalogue's unweighted `knn_regression` and provides a simple nonparametric baseline (Hastie et al., 2009).

## Algorithm

For a query `q`, `n` labeled training vectors `x[i]` with real targets `y[i]`, and `1 <= k <= n`:

1. Compute Euclidean distances `d[i]` by accumulating coordinate differences using `hypot`.
2. Select the first k rows in ascending **(distance, original zero-based row index)** order.
3. **If any selected distance is zero**, return the arithmetic mean of the targets for **selected zero-distance neighbors only**, ignoring selected nonzero-distance neighbors.
4. Otherwise let `dmin` be the smallest selected distance, and assign weights `w[i] = dmin/d[i]`. Return `sum(w[i]*y[i])/sum(w[i])`. This is algebraically the same as weights `1/d[i]`, but avoids overflow when distances are tiny.

This is deterministic, works for duplicate vectors and has a defined exact-match rule. Neighbor ties are resolved by the input row index, never by targets.

## Shared contract

- Data: a nonempty **n×d** training matrix with **1..128 rows**, **1..16 features**, each finite real coordinate in **[-1000,1000]**; a query of exactly d finite coordinates in the same range; and **n finite real targets in [-1000,1000]**.
- `k` is an integer in **[1,n]**. The output is one **finite real prediction**. Training rows and targets are not mutated; repeated rows are separate observations.
- C takes `const double *train` in flat row-major shape n×d, `const double *targets` length n, `const double *query` length d, dimensions `n,d,k` and a distinct caller-owned `double *prediction_out`. Returns `false` with output unchanged on invalid inputs.
- Python accepts nested list/tuple training rows plus targets and query vectors, returns `float`, and rejects booleans, malformed shapes, wrong types, NaN and infinity. Rust receives flat row-major training data and returns `Result<f64,&str>`. R accepts a numeric matrix of training rows and numeric vector for targets/query, returning one numeric scalar.
- This algorithm uses **Euclidean** distance and **inverse-distance** weighting; it is neither an RBF/kernel regression model nor an unweighted k-NN predictor.

## Complexity

Computing distances costs **O(nd)**; sorting neighbors costs **O(n log n)** and weighted aggregation costs O(k). Scratch memory is **O(n)** (plus copied Python input rows for validation).

## Examples

```text
train=[[0],[4]], targets=[0,20], query=[1], k=2
distances=[1,3], normalized weights=[1,1/3]
prediction=(0+20/3)/(1+1/3)=5

train=[[0],[0],[2]], targets=[3,7,20], query=[0], k=3
prediction=(3+7)/2=5  # zero-distance matches only

train=[[-1],[1]], targets=[2,8], query=[0], k=1
prediction=2          # exact distance tie chooses row 0
```

## Implementations and testing

| Language | Directory |
| --- | --- |
| C | [weighted_knn_regression_c](weighted_knn_regression_c/) |
| Python | [weighted_knn_regression_python](weighted_knn_regression_python/) |
| Rust | [weighted_knn_regression_rust](weighted_knn_regression_rust/) |
| R | [weighted_knn_regression_r](weighted_knn_regression_r/) |

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` at the repository root. Tests cover neighbor ties, k values, duplicate/zero-distance rows, asymmetric distances, negative targets, singleton and 128-row data, invalid arrays and bounds.

## Limitations

Distance weighting can magnify feature-scale effects and noise. This is an exact brute-force reference, with no index acceleration, normalization, cross-validation, learned metric or missing-value handling. No distance cutoff or prediction confidence interval is provided.

## References

Hastie, T., Tibshirani, R., & Friedman, J. (2009). *The elements of statistical learning: Data mining, inference, and prediction* (2nd ed.). Springer. https://hastie.su.domains/ElemStatLearn/
