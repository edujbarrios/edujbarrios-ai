# Binary Cross-Entropy With Logits

## Why it belongs here

Binary classifiers commonly output raw *logits*, not probabilities.
Evaluating a sigmoid followed by separate logarithms can lose precision
or overflow for extreme logits. This algorithm combines both operations
in a stable log-domain expression (Goodfellow et al., 2016).

## Mathematical definition

For each logit `z` and target `y` in `[0,1]`, the binary cross-entropy is

```text
-y * log(sigmoid(z)) - (1-y) * log(1-sigmoid(z))
```

We instead evaluate the equivalent expression

```text
max(z, 0) - z*y + log1p(exp(-abs(z)))
```

and return its arithmetic mean across all observations. This formula
works for binary targets and fractional/soft targets. It avoids
computing a potentially rounded-to-zero sigmoid.

Examples: one logit of `0` has loss `log(2)` for any valid target;
logit `1000` and target `1` has a loss near zero; logit `-1000`
and target `1` has a loss near `1000`.

## Contract

- Both sequences have the same length, `1..10000`.
- Logits are finite real numbers in `[-1000000, 1000000]`.
- Targets are finite real numbers in the inclusive interval `[0,1]`.
- No weights, broadcasting, masks, or implicit sigmoid are applied.
- C takes two arrays and a caller-owned output pointer that is
  unchanged on failure; its output must not overlap the inputs.
  Rust returns `Result<f64, &str>`; Python and R raise errors.
- Python accepts lists/tuples of numeric values but rejects booleans.
  R accepts numeric vectors (not matrices or complex values).
- The result is a finite, nonnegative double. Very small losses may
  round to zero because of finite precision.

## Complexity

**O(n)** time and **O(1)** auxiliary space in C, Rust and R;
Python checks and converts inputs using **O(n)** auxiliary space.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [binary_cross_entropy_with_logits_c](binary_cross_entropy_with_logits_c/) |
| Python | [binary_cross_entropy_with_logits_python](binary_cross_entropy_with_logits_python/) |
| Rust | [binary_cross_entropy_with_logits_rust](binary_cross_entropy_with_logits_rust/) |
| R | [binary_cross_entropy_with_logits_r](binary_cross_entropy_with_logits_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The suites cover zero and extreme logits, binary and soft labels,
mixed vectors, mismatched/empty input, invalid target bounds, and
nonfinite or out-of-range logits.

## Limitations

This is a scalar unweighted mean, not a tensor library function.
No gradients, reduction choices, class weights or automatic
differentiation are provided. The declared magnitude bound prevents
unexpected conversion overflow but is not a production-scale limit.

## References

- Goodfellow, I., Bengio, Y., & Courville, A. (2016).
  *Deep learning*. MIT Press. https://www.deeplearningbook.org/
