# Binary Mask Intersection over Union (IoU)

## Why it belongs in this repository

Intersection over Union (IoU), also called the Jaccard index, measures
agreement between a predicted segmentation mask and a reference mask.
It is a common interpretable overlap metric in computer vision evaluation,
including the PASCAL VOC benchmark family (Everingham et al., 2010).

Unlike the collection's generic set Jaccard implementation, this algorithm
accepts **aligned binary image pixels** and validates the mask contract.

## What it does

Given equally sized, nonempty flattened masks of zeros and ones:

```text
intersection = number of pixels where prediction == 1 and target == 1
union        = number of pixels where prediction == 1 or target == 1
IoU          = intersection / union, if union > 0
IoU          = 1, if union == 0 (both masks empty of foreground)
```

For prediction `[1,1,0,0]` and target `[1,0,1,0]`, IoU is
`1/3`. Both all-background masks score `1` by explicit convention.

## Contract

- Inputs: two equal-length **nonempty** flat binary masks, containing only
  integers 0 and 1, with corresponding positions referring to the same pixels.
- Output: a finite score in `[0, 1]`. Perfect equality scores 1,
  disjoint positive regions score 0. Both zero masks score 1.
- Reject empty or mismatched lengths and any value other than 0 or 1.
  Python/R raise errors, Rust returns `Err`, C returns `false`
  without modifying the caller's output.
- C accepts byte arrays and an output pointer. Rust accepts byte slices.
  Python accepts integer sequences (but not booleans); R accepts finite
  numeric vectors of zeros and ones.
- Image dimensions are omitted because flattening preserves pixel overlap
  when both masks have exactly the same position order.

## Complexity

**O(n)** time and **O(1)** auxiliary space. No external dependencies.

## Implementations

| Language | Folder |
| --- | --- |
| C | [binary_iou_c](binary_iou_c/) |
| Python | [binary_iou_python](binary_iou_python/) |
| Rust | [binary_iou_rust](binary_iou_rust/) |
| R | [binary_iou_r](binary_iou_r/) |

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests exercise partial overlap (`1/3`), identical masks, disjoint
foreground, all-background convention and invalid inputs.

## Limitations

This is a binary pixel metric, not generalized multi-class mean IoU,
differentiable soft IoU, connected-component analysis or bounding-box IoU.
It assumes aligned masks of the same underlying image. The all-background
convention is not universal; other evaluations may ignore that case.

## References

Everingham, M., Van Gool, L., Williams, C. K. I., Winn, J., & Zisserman, A.
(2010). The PASCAL visual object classes (VOC) challenge.
*International Journal of Computer Vision, 88*(2), 303–338.
https://doi.org/10.1007/s11263-009-0275-4
