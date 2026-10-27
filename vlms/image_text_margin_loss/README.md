# Bidirectional Image–Text Margin Ranking Loss

## Why it matters for VLMs

Multimodal retrieval systems can train image and text encoders to rank
paired examples above unpaired ones. A **max-margin ranking loss**
enforces a positive separation between matched image–text pairs and
their hardest mismatched candidates (Faghri et al., 2018).
This implementation calculates an educational symmetric loss over
precomputed embeddings; it does not train a model.

## Algorithm

For `N` position-matched pairs and embedding dimension `D`,
normalize each image and text row by its L2 norm, then compute
`S[i,j]=dot(normalize(I[i]),normalize(T[j]))`.

For each pair `i`, consider the largest off-diagonal similarity
in its row and column:

```text
hard_text[i]  = max_(j != i) S[i,j]
hard_image[i] = max_(j != i) S[j,i]
loss = sum_i (
  max(0, margin - S[i,i] + hard_text[i]) +
  max(0, margin - S[i,i] + hard_image[i])
) / (2*N)
```

This implements a **hardest-negative** variant and averages both
retrieval directions.

Example with `N=2,D=2`, image rows `[1,0],[0,1]` and identical
text rows:
- `margin=0.25` yields `0` (aligned pairs beat negatives by 1).
- Swapping the text rows yields `1.25`.
- Using the same nonzero row for both pairs yields `0.25`.

## Input/output contract

- `N=2..64` and `D=1..128`.
- Image and text arrays contain exactly `N*D` row-major real
  embeddings. Values must be finite within `[-1000,1000]`;
  every row must have strictly positive L2 norm.
- Finite nonnegative margin in `[0,2]`.
- The result is one finite nonnegative floating-point scalar.
- Python rejects booleans as dimensions, embedding numbers or margin.
  C writes the result into a separate caller-owned output pointer,
  unchanged on invalid input. Rust returns
  `Result<f64,&str>`; Python and R return numeric scalars.
- The diagonal `i=j` is the sole labeled match, with all
  off-diagonal pairs treated as negatives. Duplicate embeddings
  remain valid but increase the loss.

## Complexity

**O(N²D)** time to normalize and score all candidate pairs,
**O(ND+N²)** auxiliary memory for normalized embeddings and
similarity matrix. Scanning hardest negatives costs O(N²).

## Implementations

| Language | Folder |
| --- | --- |
| C | [image_text_margin_loss_c](image_text_margin_loss_c/) |
| Python | [image_text_margin_loss_python](image_text_margin_loss_python/) |
| Rust | [image_text_margin_loss_rust](image_text_margin_loss_rust/) |
| R | [image_text_margin_loss_r](image_text_margin_loss_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover matched, swapped, ambiguous and scaled embeddings,
margin boundaries, nonfinite values and malformed shape.

## Limitations

The loss assumes diagonal-only positive labels and uses just the
**single hardest** negative per query in each direction. Datasets
with multiple valid captions per image can yield **false negatives**.
It does not compute gradients, perform retrieval evaluation, or
use a pretrained vision/language encoder. Hard-negative mining
may be unstable on noisy labels.

## References

- Faghri, F., Fleet, D. J., Kiros, J. R., & Fidler, S. (2018). *VSE++: Improving visual-semantic embeddings with hard negatives*. British Machine Vision Conference. https://arxiv.org/abs/1707.05612
