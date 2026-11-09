# Same-Padded 2D Convolution (Cross-Correlation)

## CNN context

A convolutional layer typically uses *cross-correlation*: the learned kernel is **not flipped**. Same padding preserves the input's height and width, making it useful in feature extractors and skip-connected neural networks (Goodfellow et al., 2016).

## Mathematical definition

Given a single-channel image `X` of size H×W and an **odd-height and odd-width** kernel `K` of size KH×KW, calculate every output in row-major order:

```text
Y[r,c] = sum_{a=0..KH-1} sum_{b=0..KW-1}
         X_zero[r+a-floor(KH/2),c+b-floor(KW/2)] * K[a,b]

X_zero[row,col] = X[row,col]  if 0 <= row < H and 0 <= col < W
                = 0           otherwise
```

The kernel's center aligns with each output location, the stride is one, and **output dimensions are exactly H×W** even if the kernel is larger than a small image. The algorithm is **cross-correlation**, following CNN library convention, not a mathematically flipped convolution.

## Shared contract

- Image shape: **1<=H,W<=32**. Kernel shape: **odd dimensions 1,3,5** in either axis, independently; `KH` may exceed `H` (and similarly for width) because zero padding handles out-of-bounds positions.
- Flat row-major image of length `H*W` and flat row-major kernel of length `KH*KW`. Every value must be a finite real number in **[-1000,1000]**.
- Return an H×W row-major output of finite doubles. The input and kernel are not mutated. No activation, bias, batch, channel dimension, stride other than one, or dilation.
- C accepts `const double *image`, `size_t height,width`, `const double *kernel`, `size_t kh,kw`, caller-owned **distinct** `double *output` of length `H*W`, returning `false` without changing output on invalid inputs.
- Python accepts list/tuple flat arrays with integer dimensions and rejects booleans, invalid types and nonfinite values. Rust returns `Result<Vec<f64>,&str>`; R returns a numeric vector and rejects logical, complex, array and missing-value inputs.

## Complexity

Direct evaluation costs **O(HW KH KW)** time and **O(HW)** output memory, with constant additional working storage in C. Maximum image size is 1024 and maximum kernel size is 25.

## Examples

```text
3×3 image:      1 2 3        3×3 ones kernel:
                4 5 6        1 1 1
                7 8 9        1 1 1
                             1 1 1

zero-padded same result: 12 21 16
                         27 45 33
                         24 39 28

1×3 image [1,2,3] with 1×3 kernel [1,2,3] => [8,14,8]
1×1 image [5] with 3×3 ones => [5]
```

## Implementations and tests

| Language | Directory |
| --- | --- |
| C | [same_conv2d_c](same_conv2d_c/) |
| Python | [same_conv2d_python](same_conv2d_python/) |
| Rust | [same_conv2d_rust](same_conv2d_rust/) |
| R | [same_conv2d_r](same_conv2d_r/) |

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` in the repository root. Tests cover border padding, 1×1 images, 1×1 kernels, non-square kernels, negative inputs, maximum dimensions and invalid/nonfinite data.

## Limitations

This direct single-channel CPU reference is not optimized for production CNNs. There are no multi-channel filters, dilation, striding, grouped convolution, automatic differentiation or GPU kernels. The output is only a forward cross-correlation result.

## References

Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*. MIT Press. https://www.deeplearningbook.org/
