# R-squared Score (Coefficient of Determination)

## Why it belongs in this repository

R-squared describes how much of the target variation a regression model
explains relative to always predicting the target mean. It complements
mean squared and mean absolute error in the AI Systems catalogue
(scikit-learn developers, n.d.).

## Mathematical definition

For actual targets y and predictions y_hat:

    R2 = 1 - sum_i (y_i - y_hat_i)^2 / sum_i (y_i - mean(y))^2

The score is 1 for exact predictions and can be negative for models
worse than the mean baseline. The inputs must contain at least two
observations. If all actual targets are exactly equal, this implementation
returns 1 for exact predictions and 0 otherwise, following the common
finite-score convention.

To reduce overflow and underflow, the squared residual sum and target
variation are calculated in separately scaled coordinates, and then
their scales are incorporated into the final ratio.

## Contract

- Inputs: equal-length vectors of at least two finite real values.
- Output: one finite real R-squared scalar (unbounded below).
- Invalid input: shorter/mismatched vectors, nonnumeric/Boolean (Python),
  complex or nonfinite values, or an unrepresentable result.
- Python raises ValueError; R throws an error; Rust returns Err; C
  returns false and leaves its output unchanged on failure.
- C inputs must each contain n doubles; pointers may not be null.
  Python accepts one-dimensional lists/tuples; R requires numeric vectors.
- Constant-target behavior: perfect prediction -> 1, otherwise -> 0.
  A nonconstant target whose variation becomes numerically zero after
  scaling causes an error instead of silently treating it as constant.

## Complexity

O(n) time and O(1) extra memory (excluding inputs). The algorithm
uses multiple passes for stable scaling.

## Implementations

| Language | Folder |
| --- | --- |
| C | [r_squared_score_c](r_squared_score_c/) |
| Python | [r_squared_score_python](r_squared_score_python/) |
| Rust | [r_squared_score_rust](r_squared_score_rust/) |
| R | [r_squared_score_r](r_squared_score_r/) |

## Examples and tests

    R2([1,2,3], [1,2,3]) = 1
    R2([1,2,3], [1,2,2]) = 0.5
    R2([1,2,3], [0,0,0]) = -6
    R2([4,4,4], [4,4,0]) = 0

Run from the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This is unweighted single-output R-squared, not adjusted R-squared
or multi-output aggregation. Extreme dynamic ranges may cause loss
of precision; an unrepresentably negative score is rejected rather
than returned as an infinity. Do not compare R-squared values across
different target datasets as if they were absolute errors.

## References

scikit-learn developers. (n.d.). *r2_score*. scikit-learn documentation.
https://scikit-learn.org/stable/modules/generated/sklearn.metrics.r2_score.html
