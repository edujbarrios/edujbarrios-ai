# Training-Mode Batch Normalization for Dense Features

## Why it belongs in AI

**Batch normalization** normalizes each feature over examples in a
mini-batch, then applies a learnable scale and shift. It is a common
neural-network building block intended to simplify optimization. This
reference computes the **training-mode forward pass** from the current
batch; it does not manage moving averages for inference
(Ioffe & Szegedy, 2015).

## Mathematics

For an input matrix `X[n,d]`, feature `j`, scale `gamma[j]`,
shift `beta[j]`, and fixed `epsilon=1e-5`:

```text
mu[j] = (1/n) * sum_i X[i,j]
variance[j] = (1/n) * sum_i (X[i,j]-mu[j])^2
Y[i,j] = gamma[j] * (X[i,j]-mu[j]) /
         sqrt(variance[j]+1e-5) + beta[j]
```

The variance uses **population divisor n**, not sample divisor n−1.
This means even a single-row batch is well-defined:
every normalized feature is zero and the result equals beta.

Example:

```text
X = [[1,10], [3,10]]
gamma = [1,2]; beta = [0,-1]
means = [2,10]; variances = [1,0]
Y = [[-1/sqrt(1.00001), -1],
     [+1/sqrt(1.00001), -1]]
```

## Shared contract

- Input matrix has **1..512 rows** and **1..32 columns**. Input
  coordinates, gamma and beta must be finite real values with
  absolute value **<=1e6**.
- Each gamma/beta array has **exactly d elements**. Zero gamma
  and constant feature columns are valid. Epsilon is fixed to
  **1e-5**, positive, not caller-configurable.
- Return an `n×d` matrix with original row order and feature
  layout. No mutation of inputs.
- Reject mismatches, invalid numeric types, NaN/Inf, out-of-bound
  input values and nonfinite means, variances, denominators or outputs.
  Python rejects bools; R accepts base numeric data.
- C receives row-major `double X[n*d]`, `gamma[d]`, `beta[d]`
  and caller-owned `double output[n*d]`, which is non-overlapping
  and **unchanged on failure**. Rust takes flat row-major arrays
  and returns `Result<Vec<f64>,...>`. Python uses nested rows;
  R uses an n×d numeric matrix.
- This is specifically training-mode normalization. In production
  inference, use frozen running statistics estimated during training;
  recomputing them from evaluation samples would leak information.

## Complexity

O(n*d) time, O(n*d+d) output and temporary storage in C;
other implementations use O(n*d+d) storage (including result).
The reference does not implement backward gradients.

## Implementations

| Language | Location |
| --- | --- |
| C | [batch_normalization_c](batch_normalization_c/) |
| Python | [batch_normalization_python](batch_normalization_python/) |
| Rust | [batch_normalization_rust](batch_normalization_rust/) |
| R | [batch_normalization_r](batch_normalization_r/) |

Run repository tests with:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover two-feature batches, constant feature columns, single
samples, negative gamma, invalid dimensions and nonfinite data.

## Limitations

Training-mode dense batch normalization only. No moving statistics,
inference mode, backpropagation, learnable gamma/beta training,
convolutional tensors, distributed synchronization, mixed precision
or verified GPU acceleration.

## References

Ioffe, S., & Szegedy, C. (2015). Batch normalization: Accelerating
deep network training by reducing internal covariate shift.
In *Proceedings of the 32nd International Conference on Machine
Learning* (Vol. 37, pp. 448–456). PMLR.
https://proceedings.mlr.press/v37/ioffe15.html
