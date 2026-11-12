# Contrastive Pair Loss

## Why it belongs in this repository

Contrastive loss trains embedding networks to place similar examples close together and separate dissimilar pairs by a specified margin. It is a foundational metric-learning objective and a complement to the triplet margin loss already in this repository (Hadsell et al., 2006).

## Mathematical definition

For two embeddings x, z, let D be their Euclidean distance and m their nonnegative margin. Use label y=1 for a **similar** pair and y=0 for a **dissimilar** pair:

    D = sqrt(sum((x[i] - z[i])^2))
    L = 0.5 * (y * D^2 + (1 - y) * max(0, m - D)^2)

A positive pair has zero loss only when identical; a negative pair has zero loss when it is at least m units apart. This is one-pair loss, not averaged across a batch.

## Contract

- Two aligned one-dimensional vectors of d finite real components, 1 <= d <= 512, each coordinate in [-1000,1000].
- Binary integer label 1 (similar) or 0 (dissimilar), and finite margin in [0,1000].
- Output: a finite nonnegative scalar; no input mutations.
- C accepts two `const double*` arrays, a `size_t` dimension, `uint8_t` label, margin, and caller-owned double output; returns false and preserves output on invalid inputs.
- Python accepts lists/tuples and plain numeric coordinates and margin (not bool), requires label to be an exact integer; invalid arguments raise ValueError.
- Rust accepts `&[f64]`, `u8` label and `f64` margin, returns `Option<f64>`.
- R accepts one-dimensional numeric vectors, an integer-valued scalar label and numeric margin; invalid inputs raise an error.
- Bounds ensure sums of squared coordinate differences are finite in IEEE binary64.

## Complexity

O(d) time and O(1) auxiliary space.

## Implementations

| Language | Folder |
| --- | --- |
| C | [contrastive_pair_loss_c](contrastive_pair_loss_c/) |
| Python | [contrastive_pair_loss_python](contrastive_pair_loss_python/) |
| Rust | [contrastive_pair_loss_rust](contrastive_pair_loss_rust/) |
| R | [contrastive_pair_loss_r](contrastive_pair_loss_r/) |

## Examples and tests

- x=[0,0], z=[3,4], y=1, m=6: D=5 and L=12.5.
- x=[0,0], z=[3,4], y=0, m=6: L=0.5.
- x=[0,0], z=[3,4], y=0, m=3: L=0.
- x=z=[1,2], y=0, m=2: L=2.

Tests cover both labels, margin boundaries, coincidence, 512 dimensions and invalid inputs.

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This objective takes one labeled pair; it does not mine pairs, aggregate batches, normalize vectors, or compute gradients. Its positive-term normalization follows the one-half convention and it explicitly uses label 1 for similar pairs.

## References

Hadsell, R., Chopra, S., & LeCun, Y. (2006). Dimensionality reduction by learning an invariant mapping. In *2006 IEEE Computer Society Conference on Computer Vision and Pattern Recognition (CVPR'06)* (Vol. 2, pp. 1735–1742). IEEE. https://doi.org/10.1109/CVPR.2006.100
