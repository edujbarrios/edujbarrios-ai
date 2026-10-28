# Root Mean Squared Error (RMSE)

## Why it belongs here

Root mean squared error summarizes regression prediction errors
in the **same units as the target**. Unlike mean absolute error,
squaring individual residuals before averaging makes large errors
especially influential. It complements the repository's existing
mean squared error and logarithmic/symmetric error metrics
(scikit-learn developers, n.d.).

## Mathematical definition

For aligned targets `t[i]` and predictions `p[i]`:

```text
RMSE = sqrt((1/n) * sum((p[i]-t[i])^2))
```

Examples: target `[0,1,2]` vs. prediction `[0,2,4]`
produces `sqrt(5/3)`, approximately `1.2909944487358056`.
Identical vectors have RMSE zero. A single pair
`[-1,000,000]` vs. `[1,000,000]` has RMSE `2,000,000`.

## Contract

- Target and prediction vectors must have equal lengths in
  `1..10000`, containing finite real values bounded by
  `[-1,000,000,1,000,000]`.
- Return one nonnegative finite double representing error in
  target units, not squared or logarithmic units.
- C accepts `double[]` arrays and a caller-owned output scalar;
  on failure the output stays unchanged and must not overlap
  the inputs.
- Rust accepts `&[f64]` and returns `Result<f64,&str>`.
  Python accepts lists/tuples of numeric values but rejects bools.
  R requires numeric vectors, not matrices or complex numbers.
- Any mismatch, empty input, nonnumeric value, nonfinite number
  or out-of-range value is rejected. No missing-data imputation.

## Complexity

**O(n)** time, **O(1)** extra working memory in C and Rust.
Python validates using **O(n)** converted inputs; R vectorization
requires **O(n)** temporary storage.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [root_mean_squared_error_c](root_mean_squared_error_c/) |
| Python | [root_mean_squared_error_python](root_mean_squared_error_python/) |
| Rust | [root_mean_squared_error_rust](root_mean_squared_error_rust/) |
| R | [root_mean_squared_error_r](root_mean_squared_error_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover zero loss, mixed residuals, singleton and extreme
inputs, empty and mismatched vectors, nonfinite and out-of-range
values, and invalid types where representable.

## Limitations

RMSE is sensitive to large residuals, does not normalize for
target scale, and is not suitable on its own for all model
evaluation contexts. No weights, masks, uncertainty intervals,
sample grouping or gradients are supported.

## References

- Scikit-learn developers. (n.d.). *Root mean squared error*.
  Scikit-learn.
  https://scikit-learn.org/stable/modules/generated/sklearn.metrics.root_mean_squared_error.html
