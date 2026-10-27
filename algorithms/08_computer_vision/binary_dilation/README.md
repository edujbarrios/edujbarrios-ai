# 3×3 Binary Image Dilation

## Computer vision context

Morphological dilation expands the foreground of a binary image with
a structuring element. It is useful in segmentation postprocessing,
connecting nearby mask regions and filling small gaps (Soille, 2003).

## Mathematical definition

For an input image `P` of height `H` and width `W`, return a
binary image `D` with the same shape:

```text
D[r,c] = max P[r+dr,c+dc]
         for dr,dc in {-1,0,1} x {-1,0,1}
```

Coordinates outside the image are defined as **zero**. The 3×3
square structuring element includes the central pixel.
The output is flattened in row-major order.

Examples:
- A foreground pixel in the middle of a 3×3 image dilates
  to all nine foreground pixels.
- A foreground pixel at the upper-left corner becomes
  `[1,1,0, 1,1,0, 0,0,0]`.
- A zero image remains zero.

## Contract

- Input is a flat row-major image of exactly `H*W` values.
  Both dimensions are integer values in `1..64`.
- All values must be integers 0 or 1. Invalid shapes, unexpected
  intensities, nonfinite or missing values are rejected.
  Python rejects booleans and floating-point pixels.
- The output is a new flat array with exactly `H*W` binary values.
  Single-row, single-column and single-pixel images are supported.
- C accepts a caller-owned distinct output buffer of `H*W`
  bytes, which remains unchanged if validation fails.
  Rust returns `Result<Vec<u8>,&str>`; Python returns a list
  and R an integer vector.
- Border handling is **zero padding**, not edge replication.
  No grayscale dilation, arbitrary kernel or iterations are included.

## Complexity

**O(9HW) = O(HW)** time and **O(HW)** output storage.
The C implementation uses constant additional scratch space.

## Implementations

| Language | Folder |
| --- | --- |
| C | [binary_dilation_c](binary_dilation_c/) |
| Python | [binary_dilation_python](binary_dilation_python/) |
| Rust | [binary_dilation_rust](binary_dilation_rust/) |
| R | [binary_dilation_r](binary_dilation_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover a single center/corner pixel, zeros, singleton images,
narrow images, and malformed binary masks or dimensions.

## Limitations

Dilation can merge separate objects and enlarge noise. This algorithm
does not perform erosion, opening/closing, connected-component labeling,
learned segmentation or GPU acceleration.

## References

- Soille, P. (2003). *Morphological image analysis: Principles and applications* (2nd ed.). Springer. https://doi.org/10.1007/978-3-662-05088-0
