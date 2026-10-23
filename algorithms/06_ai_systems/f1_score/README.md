# Binary F1 Score

## Why it belongs in the AI hub

An F1 score evaluates the balance between **precision and recall** for
binary classification. It is especially useful alongside the repository's
Logistic Regression, Gaussian Naive Bayes and Confusion Matrix algorithms,
which produce predictions or count classification outcomes.
Different evaluation measures emphasize different error tradeoffs
(Sokolova & Lapalme, 2009).

## Definition

For aligned binary `actual` and `predicted` labels, count:

```text
TP = number of (actual=1, predicted=1)
FP = number of (actual=0, predicted=1)
FN = number of (actual=1, predicted=0)

F1 = 2 * TP / (2 * TP + FP + FN)
```

If `2*TP + FP + FN == 0`, **return 0** by explicit convention rather
than treating undefined precision/recall as perfect.

Example:

```text
actual    = [0, 0, 1, 1, 1]
predicted = [0, 1, 1, 0, 1]
TP=2, FP=1, FN=1
F1=4/6 = 2/3
```

## Shared contract

- Two **nonempty**, equal-length arrays of integer labels strictly
  `0` or `1`. No probability thresholding is performed.
- Validate **every label**, including after correct prefixes. Invalid
  labels, booleans-as-numbers in dynamic languages, missing values,
  mismatched lengths or empty input are errors.
- Output is a finite scalar in `[0,1]`, including 0 for the
  zero-denominator case (all actual and predicted labels equal zero).
  A perfect all-positive classifier yields 1.
- C requires caller-owned `uint8_t` arrays and a non-null output pointer,
  returns `bool`, and leaves output unchanged on error. Rust uses
  `Result<f64, ...>`; Python and R raise errors. R accepts finite numeric
  vectors with elements exactly equal to 0 or 1.
- F1 treats false positives and false negatives symmetrically; it is
  **not accuracy** and is sensitive to the evaluation dataset's class mix.
- We calculate the numerator/denominator using floating-point conversions
  to avoid overflow of `2 * TP` in fixed-width integer arithmetic.

## Complexity

**O(n)** time and **O(1)** auxiliary space.

## Implementations

| Language | Folder |
| --- | --- |
| C | [f1_score_c](f1_score_c/) |
| Python | [f1_score_python](f1_score_python/) |
| Rust | [f1_score_rust](f1_score_rust/) |
| R | [f1_score_r](f1_score_r/) |

## Examples and tests

```text
f1([0,0,1,1,1], [0,1,1,0,1]) -> 0.6666666666666666
f1([0,0], [0,0])             -> 0.0
f1([1,1], [1,1])             -> 1.0
f1([1,0], [0,1])             -> 0.0
```

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

Binary hard labels only. It does not support confidence scores, threshold
selection, macro/micro/weighted multiclass F1, multilabel classification,
sample weights, or statistical uncertainty. Compare F1 results only when
label conventions, decision thresholds and test-set composition agree.
No CUDA variant is needed without meaningful batched benchmarking.

## References

Sokolova, M., & Lapalme, G. (2009). A systematic analysis of performance
measures for classification tasks. *Information Processing & Management,
45*(4), 427–437. https://doi.org/10.1016/j.ipm.2009.03.002
