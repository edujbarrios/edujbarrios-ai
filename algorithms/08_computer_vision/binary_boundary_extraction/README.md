# Four-Neighbor Binary Boundary Extraction

## Computer vision context

Binary boundary extraction isolates **foreground pixels on object borders** in a segmentation mask, instead of returning the scalar edge count from `binary_perimeter`. It is a common shape-processing step and equals the original mask minus its four-neighbor erosion under zero-valued exterior padding (Gonzalez & Woods, 2018).

## Algorithm

For every foreground pixel `P[r,c]=1` in a binary image, output `B[r,c]=1` if **at least one** of the four adjacent cells (up, down, left, right) is background or outside the image. All other output pixels are zero.

```text
B[r,c] = P[r,c] AND (
    r=0 OR r=H-1 OR c=0 OR c=W-1 OR
    P[r-1,c]=0 OR P[r+1,c]=0 OR
    P[r,c-1]=0 OR P[r,c+1]=0
)
```

Out-of-image cells are treated as background. Therefore every foreground pixel of a one-row or one-column image is a boundary pixel. Interior holes also create foreground boundaries on their sides. The method retains the original image dimensions and row-major indexing.

## Contract

- Input: flat row-major mask of **exactly H×W** elements, each integer **0 or 1**, with integer dimensions `H,W` in **1..64**.
- Output: a **new binary vector** of the same shape, where `1` marks a foreground boundary pixel. All-zero masks produce all zeros; a single foreground pixel produces one.
- C takes `const uint8_t *pixels`, `size_t height,width`, and a **distinct**, caller-owned `uint8_t *output` of capacity H×W. It validates dimensions, pointers and every input value **before modifying output**, returning `false` on invalid data. Input and output may not overlap.
- Python returns `list[int]`, Rust `Result<Vec<u8>,&str>`, R an integer vector. Python rejects boolean/floating/nonnumeric pixels, R rejects logical, complex, array/matrix, missing and nonbinary values. The input is unchanged.
- This is **four-neighbor binary** boundary extraction; it is not grayscale gradient detection, eight-neighbor boundaries, or subpixel contour fitting.

## Complexity

One scan with up to four constant-time neighbor checks per pixel: **O(HW)** time. C requires **O(1)** auxiliary workspace beyond its output buffer; other languages allocate O(HW) output storage.

## Implementations

| Language | Folder |
| --- | --- |
| C | [binary_boundary_extraction_c](binary_boundary_extraction_c/) |
| Python | [binary_boundary_extraction_python](binary_boundary_extraction_python/) |
| Rust | [binary_boundary_extraction_rust](binary_boundary_extraction_rust/) |
| R | [binary_boundary_extraction_r](binary_boundary_extraction_r/) |

## Examples and tests

```text
3×3 all ones:      1 1 1          boundary: 1 1 1
                   1 1 1                    1 0 1
                   1 1 1                    1 1 1

3×3 single center: 0 0 0          boundary: 0 0 0
                   0 1 0                    0 1 0
                   0 0 0                    0 0 0
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root. Tests cover full rectangles, holes, isolated pixels, narrow images, checkerboards, 64×64 boundaries, input immutability and invalid masks.

## Limitations

The result identifies **pixels**, not geometric line segments or ordered contours. A 1-pixel-wide object has all its pixels marked as boundary, and 4-neighbor connectivity differs from 8-neighbor edge definitions. Hole boundaries are included.

## References

Gonzalez, R. C., & Woods, R. E. (2018). *Digital image processing* (4th ed.). Pearson.
