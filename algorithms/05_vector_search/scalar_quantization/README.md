# Uniform Scalar Quantization (8-bit)

## Why it belongs in the AI hub

Vector search engines can compress embedding values to reduce memory use
and support faster approximate comparisons. Uniform scalar quantization
provides an understandable first step toward compressed vector storage.
Low-precision numeric representation is also central to efficient neural
network inference (Jacob et al., 2018). This example is a **standalone
reference quantizer**, not a complete vector index or the specific method
implemented in that paper.

## Definition

Given a **fixed** finite range `minimum < maximum`, map each value `x`
to an integer bin:

```text
clamped = min(max(x, minimum), maximum)
scaled  = 255 * ((clamped - minimum) / (maximum - minimum))
q       = floor(scaled + 0.5)  # half-up rounding
q       = min(q, 255)
```

The result is an unsigned 8-bit integer in `[0, 255]`. Values below and
above the calibration range **saturate** to 0 or 255. Examples for
`minimum=0` and `maximum=1`:

```text
0.0 -> 0
0.5 -> 128
1.0 -> 255
-3.0 -> 0
 8.0 -> 255
```

## Contract and numeric edge cases

- Accept an array of finite real numbers, including the empty vector.
- Both range endpoints must be finite, with `minimum < maximum` and
  representable positive span `maximum - minimum`.
- Reject non-finite input components and invalid bounds even if a component
  would otherwise saturate.
- The calibration range is **provided by the caller**, not inferred from
  each vector. Use the same bounds for vectors you intend to compare.
- C uses caller-owned double inputs and `uint8_t` outputs, validates the
  entire input before writing, and supports `n=0`; its caller must
  guarantee buffer sizes. Python/R raise errors and return integer vectors.
  Rust returns `Result<Vec<u8>, ...>`.
- R uses base numeric vectors, rejecting non-finite and complex inputs.
- The formula uses division before multiplying by 255, preventing overflow
  from `255 * difference`. Rounding is half-up, not ties-to-even.

## Complexity

**O(n)** time. C uses **O(1)** auxiliary memory outside caller-provided
buffers; Python, Rust and R allocate an O(n) output.

## Implementations

| Language | Folder |
| --- | --- |
| C | [scalar_quantization_c/](scalar_quantization_c/) |
| Python | [scalar_quantization_python/](scalar_quantization_python/) |
| Rust | [scalar_quantization_rust/](scalar_quantization_rust/) |
| R | [scalar_quantization_r/](scalar_quantization_r/) |

CUDA is not included in this first implementation. A CUDA variant needs
GPU hardware tests and measured transfer overhead.

## Testing

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover endpoints, midpoint rounding, saturation, empty vectors, input
validation, degenerate calibration ranges, and overflow in the range span.

## Limitations

Quantization is **lossy** and may change rankings or cosine similarity.
This example omits dequantization, product quantization, per-channel scales,
zero-point affine mapping, dynamic calibration, SIMD and index integration.
It makes no compression-throughput or retrieval-recall claim.

## References

Jacob, B., Kligys, S., Chen, B., Zhu, M., Tang, M., Howard, A., Adam, H.,
& Kalenichenko, D. (2018). Quantization and training of neural networks for
efficient integer-arithmetic-only inference. In *Proceedings of the IEEE
Conference on Computer Vision and Pattern Recognition* (pp. 2704–2713).
https://arxiv.org/abs/1712.05877
