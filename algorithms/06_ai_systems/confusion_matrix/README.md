# Binary Classification Confusion Matrix

## Why it belongs in the AI hub

A confusion matrix is a basic **supervised classification evaluation**
primitive. It separates correct predictions from the two types of binary
classification error, making the tradeoffs behind accuracy, precision,
recall, specificity and other evaluation metrics explicit
(Hastie et al., 2009). It complements the repository's Logistic Regression
algorithm and the information-retrieval metrics.

## Mathematical definition

Given aligned ground-truth labels `actual` and predictions `predicted`,
both containing only 0 and 1, return four nonnegative counts in the
canonical order:

```text
(TN, FP, FN, TP)

             predicted 0    predicted 1
actual 0        TN              FP
actual 1        FN              TP
```

Each sample contributes **exactly one** to exactly one cell, so
`TN + FP + FN + TP = n`.

Example:

```text
actual    = [0, 0, 1, 1, 1]
predicted = [0, 1, 1, 0, 1]
result    = (1, 1, 1, 2)
```

## Shared contract

- Inputs: two **nonempty**, equal-length aligned label arrays.
  Valid labels are **integer** 0 or 1; probabilities and floating-point
  score thresholds are not accepted here.
- Output: `(TN, FP, FN, TP)` as nonnegative counts, following that
  exact order in every language. Python/R raise errors; Rust uses
  `Result<(usize, usize, usize, usize), ...>`; C returns `bool`.
- C receives `uint8_t` arrays of at least `n` elements and a
  caller-provided `size_t output[4]`. On error the output is not modified.
  Rust likewise receives byte slices; labels outside 0/1 are invalid.
  R returns an **integer** vector of four named counts
  `c(TN=..., FP=..., FN=..., TP=...)`.
- Inputs cannot contain NaN, infinity, missing values, nonintegers or
  booleans-as-numbers in languages accepting dynamic types.
- Even if some classes do not occur, all four matrix cells are returned,
  possibly with zeros; this is a **valid evaluation**, not a division by
  zero. Derived rates are intentionally out of scope.

## Complexity

**O(n)** time and **O(1)** auxiliary memory, excluding the input arrays
and the four result counters. All counts are bounded by the number of
observations, and require no temporary ML-library objects.

## Implementations

| Language | Directory |
| --- | --- |
| C | [confusion_matrix_c](confusion_matrix_c/) |
| Python | [confusion_matrix_python](confusion_matrix_python/) |
| Rust | [confusion_matrix_rust](confusion_matrix_rust/) |
| R | [confusion_matrix_r](confusion_matrix_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover mixed outcomes, perfect classifiers, only-negative samples,
invalid labels after already valid prefixes, empty arrays, unequal lengths
and rejection of dynamic boolean labels.

## Limitations

Binary labels only, no sample weights, score thresholding, multiclass
matrices, probabilistic calibration, class imbalances adjustments or
confidence intervals. This primitive does not define precision/recall
when their denominators are zero; a downstream metric must choose its
own policy. No CUDA acceleration is included or benchmarked.

## References

Hastie, T., Tibshirani, R., & Friedman, J. (2009). *The elements of
statistical learning: Data mining, inference, and prediction* (2nd ed.).
Springer. https://doi.org/10.1007/978-0-387-84858-7
