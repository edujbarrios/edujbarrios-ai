# Sobel Edge Magnitude

## Use in computer vision

Sobel filtering estimates the horizontal and vertical intensity gradients
of an image. Their magnitude highlights high-contrast edges and is a
classic image-processing building block (Sobel, n.d.).

## Algorithm

Convolve each pixel with the 3-by-3 kernels

```text
Gx kernel          Gy kernel
-1  0  1           -1 -2 -1
-2  0  2            0  0  0
-1  0  1            1  2  1
```

and return `magnitude = sqrt(Gx*Gx + Gy*Gy)` for every pixel.
Out-of-range input coordinates are handled by **replicating the
nearest edge pixel**, not by injecting zeros.

For a 3×3 horizontal ramp with each row `[0,1,2]`, the output
rows are `[4,8,4]`. A constant or single-pixel image produces zeros.

## Contract

- Source grayscale pixels are a flat row-major vector of exactly
  `height * width` elements, with integer dimensions `1..64`.
- Input pixels must be finite real numbers in `[-1000,1000]`.
  Nonfinite, empty, malformed or out-of-bounds input is rejected;
  Python rejects boolean numeric values.
- Output is one nonnegative floating-point magnitude per input pixel,
  retaining the original height, width and row-major order.
- C requires a separate caller-owned output buffer of length
  `height*width`; it remains unchanged on validation failure and
  must not overlap input. Python returns a list of floats;
  Rust returns `Result<Vec<f64>,&str>`; R returns a numeric vector.
- No thresholding, non-maximum suppression or edge-direction
  output is included.

## Complexity

**O(HW)** time and **O(HW)** storage for the returned output.
C uses constant extra scratch beyond caller-provided output.

## Implementations

| Language | Directory |
| --- | --- |
| C | [sobel_edges_c](sobel_edges_c/) |
| Python | [sobel_edges_python](sobel_edges_python/) |
| Rust | [sobel_edges_rust](sobel_edges_rust/) |
| R | [sobel_edges_r](sobel_edges_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` from repository root.

Tests cover horizontal/vertical ramps, constants, singleton images,
border replication and invalid dimensions/nonfinite data.

## Limitations

This is a simple gradient operator, not Canny edge detection or a
learned convolutional layer. It does not perform denoising, gradient
orientation analysis, normalization, thresholding, or GPU processing.
Noise may produce high gradient magnitudes.

## References

- Sobel, I. (n.d.). *An isotropic 3×3 image gradient operator*. Stanford University, Artificial Intelligence Project. (Historical operator commonly referred to as the Sobel filter.)
