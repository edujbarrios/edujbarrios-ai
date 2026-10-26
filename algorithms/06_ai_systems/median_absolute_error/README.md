# Median Absolute Error

## Why it belongs in this repository

Median absolute error measures the typical absolute prediction error
without allowing a few extremely large residuals to dominate the metric.
It complements mean absolute error and mean squared error in regression
evaluation (scikit-learn developers, n.d.).

## Mathematical definition

For n observed and predicted pairs:

    errors_i = |actual_i - predicted_i|
    MedAE = median(errors)

With an even number of observations, take the arithmetic mean of the
two middle residuals after sorting. There are no sample weights.

## Contract

- Inputs: two equally long, **nonempty** vectors of finite real numbers.
- Output: a finite nonnegative scalar.
- Invalid input: empty, unequal-length, nonnumeric, Boolean (Python),
  nonfinite values, and residuals whose subtraction overflows floating
  point. Python raises ValueError; R stops; Rust returns Err; C returns
  false and leaves output unchanged.
- C arrays must each contain n doubles and all pointers must be valid.
- To avoid overflow averaging large finite middle values, calculate the
  midpoint as lower + (upper - lower)/2 (the two middle errors are
  nonnegative and sorted).

## Complexity

O(n log n) time for sorting residuals and O(n) auxiliary storage.

## Implementations

| Language | Folder |
| --- | --- |
| C | [median_absolute_error_c](median_absolute_error_c/) |
| Python | [median_absolute_error_python](median_absolute_error_python/) |
| Rust | [median_absolute_error_rust](median_absolute_error_rust/) |
| R | [median_absolute_error_r](median_absolute_error_r/) |

## Examples and tests

    MedAE([0,10,0], [0,0,0])   = 0
    MedAE([1,2,3,4], [0,0,0,0]) = 2.5
    MedAE([1,2], [1,2])       = 0

From the repository root, run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This metric summarizes absolute prediction errors but does not capture
prediction bias or error direction. It is a single-output, unweighted,
nonstreaming educational implementation; sorting may be unnecessary
for optimized large-scale implementations. A nonrepresentable residual
is considered invalid rather than silently clipped.

## References

scikit-learn developers. (n.d.). *median_absolute_error*.
scikit-learn documentation.
https://scikit-learn.org/stable/modules/generated/sklearn.metrics.median_absolute_error.html
