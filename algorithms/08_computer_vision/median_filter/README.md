# 3×3 Median Filter

## Why it belongs here

A 3×3 median filter suppresses isolated impulse noise without averaging it into neighboring pixels. The nonlinear order-statistic operation is frequently used before segmentation and edge detection (Gonzalez & Woods, 2018).

## Mathematical definition

For each pixel, collect the 3×3 neighborhood (nine samples, with
out-of-range coordinates **replicated** from the nearest border), sort the nine
numbers and select the fifth (the median). For the 3×3 input with value 16
at its center and zero elsewhere, the output is nine zeros. For the
single-row input `[0,0,8]`, the result is `[0,0,8]`.

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

Time: **O(HW)** (nine samples and a nine-value insertion sort per pixel).
Output space: **O(HW)** for Python, Rust and R. C uses **O(1)** additional
storage beyond the caller-owned output array.

## Implementations

| Language | Directory |
| --- | --- |
| C | [median_filter_c](median_filter_c/) |
| Python | [median_filter_python](median_filter_python/) |
| Rust | [median_filter_rust](median_filter_rust/) |
| R | [median_filter_r](median_filter_r/) |

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

This is only a fixed 3×3 median: it does not implement larger windows, adaptive median filtering, percentile selection, or GPU acceleration. A median can remove thin structures along with noise.

## References

- Gonzalez, R. C., & Woods, R. E. (2018). *Digital image processing*
  (4th ed.). Pearson.
  https://www.mathworks.com/academia/books/digital-image-processing-gonzalez.html
