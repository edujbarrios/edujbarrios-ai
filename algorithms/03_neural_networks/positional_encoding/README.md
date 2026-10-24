# Sinusoidal Positional Encoding

## AI motivation

Self-attention alone has no built-in awareness of token ordering.
The original Transformer supplied **sinusoidal positional encodings**
to its token embeddings so the model could use position information
without learning a separate lookup table (Vaswani et al., 2017).
This reference implements the deterministic position matrix independently
of tokenization, embedding lookup or multi-head attention.

## Mathematical definition

For zero-based position `p` and feature dimension `j`, let
`i=floor(j/2)`. Define

```text
angle(p,j) = p / 10000 ** (2*i/d)

PE[p,j] = sin(angle(p,j))  if j is even
PE[p,j] = cos(angle(p,j))  if j is odd
```

Here `d` is the width of the output embedding. The even/odd pairing
is defined even for odd `d`: the final even index simply lacks a
matching cosine column.

Example with `length=2`, `d=4`:

```text
PE[0,:] = [0,1,0,1]
PE[1,:] = [sin(1),cos(1),sin(0.01),cos(0.01)]
```

The output is a matrix of **L rows × d columns**. It is typically
**added** to a separate sequence of token embeddings of the same shape.
This algorithm returns the encoding only, without modifying embeddings.

## Shared contract

- `length` (number of positions) is an integer in **[1,4096]**;
  embedding width `d` is an integer in **[1,1024]**.
  These upper bounds keep the educational implementation's allocations
  and double-precision position conversion predictable.
- The first row always corresponds to position zero. Output contains
  only finite values in [-1,1], calculated in double precision.
- For C and Rust, the output is flattened **row-major** (`length*d`)
  with element at `p*d+j`. Python returns a nested list; R returns a
  numeric matrix with `length` rows and `d` columns.
- C requires a caller-owned output array with at least `length*d`
  doubles. On invalid dimensions the function returns `false` without
  writing the array. Python/R raise errors; Rust returns a `Result`.
- Inputs are integer counts, **not** a vector of arbitrary positions;
  there is no custom starting offset, negative positions or padding
  mask. All languages use the same zero-based mathematical coordinates.

## Complexity

**O(length*d)** time and **O(length*d)** output storage.
C computes in place in caller-allocated output with O(1) additional
memory. Rust/Python/R allocate the matrix they return.

## Implementations

| Language | Path |
| --- | --- |
| C | [positional_encoding_c](positional_encoding_c/) |
| Python | [positional_encoding_python](positional_encoding_python/) |
| Rust | [positional_encoding_rust](positional_encoding_rust/) |
| R | [positional_encoding_r](positional_encoding_r/) |

Run the repository's strict CPU tests:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the known positions, odd widths, single features, dimensions
outside the accepted range and finite/bounded results. Floating-point
functions vary slightly between platforms, so tests use tolerances.

## Limitations

No learned positional embeddings, rotary positions (RoPE), ALiBi,
relative position bias, token embeddings, causal masking or
trainable Transformer. Classical sinusoids have limitations for long
context extrapolation and are not always used in current architectures.
No CUDA performance claim is made.

## References

Vaswani, A., Shazeer, N., Parmar, N., Uszkoreit, J., Jones, L., Gomez,
A. N., Kaiser, Ł., & Polosukhin, I. (2017). Attention is all you need.
In *Advances in Neural Information Processing Systems* (Vol. 30).
https://proceedings.neurips.cc/paper_files/paper/2017/hash/3f5ee243547dee91fbd053c1c4a845aa-Abstract.html
