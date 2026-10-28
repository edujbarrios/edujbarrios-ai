# Numerically Stable Log-Softmax

## Why it belongs here

Neural classifiers and language models often need **log-probabilities**
for likelihoods or cross-entropy. Computing `log(softmax(x))` naively
may lose small probabilities to underflow, while direct exponentiation
of large logits may overflow. Log-softmax uses the same
max-shift stabilization as log-sum-exp (Goodfellow et al., 2016).

## Definition

Given a vector of logits `x` and its maximum `m = max(x)`,
calculate

```text
s = sum(exp(x[i] - m))
output[i] = (x[i] - m) - log(s)
```

Each output is `log(exp(x[i]) / sum(exp(x)))` mathematically,
without explicitly computing either large numerator or denominator.
The sum of exponentiated outputs is approximately 1. For `[0,0]`
the result is `[-log(2), -log(2)]`. For
`[1,000,000, -1,000,000]` it is approximately `[0, -2,000,000]`
rather than a negative infinity for the second entry.

## Contract

- Input vector length `1..10000`.
- Entries must be finite real values within
  `[-1,000,000,1,000,000]`; no NaNs or infinities.
- A singleton returns `[0]`. Equal logits produce equal
  log-probabilities. Output values are finite and nonpositive
  (subject to negligible floating-point rounding).
- C accepts a double array and writes a length-n caller-owned
  output **only on success**; input and output must not overlap.
  Rust returns `Result<Vec<f64>, &str>`. Python takes a list
  or tuple (rejects bools/non-numeric entries). R takes a numeric
  vector, not a matrix.
- No temperature scaling, masking, or reduction is performed.

## Complexity

**O(n)** time. C uses **O(n)** temporary space to guarantee that
the output stays unchanged on error; Rust and Python also allocate
**O(n)** output, while R vectorizes the expression.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [log_softmax_c](log_softmax_c/) |
| Python | [log_softmax_python](log_softmax_python/) |
| Rust | [log_softmax_rust](log_softmax_rust/) |
| R | [log_softmax_r](log_softmax_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover uniform and nonuniform logits, extreme values,
singletons, shift invariance, normalizing sums, invalid types,
empty inputs and nonfinite/out-of-range values.

## Limitations

This is a CPU reference for one flat vector; it is not a batched
tensor operator or a GPU kernel. With extreme values, exponentials
of very negative shifted logits underflow to zero as expected,
while their *log*-probabilities remain finite. Roundoff depends
slightly on hardware and language.

## References

- Goodfellow, I., Bengio, Y., & Courville, A. (2016).
  *Deep learning*. MIT Press. https://www.deeplearningbook.org/
