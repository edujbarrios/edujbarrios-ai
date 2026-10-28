# Valid 2D Convolution (CNN Cross-Correlation)

## Purpose

Spatial convolutions are the fundamental primitive of CNNs. Most CNN
libraries actually implement a *cross-correlation*: the kernel is not
flipped (Goodfellow et al., 2016).

## Definition

For a single-channel input `X` of height `H` and width `W` and
kernel `K` of height `KH` and width `KW`:

```text
Y[r,c] = sum_{i=0..KH-1} sum_{j=0..KW-1} X[r+i,c+j] * K[i,j]
Hout = H-KH+1; Wout = W-KW+1
```

Inputs and outputs are **flat row-major** arrays. For
`X=[1,2,3,4,5,6,7,8,9]` with `H=W=3` and
`K=[1,0,0,-1]` with `KH=KW=2`, output is `[-4,-4,-4,-4]`.

## Contract

- `1<=H,W<=32`; `1<=KH,KW<=8` and kernel dimensions must
  not exceed the image dimensions.
- Image and kernel lengths must equal `H*W` and `KH*KW`,
  respectively. Every element is finite and in `[-1000,1000]`.
- Return `Hout*Wout` real numbers in row-major order.
  No padding, stride larger than 1, channel dimension, bias,
  activation, or flipped kernel is provided.
- C accepts pointers to correctly sized image, kernel and distinct
  output buffers; output remains unchanged on validation failure.
  Python raises `ValueError` for invalid inputs and rejects
  booleans. Rust uses `Result<Vec<f64>,&str>`; R raises errors.

## Complexity

**O(Hout*Wout*KH*KW)** time and **O(Hout*Wout)** output space,
with constant auxiliary memory in C.

## Testing

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` from repository root.

| Language | Reference |
| --- | --- |
| C | [valid_conv2d_c](valid_conv2d_c/) |
| Python | [valid_conv2d_python](valid_conv2d_python/) |
| Rust | [valid_conv2d_rust](valid_conv2d_rust/) |
| R | [valid_conv2d_r](valid_conv2d_r/) |

Tests check a hand-computed 2×2 kernel, 1×1 kernels, negative
values, rectangular inputs and invalid sizes/nonfinite values.

## Limitations

Single-channel educational forward operation only; real CNN layers
commonly use batched multi-channel tensors, bias, strides, dilation,
padding, backpropagation and optimized SIMD/GPU kernels.

## References

- Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*. MIT Press. https://www.deeplearningbook.org/
