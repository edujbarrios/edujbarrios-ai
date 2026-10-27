# Greedy Bounding-Box Non-Maximum Suppression

## Why it belongs here

Object detectors often predict several boxes around the same object.
**Non-maximum suppression (NMS)** keeps high-confidence detections and
removes boxes that overlap them too strongly; this is a standard step
in region-proposal/object-detection pipelines (Ren et al., 2015).

## Algorithm

Each box is `(x1, y1, x2, y2, score)`, with half-open geometric
coordinates (no extra pixel-area `+1`).

```text
intersection_width = max(0, min(a.x2,b.x2)-max(a.x1,b.x1))
intersection_height = max(0, min(a.y2,b.y2)-max(a.y1,b.y1))
IoU(a,b) = intersection_area / (area(a)+area(b)-intersection_area)
```

Consider boxes in descending score order. A tie is resolved by the
**smaller original zero-based index**. Keep the first unsuppressed box,
then suppress each remaining box with `IoU > threshold`; equality
is *not* suppressed. Repeat until no unsuppressed boxes remain.
Return selected indices in that same priority order.

Example with threshold `0.3`: boxes
`[(0,0,2,2,.9),(.5,.5,2.5,2.5,.8),(3,3,5,5,.7)]`
give selected indices `[0,2]`.

## Contract

- Input contains **0..256** axis-aligned boxes with five real
  values each. Every coordinate is finite in `[-1e6, 1e6]`,
  `x2>x1` and `y2>y1`, and score is finite in `[0,1]`.
- The threshold is a finite real number in `[0,1]`.
  At `0`, any positive overlap is suppressed; at `1`, no
  positive-area box is suppressed.
- Empty input returns an empty selection.
- Invalid boxes, NaN, infinity, malformed shapes or thresholds cause
  errors; Python explicitly rejects booleans in numeric fields.
- C requires output indices with capacity at least the number of boxes,
  and a separate selected-count output. Outputs remain unchanged on
  failure; input/output buffers may not overlap. Rust returns
  `Result<Vec<usize>, &str>`; Python returns an index list; R uses
  a five-column numeric matrix and returns **zero-based** indices.

## Complexity

Selection and suppression: **O(N²)** time and **O(N)** auxiliary
memory, where N is the number of detections. These are readable
reference implementations rather than batched GPU kernels.

## Implementations

| Language | Directory |
| --- | --- |
| C | [non_max_suppression_c](non_max_suppression_c/) |
| Python | [non_max_suppression_python](non_max_suppression_python/) |
| Rust | [non_max_suppression_rust](non_max_suppression_rust/) |
| R | [non_max_suppression_r](non_max_suppression_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover overlapping and disjoint boxes, identical boxes, tie scores,
edge-touching boxes, empty inputs, inclusive threshold boundaries,
malformed data and invalid nonfinite values.

## Limitations

Class-agnostic hard NMS only: no class labels, Soft-NMS, rotated
boxes, batched inference, or GPU acceleration. Threshold choice
changes how many nearby true objects survive.

## References

- Ren, S., He, K., Girshick, R., & Sun, J. (2015).
  Faster R-CNN: Towards real-time object detection with region
  proposal networks. *Advances in Neural Information Processing
  Systems, 28*, 91–99.
  https://papers.nips.cc/paper/2015/hash/14bfa6bb14875e45bba028a21ed38046-Abstract.html
