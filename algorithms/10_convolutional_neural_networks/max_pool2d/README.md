# 2×2 Max Pooling for CNNs

## Purpose

Max pooling is a spatial downsampling operator used in conventional
CNN architectures. It selects the largest activation within each
local receptive field, reducing spatial resolution (Goodfellow
et al., 2016).

## Algorithm

For an input single-channel feature map `X` of shape `H×W`,
this reference uses a fixed **2×2 window with stride 2** and no
padding or overlap:

```text
Y[r,c] = max(X[2r,2c], X[2r,2c+1],
             X[2r+1,2c], X[2r+1,2c+1])
Hout = floor(H/2); Wout = floor(W/2)
```

Both inputs and outputs are flat in row-major order. Any final
unpaired row or column is dropped. For
`X=[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]` with
`H=W=4`, output is `[6,8,14,16]`.

## Contract

- `2<=H,W<=32` as integer dimensions.
- Exactly `H*W` finite real values in `[-1000,1000]`.
  Negative activations are allowed: the max is not clamped to zero.
- Output length is `floor(H/2)*floor(W/2)`.
- C takes a distinct caller-provided result buffer of that
  length, left unchanged if input validation fails.
  Python/R raise exceptions and Rust returns `Result<Vec<f64>,&str>`.
- Incomplete border windows are skipped. No padding, overlap,
  configurable strides, channel/batch dimensions or argmax
  indices are included.

## Complexity

**O(HW)** time and **O(floor(H/2)*floor(W/2))** output memory.
C uses O(1) scratch beyond the output.

## Implementations and tests

| Language | Reference |
| --- | --- |
| C | [max_pool2d_c](max_pool2d_c/) |
| Python | [max_pool2d_python](max_pool2d_python/) |
| Rust | [max_pool2d_rust](max_pool2d_rust/) |
| R | [max_pool2d_r](max_pool2d_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover square/rectangular maps, odd dimensions,
all-negative inputs, nonfinite values and invalid shapes.

## Limitations

No gradient (argmax) bookkeeping, average pooling, adaptive pooling,
GPU execution or multi-channel tensors. Max pooling discards
fine-grained spatial information.

## References

- Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*. MIT Press. https://www.deeplearningbook.org/
