# Softsign Activation

## Why it belongs here

Softsign is a smooth, bounded nonlinear activation for neural-network
hidden units. It maps any real input to the interval (-1,1), with
a gentler saturation than tanh. Comparing alternative activation
functions helps reveal their gradient and saturation behavior
(Glorot & Bengio, 2010).

## Mathematical definition

For each scalar input `x`:

```text
softsign(x) = x / (1 + abs(x))
softsign'(x) = 1 / (1 + abs(x))^2
```

The second formula gives the derivative, but **this reference
implements only the forward activation**, not backpropagation.

Examples: `[-2,-1,0,1,2]` becomes
`[-2/3,-1/2,0,1/2,2/3]`. A value of `1,000,000`
maps to `1,000,000/1,000,001`.

## Contract

- One sequence of **1 to 10,000** finite real values, with every
  input in `[-1,000,000,1,000,000]`.
- Output is a same-length sequence of doubles. Every returned
  element is strictly between -1 and 1.
- C accepts `double[n]` and a separate caller-owned output
  array `double[n]`; output is not modified on failure and
  must not overlap input. Rust returns `Result<Vec<f64>, &str>`.
  Python takes a list or tuple, rejecting bools and invalid types.
  R takes a numeric vector, not a matrix.
- Empty, nonfinite, and out-of-bound inputs are errors, not
  silently coerced or clipped.

## Complexity

**O(n)** time and **O(1)** auxiliary working memory for C/R
(excluding output); Rust and Python allocate an **O(n)** output
vector. No external dependencies are required.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [softsign_c](softsign_c/) |
| Python | [softsign_python](softsign_python/) |
| Rust | [softsign_rust](softsign_rust/) |
| R | [softsign_r](softsign_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover signs, zero, symmetry, singleton arrays, extreme finite
bounds and invalid/oversized input.

## Limitations

Softsign can suffer vanishing gradients for large magnitudes;
this tutorial does not implement gradients, a trainable layer,
vectorization libraries or a GPU kernel. The bounded domain is
an educational contract, not a mathematical restriction on softsign.

## References

- Glorot, X., & Bengio, Y. (2010). Understanding the difficulty
  of training deep feedforward neural networks. In Y. W. Teh &
  M. Titterington (Eds.), *Proceedings of the Thirteenth International
  Conference on Artificial Intelligence and Statistics*
  (Vol. 9, pp. 249–256). PMLR.
  https://proceedings.mlr.press/v9/glorot10a.html
