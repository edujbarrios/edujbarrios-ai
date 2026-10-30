# Global L2 Gradient Norm Clipping

## Why it belongs here

Clipping an entire gradient vector to a maximum L2 norm prevents unusually
large updates from dominating neural-network training, especially in recurrent
models (Pascanu et al., 2013). It is a distinct step from choosing an
optimizer or applying per-coordinate clipping.

## Algorithm

For finite gradients `g` and a finite positive norm bound `M`:

```text
norm = sqrt(sum(g[i] ** 2))
factor = min(1, M / norm)  # factor = 1 when norm = 0
clipped[i] = g[i] * factor
```

Implementations first divide by the maximum absolute component and compute
the scaled norm. This avoids squaring enormous finite gradients or directly
computing an L2 norm that may overflow the floating-point range. The
mathematical transformation remains unchanged.

Example: `g=[3,4], M=1` returns `[0.6,0.8]`; with `M=5` the
vector stays `[3,4]`. A zero vector is unchanged.

## Contract

- Input: a nonempty vector of finite real gradients and a finite, strictly
  positive real `max_norm`.
- Output: the same-length clipped vector. If the true L2 norm is at most
  `max_norm`, the vector is unchanged (apart from any representational
  details such as signed zero).
- Reject empty, nonnumeric, nonfinite gradients or non-positive/nonfinite
  bound. Python/R raise an error, Rust returns `Err`, C returns `false`.
- C uses caller-owned buffers of length `n` and supports exact in-place
  clipping. On invalid input the output is left untouched.
- Floating-point rounding means output norms may differ slightly from the
  requested maximum; extreme subnormal bounds can underflow to zero.

## Complexity

**O(n)** time and **O(1)** scratch space for C (aside from output).
Python, Rust and R additionally allocate **O(n)** output storage.

## Implementations

| Language | Folder |
| --- | --- |
| C | [gradient_clip_global_norm_c](gradient_clip_global_norm_c/) |
| Python | [gradient_clip_global_norm_python](gradient_clip_global_norm_python/) |
| Rust | [gradient_clip_global_norm_rust](gradient_clip_global_norm_rust/) |
| R | [gradient_clip_global_norm_r](gradient_clip_global_norm_r/) |

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover clipping and no-op cases, zeros, mixed signs, very large finite
values, an in-place C call, and invalid gradients or norm bounds.

## Limitations

This clips a single flat gradient vector. Callers must flatten/concatenate
parameters when a **global** norm across multiple tensors is desired. No
autograd integration, distributed reduction, GPU kernel or optimizer updates.

## References

Pascanu, R., Mikolov, T., & Bengio, Y. (2013). On the difficulty of training
recurrent neural networks. In *Proceedings of the 30th International Conference
on Machine Learning* (Vol. 28, pp. 1310–1318). Proceedings of Machine
Learning Research. https://proceedings.mlr.press/v28/pascanu13.html
