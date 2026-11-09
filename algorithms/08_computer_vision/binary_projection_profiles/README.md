# Binary Projection Profiles

## Computer vision context

Projection profiles summarize where foreground pixels occur along each image axis. **Horizontal** (row) and **vertical** (column) histograms can help segment lines, detect occupied regions, and identify coarse spatial structure in a binary image (Gonzalez & Woods, 2018).

## Algorithm

For a binary image `P` of shape `H×W`, with foreground pixel `1` and background pixel `0`, define:

```text
row_count[r] = sum(P[r,c] for c = 0..W-1)
col_count[c] = sum(P[r,c] for r = 0..H-1)
```

Return a **single flattened result** with row counts followed by column counts:

```text
[row_count[0], ..., row_count[H-1],
 col_count[0], ..., col_count[W-1]]
```

This convention keeps the output identical in C, Python, Rust and R. Every row and column count is in `0..64`. The sum of the first H entries always equals the sum of the final W entries and equals the total foreground pixel count.

## Input/output contract

- Input: **flat row-major** image of exactly `height*width` binary integer pixels, each **0 or 1**. Dimensions `height, width` are **integers 1..64**.
- Output: a newly computed vector of **height+width nonnegative integers**: the first height entries are horizontal row sums and the last width entries are vertical column sums. The input is not changed.
- C: `bool binary_projection_profiles(const uint8_t *pixels, size_t height, size_t width, uint16_t *output)`. The caller supplies a **distinct output buffer** of length `height+width`; on null pointers, invalid dimensions, or nonbinary pixels it returns `false` without changing the output.
- Python returns `list[int]` and rejects wrong types, bools, floats or malformed shapes. Rust returns `Result<Vec<u16>,&str>`. R returns an integer vector and rejects logical/complex, matrices, missing and nonintegral values.
- No grayscale intensities, adaptive threshold, connected-component labeling or text-line inference are included.

## Complexity

A single pass through the mask increments a row and column counter for each foreground pixel: **O(HW)** time and **O(H+W)** output storage, with O(1) additional space beyond the output.

## Examples

```text
2×3 mask:   1 0 1       rows: [2,2]
            0 1 1       cols: [1,1,2]
projection_profiles = [2,2,1,1,2]

1×4 mask: [1,0,1,1]      -> [3,1,0,1,1]
2×2 all zeros           -> [0,0,0,0]
2×2 all ones            -> [2,2,2,2]
```

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [binary_projection_profiles_c](binary_projection_profiles_c/) |
| Python | [binary_projection_profiles_python](binary_projection_profiles_python/) |
| Rust | [binary_projection_profiles_rust](binary_projection_profiles_rust/) |
| R | [binary_projection_profiles_r](binary_projection_profiles_r/) |

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the root. Tests cover rectangular and singleton images, row/column ordering, constant masks, 64×64 images, total-count invariants and invalid inputs.

## Limitations

Global projections lose local spatial detail; different masks can have identical row and column profiles. Background/foreground classification must already be supplied as a binary image.

## References

Gonzalez, R. C., & Woods, R. E. (2018). *Digital image processing* (4th ed.). Pearson.
