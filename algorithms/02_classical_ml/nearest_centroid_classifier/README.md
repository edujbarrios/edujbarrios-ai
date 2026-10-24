# Binary Nearest Centroid Classifier

## Why this algorithm belongs here

A **nearest-centroid classifier** represents each class by the mean
feature vector of its labeled training observations. It classifies a
new point according to its nearest class representative. This makes
a simple baseline for understanding supervised geometric learning,
compared with the catalogue's k-nearest neighbors and Gaussian
Naive Bayes (Hastie et al., 2009).

## Mathematics

Given `n` labeled vectors with `d` features and binary labels
`y_i∈{0,1}`, calculate the mean of each class:

```text
centroid[c,j] = (1 / N_c) * sum_{i:y_i=c} X[i,j]

distance[c] = sqrt(sum_j (query[j]-centroid[c,j])^2)
predicted = 1 if distance[1] < distance[0], else 0
```

The predictor uses iterative **hypot** accumulation for Euclidean
distance to avoid avoidable overflow from squared coordinate
differences. The exact-tie convention predicts class **0**.

Example:

```text
X=[[0,0],[2,0],[8,0],[10,0]]
y=[0,0,1,1]
centroid[0]=[1,0]
centroid[1]=[9,0]

predict([2,0])=0
predict([8,0])=1
predict([5,0])=0  # tie
```

## Shared contract

- Train on a rectangular `n×d` matrix of finite real coordinates
  with **2<=n<=4096** and **1<=d<=64**, together with exactly
  `n` integer labels 0 or 1. **Both classes must be present**.
- Return two d-dimensional class centroids in **class-major** order.
  Each trained centroid coordinate must be finite. Fit rejects
  accumulated feature sums outside the representable float range.
- Inference takes the two fitted centroids and one finite query of
  length d. Returns an integer label, choosing 0 for exact distance
  ties. All coordinates and intermediate differences/distances must
  be representable and finite.
- C uses flat **row-major** `features[n*d]`, `uint8_t labels[n]`
  and caller-owned `double centroids[2*d]` with no aliasing.
  The output array **remains unchanged on fit failure**.
  Prediction writes caller-owned `uint8_t* label` only on success.
- Rust also accepts flat input with explicit n,d and returns
  `Result<Vec<f64>,...>` for centroids, `Result<u8,...>` for
  prediction. Python uses nested rows and returns a pair of
  centroid lists. R uses a base numeric matrix and named list.
- These centroids are **training-only statistics**. Never fit the
  centroids on test samples when evaluating generalization.

## Complexity

Training: **O(n*d)** time, O(d) auxiliary memory beyond the centroid
output. Prediction: **O(d)** time, O(1) extra space.

## Implementations

| Language | Directory |
| --- | --- |
| C | [nearest_centroid_classifier_c](nearest_centroid_classifier_c/) |
| Python | [nearest_centroid_classifier_python](nearest_centroid_classifier_python/) |
| Rust | [nearest_centroid_classifier_rust](nearest_centroid_classifier_rust/) |
| R | [nearest_centroid_classifier_r](nearest_centroid_classifier_r/) |

Run:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover learned centers, nearest-neighbor predictions, exact
distance ties, invalid labels, missing class, malformed and nonfinite
data, and out-of-range coordinates.

## Limitations

Binary labels, arithmetic-mean centroids and Euclidean distance
only. No covariance normalization, regularization, shrinkage,
nonlinear boundaries, class weights, batch inference or device-tested
CUDA implementation. Mean centroids can be sensitive to outliers.

## Reference

Hastie, T., Tibshirani, R., & Friedman, J. (2009).
*The elements of statistical learning: Data mining, inference,
and prediction* (2nd ed.). Springer.
https://doi.org/10.1007/978-0-387-84858-7
