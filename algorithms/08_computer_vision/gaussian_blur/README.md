# 3×3 Gaussian Blur

## Why it belongs here

A fixed 3×3 separable Gaussian approximation smooths high-frequency image variation before feature extraction. Unlike a box average, it weights the center most heavily (Gonzalez & Woods, 2018).

## Mathematical definition

For each pixel, compute the weighted sum of its 3×3 neighborhood with

```text
1 2 1
2 4 2   / 16
1 2 1
```

Coordinates outside the image replicate the nearest edge pixel. This fixed
binomial kernel approximates a Gaussian blur; it does **not** accept a tunable
sigma. For the 3×3 input with value 16 at its center and zero elsewhere,
the 3×3 output is `[1,2,1,2,4,2,1,2,1]` (row-major).

## Contract

- Inputs: a flat row-major grayscale vector of exactly `height * width`
  real-valued pixels and integer dimensions `height` and `width` in `1..64`.
- Each pixel must be finite and in `[-1,000,000, 1,000,000]`.
  Empty/mismatched input, nonfinite/out-of-range values, invalid dimensions and
  non-vector input are rejected. Python also rejects boolean pixels/dimensions.
- Returns a flat row-major vector of the same size; 1×1, 1×N and N×1
  images are supported. The border rule is edge replication.
- C returns `false` on invalid inputs, otherwise fills a separate
  caller-allocated output array of `height * width` doubles. The output
  must not alias input, and is unchanged on validation failure.
  Python and R raise errors. Rust returns `Result<Vec<f64>, &str>`.
- Small floating-point differences between platforms are possible; tests
  use representable reference values.

## Complexity

Time: **O(HW)** (nine samples and a weighted sum per pixel).
Output space: **O(HW)** for Python, Rust and R. C uses **O(1)** additional
storage beyond the caller-owned output array.

## Implementations

| Language | Directory |
| --- | --- |
| C | [gaussian_blur_c](gaussian_blur_c/) |
| Python | [gaussian_blur_python](gaussian_blur_python/) |
| Rust | [gaussian_blur_rust](gaussian_blur_rust/) |
| R | [gaussian_blur_r](gaussian_blur_r/) |

## Examples and tests

The examples above and the tests verify an impulse, a single-row image,
single pixels, constant values, replicated borders, invalid dimensions,
wrong lengths, nonfinite pixels and out-of-range inputs.

From the repository root:

```sh
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

This is a single fixed-radius approximation, not a configurable Gaussian sigma, a bilateral filter, or an antialiased downsampler. It blurs sharp edges and uses no GPU acceleration.

## References

- Gonzalez, R. C., & Woods, R. E. (2018). *Digital image processing*
  (4th ed.). Pearson.
  https://www.mathworks.com/academia/books/digital-image-processing-gonzalez.html
