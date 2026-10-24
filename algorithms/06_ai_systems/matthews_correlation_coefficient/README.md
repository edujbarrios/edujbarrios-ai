# Binary Matthews Correlation Coefficient (MCC)

## Motivation

**Matthews Correlation Coefficient** summarizes binary classification
performance across all four confusion-matrix cells. Unlike raw
accuracy, it accounts for positive and negative outcomes, making it
particularly informative for class-imbalanced data (Chicco & Jurman,
2020). It complements the catalogue's confusion matrix, F1, Brier
score, ROC AUC and calibration metrics.

## Formula

For binary truth and prediction arrays, count:

```text
TP = truth=1,pred=1    TN = truth=0,pred=0
FP = truth=0,pred=1    FN = truth=1,pred=0

MCC = (TP*TN - FP*FN) /
      sqrt((TP+FP)*(TP+FN)*(TN+FP)*(TN+FN))
```

The score is in **[-1,1]**: 1 means perfect agreement, -1 perfect
inverse prediction, and 0 no linear association. If any denominator
factor is zero, define **MCC=0** as a deterministic convention.

To avoid overflow in intermediate count products, compute the
numerator in scaled double precision, dividing it by `n²` before
normalizing each denominator factor by `n`:

```text
p11=TP/n; p00=TN/n; p10=FP/n; p01=FN/n
MCC = (p11*p00 - p10*p01) /
  sqrt((p11+p10)*(p11+p01)*(p00+p10)*(p00+p01))
```

The two formulas are mathematically equivalent.

Example:

```text
truth = [1,1,0,0]
pred  = [1,0,1,0]

TP=TN=FP=FN=1  -> MCC=0
```

## Shared contract

- Both arrays are aligned, nonempty and of equal length.
- Each element is the **integer 0 or 1**, never a probability.
- Return finite MCC in `[-1,1]`. Undefined denominators return 0
  rather than NaN.
- Reject missing arrays, invalid class values or different lengths.
  C accepts `uint8_t truth[n],predictions[n]`, with caller-owned
  `double* result`, unchanged on error. Rust returns
  `Result<f64,...>`; Python/R raise errors.
- Python disallows bool labels. R accepts finite numeric 0 and 1
  but not fractional or missing values.
- All four implementations use the same scaled-count formula and
  no model-dependent threshold.

## Complexity

O(n) time and **O(1)** auxiliary space. No ML library needed.

## Implementations

| Language | Directory |
| --- | --- |
| C | [matthews_correlation_coefficient_c](matthews_correlation_coefficient_c/) |
| Python | [matthews_correlation_coefficient_python](matthews_correlation_coefficient_python/) |
| Rust | [matthews_correlation_coefficient_rust](matthews_correlation_coefficient_rust/) |
| R | [matthews_correlation_coefficient_r](matthews_correlation_coefficient_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover agreement, inverse predictions, uncorrelated predictions,
imbalanced confusion matrices, a degenerate denominator, and malformed
labels or vector lengths.

## Limitations

Binary labels only; no sample weights, multiclass extension,
probability optimization or uncertainty intervals. MCC does not
explain error costs or fairness properties across groups. There is
no GPU acceleration claim.

## Reference

Chicco, D., & Jurman, G. (2020). The advantages of the Matthews
correlation coefficient (MCC) over F1 score and accuracy in binary
classification evaluation. *BMC Genomics, 21*, Article 6.
https://doi.org/10.1186/s12864-019-6413-7
