# Scaled Dot-Product Attention — One Query

## Why it belongs in the AI hub

Scaled dot-product attention computes how strongly a query attends to
different keys, then forms a weighted combination of their value vectors.
It is a core operation in the **Transformer** architecture (Vaswani et al.,
2017). This implementation isolates **one query and one attention head**
so the mathematical steps can be understood and tested independently.

## Formula

Given query `q` of dimension `d`, keys `K` of shape `m×d`, and
values `V` of shape `m×v`:

```text
score[j] = dot(q, K[j]) / sqrt(d)
weight[j] = exp(score[j] - max(score)) / sum_t exp(score[t] - max(score))
output[k] = sum_j weight[j] * V[j,k]
```

**Subtracting the maximum score** before exponentiation is essential to
avoid overflow in softmax for large positive scores. When all scores tie,
the weights are uniform. A single key/value pair is assigned weight 1.

Example:

```text
q = [1,1]
K = [[1,0],[0,1]]
V = [[2,4],[6,8]]
scores tie, attention weights = [0.5,0.5]
output = [4,6]
```

## Shared contract

- At least one key/value row (`m>=1`); query/key feature dimension
  `d>=1`; value dimension `v>=1`.
- Query has length `d`; keys and values must be finite real rectangular
  matrices with exactly `m` aligned rows and dimensions `d` and `v`.
- All operations use double precision. Reject NaN/infinity, dimension
  mismatch and overflowing dot products, logits, score shifts, softmax
  denominator, weighted products or output sums.
- Return a finite vector of length `v`. No attention weights are returned
  separately. There is no dropout, mask, positional bias or trainable
  parameter in this primitive.
- C receives row-major flat `keys[m*d]` and `values[m*v]`, plus output
  `[v]`. The caller guarantees sufficient buffer lengths and disjoint
  input/output arrays; the output remains unchanged on error.
  Rust uses row-major flat arrays; Python accepts nested lists/tuples;
  R uses base numeric matrices with rows corresponding to keys/values.
- Large inputs with mathematically finite results may still be rejected
  if intermediate double-precision arithmetic overflows. This strict
  reference prioritizes visible failures over hidden numerical errors.

## Complexity

`O(m*d + m*v)` time; O(m+v) auxiliary memory for softmax scores and
the output in C/Rust. Python also copies all inputs during validation,
requiring O(m*(d+v)) memory. R uses O(m+v) explicit scratch space
excluding matrix input copies.

## Implementations

| Language | Folder |
| --- | --- |
| C | [scaled_dot_product_attention_c](scaled_dot_product_attention_c/) |
| Python | [scaled_dot_product_attention_python](scaled_dot_product_attention_python/) |
| Rust | [scaled_dot_product_attention_rust](scaled_dot_product_attention_rust/) |
| R | [scaled_dot_product_attention_r](scaled_dot_product_attention_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover a known softmax-weighted result, exactly tied scores, one key,
extreme scores with stable softmax, invalid dimensions, non-finite data
and overflowing intermediate dot products.

## Limitations

Not a full Transformer, multi-query attention, causal masking, multi-head
attention, flash-attention, cache, batching, learned projections or GPU
implementation. Computing many queries with independent calls may be far
slower than a vectorized implementation. GPU acceleration must be tested
on NVIDIA hardware before making performance claims.

## References

Vaswani, A., Shazeer, N., Parmar, N., Uszkoreit, J., Jones, L., Gomez,
A. N., Kaiser, Ł., & Polosukhin, I. (2017). Attention is all you need.
In *Advances in Neural Information Processing Systems* (Vol. 30,
pp. 5998–6008).
https://proceedings.neurips.cc/paper_files/paper/2017/hash/3f5ee243547dee91fbd053c1c4a845aa-Abstract.html
