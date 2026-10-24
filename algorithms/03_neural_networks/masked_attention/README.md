# Masked Scaled Dot-Product Attention (One Query)

## Why it belongs in the AI hub

Transformers use **attention masks** to control which tokens a query can
attend to. Padding masks exclude irrelevant positions; causal masks block
future positions during autoregressive decoding. This reference extends
the repository's `scaled_dot_product_attention` algorithm with a
caller-supplied mask (Vaswani et al., 2017).

It computes **one query and one attention head**, not a full Transformer,
and takes an explicit per-key binary mask rather than deriving a causal
mask from token positions.

## Formula

For query `q ∈ R^d`, `m` key rows `K[j] ∈ R^d`, value rows
`V[j] ∈ R^v`, and `mask[j] ∈ {0,1}`:

```text
allowed = {j : mask[j] == 1}
score[j] = dot(q, K[j]) / sqrt(d)      for j in allowed
maximum = max(score[j] for j in allowed)
weight[j] = exp(score[j] - maximum) / sum_t exp(score[t] - maximum)
output[i] = sum(j in allowed) weight[j] * V[j,i]
```

Excluded rows get effective weight zero. **At least one mask element
must equal 1**; all-zero masks are rejected instead of producing NaNs.
The max-shifted softmax avoids overflow from exponentiating large logits.

Example:

```text
query = [1,0]
keys = [[1,0], [0,1], [1,1]]
values = [[10,0], [0,20], [4,6]]
mask = [1,0,1]
scores of included keys tie
output = [7,3]
```

## Shared contract

- Query dimension `d>=1`, number of key/value rows `m>=1`, value
  dimension `v>=1`. Key matrix has shape `m×d`, value matrix `m×v`.
- The mask has length **exactly m**, with integer values strictly 0 or 1.
  At least one entry must be 1. Unlike a float additive mask, 1 here
  means **allowed**, 0 means **excluded**.
- All input arrays/matrices, including masked-out rows, must contain
  finite real values; all masks are validated. For excluded rows the
  projection is **not evaluated**, so a finite excluded row with a
  would-overflow dot product cannot affect the output.
- For allowed rows, reject any nonfinite dot product, shifted score,
  exponent, denominator, weighted value product or accumulated output.
- C accepts flattened row-major keys/values, `uint8_t mask[m]`, and
  caller-owned output of length `v`; buffers must be large enough and
  non-overlapping. C does not modify output on error. Rust also uses
  flat row-major arrays and returns `Result<Vec<f64>, ...>`. Python
  takes nested arrays and returns a list; R takes base matrices and
  returns a numeric vector. Python/R raise errors.
- Softmax ties across allowed keys produce equal weights. With exactly
  one allowed key, output equals its value row.
- The mask selects keys and values per query; it does **not** train
  query/key/value projection parameters.

## Complexity

O(m*d + a*v) arithmetic plus O(m*v) validation, where `a` is the
number of allowed keys. Working memory is O(m+v) in C/Rust; Python
additionally validates and copies both matrices (O(m*(d+v))).
R works on its input matrices with O(m+v) explicit scratch space.

## Implementations

| Language | Directory |
| --- | --- |
| C | [masked_attention_c](masked_attention_c/) |
| Python | [masked_attention_python](masked_attention_python/) |
| Rust | [masked_attention_rust](masked_attention_rust/) |
| R | [masked_attention_r](masked_attention_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` at repository root.
Tests cover tied included scores, one allowed key, all-masked error,
invalid binary masks, overflow on included keys, validation of excluded
nonfinite rows, and safely skipping finite-but-overflowing excluded
dot products.

## Limitations

No batching, causal-mask construction, multi-query/multi-head attention,
KV caching, fused kernels, dropout, learned projections, flash-attention
or backpropagation through attention. A CUDA version requires hardware
testing and explicit speed/memory measurements before inclusion.

## References

Vaswani, A., Shazeer, N., Parmar, N., Uszkoreit, J., Jones, L., Gomez,
A. N., Kaiser, Ł., & Polosukhin, I. (2017). Attention is all you need.
In *Advances in Neural Information Processing Systems* (Vol. 30,
pp. 5998–6008).
https://proceedings.neurips.cc/paper_files/paper/2017/hash/3f5ee243547dee91fbd053c1c4a845aa-Abstract.html
