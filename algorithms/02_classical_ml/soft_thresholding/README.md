# Soft Thresholding (L1 Proximal Operator)

## Why it belongs here

Soft thresholding is the proximal operator of the L1 penalty, commonly used
for sparse optimization and wavelet denoising. It shrinks each component
toward zero and sets small-magnitude components exactly to zero
(Donoho, 1995).

## Mathematical definition

Given finite real vector `x` and a finite nonnegative threshold `lambda`:

```text
soft_threshold(x_i, lambda) = sign(x_i) * max(abs(x_i) - lambda, 0)
```

Equivalently, `x_i - lambda` for `x_i > lambda`, `x_i + lambda`
for `x_i < -lambda`, and 0 otherwise. This is the exact minimizer
of `0.5*(z - x_i)^2 + lambda*abs(z)` for each scalar coordinate.

Example: for `x=[-3,-0.5,0,0.5,3]` and `lambda=1`,
the result is `[-2,0,0,0,2]`.

## Contract

- Input: a nonempty sequence of finite real values and a finite threshold
  `lambda >= 0`.
- Output: an equally sized sequence, with independently thresholded entries.
- Invalid input: empty vector, nonfinite/nonreal components or invalid threshold.
  C returns `false`, Rust returns `Err`, Python/R raise errors.
- C requires valid input/output buffers for `n` doubles; exact in-place
  writes are supported. Invalid input leaves output unchanged.
- Python rejects booleans and nonnumeric types. R requires a real numeric
  vector, not logical or complex. IEEE-754 rounding applies at extremes.

## Complexity

O(n) time; O(1) auxiliary memory for C, and O(n) for newly allocated
Python/Rust/R output vectors.

## Implementations

| Language | Directory |
| --- | --- |
| C | [soft_thresholding_c](soft_thresholding_c/) |
| Python | [soft_thresholding_python](soft_thresholding_python/) |
| Rust | [soft_thresholding_rust](soft_thresholding_rust/) |
| R | [soft_thresholding_r](soft_thresholding_r/) |

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover positive/negative shrinkage, exact-zero boundaries, a zero
threshold, large finite magnitudes, singleton and invalid inputs.

## Limitations

This is a single proximal update, not a full LASSO solver, a training
pipeline or wavelet transform. Finite floating-point arithmetic is not
exact for very large numbers or values close to the threshold.

## References

Donoho, D. L. (1995). De-noising by soft-thresholding.
*IEEE Transactions on Information Theory, 41*(3), 613–627.
https://doi.org/10.1109/18.382009
