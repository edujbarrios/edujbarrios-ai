# Binary Morphological Closing (3×3)

## Motivation

Morphological closing is dilation followed by erosion with the same
structuring element. It can fill small holes and narrow gaps in a foreground
mask. This implementation complements the existing binary opening (erosion
then dilation) reference (OpenCV, n.d.).

## Algorithm

For a flattened, row-major 0/1 image of dimensions `width × height`:

```text
dilated[y,x] = OR  input[ny,nx]   over a 3×3 square
closed[y,x]  = AND dilated[ny,nx] over a 3×3 square
```

All out-of-bounds pixels are treated as **zero**. Dilation examines only
valid neighbors; erosion sees missing neighbors as zeros. A central 3×3
foreground ring in a 7×7 background image becomes a solid 3×3 square
after closing.

## Contract

- `width` and `height` must each be integers in 1..64.
- The input contains exactly `width*height` binary values, each **0 or 1**,
  ordered row-major. Empty, mismatched, invalid or nonbinary inputs fail.
- The output has the same shape and binary encoding.
- C accepts caller-owned image and output buffers, including exact in-place
  operation. It validates before writing and returns `false` on error,
  leaving the output untouched. Rust returns `Result<Vec<u8>, &str>`,
  while Python and R raise exceptions on invalid input.
- Python rejects boolean pixels and dimensions; R accepts real numeric
  pixels but rejects logical and complex values.
- A 1×1 foreground pixel closes to zero because the final erosion cannot
  fit a 3×3 all-one kernel at its boundary.

## Complexity

O(width × height) time for two 3×3 passes; O(width × height) auxiliary
space for the intermediate dilated image.

## Implementations and tests

| Language | Location |
| --- | --- |
| C | [binary_closing_c](binary_closing_c/) |
| Python | [binary_closing_python](binary_closing_python/) |
| Rust | [binary_closing_rust](binary_closing_rust/) |
| R | [binary_closing_r](binary_closing_r/) |

Run these from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests check filling an internal hole, all-background input, singleton border
behavior, in-place C operation, malformed dimensions and invalid pixel values.

## Limitations

This is binary, single-channel morphology with a fixed 3×3 square, fixed
zero-border policy and one iteration. It does not support configurable kernels,
grayscale processing, GPU execution or connected-component labeling.

## References

OpenCV. (n.d.). *Morphological transformations*. OpenCV documentation.
https://docs.opencv.org/4.x/d9/d61/tutorial_py_morphological_ops.html
