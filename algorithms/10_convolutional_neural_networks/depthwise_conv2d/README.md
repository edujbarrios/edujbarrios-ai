# Depthwise 2D Convolution with Same Padding

## CNN context

Depthwise convolution filters each input feature channel separately,
rather than mixing every input channel into each output channel.
It is a central building block of depthwise-separable convolutions
in lightweight CNNs (Howard et al., 2017).

## Algorithm

Input is a flat **CHW** tensor `X` with `C` channels, `H`
rows and `W` columns. Kernels are flat `C,KH,KW` in
channel-first order. With odd kernel sizes and zero padding:

```text
pad_y = floor(KH/2), pad_x = floor(KW/2)
Y[c,r,s] = sum_{i=0..KH-1} sum_{j=0..KW-1}
           X[c,r+i-pad_y,s+j-pad_x] * K[c,i,j]
```

Out-of-image values are **zero**. CNN-style cross-correlation
is used: kernels are not reversed. Stride is one and the output
shape is the same as the input.

Example: for `C=2,H=W=2`, input
`[1,2,3,4, 5,6,7,8]`, and two 3×3 kernels
with a center value of +1 and -1 respectively, output
is `[1,2,3,4, -5,-6,-7,-8]`.

## Contract

- `C=1..8`, `H,W=1..16`, and **odd** `KH,KW=1,3,5`.
- Image and kernel are flat numeric vectors of exact lengths
  `C*H*W` and `C*KH*KW`, respectively.
- Elements are finite and in `[-1000,1000]`; negative
  values and kernels larger than the spatial image are allowed.
- Result is an equally sized CHW output tensor.
- C takes a distinct caller-owned `C*H*W` output buffer,
  which remains unchanged on validation failure.
  Python rejects boolean numeric inputs; Rust returns
  `Result<Vec<f64>,&str>`, and R a numeric vector.

## Complexity

**O(CHW*KH*KW)** time and **O(CHW)** output memory.
The C reference uses O(1) additional scratch space.

## Implementations and tests

| Language | Directory |
| --- | --- |
| C | [depthwise_conv2d_c](depthwise_conv2d_c/) |
| Python | [depthwise_conv2d_python](depthwise_conv2d_python/) |
| Rust | [depthwise_conv2d_rust](depthwise_conv2d_rust/) |
| R | [depthwise_conv2d_r](depthwise_conv2d_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover independent channel kernels, all-ones kernels,
singleton images, odd dimensions and invalid/nonfinite values.

## Limitations

No pointwise (1×1) channel mixing, batch dimension, bias,
activation, gradient, dilation, alternative stride or GPU kernel.
This forward primitive alone is not a complete MobileNet block.

## References

- Howard, A. G., et al. (2017). *MobileNets: Efficient convolutional neural networks for mobile vision applications* [Preprint]. arXiv. https://arxiv.org/abs/1704.04861
