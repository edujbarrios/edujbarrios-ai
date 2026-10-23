# Min-Max Feature Scaling

## Why it belongs in the AI hub

Machine-learning pipelines often transform features to comparable numerical
ranges before clustering, nearest-neighbor search or gradient-based
optimization. **Min-max scaling** provides an elementary example of a
data-dependent transformation that illustrates both the benefits of feature
scaling and the risk of data leakage. Feature preprocessing is part of
standard statistical learning practice (Hastie et al., 2009).

## Definition

For a **nonempty, nonconstant** vector `x` with finite components:

```text
minimum = min(x)
maximum = max(x)
range   = maximum - minimum
scaled[i] = (x[i] - minimum) / range
```

Example: `[-5, 0, 5]` becomes `[0, 0.5, 1]`. All outputs lie in
`[0,1]`, subject to normal floating-point rounding. Endpoints are
mapped to exact 0 and 1, and intermediate outputs are clamped to the range
to avoid tiny rounding excursions.

## Shared contract

- Input: a nonempty vector of finite real numbers with at least **two
  distinct values**. A singleton/constant series is rejected; no arbitrary
  choice such as mapping all constants to zero is made.
- Output: a new vector of the same length with components in `[0,1]`.
- Reject NaN, infinity, booleans-as-numbers, nonnumeric values, or
  **non-representable range** (for example `[-1e308,1e308]`).
  The range calculation is intentionally straightforward and errs rather
  than returning an inaccurate result after double overflow.
- Python/R raise errors; Rust returns `Result<Vec<f64>, ...>`.
  C uses caller-provided buffers (at least `n` doubles each), returns
  `bool`, and **does not modify the output on error**. Exact in-place C
  use (`input == output`) is supported; partial overlapping buffers are
  not supported.
- **Important:** this implementation fits minimum and maximum to its
  supplied input. In production ML, compute range parameters on **training
  data only**, then apply the fixed parameters to held-out data. Scaling
  test data using its own extrema leaks information and changes evaluation.
- Rounding and very small differences can affect results for values close
  to the floating-point precision limit.

## Complexity

**O(n)** time with two passes (find extrema, compute output).
C uses **O(1)** auxiliary memory outside the caller's output; Python and
Rust use O(n) for the output and Python's validated copy; R allocates O(n)
result storage.

## Implementations

| Language | Folder |
| --- | --- |
| C | [min_max_scaling_c](min_max_scaling_c/) |
| Python | [min_max_scaling_python](min_max_scaling_python/) |
| Rust | [min_max_scaling_rust](min_max_scaling_rust/) |
| R | [min_max_scaling_r](min_max_scaling_r/) |

## Examples and tests

```text
scale([-5,0,5])             -> [0,0.5,1]
scale([10,20,30])           -> [0,0.5,1]
scale([7,7,7])              -> error
scale([-1e308,1e308])      -> error (unrepresentable span)
```

Run from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

This is a one-vector educational reference: it does not scale multivariate
matrices column by column, store fitted transformers, support fixed ranges
for inference, apply robust scaling or clip outliers intelligently. It is
sensitive to extreme feature values and does not guarantee that transformed
training features follow a useful distribution.

No CUDA implementation is claimed; GPU vector preprocessing requires an
independently tested kernel and meaningful batch benchmarks.

## References

Hastie, T., Tibshirani, R., & Friedman, J. (2009). *The elements of
statistical learning: Data mining, inference, and prediction* (2nd ed.).
Springer. https://doi.org/10.1007/978-0-387-84858-7
