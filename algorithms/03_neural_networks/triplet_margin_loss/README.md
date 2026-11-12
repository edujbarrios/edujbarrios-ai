# Triplet Margin Loss

## Why it belongs in this repository

Metric learning trains neural embeddings so examples with the same identity or class lie closer together than dissimilar ones. Triplet loss enforces a distance gap between an anchor, a matching positive example and a contrasting negative example. It is commonly used in face recognition and retrieval (Schroff et al., 2015).

## Mathematical definition

For three equal-length embeddings a, p, n and margin m >= 0, this algorithm computes

    d_positive = sqrt(sum((a[i] - p[i])^2))
    d_negative = sqrt(sum((a[i] - n[i])^2))
    loss = max(0, d_positive - d_negative + m)

The loss is zero when the negative is at least margin m farther from the anchor than the positive. This is an **unreduced single-triplet** loss, not a batch average. No network gradients are calculated.

## Contract

- Input: three one-dimensional numeric vectors of the same length d, where 1 <= d <= 512, plus one finite margin in [0,1000].
- All embedding coordinates must be finite real numbers within [-1,000,000, 1,000,000].
- Output: a nonnegative finite scalar Euclidean triplet loss; values are not modified.
- C: boolean success with unchanged output on failure, including null pointers. Python and R raise errors for invalid types, dimensions or values; Rust returns `Option<f64>`.
- Python accepts lists or tuples, disallowing boolean elements; R accepts numeric one-dimensional vectors, not matrices or logical vectors.
- The bounds ensure that coordinate subtraction, squaring and summation are finite in binary64 for the declared maximum length.

## Complexity

O(d) time and O(1) auxiliary space.

## Implementations

| Language | Directory |
| --- | --- |
| C | [triplet_margin_loss_c](triplet_margin_loss_c/) |
| Python | [triplet_margin_loss_python](triplet_margin_loss_python/) |
| Rust | [triplet_margin_loss_rust](triplet_margin_loss_rust/) |
| R | [triplet_margin_loss_r](triplet_margin_loss_r/) |

## Examples and tests

    anchor = [0,0], positive = [1,0], negative = [3,0]
    margin = 1 -> loss = 0
    margin = 3 -> loss = 1

    anchor = [0,0], positive = [3,4], negative = [0,0]
    margin = 1 -> loss = 6

Tests cover zero loss, positive loss, coincident embeddings, zero margin, 512-dimensional bounds, negative coordinates, and invalid lengths/non-finite values.

Run from the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This is a single triplet with Euclidean distances and a fixed margin. It does not mine hard negatives, reduce over batches, normalize embeddings, or compute gradients. Distance evaluation is direct rather than using a specially scaled norm; input limits intentionally avoid overflow.

## References

Schroff, F., Kalenichenko, D., & Philbin, J. (2015). FaceNet: A unified embedding for face recognition and clustering. In *2015 IEEE Conference on Computer Vision and Pattern Recognition (CVPR)* (pp. 815–823). IEEE. https://doi.org/10.1109/CVPR.2015.7298682
