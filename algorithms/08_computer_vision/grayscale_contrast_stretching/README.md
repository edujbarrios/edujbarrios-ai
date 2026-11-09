# Grayscale Contrast Stretching

## Computer vision context

Linear contrast stretching expands a grayscale image's occupied intensity range to the full 8-bit range, making low-contrast images easier to inspect or preprocess. It differs from **histogram equalization**, which uses the full cumulative histogram rather than a single linear transformation (Gonzalez & Woods, 2018).

## Algorithm

For an image `P`, set `lo = min(P)`, `hi = max(P)` and `range = hi-lo`. When `range > 0`, map each pixel `x` to:

```text
round_half_up(255 * (x-lo) / (hi-lo))
= floor((2*255*(x-lo) + (hi-lo)) / (2*(hi-lo)))
```

The integer expression avoids floating-point ties and guarantees an 8-bit result. The lowest occupied value maps to **0**, the highest to **255**. When the image is constant (`hi=lo`), return an **unchanged copy**, avoiding division by zero.

## Input/output contract

- Input: a **flat row-major** 8-bit grayscale image of exactly `H*W` pixel values, with integer dimensions `H,W` in **1..64** and integer pixel intensities **0..255**.
- Return a new flat row-major image with the same shape and pixels in **0..255**; never mutate the input.
- C takes `const uint8_t *pixels`, `size_t height,width`, a caller-owned **distinct** `uint8_t *output` of length `H*W`, and returns `bool`. It rejects null pointers or invalid dimensions **before writing**, leaving output unchanged. Its input type already constrains intensities to 0..255.
- Python returns `list[int]` and rejects booleans, floats, invalid dimensions and malformed sequences. Rust returns `Result<Vec<u8>, &str>`. R returns an integer vector and rejects invalid types, array/matrix dimensions, missing values and nonintegral inputs.
- No clipping percentage, gamma correction, RGB processing or spatially adaptive contrast methods are included.

## Complexity

Find extrema and apply the map in **O(HW)** time, using **O(HW)** output storage and **O(1)** extra workspace (C).

## Implementations

| Language | Folder |
| --- | --- |
| C | [grayscale_contrast_stretching_c](grayscale_contrast_stretching_c/) |
| Python | [grayscale_contrast_stretching_python](grayscale_contrast_stretching_python/) |
| Rust | [grayscale_contrast_stretching_rust](grayscale_contrast_stretching_rust/) |
| R | [grayscale_contrast_stretching_r](grayscale_contrast_stretching_r/) |

## Examples and tests

```text
stretch([10,10,15,20], 2, 2) = [0,0,128,255]
stretch([5,6,7,8],    1, 4) = [0,85,170,255]
stretch([99,99],      1, 2) = [99,99]
stretch([0,255],      1, 2) = [0,255]
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`. Tests cover odd-half rounding, singleton/constant images, full 64×64 dimensions, grayscale endpoints, invalid data types and shapes.

## Limitations

Min-max contrast stretching is highly sensitive to isolated extreme pixels; it does not preserve the original histogram shape or guarantee perceptual contrast improvement. This is a fixed whole-image mapping, not local or adaptive enhancement.

## References

Gonzalez, R. C., & Woods, R. E. (2018). *Digital image processing* (4th ed.). Pearson.
