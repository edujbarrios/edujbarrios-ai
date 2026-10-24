# Gaussian Radial Basis Function (RBF) Kernel Matrix

## Motivation

The **Gaussian radial basis function (RBF) kernel** maps Euclidean
distances between observations into similarities in `(0,1]`.
It is a standard building block in kernel methods, especially SVMs
and Gaussian-process models. By constructing a **Gram matrix** of
kernel values directly, this reference makes the operation visible
without depending on a machine-learning framework
(Schölkopf & Smola, 2002).

## Mathematical definition

For observation vectors `x_i,x_j∈R^d` and a positive
length scale `sigma`:

```text
K[i,j] = exp(- ||x_i-x_j||² / (2*sigma²))
       = exp(-0.5*(||x_i-x_j||/sigma)²)
```

This yields `K[i,i]=1`, `K[i,j]=K[j,i]`, and
`0 <= K[i,j] <= 1`, with the lower endpoint reached only
through ordinary floating-point underflow for well-separated data.
The implementation computes Euclidean distance with a
**running hypotenuse recurrence** before squaring the scaled norm.

Example:

```text
X=[[0],[1],[2]], sigma=1

K = [[1, exp(-0.5), exp(-2)],
     [exp(-0.5), 1, exp(-0.5)],
     [exp(-2), exp(-0.5), 1]]
```

Duplicate input rows have similarity 1 even when they occur at
different row indices. Smaller sigma produces a more localized
similarity; larger sigma makes all vectors more similar.

## Shared contract

- Nonempty rectangular **n×d** matrix, with **1<=n<=128** and
  **1<=d<=32**, of finite real coordinates in **[-1e6,1e6]**.
- Length scale is a finite real `sigma∈[1e-6,1e6]`.
  The bounded domain guarantees that intermediate squared
  scaled distances remain representable in binary64 arithmetic.
- Return the full symmetric `n×n` matrix in the same original
  row order. The kernel is a similarity, **not a probability
  distribution** and does not require rows to sum to one.
- Reject malformed matrices, unsupported coordinate or scale ranges,
  NaN/Inf, and nonfinite intermediate/output values.
- C takes a flat row-major `double X[n*d]` and caller-owned,
  non-overlapping `double K[n*n]`, **unchanged on error**.
  Rust uses row-major flat arrays and returns
  `Result<Vec<f64>,...>`. Python uses nested lists; R a numeric
  matrix. Python rejects bools; R rejects nonnumeric matrices.
- No trainable parameters, loss, labels, or kernel SVM fitting
  are included.

## Complexity

**O(n²d)** time and O(n²) output storage. C allocates one scratch
output buffer to prevent partially written results on failure.

## Implementation and testing

| Language | Directory |
| --- | --- |
| C | [gaussian_rbf_kernel_c](gaussian_rbf_kernel_c/) |
| Python | [gaussian_rbf_kernel_python](gaussian_rbf_kernel_python/) |
| Rust | [gaussian_rbf_kernel_rust](gaussian_rbf_kernel_rust/) |
| R | [gaussian_rbf_kernel_r](gaussian_rbf_kernel_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover an analytic three-vector Gram matrix, symmetry,
duplicate data, singleton matrices, sigma scaling, invalid
shapes, nonfinite values and unsupported numeric ranges.

## Limitations

Dense Gram-matrix construction only. Large datasets should employ
blockwise kernels, approximation methods or sparse/implicit
representations. No automatic gamma search, normalization,
support-vector optimization, kernel ridge regression, neural
training or GPU performance claim.

## References

Schölkopf, B., & Smola, A. J. (2002). *Learning with kernels:
Support vector machines, regularization, optimization, and beyond*.
MIT Press. https://mitpress.mit.edu/9780262194754/learning-with-kernels/
