# Principal Component Analysis — 2D to 1D

## Why it belongs in the AI hub

Principal Component Analysis (PCA) is a core unsupervised **dimensionality
reduction** technique. It finds orthogonal directions of maximum variance,
allowing vectors to be projected into fewer dimensions before visualization,
clustering or retrieval. PCA can be derived from the eigendecomposition of
the centered data covariance (Jolliffe, 2002).

This is a narrowly scoped educational reference: **two real input features
reduced to one principal component** with a closed-form 2×2 covariance
eigenvector, not an arbitrary-dimensional SVD/PCA library.

## Mathematics and reproducibility

For `n >= 2` rows `x_i = (x_i0, x_i1)`, compute means `mu0, mu1`
and the (population-normalized) symmetric covariance:

```text
a = sum((x_i0 - mu0)^2) / n
b = sum((x_i0 - mu0)*(x_i1 - mu1)) / n
c = sum((x_i1 - mu1)^2) / n
C = [[a, b], [b, c]]

disc = hypot(a-c, 2*b)
dominant eigenvalue = (a+c+disc)/2
principal direction v = normalized eigenvector of C
projection_i = dot(x_i - mu, v)
```

Eigenvector signs are arbitrary mathematically. For deterministic results,
choose the sign making the **first nonzero component positive**. If `b=0`,
use the X axis when `a >= c` and the Y axis otherwise, resolving
equal-variance covariance ties toward X. If both variances are zero,
PCA is undefined and this reference rejects the input.

Example:

```text
samples = [[0,0], [2,0], [4,0]]
mean = [2,0]
principal_direction = [1,0]
projections = [-2,0,2]
```

## Contract

- Accept a dense **n×2** matrix with **n≥2** and all finite real values.
- Return a mean vector (2), a normalized principal direction (2), and
  projections (n). The outputs are deterministic for exactly repeated data
  and specified tie rules.
- Reject constant datasets, malformed rows, NaN/infinity and overflowing
  means, covariance sums, eigendirection computations or projections.
- C and Rust accept flattened row-major sample buffers. C outputs caller-
  owned mean[2], direction[2] and projection[n] buffers (all non-overlapping);
  it only commits outputs after full validation. Python returns three
  lists in a tuple. R accepts a base matrix and returns a named list.
- Floating-point results are approximate; tests use numerical tolerances.
  Even when eigengaps are tiny, small input perturbations can rotate the
  resulting principal direction drastically.
- All implementations use covariance normalized by `n`; using `n-1`
  would not change the direction for `n≥2`, but would rescale eigenvalues.
  This block does not return the eigenvalues themselves.

## Complexity

Two passes over the data and one projection pass: **O(n)** time.
**O(n)** output storage for the projected values; **O(1)** auxiliary memory
apart from caller-owned outputs in C. Python/R use additional O(n) input
conversion/validation storage. No numerical libraries required.

## Implementations

| Language | Directory |
| --- | --- |
| C | [pca_2d_c](pca_2d_c/) |
| Python | [pca_2d_python](pca_2d_python/) |
| Rust | [pca_2d_rust](pca_2d_rust/) |
| R | [pca_2d_r](pca_2d_r/) |

## Tests

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` in the repository root.
Tests include X- and Y-aligned data, diagonal data, isotropic covariance,
constant and nonfinite input, malformed dimensions and overflow.

## Limitations

Not a production PCA algorithm: supports exactly 2→1 dimensions, no
eigenvalues/explained-variance ratios, whitening, missing data, sample
weights, incremental updates, randomized SVD, GPU processing or
transformation of separate held-out samples. The naïve covariance
calculation can suffer cancellation on nearly constant or very large data;
such inputs may require rescaling or a numerically optimized library.

## References

Jolliffe, I. T. (2002). *Principal component analysis* (2nd ed.).
Springer. https://doi.org/10.1007/b98835
