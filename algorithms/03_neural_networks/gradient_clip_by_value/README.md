# Element-wise Gradient Value Clipping

## Why it belongs here

Gradient value clipping constrains each component of a gradient vector to
a fixed range before an optimizer update. It is an elementary alternative
to global L2 norm clipping: it limits each value separately rather than
scaling the entire vector (PyTorch, n.d.).

## Algorithm

For finite gradients `g` and a finite positive clip limit `c`:

```text
clipped[i] = min(c, max(-c, g[i]))
```

For `[-3,-0.5,0,0.5,3]` and `c=1`, the result is
`[-1,-0.5,0,0.5,1]`. If every component is already within the
limit, it is unchanged.

## Contract

- Input: a nonempty vector of finite real gradients and a finite
  strictly positive clipping limit.
- Output: same-length vector of components in `[-c,c]`.
- Reject empty, nonfinite, nonnumeric, zero or negative clip limits.
  Python rejects booleans; R uses real numeric input and rejects
  logical/complex data. Rust returns `Err`, C returns `false`,
  Python/R raise exceptions.
- C accepts caller-owned buffers and supports exact in-place clipping.
  Invalid inputs leave output unchanged.
- This is a forward vector transform only, not an optimizer, a norm
  calculation, gradient aggregation or differentiable clipping layer.

## Complexity

**O(n)** time, **O(1)** auxiliary memory for C and **O(n)** returned
storage for Python, Rust and R.

## Implementations

| Language | Folder |
| --- | --- |
| C | [gradient_clip_by_value_c](gradient_clip_by_value_c/) |
| Python | [gradient_clip_by_value_python](gradient_clip_by_value_python/) |
| Rust | [gradient_clip_by_value_rust](gradient_clip_by_value_rust/) |
| R | [gradient_clip_by_value_r](gradient_clip_by_value_r/) |

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Test cases cover positive/negative clipping, unchanged interior values,
limit boundaries, singleton gradients, extreme but finite input, in-place C
operation and rejection of invalid arguments.

## Limitations

Per-element clipping can change gradient direction and does not impose a
global L2 norm bound. This implementation handles only flat vectors, not
tensor trees, distributed gradients, automatic differentiation or CUDA.

## References

PyTorch. (n.d.). *torch.nn.utils.clip_grad_value_*.
https://docs.pytorch.org/docs/stable/generated/torch.nn.utils.clip_grad_value_.html
