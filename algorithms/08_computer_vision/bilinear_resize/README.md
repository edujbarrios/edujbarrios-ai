# Bilinear Resize for Grayscale Images

## Why it belongs here

Resizing image tensors is fundamental in vision preprocessing and
in constructing input images for convolutional or patch-based vision
encoders. **Bilinear interpolation** estimates intermediate pixel
intensities from four neighboring samples. This reference uses the
half-pixel coordinate convention common to image-resize operations
(ONNX, n.d.).

## Mathematical definition

For a source of height `H` and width `W`, and output size
`Hout × Wout`, map each destination center `(r,c)` to a
continuous source coordinate:

```text
y = clamp((r + 0.5)*H/Hout - 0.5, 0, H-1)
x = clamp((c + 0.5)*W/Wout - 0.5, 0, W-1)
y0 = floor(y); y1 = min(y0+1, H-1); wy = y-y0
x0 = floor(x); x1 = min(x0+1, W-1); wx = x-x0
v = (1-wy)*((1-wx)*P[y0,x0] + wx*P[y0,x1])
  + wy*((1-wx)*P[y1,x0] + wx*P[y1,x1])
```

The half-pixel coordinate rule is followed by **edge clamping**.
For the 2×2 input `[0,10,20,30]` in row-major order, resizing
to 3×3 returns `[0,5,10,10,15,20,20,25,30]`.

## Contract

- Grayscale image pixels are a flat row-major real-valued array
  of length exactly `H*W`; `1<=H,W,Hout,Wout<=64`.
- Every pixel must be finite and in `[-1,000,000,1,000,000]`.
  Empty inputs, mismatched lengths and invalid dimensions fail.
  Python explicitly rejects boolean dimensions/pixels.
- Returns the new flat row-major `Hout*Wout` image. C requires
  a caller-allocated output buffer of that length, must not alias
  the input, and leaves output untouched on invalid input.
  Rust uses `Result<Vec<f64>,&str>`. Python and R raise errors.
- Single-row/column/single-pixel inputs are supported. Equal input
  and output sizes reproduce their pixel values (up to floating-point
  rounding).

## Complexity

**O(H*W + Hout*Wout)** time (input validation plus one interpolation
per destination pixel). **O(Hout*Wout)** space for the returned output;
the C version uses O(1) auxiliary scratch besides caller-owned output.

## Implementations

| Language | Directory |
| --- | --- |
| C | [bilinear_resize_c](bilinear_resize_c/) |
| Python | [bilinear_resize_python](bilinear_resize_python/) |
| Rust | [bilinear_resize_rust](bilinear_resize_rust/) |
| R | [bilinear_resize_r](bilinear_resize_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover identity sizing, enlargement, singleton inputs,
rectangular images, boundaries, invalid dimensions, nonfinite
pixels and oversized inputs.

## Limitations

Only a single-channel floating-point image is supported. Downsampling
has **no antialiasing filter**; fine details may alias. This does
not implement bicubic resampling, letterboxing, aspect ratio
preservation, batched tensor operations or GPU kernels.

## References

- ONNX. (n.d.). *Resize - 19*. https://onnx.ai/onnx/operators/onnx__Resize.html
