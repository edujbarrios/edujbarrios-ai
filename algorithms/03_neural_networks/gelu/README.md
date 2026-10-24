# GELU Activation (Tanh Approximation) and Derivative

## Why it belongs in AI

The **Gaussian Error Linear Unit (GELU)** is a smooth, widely used
neural-network activation, including in Transformer architectures.
It differs from ReLU and Swish by applying a Gaussian-inspired input
gate. A commonly used **tanh approximation** avoids requiring a Gaussian
CDF implementation while retaining the shape of the activation
(Hendrycks & Gimpel, 2016).

This algorithm implements **the tanh approximation**, *not* the exact
normal-CDF GELU. Its derivative is with respect to that approximation.

## Mathematics

For a real-valued input `x`, define:

```text
a = sqrt(2/pi)
b = 0.044715
u = a * (x + b*x^3)
t = tanh(u)

gelu_tanh(x) = 0.5 * x * (1 + t)

gelu_tanh'(x) =
    0.5 * (1 + t)
    + 0.5 * x * (1 - t^2) * a * (1 + 3*b*x^2)
```

Special cases:

```text
gelu(0) = 0
gelu'(0) = 0.5

gelu(1) ≈ 0.841192
gelu(-1) ≈ -0.158808
```

For large positive x, the activation approaches x and its derivative
approaches 1. For large negative x, both approach zero.

## Shared contract

- Input is a **nonempty** one-dimensional vector of finite real values.
- Return two aligned vectors, **activations and derivatives**, evaluated
  from the same intermediate `u`.
- Reject NaN, infinity, invalid types, empty arrays and **overflow in any
  intermediate** (including x³, u, derivative factors and output).
  Finite extremely large x values can cause overflow even when the
  eventual mathematical GELU value would be finite.
- C accepts `double x[n]` and caller-owned `values[n]` and
  `derivatives[n]`, all non-overlapping. On error C returns `false`
  without modifying either output. Rust returns
  `Result<(Vec<f64>, Vec<f64>),...>`; Python/R raise errors.
- Python rejects boolean values; R accepts finite base numeric vectors.
  Results are approximate and small cross-language differences in
  `tanh` rounding are expected.
- There is no training loop, tensor broadcast, learnable parameter
  or automatic differentiation framework.

## Complexity

O(n) time and O(n) storage for the two output vectors. The C version
uses temporary output buffers so failures do not expose partial results.

## Implementations

| Language | Directory |
| --- | --- |
| C | [gelu_c](gelu_c/) |
| Python | [gelu_python](gelu_python/) |
| Rust | [gelu_rust](gelu_rust/) |
| R | [gelu_r](gelu_r/) |

Run tests from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include zero, ±1, saturation at ±100, NaN, infinity, empty
input and finite values that overflow cubic intermediates.

## Limitations

Approximate GELU only; the exact definition is
`x*Phi(x)` for the standard Gaussian cumulative distribution Phi.
For precise parity with a production ML framework, its chosen GELU
variant and floating-point rounding must match. No GPU acceleration,
tensor fusion, SIMD benchmark, or full network training is claimed.

## References

Hendrycks, D., & Gimpel, K. (2016). *Gaussian error linear units (GELUs)*
[Preprint]. arXiv. https://arxiv.org/abs/1606.08415
