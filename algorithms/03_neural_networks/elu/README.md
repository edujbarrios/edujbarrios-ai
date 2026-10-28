# Exponential Linear Unit (ELU) Activation

## Why it belongs here

The exponential linear unit is a neural-network nonlinearity with a
linear positive branch and a smooth, saturating negative branch.
Unlike ReLU it allows negative activation outputs. The original
work introduces the ELU family with a positive parameter
`alpha` (Clevert et al., 2016).

## Mathematical definition

This standalone reference fixes **alpha = 1**:

```text
ELU(x) = x                 if x >= 0
         exp(x) - 1        if x < 0
```

For negative values the implementation uses `expm1(x)`
(or `exp(x)-1` in R with stable selection), which avoids
loss of precision for values very close to zero.

Examples: `[-1, 0, 1]` yields approximately
`[-0.6321205588285577, 0, 1]`. For `x=-1000000`
the output is `-1` to floating-point precision.

## Contract

- Input is a plain vector of **1 to 10,000** finite real values,
  each between `-1,000,000` and `1,000,000`.
- Output is an equally long vector of finite doubles.
- Alpha is fixed at 1; there is no tunable parameter.
- C accepts an input array and distinct output array; it validates
  *all* inputs before modifying output and returns false on invalid
  data. The arrays must not overlap. Rust returns a `Result<Vec<f64>,
  &str>`; Python accepts list/tuple but not bools/nonnumeric types;
  R requires a numeric vector, not a matrix.
- Empty, oversized, nonfinite and out-of-range inputs are rejected
  rather than filtered or clipped.

## Complexity

**O(n)** time and **O(1)** working memory in C/R (excluding
outputs); Rust and Python allocate **O(n)** result vectors.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [elu_c](elu_c/) |
| Python | [elu_python](elu_python/) |
| Rust | [elu_rust](elu_rust/) |
| R | [elu_r](elu_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the negative and positive branches, values near
zero, saturation, input boundaries and invalid inputs.

## Limitations

This is the **forward activation only**. No derivative,
trainable alpha, automatic differentiation, vectorized framework
or GPU kernel is provided. Extreme negatives saturate to -1.

## References

- Clevert, D.-A., Unterthiner, T., & Hochreiter, S. (2016).
  Fast and accurate deep network learning by exponential linear
  units (ELUs). *International Conference on Learning
  Representations*. https://arxiv.org/abs/1511.07289
