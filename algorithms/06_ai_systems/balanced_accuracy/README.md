# Binary Balanced Accuracy

## Why it matters

Raw classification accuracy can be dominated by the majority class when examples are imbalanced. **Balanced accuracy** gives the positive and negative classes equal weight by averaging their recalls. It complements this catalogue's F1, MCC and confusion-matrix metrics; it does not measure probability calibration.

## Definition and example

For aligned binary truth and predicted labels:

```text
TPR = TP / (TP + FN)      # positive-class recall
TNR = TN / (TN + FP)      # negative-class recall
balanced_accuracy = (TPR + TNR) / 2

truth       = [0, 0, 0, 1, 1]
predictions = [0, 0, 1, 1, 0]
TN=2, FP=1, TP=1, FN=1
balanced_accuracy = (2/3 + 1/2) / 2 = 7/12
```

Values range from **0** (both classes misclassified) to **1** (perfect predictions). A constant-class predictor scores **0.5** when both classes occur in the truth.

## Shared contract

- Input vectors must be **nonempty**, aligned and of equal length; labels are binary integers 0 or 1, **not probabilities**.
- Both classes must appear in the ground truth. If either class is absent, balanced accuracy is **undefined under this contract**, so all implementations reject the input rather than inventing a convention.
- C receives `uint8_t truth[n]`, `uint8_t predicted[n]`, and `double *out`; invalid inputs leave `*out` unchanged. Inputs and output must not alias.
- Rust returns `Result<f64, &'static str>`. Python rejects bools and non-integer labels. R accepts finite numeric values precisely equal to 0 or 1 but not missing/fractional values.
- Unweighted binary metric only; no confidence thresholds are applied.

## Complexity

**O(n)** time and **O(1)** auxiliary space in all four implementations.

## Implementations

| Language | Code and tests |
| --- | --- |
| C | [balanced_accuracy_c](balanced_accuracy_c/) |
| Python | [balanced_accuracy_python](balanced_accuracy_python/) |
| Rust | [balanced_accuracy_rust](balanced_accuracy_rust/) |
| R | [balanced_accuracy_r](balanced_accuracy_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover unequal class support, perfect and fully inverted predictions, constant predictions, missing classes, empty/mismatched vectors and invalid labels.

## Limitations

The contract is binary, unweighted and requires both classes in ground truth. It does not provide per-group confidence intervals, costs of different mistakes, multiclass handling or calibration analysis. No GPU acceleration is claimed.

## References

Scikit-learn developers. (n.d.). *sklearn.metrics.balanced_accuracy_score*. Scikit-learn documentation. https://scikit-learn.org/stable/modules/generated/sklearn.metrics.balanced_accuracy_score.html
