# Four-Neighbor Binary Distance Transform

## Computer vision context

A distance transform assigns every foreground pixel a distance to the nearest background pixel. Distance transforms are useful for morphology, shape measurement, skeletonization, and segmentation postprocessing (Borgefors, 1986).

## Algorithm and definition

Given a **flat row-major binary mask** `P` of `H×W`, every background (`0`) pixel is a source with distance zero. Perform **multi-source breadth-first search** over the four-neighbor grid (up, down, left, right) to assign each foreground pixel its minimum **Manhattan distance** to a background pixel:

```text
D[r,c] = min(|r-u| + |c-v| for P[u,v] == 0)
```

**Only pixels inside the input image count as background**; no zero-valued padding is assumed outside its boundaries. When the mask contains **no zeros**, return a sentinel value of **H+W** for every pixel; this is larger than any possible distance to an in-image pixel (H+W-2). The sentinel also applies to the single-pixel all-foreground image.

## Contract

- Input: flat row-major pixels of exactly `height*width` values, each integer **0 or 1**; dimensions integer in **1..64**. Python rejects bools/floats and invalid sequences. R rejects logical, complex, nonnumeric, nonflat, nonfinite and nonbinary inputs.
- Output: a flat row-major **integer** distance vector of length `H×W`; background positions have distance zero. The maximum finite distance is 126, and the no-background sentinel is at most 128.
- C takes `const uint8_t *pixels`, `size_t height,width`, and caller-owned **distinct** `uint16_t *output` capacity `H×W`. It validates **all pixels before output mutation** and returns `false` on failure, leaving output unchanged.
- Python returns `list[int]`, Rust `Result<Vec<u16>,&str>`, R an integer vector. The input is never mutated.
- This is the **4-neighbor Manhattan** transform, not Euclidean, chessboard, or an 8-neighbor chamfer distance.

## Complexity

Multi-source BFS visits each pixel at most once, with at most four edge inspections per pixel: **O(HW)** time and **O(HW)** extra queue plus output storage. C uses a fixed 4096-entry queue.

## Implementations

| Language | Folder |
| --- | --- |
| C | [binary_distance_transform_c](binary_distance_transform_c/) |
| Python | [binary_distance_transform_python](binary_distance_transform_python/) |
| Rust | [binary_distance_transform_rust](binary_distance_transform_rust/) |
| R | [binary_distance_transform_r](binary_distance_transform_r/) |

## Examples and tests

```text
3×3 mask:     1 1 1       distance: 2 1 2
              1 0 1                 1 0 1
              1 1 1                 2 1 2

[1,1,1,1] with H=W=2 -> [4,4,4,4]  # no in-image zeros
[0] with H=W=1       -> [0]
[1] with H=W=1       -> [2]
```

From the repository root, run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`.

Tests cover center/corner background pixels, singleton/line images, all-background and all-foreground images, 64×64 boundaries, and malformed dimensions and mask values.

## Limitations

Distances are measured on the discrete four-connected grid. A diagonal step costs two, not `√2`. When no background exists, a documented finite sentinel is returned rather than infinity or a missing value. No feature-coordinate/nearest-site output or GPU variant is included.

## References

Borgefors, G. (1986). Distance transformations in digital images. *Computer Vision, Graphics, and Image Processing, 34*(3), 344–371. https://doi.org/10.1016/S0734-189X(86)80047-0
