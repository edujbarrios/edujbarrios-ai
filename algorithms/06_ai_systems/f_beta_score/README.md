# F-beta Score

## Why it belongs in this repository

The F-beta score generalizes F1 to emphasize either recall (beta > 1)
or precision (beta < 1). It is useful when evaluating imbalanced classifiers
and retrieval pipelines with different costs for missed relevant results
and false alarms (van Rijsbergen, 1979).

## Mathematical definition

For true positives (TP), false positives (FP) and false negatives (FN):

    precision = TP / (TP + FP)
    recall    = TP / (TP + FN)
    F_beta    = (1 + beta^2) * precision * recall
                / (beta^2 * precision + recall)

For numerical stability, implementations use an equivalent weighted
harmonic mean with weight w = beta^2/(1+beta^2):

    F_beta = precision * recall / (w * precision + (1-w) * recall)

Compute w without squaring beta when beta > 1. If TP is zero,
define F_beta as zero, including the all-zero confusion counts.
When TP is positive, both precision and recall are well-defined.

## Contract

- Input: TP, FP, FN as nonnegative signed 32-bit integer counts and
  beta as a finite, strictly positive real number.
- Output: one finite score in [0,1].
- Beta = 1 is F1; beta = 2 emphasizes recall. No true negatives are needed.
- Python raises ValueError on invalid counts, bools, or beta; R throws
  an error; Rust returns Err; C returns false and does not modify output.
- C expects a nonnull output pointer; integer counts are typed int32_t.
  Rust uses i32 counts. Extremely small/large beta values are supported
  without overflowing beta squared.

## Complexity

O(1) time and O(1) auxiliary space.

## Implementations

| Language | Folder |
| --- | --- |
| C | [f_beta_score_c](f_beta_score_c/) |
| Python | [f_beta_score_python](f_beta_score_python/) |
| Rust | [f_beta_score_rust](f_beta_score_rust/) |
| R | [f_beta_score_r](f_beta_score_r/) |

## Examples and tests

Given TP = 8, FP = 2, FN = 4:

    F_1   = 8/11
    F_2   = 20/29
    F_0.5 = 10/13

Run from the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

Binary-class positive-class score only; this implementation does not
compute macro/micro/multiclass or sample-weighted F-beta. A zero score
when TP = 0 is a declared convention, not a statement about negative
class predictions. Like F1, the metric ignores true negatives.

## References

van Rijsbergen, C. J. (1979). *Information retrieval* (2nd ed.).
Butterworths.
