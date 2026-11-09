# Four-Connected Binary Hole Filling

## Computer vision context

Hole filling is a common morphology operation for repairing segmented objects and binary masks. A hole is a background region **not connected to the image border**. This algorithm uses an exterior-background flood fill rather than a fixed-size closing operation (Gonzalez & Woods, 2018).

## Algorithm and definition

Take a flat row-major `H×W` binary image `P`, with `1` meaning foreground and `0` meaning background. Start a breadth-first search from **all zero pixels along the four borders**, then traverse the four-neighbor grid (up, down, left and right) through zero pixels. Mark every reachable zero as *exterior background*. Fill each unmarked zero with one. Preserve all original foreground pixels and exterior background zeros.

**Connectivity is four-neighbor**: diagonally adjacent zeros do not connect. The image exterior is reached only from border-zero pixels; there is no artificial padding.

## Contract

- A flat row-major sequence of exactly `height*width` integers, each **0 or 1**, with integer dimensions **1..64**. Python rejects booleans and nonintegers; R rejects logical/complex/matrix/missing/nonbinary inputs.
- Return a new flat row-major **0/1 image** of identical shape. All-one or all-zero masks remain unchanged. A zero at the border remains zero; an isolated interior zero becomes one.
- C takes `const uint8_t *pixels`, `size_t height,width`, a caller-owned **distinct** `uint8_t *output` of length `H×W`, and returns `bool`. It validates **all input pixels first** and leaves the output unchanged on invalid dimensions, pixels or null pointers.
- Python returns `list[int]`, Rust `Result<Vec<u8>,&str>`, R an integer vector.
- No 8-neighbor connectivity, grayscale thresholding, structuring element or iterative dilation/erosion is applied.

## Complexity

Multi-source BFS visits each pixel at most once. **O(HW)** time and **O(HW)** auxiliary space for the visited array and queue; each output contains H×W pixels.

## Implementations

| Language | Folder |
| --- | --- |
| C | [binary_hole_filling_c](binary_hole_filling_c/) |
| Python | [binary_hole_filling_python](binary_hole_filling_python/) |
| Rust | [binary_hole_filling_rust](binary_hole_filling_rust/) |
| R | [binary_hole_filling_r](binary_hole_filling_r/) |

## Examples and tests

```text
3×3 input:  1 1 1         output: 1 1 1
            1 0 1                 1 1 1
            1 1 1                 1 1 1

3×3 input:  0 1 1         output: 0 1 1
            1 0 1                 1 1 1
            1 1 1                 1 1 1
```

The second example demonstrates that a **diagonal** connection from the center to a corner background pixel does not count. Tests cover border leakage, tunnels, multiple holes, singleton/narrow grids, 64×64 images, and invalid masks.

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root.

## Limitations

Four-connectivity is a deliberate choice; eight-connectivity would produce different results for diagonal paths. This operation fills all enclosed holes regardless of area and does not preserve internal voids needed in some applications.

## References

Gonzalez, R. C., & Woods, R. E. (2018). *Digital image processing* (4th ed.). Pearson.
