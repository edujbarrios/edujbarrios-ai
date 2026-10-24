# Rotary Positional Embedding (RoPE)

## Why it belongs in the AI hub

Rotary Positional Embedding (RoPE) encodes token positions by rotating
pairs of query/key coordinates before computing attention scores.
Relative phase differences naturally enter query-key dot products.
This is a widely used alternative to adding sinusoidal position vectors
and complements this repository's attention and positional-encoding
primitives (Su et al., 2021).

This is a **single query/key pair** reference implementation. It
does not construct token embeddings or perform an attention softmax.

## Mathematics

For an even-dimensional vector with `d` coordinates, zero-based
positions `p_q` and `p_k`, and coordinate pair `i=0,...,d/2-1`:

```text
theta_i(p) = p / 10000^(2*i/d)

rot_p([a,b]) =
    [a*cos(theta_i(p)) - b*sin(theta_i(p)),
     a*sin(theta_i(p)) + b*cos(theta_i(p))]
```

Rotate each consecutive pair `(2*i,2*i+1)` independently, using the
query position for `q` and key position for `k`.

Example with dimension 2:

```text
q=[1,0],  query_position=1 -> q_rot=[cos(1),sin(1)]
k=[0,1],  key_position=0   -> k_rot=[0,1]
dot(q_rot,k_rot)=sin(1)
```

Rotating both vectors by the same position preserves their dot product
(up to floating-point rounding). Different positions produce relative
angular phases.

## Contract

- Both query and key must contain **exactly the same even dimension**
  `d` in `[2,1024]`; the two inputs must contain only finite reals.
- Each position must be an **integer** in `[0,1_000_000]`.
  This numerical bound is a documented educational limit, not a model
  context-length recommendation.
- Return two vectors, the rotated query and key, each of length `d`.
  For C and Rust, both are flat vectors in original coordinate order.
- Reject malformed dimensions, nonfinite inputs, invalid positions,
  and any nonfinite rotated coordinate. All calculations use
  double-precision trigonometric functions.
- C takes caller-allocated `q_out[d]` and `k_out[d]`, with output
  untouched on any failure; inputs and outputs must not overlap.
  Python/R raise errors. Rust returns `Result<(Vec<f64>,Vec<f64>),...>`.
- The implementation uses the **interleaved adjacent-pair convention**
  for RoPE. Some production models use a different coordinate layout
  or frequency base; those implementations are not interchangeable
  without translating their weights and positions.

## Complexity

**O(d)** time and **O(d)** output storage. C uses O(d) temporary space
to guarantee that output buffers remain unchanged on error. Python,
Rust and R allocate the vectors they return.

## Implementations

| Language | Folder |
| --- | --- |
| C | [rotary_positional_embedding_c](rotary_positional_embedding_c/) |
| Python | [rotary_positional_embedding_python](rotary_positional_embedding_python/) |
| Rust | [rotary_positional_embedding_rust](rotary_positional_embedding_rust/) |
| R | [rotary_positional_embedding_r](rotary_positional_embedding_r/) |

## Examples and tests

```text
RoPE(q=[1,0], k=[0,1], query_pos=1, key_pos=0)
    -> ([cos(1),sin(1)], [0,1])

RoPE(q=[3,4], k=[5,6], query_pos=0, key_pos=0)
    -> ([3,4], [5,6])
```

Run from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests exercise zero positions, different query/key phases,
four-dimensional frequency pairing, dot-product invariance when
positions match, malformed dimensions and overflowing rotations.

## Limitations

No batching, attention mask, learnable weights, attention scores,
frequency scaling, partial rotary dimensions, KV-cache, interleaving
conversion, NTK-aware extrapolation, or GPU kernels. This reference
does not imply that RoPE solves long-context generalization.
CUDA requires independent device testing and performance measurements.

## References

Su, J., Lu, Y., Pan, S., Murtadha, A., Wen, B., & Liu, Y. (2021).
*RoFormer: Enhanced transformer with rotary position embedding*
[Preprint]. arXiv. https://arxiv.org/abs/2104.09864
