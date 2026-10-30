# Binary Morphological Opening (3×3)

## Why it belongs in this repository

Morphological opening removes isolated foreground pixels and small protrusions
while retaining sufficiently large structures. It is widely used as a
preprocessing step for image segmentation and object analysis. Opening is
**erosion followed by dilation** using the same structuring element
(OpenCV, n.d.).

## Algorithm

Given a row-major binary image of `width × height` pixels, first erode
using a 3×3 all-one square, then dilate the eroded image with the same
square. Pixels outside the image are considered 0 for both passes.

```text
eroded[y,x] = AND input[ny,nx] over the 3x3 window
opened[y,x] = OR eroded[ny,nx] over the 3x3 window
```

All neighbors outside image bounds contribute zero to erosion. In dilation,
only in-bounds neighbors are examined. For a 7×7 image with a centered
3×3 foreground square and one isolated foreground pixel in the top-left
corner, opening removes the isolated pixel and preserves the square.

## Contract

- Input: flattened row-major 0/1 image of dimensions
  `1 <= width,height <= 64`, with exactly `width*height` pixels.
- Output: equally sized binary row-major image after one opening.
- Invalid dimensions, length, nonbinary values or nonnumeric types are
  rejected. Python rejects bool pixels and dimensions. R accepts integer
  or numeric pixels in {0,1}, not logicals.
- C accepts caller-owned input/output of at least width*height bytes,
  returns `false` on errors without modifying output, and supports exact
  in-place output. Rust uses slices and returns `Result<Vec<u8>, &str>`;
  Python/R raise errors.
- A 1×1 foreground image becomes background because the kernel cannot
  fit within the image. The all-background image remains all background.

## Complexity

Two 3×3 passes yield **O(width*height)** time and **O(width*height)**
additional memory for an intermediate eroded image.

## Implementations

| Language | Folder |
| --- | --- |
| C | [binary_opening_c](binary_opening_c/) |
| Python | [binary_opening_python](binary_opening_python/) |
| Rust | [binary_opening_rust](binary_opening_rust/) |
| R | [binary_opening_r](binary_opening_r/) |

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Fixtures test isolated foreground removal, preservation of a 3×3 block,
zero input, 1×1 boundary, valid in-place C calls, invalid image sizes and
nonbinary pixels.

## Limitations

Fixed 3×3 square, zero-padding border policy, single binary channel, and
one opening iteration only. No custom structuring elements, grayscale
morphology, connectivity assumptions, batched tensors or GPU acceleration.

## References

OpenCV. (n.d.). *Morphological transformations*. OpenCV documentation.
https://docs.opencv.org/4.x/d9/d61/tutorial_py_morphological_ops.html
