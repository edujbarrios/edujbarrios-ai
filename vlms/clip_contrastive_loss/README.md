# Symmetric CLIP Image–Text Contrastive Loss

## Why it belongs here

Vision–language encoders learn aligned image and text embeddings using a
contrastive objective. CLIP trains on paired image/caption batches by
comparing all image–text combinations and averaging the two directions
of cross-entropy (Radford et al., 2021).

This module implements the **loss calculation**, not gradient updates,
image encoding, text tokenization or pretrained weights.

## Mathematical definition

For `N` matched image/text pairs with embedding dimension `D`,
L2-normalize every row. For images `I_i` and texts `T_j`:

```text
logits[i,j] = logit_scale * dot(normalize(I_i), normalize(T_j))
L = (1/(2*N)) * sum_i [
       logsumexp_j(logits[i,j]) - logits[i,i] +
       logsumexp_j(logits[j,i]) - logits[i,i]
    ]
```

We use the max-subtraction form of `logsumexp` to remain finite for
large similarity scales. For identity embeddings with
`N=2, D=2, logit_scale=1`, the loss is
`log(1+exp(-1)) ≈ 0.3132616875`.

## Contract

- `2<=N<=64`, `1<=D<=128`. Every embedding entry must be
  finite and within `[-1000,1000]`, and every image and text row
  must have **strictly positive** L2 norm.
- `logit_scale` is a finite multiplier in `(0,100]`.
  This is **not the temperature**: a temperature-based formulation
  divides the similarity by temperature.
- Input rows are paired by their position: image row `i` matches
  text row `i`. Cross-batch mismatches remain negatives, even if
  two different examples happen to have the same embedding.
- C, Python and Rust use flat row-major numeric arrays of length
  exactly `N*D`; R accepts the same flat row-major numeric vectors
  and dimensions. Validation rejects incorrect lengths, booleans
  (Python), nonfinite numbers, invalid shapes or zero vectors.
- Returns one finite nonnegative loss (within floating-point rounding).
  C returns success/failure and leaves its caller-owned output
  unchanged on failure (output cannot overlap either input).
  Rust returns `Result<f64,&str>`; Python and R raise an error.
- All implementations evaluate the two cross-entropies with
  double-precision arithmetic.

## Complexity

Computing every pairwise similarity costs **O(N²D)** time; the
normalized embeddings and similarity table use **O(ND+N²)** auxiliary
memory. No automatic differentiation is included.

## Implementations

| Language | Directory |
| --- | --- |
| C | [clip_contrastive_loss_c](clip_contrastive_loss_c/) |
| Python | [clip_contrastive_loss_python](clip_contrastive_loss_python/) |
| Rust | [clip_contrastive_loss_rust](clip_contrastive_loss_rust/) |
| R | [clip_contrastive_loss_r](clip_contrastive_loss_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests include aligned and reversed pairs, identical embeddings
(`log(2)` reference), large scales, invalid dimensions, zero
vectors, nonfinite values and malformed inputs.

## Limitations

CLIP-style in-batch negatives treat other examples as negatives,
including possibly equivalent captions/images. This reference
does not return gradients, learn projections, use distributed batch
negatives or support multi-positive supervision. Avoid interpreting
a small value as a guaranteed measure of model usefulness.

## References

- Radford, A., Kim, J. W., Hallacy, C., Ramesh, A., Goh, G.,
  Agarwal, S., Sastry, G., Askell, A., Mishkin, P., Clark, J.,
  Krueger, G., & Sutskever, I. (2021).
  Learning transferable visual models from natural language supervision.
  *Proceedings of the 38th International Conference on Machine
  Learning, 139*, 8748–8763.
  https://proceedings.mlr.press/v139/radford21a.html
