# Weighted Binary Brier Score

## Motivation

The Brier score is the mean squared error of probabilistic forecasts for
binary outcomes. A weighted version gives different influence to different
observations, e.g. under per-example importance weighting, and complements
the unweighted `brier_score` already in this collection
(scikit-learn developers, n.d.).

## Algorithm

For binary outcomes `y_i∈{0,1}`, predicted probabilities
`p_i∈[0,1]`, and nonnegative weights `w_i`:

```text
weighted_brier = sum_i( w_i * (p_i - y_i)^2 ) / sum_i( w_i )
```

The sum of weights must be positive. For `y=[0,1]`,
`p=[0.5,0]`, `w=[1,3]`, the score is
`(0.25+3)/4 = 0.8125`. Perfect predictions score 0. A
zero-weight observation has no effect on the result.

## Contract

- All three input vectors have identical length in 1..4096.
- Binary targets are **exactly 0 or 1**. Predictions are finite
  probabilities in `[0,1]`; weights are finite in `[0,1e6]`
  and their sum must be strictly positive.
- Output: one finite normalized squared-error score in `[0,1]`.
- Invalid lengths, labels, probabilities, weights and all-zero weights
  are rejected. Python refuses booleans and nonnumeric values. R accepts
  real numeric values but not logical or complex inputs. Rust returns
  `Result<f64, &str>` and C returns `false`.
- The caller-owned C output scalar is updated **only after validation**.
  Python/R raise an error on invalid inputs.

## Complexity

O(n) time and O(1) additional memory.

## Implementations

| Language | Directory |
| --- | --- |
| C | [weighted_brier_score_c](weighted_brier_score_c/) |
| Python | [weighted_brier_score_python](weighted_brier_score_python/) |
| Rust | [weighted_brier_score_rust](weighted_brier_score_rust/) |
| R | [weighted_brier_score_r](weighted_brier_score_r/) |

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Fixtures include unequal sample weights, a zero-weight example, perfect
forecasts, a one-example case, invalid labels/probabilities, nonfinite
weights, mismatched arrays and all-zero weights.

## Limitations

This implementation uses the **binary** convention with outcomes 0/1,
not multiclass Brier score. It assumes nonnegative sample weights and
does not calibrate predictions or infer confidence intervals. Weighted
metrics may be misleading if sample weights do not represent the intended
evaluation population.

## References

scikit-learn developers. (n.d.). *brier_score_loss*. scikit-learn
documentation. https://scikit-learn.org/stable/modules/generated/sklearn.metrics.brier_score_loss.html
