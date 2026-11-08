# Grayscale Histogram Equalization

## Computer vision context

Histogram equalization spreads the occupied grayscale levels according to their cumulative histogram, improving global contrast in many low-contrast images (Gonzalez & Woods, 2018). This implementation uses a deterministic 8-bit mapping shared by four languages.

## Mathematical definition

For an image of `N=H×W` pixels with intensities `0..255`, let `hist[v]` count value `v`, and `cdf[v] = sum(hist[t] for t <= v)`. Let `cdf_min` be the first **positive** CDF value (the count of the lowest occupied intensity).

For images with **more than one distinct intensity**:

```text
mapped[v] = floor(255 * (cdf[v] - cdf_min) / (N - cdf_min) + 0.5)
```

This is rounded to the nearest integer, with half ties **up**. The lowest present intensity maps to 0 and the highest to 255. For a **constant image**, return the input unchanged (avoid a zero denominator). Output has the same shape, flattened in row-major order.

## Contract

- Input: a flat row-major grayscale image of exactly `height*width` values, with integer dimensions **1..64** and integer pixels **0..255**. Python rejects booleans and floats; R rejects logical/nonfinite/complex/noninteger pixels and array dimensions.
- C accepts `const uint8_t *pixels`, `size_t height,width`, and a distinct caller-owned `uint8_t *output` of length `H*W`. Reject invalid dimensions/null pointers without changing output; the input type guarantees 0–255.
- Python returns `list[int]`, Rust `Result<Vec<u8>,&str>`, R an integer vector; no input mutation.
- No contrast-limited adaptive histogram equalization (CLAHE), RGB processing, or per-channel operation.

## Complexity

Building the 256-bin histogram and prefix sums and mapping each pixel takes **O(HW + 256)** time; storage is **O(256 + HW)** including output. C needs O(256) auxiliary memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [grayscale_histogram_equalization_c](grayscale_histogram_equalization_c/) |
| Python | [grayscale_histogram_equalization_python](grayscale_histogram_equalization_python/) |
| Rust | [grayscale_histogram_equalization_rust](grayscale_histogram_equalization_rust/) |
| R | [grayscale_histogram_equalization_r](grayscale_histogram_equalization_r/) |

## Examples and tests

```text
equalize([10,10,20,30], 2, 2) = [0,0,128,255]
equalize([20,20,20], 1, 3)    = [20,20,20]
equalize([0,255], 1, 2)       = [0,255]
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`. Tests cover constant pixels, rounding ties, singletons, full 64×64 images, malformed shapes, and invalid intensities.

## Limitations

Global equalization can amplify noise and harm locally uniform regions. This uses a histogram of the entire image, not tiles, and intentionally leaves constant images unchanged.

## References

Gonzalez, R. C., & Woods, R. E. (2018). *Digital image processing* (4th ed.). Pearson.
