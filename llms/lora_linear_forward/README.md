# LoRA Low-Rank Linear Adapter: Forward Pass

## Why it belongs in this repository

**Low-Rank Adaptation (LoRA)** is a parameter-efficient fine-tuning
technique for large language models (Hu et al., 2022). Instead of
updating a full dense weight matrix, it learns a pair of low-rank
matrices and adds their product to a frozen layer. This implementation
isolates the **forward calculation** to show why it is efficient.

## Algorithm

For input vector `x` of length `d_in`, frozen weight matrix
`W` with shape `d_out x d_in`, rank-`r` matrices
`A` (`r x d_in`) and `B` (`d_out x r`), and
scaling factor `alpha`, compute:

```text
hidden = A @ x                           # shape (r,)
output = W @ x + (alpha / r) * B @ hidden
```

An optional output bias vector is added at the end. This avoids
materializing the full dense update `B @ A`. A zero `B`
initialization reproduces the unadapted frozen layer.

## Contract

- `lora_linear_forward(x, weight, a, b, alpha, bias=None)`
  accepts lists/tuples and outputs one list of real numbers.
- Input, output and rank dimensions must each lie in `1..128`, and all
  shapes must be rectangular and mutually compatible. Rank need not be
  less than the input/output dimension, though small ranks are the
  intended use case.
- All elements must be finite, non-boolean real numbers of magnitude
  at most 1,000. `alpha` is finite, non-boolean, and must be
  **strictly positive** and at most 1,000.
- When given, `bias` has exactly `d_out` elements; otherwise it is
  treated as zero. Invalid inputs raise `ValueError`.
- Neither the original weight matrix nor adapter matrices are mutated.
  Computations use binary64; nonfinite intermediate results are rejected.

## Worked example

```python
x = [2, 3]
W = [[1, 0], [0, 1]]
A = [[1, 1]]
B = [[2], [-1]]
lora_linear_forward(x, W, A, B, alpha=1)
# [12.0, -2.0]
```

Here `W @ x=[2,3]`, `A @ x=[5]`, and `B @ [5]=[10,-5]`.

## Complexity

**O(d_out d_in + r d_in + d_out r)** time for one vector,
and **O(d_in d_out + r d_in + d_out r)** storage when counting
validated copies of all matrices. The two temporary projected/output
vectors alone require O(r + d_out) space. This avoids explicitly forming
a dense `d_out x d_in` adapter update.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [lora_linear_forward_python](lora_linear_forward_python/) |

Other languages are not yet implemented.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests exercise the worked example, zero adapters, multiple ranks,
optional bias, scaling, shape errors, nonfinite inputs and
input immutability.

## Limitations

This is a **single-vector forward pass**, not a LoRA training algorithm:
it does not compute gradients, maintain optimizer states, quantize frozen
weights, merge adapter weights or load an actual transformer. It is
designed to explain a core computation, not benchmark LLM throughput.

## References

Hu, E. J., Shen, Y., Wallis, P., Allen-Zhu, Z., Li, Y., Wang, S.,
Wang, L., & Chen, W. (2022). *LoRA: Low-rank adaptation of large
language models*. International Conference on Learning Representations.
https://openreview.net/forum?id=nZeVKeeFYf9
