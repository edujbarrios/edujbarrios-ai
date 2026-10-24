# Causal Self-Attention over a Token Sequence

## Motivation

An autoregressive Transformer processes an ordered sequence using
**causal self-attention**: the representation of token `i` may depend
on tokens `0..i`, but **never on future tokens**. This lower-triangular
mask prevents future-token information leakage and is a foundation of
decoder-only language models (Vaswani et al., 2017).

This implementation computes scaled dot-product attention for **all
sequence positions**, one head and no learned Q/K/V projections. It
extends the repository's single-query `masked_attention` primitive by
forming the causal mask implicitly for each query row.

## Equations

For a sequence of length `T >= 1`, query/key matrices of shape `T×d`,
and value matrix of shape `T×v`:

```text
score[i,j] = dot(Q[i], K[j]) / sqrt(d), for j <= i
score[i,j] = masked,                     for j > i

prob[i,j] = exp(score[i,j] - max_{j<=i}(score[i,j])) /
            sum_{r<=i} exp(score[i,r] - max_{j<=i}(score[i,j]))

output[i,k] = sum_{j<=i} prob[i,j] * V[j,k]
```

The maximum subtraction keeps exponentiation stable for large logits.
Only the prefix `j<=i` is read for the attention arithmetic. All input
elements, including those that happen to be in future positions for
earlier queries, are validated as finite.

Example when every query is zero (all allowed scores are tied):

```text
Q = [[0], [0], [0]]
K = [[1], [2], [3]]
V = [[2,4], [6,8], [10,12]]

output[0] = [2,4]
output[1] = [4,6]    # average of first two values
output[2] = [6,8]    # average of first three values
```

## Contract

- `T >= 1`, `d >= 1`, `v >= 1`.
- `Q` and `K` are finite real matrices of exactly `T×d`; `V` is
  a finite real matrix of exactly `T×v`. No missing data or NaNs.
- Output is a matrix `T×v`, with each row based **only on prior and
  current value rows**, following the formula.
- Reject dimension mismatch, NaN/inf, overflow in dot-product terms or
  sums, score shifts, exponentiation denominator, weighted products or
  output sums; do not silently conceal numerical problems with masking.
- C uses caller-owned flattened **row-major** `Q[T*d]`, `K[T*d]`,
  `V[T*v]` and `output[T*v]`, all disjoint and correctly sized.
  C keeps output unchanged on any failure by using scratch output.
- Rust similarly uses flat row-major matrices and
  `Result<Vec<f64>, ...>`. Python accepts nested lists/tuples and
  returns nested lists; R accepts base numeric matrices and returns
  a base matrix. Python/R raise errors.
- There is **no dropout, batch dimension, multi-head projection,
  positional embedding, learned weight matrix, KV caching or backward
  pass** in this reference. It does not implement a full Transformer.

## Complexity

Exactly `sum_{i=0}^{T-1}(i+1)` attended key positions.

- **Time:** `O(T^2 * (d + v))`.
- **Auxiliary memory:** O(T + T*v) in C and Rust for scores and output;
  Python additionally copies validated inputs, O(T*(d+v)).
- This is a clear quadratic CPU reference, not a fused FlashAttention
  kernel. No GPU speed claim is made.

## Implementations

| Language | Directory |
| --- | --- |
| C | [causal_attention_c](causal_attention_c/) |
| Python | [causal_attention_python](causal_attention_python/) |
| Rust | [causal_attention_rust](causal_attention_rust/) |
| R | [causal_attention_r](causal_attention_r/) |

## Verification

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover uniform causal prefixes, single-token attention, large-score
stable softmax, invalid shapes/values, and overflowing dot products.

## Limitations

Quadratic compute and a non-fused CPU implementation make this unsuitable
for long contexts. The mask is strictly lower-triangular including the
diagonal; no cross-attention, padding mask, custom attention bias, sliding
window, KV cache, rotary embedding or multihead transformer logic.
CUDA support requires independently tested GPU implementations.

## References

Vaswani, A., Shazeer, N., Parmar, N., Uszkoreit, J., Jones, L., Gomez,
A. N., Kaiser, Ł., & Polosukhin, I. (2017). Attention is all you need.
In *Advances in Neural Information Processing Systems* (Vol. 30,
pp. 5998–6008).
https://proceedings.neurips.cc/paper_files/paper/2017/hash/3f5ee243547dee91fbd053c1c4a845aa-Abstract.html
