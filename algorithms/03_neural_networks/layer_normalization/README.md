# Layer Normalization for a Feature Vector

## Why it belongs in the AI hub

**Layer Normalization** standardizes activations across the feature
dimension for each individual example, then applies learned
per-feature scale and bias. It is a core Transformer building block,
unlike Batch Normalization, which depends on batch statistics
(Ba et al., 2016).

This reference performs the **forward computation for one feature
vector** with caller-supplied affine parameters. It does not learn
those parameters or perform a backward pass.

## Mathematical definition

For input `x` and affine vectors `gamma`, `beta`, all of length
`d >= 1`, and numerical constant `epsilon > 0`:

```text
mean       = sum_i x[i] / d
variance   = sum_i (x[i] - mean)^2 / d   # population variance
normalizer = sqrt(variance + epsilon)
output[i]  = gamma[i]*(x[i] - mean)/normalizer + beta[i]
```

The implementation computes the mean incrementally to avoid
overflow of a large running **sum of inputs**. It rejects any
nonfinite intermediate arithmetic and does not conceal overflow.

Example:

```text
x     = [1,2,3]
gamma = [1,1,1]
beta  = [0,0,0]
eps   = 1e-5

mean = 2
variance = 2/3
output ≈ [-1.2247356859, 0, 1.2247356859]
```

For a constant vector `x=[7,7]`, the output is exactly `beta`.
For `d=1`, the output is also `beta`; zero variance is valid
because `epsilon` keeps the denominator strictly positive.

## Shared contract

- `x`, `gamma` and `beta` are **nonempty equal-length** finite real
  feature vectors. `epsilon` must be finite and strictly positive.
- Output has exactly `d` finite components. Reject all malformed
  dimensions, nonfinite values, nonfinite differences/squared terms,
  variance accumulation, denominator, affine operations or output.
- C accepts caller-owned vectors each with at least `d` elements,
  plus a disjoint output vector. Returns `bool` and preserves output
  on *any* error by using a scratch array. Rust returns
  `Result<Vec<f64>, ...>`, Python returns a list, and R returns a
  numeric vector. Python/R raise errors on bad inputs.
- The order of floating-point operations is fixed within each reference,
  though minor rounding differences between toolchains are expected.
- Epsilon is added **to variance before square root**, not outside
  the square root. This distinction changes behavior for tiny variances.

## Complexity

**O(d)** time using a few linear passes and **O(d)** output storage.
C requires O(d) scratch to preserve output on failure. Python also
converts input to validated double-precision arrays, O(d) storage.
R and Rust construct O(d) output.

## Implementations

| Language | Folder |
| --- | --- |
| C | [layer_normalization_c](layer_normalization_c/) |
| Python | [layer_normalization_python](layer_normalization_python/) |
| Rust | [layer_normalization_rust](layer_normalization_rust/) |
| R | [layer_normalization_r](layer_normalization_r/) |

## Verification

Run from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover standard normalized data, nontrivial gamma/beta, constant
input, one feature, wrong shapes, invalid epsilon, NaN/inf input and
overflowing variance.

## Limitations

One feature vector at a time; no batching, optimized kernels, mixed
precision, learnable-parameter updates, backward gradients, GroupNorm,
RMSNorm, or full Transformer composition. The naïve centered-squares
variance can overflow for extreme finite data; these cases are rejected,
not silently rescaled. A GPU implementation requires device tests.

## References

Ba, J. L., Kiros, J. R., & Hinton, G. E. (2016). Layer normalization.
*arXiv*. https://doi.org/10.48550/arXiv.1607.06450
