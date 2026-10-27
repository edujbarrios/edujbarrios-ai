# Image–Text Hard Negative Mining

## Why it matters for VLMs

Image–text contrastive training contrasts matching image/caption pairs
against mismatched in-batch candidates. Selecting the most confusable
mismatched candidate yields a **hard negative** for each query.
This reference finds those candidates using normalized embedding
cosine similarity, a common primitive in cross-modal contrastive
learning (Radford et al., 2021).

It does **not** train a model or guarantee that an unpaired caption
is semantically irrelevant.

## Algorithm

Given `N` paired image embeddings `I[i]` and text embeddings
`T[j]`, each of dimension `D`, normalize each nonzero row and
calculate

```text
similarity[i,j] = dot(I[i] / ||I[i]||, T[j] / ||T[j]||)
image_to_text_negative[i] = argmax_(j != i) similarity[i,j]
text_to_image_negative[i] = argmax_(j != i) similarity[j,i]
```

The diagonal `i == j` is excluded because that pair is labeled
positive. Resolve equal similarities in favor of the **lowest
zero-based candidate index**.

Return a flat length-`2N` vector, first all `N` image-to-text
negative indices, then all `N` text-to-image negative indices.

Examples:
- With `N=2,D=2` and both modalities `[1,0, 0,1]`, the
  result is `[1,0, 1,0]`.
- With three identical `[1,0]` vectors per modality,
  the result is `[1,0,0, 1,0,0]`.
- With paired rows `[1,0]`, `[0,1]`, `[0.6,0.8]` in
  each modality, the result is `[2,2,1, 2,2,1]`.

## Contract

- `2 <= N <= 64`, `1 <= D <= 128`; images and texts are
  flat **row-major** vectors with length exactly `N*D`.
- Entries must be finite in `[-1000,1000]`, with strictly
  positive L2 norm for every image and text row.
- Returns zero-based integer indices in `[0,N-1]`, none equal
  to the query index `i`. Supports duplicate vectors.
- Invalid dimensions/lengths, missing data, nonfinite values
  and zero embeddings are rejected. Python rejects boolean
  numbers and dimensions.
- C writes to a distinct caller-provided length-`2N` buffer
  and leaves it unchanged on validation failure. Rust returns
  `Result<Vec<usize>,&str>`; Python and R return integer vectors.
- These are **in-batch** negatives only. A maximum similarity of
  1 does not establish that a mismatched pair is actually a negative.

## Complexity

Computing normalized embeddings and the full similarity matrix
takes **O(N²D)** time and **O(ND+N²)** scratch space.
Selecting one hard negative per direction takes O(N²).

## Implementations

| Language | Directory |
| --- | --- |
| C | [image_text_hard_negatives_c](image_text_hard_negatives_c/) |
| Python | [image_text_hard_negatives_python](image_text_hard_negatives_python/) |
| Rust | [image_text_hard_negatives_rust](image_text_hard_negatives_rust/) |
| R | [image_text_hard_negatives_r](image_text_hard_negatives_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests include orthogonal pairs, ambiguous ties, distinct
hard negatives, scaling invariance, invalid dimensions, zero
norms and nonfinite embeddings.

## Limitations

One-to-one diagonal positives are assumed. The most similar
non-diagonal candidate may be a **false negative** if another
caption accurately describes the image. The method does not
mine external memory banks, use labels, weight the selected
negative or compute a training loss.

## References

- Radford, A., Kim, J. W., Hallacy, C., Ramesh, A., Goh, G., Agarwal, S., Sastry, G., Askell, A., Mishkin, P., Clark, J., Krueger, G., & Sutskever, I. (2021). *Learning transferable visual models from natural language supervision*. Proceedings of Machine Learning Research, 139, 8748–8763. https://proceedings.mlr.press/v139/radford21a.html
