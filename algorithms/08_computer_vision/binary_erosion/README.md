# 3×3 Binary Morphological Erosion

## Computer vision context

Binary erosion contracts foreground regions. It removes small protrusions,
separates narrow connections and is a building block for morphological
opening. It complements the `binary_dilation` implementation
(Soille, 2003).

## Algorithm

For each image location `(r,c)`, inspect its 3×3 neighborhood,
including the center. The output is 1 only if **all nine** values
are 1. Pixels beyond any border are treated as **zero**:

```text
output[r,c] = min input[r+dr,c+dc], dr,dc in {-1,0,1}
```

Input/output are row-major vectors. For a 3×3 block of ones, only
its central pixel remains 1. A 5×5 block of ones becomes a 3×3
interior block surrounded by zeros. A single foreground pixel
erodes to zero because its neighbors include out-of-bounds zeros.

## Contract and edge cases

- `height` and `width` must be integers in `1..64`.
- Flat row-major input must contain exactly `height*width` entries
  equal to 0 or 1. Missing, fractional, nonfinite or out-of-range
  entries are invalid; Python rejects booleans as pixels.
- Output retains the input dimensions and contains only 0 or 1.
- C receives a separate caller-owned output array of `height*width`
  bytes. It stays unchanged on failure; aliasing input is not supported.
  Rust returns `Result<Vec<u8>,&str>`, Python a list of ints,
  and R an integer vector.
- Because of zero padding, any image thinner than three pixels
  in either dimension produces an all-zero result.

## Complexity

A fixed nine-element neighborhood is scanned for every pixel:
**O(HW)** time, **O(HW)** output memory, and constant scratch space
in the C implementation.

## Implementations and tests

| Language | Source and tests |
| --- | --- |
| C | [binary_erosion_c](binary_erosion_c/) |
| Python | [binary_erosion_python](binary_erosion_python/) |
| Rust | [binary_erosion_rust](binary_erosion_rust/) |
| R | [binary_erosion_r](binary_erosion_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` at repository root.
Tests include full, empty, thin and single-pixel images, malformed
dimensions, and invalid mask values.

## Limitations

This is one iteration using a fixed square element and zero padding.
It does not support grayscale morphology, custom kernels, arbitrary
iterations or connected-component analysis. Erosion can remove
legitimate narrow structures in segmentation masks.

## References

- Soille, P. (2003). *Morphological image analysis: Principles and applications* (2nd ed.). Springer. https://doi.org/10.1007/978-3-662-05088-0
