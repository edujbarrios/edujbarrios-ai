# Integral Image (Summed-Area Table)

## Why it belongs in this repository

Integral images support fast rectangular region summation, box filtering, and feature extraction in computer vision. After one preprocessing pass, sums for axis-aligned rectangles can be computed with four table lookups (Crow, 1984).

## Mathematical definition

For a row-major grayscale image `P` of height `H` and width `W`, the output is a same-shape **inclusive prefix sum**:

```text
S[r, c] = sum(P[y, x] for 0 <= y <= r, 0 <= x <= c)
S[r, c] = P[r, c] + S[r-1, c] + S[r, c-1] - S[r-1, c-1]
```

Any out-of-bounds term in the recurrence is zero. The output is a flattened, row-major table. A rectangular sum with inclusive corners `(top,left)` and `(bottom,right)` can be found as `S[bottom,right] - S[top-1,right] - S[bottom,left-1] + S[top-1,left-1]`, treating negative indices as zero.

## Contract

- Input is a **flat row-major 8-bit grayscale image** of exactly `height*width` pixels. Dimensions are integers in **1..64**. Each pixel is an integer in **0..255**.
- Return a new flattened array of `height*width` nonnegative **integer sums**, each at most `64*64*255 = 1,044,480`. Python and R reject invalid type/dimensions, booleans and non-integer pixels.
- C uses a `const uint8_t *pixels` input, `size_t height, width`, and a caller-provided **distinct** `uint32_t *output` buffer of `height*width` elements. It returns `false` on invalid dimensions/null pointers before writing anything. A `uint8_t` input is already guaranteed to be in range.
- Rust returns `Result<Vec<u32>, &str>`; Python returns `list[int]`; R returns an integer vector.
- No input mutation, padding row or column, overflow, parallelism or floating-point computations.

## Complexity

Time **O(HW)** and **O(HW)** output memory. The C implementation needs O(1) auxiliary space besides the caller-provided output.

## Implementations

| Language | Folder |
| --- | --- |
| C | [integral_image_c](integral_image_c/) |
| Python | [integral_image_python](integral_image_python/) |
| Rust | [integral_image_rust](integral_image_rust/) |
| R | [integral_image_r](integral_image_r/) |

## Examples and tests

For `pixels = [1,2,3,4]` with `height=2, width=2`:

```text
input:    1 2       integral image: 1  3
          3 4                       4 10
```

The result is `[1,3,4,10]`. Tests also cover empty-by-value images, singleton and narrow shapes, all-255 64×64 images, invalid dimensions/shape, and invalid Python/R pixel types.

From the repository root, run:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

Fixed 8-bit unsigned grayscale input and small images only. There is no built-in rectangle-query function, tilted integral image, histogram, or GPU implementation. The output can be larger than 8 bits; callers must not truncate it.

## References

Crow, F. C. (1984). Summed-area tables for texture mapping. *Proceedings of the 11th Annual Conference on Computer Graphics and Interactive Techniques*, 207–212. https://doi.org/10.1145/800031.808600
