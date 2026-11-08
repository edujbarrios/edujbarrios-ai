# Four-Connected Binary Component Labeling

## Why it belongs in this repository

Connected-component labeling converts a binary foreground mask into separate regions for counting objects, filtering detected regions, and postprocessing segmentation outputs. Sequential labeling is a foundational digital-image operation (Rosenfeld & Pfaltz, 1966).

## Algorithm

Scan pixels in **row-major order**. For each as-yet-unlabeled foreground pixel, increment the component ID and perform a breadth-first flood fill through **four-neighbor adjacency** (up, down, left and right). Assign the same ID to every pixel reached. Diagonally touching foreground pixels are distinct unless connected by a four-neighbor path.

## Contract

- Input is a flattened row-major binary mask of exactly `height × width` pixels, each an integer **0 or 1**. Dimensions must be integers in **1..64** inclusive.
- Output is a flattened row-major label image of length `height × width`, plus the nonnegative component count. Background is labeled **0**; component IDs are **1,2,...** in row-major first-appearance order.
- A zero mask returns all-zero labels and count 0. Single-pixel, single-row and single-column images are supported; diagonal adjacency does **not** connect.
- Reject invalid shapes, dimensions, missing data, nonbinary values, booleans and floating-point pixel values (Python), and invalid types (dynamic languages). R accepts a flat numeric 0/1 vector without attributes/dimensions.
- C accepts a caller-owned distinct labels buffer of `height × width` `size_t` values and `size_t *component_count_out`; outputs are unchanged on validation failure. Rust returns `Result<(Vec<usize>, usize), &str>`, Python a `tuple[list[int], int]`, R a `list(labels=integer, count=integer)`.
- Inputs are not modified. No allocation besides labels/queue storage proportional to image size.

## Complexity

Each pixel is labeled and queued at most once; checking up to four neighbors per foreground pixel costs **O(HW)** time and **O(HW)** space for the queue and output labels.

## Implementations

| Language | Folder |
| --- | --- |
| C | [binary_connected_components_c](binary_connected_components_c/) |
| Python | [binary_connected_components_python](binary_connected_components_python/) |
| Rust | [binary_connected_components_rust](binary_connected_components_rust/) |
| R | [binary_connected_components_r](binary_connected_components_r/) |

## Examples and tests

```text
mask (3×3):    1 1 0      labels: 1 1 0
               0 1 0              0 1 0
               0 0 1              0 0 2

component_count = 2
```

From the root, run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`.

Tests cover disconnected diagonal pixels, branching regions, fully foreground/background masks, narrow images, 64×64 boundaries and invalid dimensions/pixels.

## Limitations

Only two-dimensional binary images, fixed four-neighbor connectivity and deterministic BFS labels. It does not calculate component areas, bounding boxes, holes, eight-neighbor adjacency, grayscale regions or GPU parallel labeling.

## References

Rosenfeld, A., & Pfaltz, J. L. (1966). Sequential operations in digital picture processing. *Journal of the ACM, 13*(4), 471–494. https://doi.org/10.1145/321356.321357
