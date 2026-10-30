# Linear Warmup and Cosine Learning Rate Decay

## Why it belongs here

A linear learning-rate warmup can avoid immediately taking large optimization
steps, while a cosine decay smoothly reduces the rate as training progresses.
This educational schedule combines warmup (Goyal et al., 2017) with cosine
annealing (Loshchilov & Hutter, 2017). It does **not** implement restarts.

## Algorithm

For `0 <= minimum_rate <= peak_rate <= 1e6`, positive
`warmup_steps < total_steps <= 100000`, and integer
`step in [0,total_steps]`:

```text
if step <= warmup_steps:
    rate = peak_rate * step / warmup_steps
else:
    progress = (step - warmup_steps) / (total_steps - warmup_steps)
    rate = minimum_rate + 0.5 * (peak_rate - minimum_rate)
           * (1 + cos(pi * progress))
```

The schedule starts at 0, reaches `peak_rate` at the end of
warmup and finishes at `minimum_rate`. For `peak=1`,
`minimum=0`, `warmup=2`, `total=6`, the rates are:
`[0, 0.5, 1, 0.85355339, 0.5, 0.14644661, 0]`.

## Contract

- Inputs: finite real `peak_rate`, `minimum_rate` in `[0,1e6]`,
  with `minimum_rate <= peak_rate`. `warmup_steps` and
  `total_steps` are positive integers, with
  `1 <= warmup_steps < total_steps <= 100000`.
- `step` is an integer in `[0,total_steps]`, inclusive.
- Output: a finite nonnegative scalar learning rate.
- Invalid parameters are errors. Python/R raise exceptions, Rust returns
  `Err`, C returns `false` without changing the output scalar.
  Python rejects boolean values as numeric parameters.
- There is no optimizer state, warm restart, or extrapolation beyond the
  prescribed schedule.

## Complexity

**O(1)** time and **O(1)** additional memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [warmup_cosine_learning_rate_c](warmup_cosine_learning_rate_c/) |
| Python | [warmup_cosine_learning_rate_python](warmup_cosine_learning_rate_python/) |
| Rust | [warmup_cosine_learning_rate_rust](warmup_cosine_learning_rate_rust/) |
| R | [warmup_cosine_learning_rate_r](warmup_cosine_learning_rate_r/) |

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Fixtures check the exact endpoints, warmup midpoint, decay midpoint, equal
rate case, very short schedules, invalid step bounds and nonfinite rates.

## Limitations

This is a scalar schedule with one cosine decay period, no restarts,
adaptive tuning, optimizer integration, or batch accumulation. Learning-rate
boundaries are defined in discrete steps; different training loops may apply
the update before or after advancing the step counter.

## References

Goyal, P., Dollár, P., Girshick, R., Noordhuis, P., Wesolowski, L., Kyrola, A.,
Tulloch, A., Jia, Y., & He, K. (2017). *Accurate, large minibatch SGD: Training
ImageNet in 1 hour*. https://arxiv.org/abs/1706.02677

Loshchilov, I., & Hutter, F. (2017). *SGDR: Stochastic gradient descent with
warm restarts*. International Conference on Learning Representations.
https://arxiv.org/abs/1608.03983
