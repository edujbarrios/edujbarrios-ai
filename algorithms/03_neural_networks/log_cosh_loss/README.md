# Log-Cosh Regression Loss

## Why it belongs here

Log-cosh is a smooth regression loss with approximately quadratic
behavior near zero residual and nearly linear growth for large
errors. It is a useful alternative to squared error when a few
large residuals should not dominate the objective (Keras Team, n.d.).

## Definition

For `n` paired scalar targets `t[i]` and predictions `p[i]`,
compute the arithmetic mean

```text
L = (1/n) * sum(log(cosh(p[i] - t[i])))
```

Directly calling `cosh` can overflow on large finite errors.
Let `a = abs(p[i] - t[i])`; the equivalent expression is

```text
log(cosh(e)) = a + log1p(exp(-2*a)) - log(2)
```

This implementation uses the stable expression and clamps tiny
negative results caused by roundoff to zero. This is a **scalar
unweighted mean** across all entries, not a batched tensor API.

Examples: exact predictions yield 0; residuals `[0,1,-2]`
give approximately **0.5862611926136305**. Values `1e6` and
`-1e6` yield a finite result close to `2e6 - log(2)`.

## Contract

- Target and prediction arrays have matching lengths in
  `1..10000`.
- Each input is a finite real number between
  `-1,000,000` and `1,000,000`, inclusive.
- C and Rust accept double arrays, Python accepts lists/tuples
  with numeric entries (excluding bool), and R accepts numeric
  vectors (not matrices).
- Invalid or empty input is rejected; no NaN/Inf propagation.
  C returns false without modifying its caller-owned output,
  which must not overlap the inputs. Rust returns `Result`;
  Python and R raise errors.
- Floating-point precision varies slightly across languages.
  The result is finite and nonnegative.

## Complexity

**O(n)** time and **O(1)** additional working space
(C/Rust/R); Python allocates a converted list of pairs, **O(n)**
temporary space in this reference implementation.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [log_cosh_loss_c](log_cosh_loss_c/) |
| Python | [log_cosh_loss_python](log_cosh_loss_python/) |
| Rust | [log_cosh_loss_rust](log_cosh_loss_rust/) |
| R | [log_cosh_loss_r](log_cosh_loss_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests exercise zero, unit, mixed and extreme residuals, single
observations, mismatched and empty arrays, nonfinite and invalid
entries, and boundary magnitudes.

## Limitations

This is not a full differentiable tensor operator or optimizer.
It provides no gradient API, sample weights, masks or broadcasting.
The stable log-domain expression avoids overflow for the stated
bounded domain but cannot eliminate roundoff entirely.

## References

- Keras Team. (n.d.). *Regression losses: LogCosh*.
  https://keras.io/api/losses/regression_losses/
