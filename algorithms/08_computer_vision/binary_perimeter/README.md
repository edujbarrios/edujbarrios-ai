# Four-Neighbor Binary Perimeter

## Computer vision context

A digital binary perimeter measures the boundary of foreground objects in a segmentation mask. This implementation counts **exposed unit grid edges**, providing a simple deterministic perimeter statistic for pixel-aligned regions (Gonzalez & Woods, 2018).

## Mathematical definition

For a binary row-major image `P` of shape `H×W`, count each of the four sides of every foreground pixel (`1`) for which the neighboring pixel is background (`0`) **or the side lies outside the image**:

```text
perimeter = sum over P[r,c]=1 of (
    [r=0 or P[r-1,c]=0] +
    [r=H-1 or P[r+1,c]=0] +
    [c=0 or P[r,c-1]=0] +
    [c=W-1 or P[r,c+1]=0]
)
```

This is an **edge-count (Manhattan)** perimeter. For a solid rectangle of height `H` and width `W`, the result is `2(H+W)`. Enclosed holes also contribute their interior edge boundary. Disconnected components have additive perimeters.

## Contract

- Input: a flat row-major binary image of exactly `H*W` pixels, each integer **0 or 1**, with integer dimensions **1..64**. Reject nonbinary values, malformed sizes and invalid dimensions. Python rejects booleans/floats/nonsequences; R rejects logical, nonnumeric, nonflat and nonfinite inputs.
- Return a single **nonnegative integer perimeter** measured in unit pixel edges, between 0 and 16384. All-zero input returns 0. A lone `1` returns 4.
- C accepts `const uint8_t *pixels`, `size_t height,width`, and a caller-owned **distinct** `uint32_t *perimeter_out`. Return `false` and preserve output on any invalid dimensions, null pointer, or nonbinary pixel. Rust returns `Result<u32,&str>`, Python `int`, R an integer scalar.
- No implicit periodic boundary, subpixel interpolation, diagonal 8-neighbor perimeter, physical pixel spacing, or contour tracing is included.

## Complexity

Single scan of the image; **O(HW)** time and **O(1)** extra memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [binary_perimeter_c](binary_perimeter_c/) |
| Python | [binary_perimeter_python](binary_perimeter_python/) |
| Rust | [binary_perimeter_rust](binary_perimeter_rust/) |
| R | [binary_perimeter_r](binary_perimeter_r/) |

## Examples and tests

```text
perimeter([1], 1, 1) = 4
perimeter([1,1,1,1], 2, 2) = 8
perimeter([1,0,1], 1, 3) = 8

3×3 ring:   1 1 1
            1 0 1        perimeter = 16
            1 1 1
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` in the repository root. Tests cover all-zero/all-one masks, internal holes, disconnected pixels, single rows/columns, 64×64 masks, checkerboard and validation failures.

## Limitations

The edge count does not approximate a diagonal Euclidean arc length without a grid correction. It includes boundaries of holes and does not distinguish object instances or return individual contours.

## References

Gonzalez, R. C., & Woods, R. E. (2018). *Digital image processing* (4th ed.). Pearson.
