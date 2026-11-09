# Multiclass Macro-F1 Score

## AI systems context

**Macro-F1** measures multiclass classification quality by averaging F1 scores across classes with equal weight, even when some classes are rare. It complements the repository's binary F1 implementation and exposes poor minority-class performance that accuracy alone can obscure (Sokolova & Lapalme, 2009).

## Mathematical definition

For each class `c` among the **declared** classes `0..K-1`, count its true positives `TP[c]`, predicted occurrences `P[c]`, and actual occurrences `A[c]`. Its one-vs-rest F1 is:

```text
F1[c] = 2*TP[c] / (A[c] + P[c])    if A[c]+P[c] > 0
F1[c] = 0                           otherwise

macro_F1 = (1/K) * sum_{c=0..K-1} F1[c]
```

The zero-denominator rule is **zero**, including for classes missing from both truth and predictions. Thus adding an unused class decreases the macro score. This is a documented convention, not an automatic class exclusion. For `K=2`, macro-F1 averages *both* class-specific F1 scores; it does not equal the positive-class-only binary F1.

## Contract

- `actual` and `predicted`: aligned nonempty flat arrays of **1..512 integer class labels**. `classes`: **integer 2..16**.
- Every label must be within **0..classes-1**. Booleans, nonintegral values, NaNs, infinities, and mismatched lengths are invalid.
- Result is a finite floating-point scalar between 0 and 1. No inputs are changed.
- C: `bool macro_f1_score(const uint8_t *actual, const uint8_t *predicted, size_t n, size_t classes, double *score_out)`; invalid input returns `false` and leaves the output untouched.
- Python takes list/tuple sequences and returns `float`. Rust returns `Result<f64,&str>`; R accepts flat numeric integer-valued vectors and returns one numeric scalar.
- Labels are **hard class assignments**, not probabilities; there is no thresholding, class weighting, or ignored-label option.

## Complexity

One pass through `n` observations, then through `K` classes: **O(n+K)** time and **O(K)** extra storage.

## Examples

```text
actual=[0,1,2,2], predicted=[0,2,1,2], classes=3
class F1=[1,0,0.5]; macro_F1=0.5

actual=[0,0], predicted=[0,0], classes=3
class F1=[1,0,0]; macro_F1=1/3

actual=[0,1,2], predicted=[0,1,2], classes=3
macro_F1=1
```

## Implementations and tests

| Language | Reference |
| --- | --- |
| C | [macro_f1_score_c](macro_f1_score_c/) |
| Python | [macro_f1_score_python](macro_f1_score_python/) |
| Rust | [macro_f1_score_rust](macro_f1_score_rust/) |
| R | [macro_f1_score_r](macro_f1_score_r/) |

From repository root, run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`. Tests cover three-class confusion, absent classes, perfect predictions, all-wrong predictions, class bounds, 512 examples, wrong types and mismatched arrays.

## Limitations

Macro averaging gives each declared class the same contribution regardless of support. The zero-for-absent convention is not universal; specify it when comparing scores across libraries. This metric does not reveal probability calibration or per-class precision/recall.

## References

Sokolova, M., & Lapalme, G. (2009). A systematic analysis of performance measures for classification tasks. *Information Processing & Management, 45*(4), 427–437. https://doi.org/10.1016/j.ipm.2009.03.002
