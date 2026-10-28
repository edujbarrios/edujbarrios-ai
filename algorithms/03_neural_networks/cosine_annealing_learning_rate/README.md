# Cosine Annealing Learning Rate

## Why it belongs here

A learning-rate schedule controls step sizes during iterative
optimization. Cosine annealing gradually decreases a step size
between a specified maximum and minimum, and is a key component
of cosine warm-restart scheduling (Loshchilov & Hutter, 2017).

## Definition

For `max_rate >= min_rate >= 0`, a horizon `T >= 1` and an
integer step `t` in `[0,T]`, calculate

```text
rate(t) = min_rate + (max_rate - min_rate)
          * (1 + cos(pi * t / T)) / 2
```

The schedule starts at `max_rate`, reaches `min_rate` at
the final step, and reaches their midpoint at `t=T/2`.
For `max_rate=1, min_rate=0, T=4`, the five values are
approximately `[1, .85355339, .5, .14644661, 0]`.

## Contract

- Maximum and minimum rates are finite doubles in `[0,1000000]`.
  Require `min_rate <= max_rate`. Equal rates are allowed.
- `total_steps` is a positive integer `1..100000`.
  `step` is an integer `0..total_steps`, inclusive.
- Returns one finite scalar rate in `[min_rate,max_rate]`.
  Clamping only corrects floating-point rounding at boundaries.
- C returns true/false and writes a caller-owned scalar only on
  success; Rust returns `Result<f64,&str>`; Python raises
  `ValueError` on invalid input, including bool step values;
  R accepts finite scalar numeric rates and integer-valued
  scalar step and total steps.
- No internal state is held and no optimizer is updated.

## Complexity

**O(1)** time and **O(1)** auxiliary memory.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [cosine_annealing_learning_rate_c](cosine_annealing_learning_rate_c/) |
| Python | [cosine_annealing_learning_rate_python](cosine_annealing_learning_rate_python/) |
| Rust | [cosine_annealing_learning_rate_rust](cosine_annealing_learning_rate_rust/) |
| R | [cosine_annealing_learning_rate_r](cosine_annealing_learning_rate_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests verify endpoints, midpoint, quarter steps, constant rate,
minimum horizon and invalid ranges, steps, nonfinite rates.

## Limitations

This is one **nonrestarting** cosine period. It does not implement
warm restarts, warmup, optimizer state, stochastic sampling,
per-batch scheduling, or policies for steps beyond the horizon.

## References

- Loshchilov, I., & Hutter, F. (2017). SGDR: Stochastic gradient
  descent with warm restarts. *International Conference on
  Learning Representations*. https://arxiv.org/abs/1608.03983
