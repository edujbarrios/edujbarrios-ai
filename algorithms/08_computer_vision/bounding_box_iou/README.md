# Axis-Aligned Bounding Box Intersection over Union (IoU)

## Why it belongs in this repository

Object detection evaluates how closely a predicted box overlaps a reference
box. Intersection over Union measures that overlap relative to the occupied
union area and is central to matching detections to ground truth in vision
benchmarks (Everingham et al., 2010).

This example exposes IoU as a standalone **box geometry** primitive; binary
mask IoU is a separate pixelwise algorithm.

## What it does

Each box is `[xmin, ymin, xmax, ymax]` with strictly positive width and
height. For two boxes A and B:

```text
intersection_width  = max(0, min(A.xmax, B.xmax) - max(A.xmin, B.xmin))
intersection_height = max(0, min(A.ymax, B.ymax) - max(A.ymin, B.ymin))
intersection_area   = intersection_width * intersection_height
union_area          = area(A) + area(B) - intersection_area
IoU                 = intersection_area / union_area
```

For boxes `[0,0,2,2]` and `[1,1,3,3]`, intersection area is 1,
union area is 7, and IoU is `1/7`. Touching but non-overlapping edges
score 0; identical boxes score 1.

## Contract

- Input: two boxes, each containing exactly four finite numbers in
  `[-1_000_000,1_000_000]`, with `xmin < xmax`, `ymin < ymax`.
  Bounds keep the reference double-precision area arithmetic finite.
- Output: an IoU score in `[0,1]`; no geometric overlap returns 0.
- Invalid length, nonfinite coordinates, invalid ordering, out-of-range
  coordinates and zero-area boxes are errors.
- C accepts pointers to four doubles and writes to a distinct double result;
  failure leaves output unchanged. Python/R raise errors; Rust returns
  `Result<f64, &str>`.
- Float boundary comparisons are exact: tiny nonzero overlap can round
  away under finite precision.

## Complexity

**O(1)** time and auxiliary memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [bounding_box_iou_c](bounding_box_iou_c/) |
| Python | [bounding_box_iou_python](bounding_box_iou_python/) |
| Rust | [bounding_box_iou_rust](bounding_box_iou_rust/) |
| R | [bounding_box_iou_r](bounding_box_iou_r/) |

## Examples and tests

Run from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Fixture cases cover 1/7 overlap, disjoint and edge-touching boxes, identical
and negative-coordinate boxes, invalid ordering, nonfinite values and bad
coordinate lengths.

## Limitations

This evaluates **axis-aligned**, unrotated boxes and does not implement
polygon overlap, generalized IoU, non-maximum suppression or matching
detections to labels. Coordinates must be in the same reference frame.
The coordinate limit is an explicit educational implementation constraint.

## References

Everingham, M., Van Gool, L., Williams, C. K. I., Winn, J., & Zisserman, A.
(2010). The PASCAL visual object classes (VOC) challenge.
*International Journal of Computer Vision, 88*(2), 303–338.
https://doi.org/10.1007/s11263-009-0275-4
