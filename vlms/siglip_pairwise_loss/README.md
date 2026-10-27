# SigLIP-Style Pairwise Sigmoid Loss

## Motivation

Vision-language models can align matched image/text representations using
a **pairwise sigmoid contrastive objective**, instead of a softmax-based
batch contrastive loss. SigLIP is a well-known example of this approach
(Zhai et al., 2023). This reference takes **precomputed embeddings**
rather than pretrained encoders, and normalizes each vector before
comparing them.

## Mathematical definition

Let `I[i]` and `T[j]` be image and text vectors from `N`
aligned image-text pairs, with embedding dimension `D`.
After row-wise L2 normalization, calculate

```text
cos(i,j) = dot(I[i]/||I[i]||, T[j]/||T[j]||)
score(i,j) = logit_scale * cos(i,j) + bias
label(i,j) = +1 if i == j, otherwise -1
loss(i,j) = log(1 + exp(-label(i,j) * score(i,j)))
result = (1/N^2) * sum_{i,j} loss(i,j)
```

The stable softplus identity
`softplus(x) = max(x,0) + log1p(exp(-abs(x)))`
avoids overflow for large magnitudes.

For two identical orthogonal pairs `I=T=[[1,0],[0,1]]`,
`logit_scale=1`, and `bias=0`, the positive logits are 1
and the negatives 0. The mean loss is
`(softplus(-1) + softplus(0))/2 ≈ 0.5032044`.

**Normalization note:** This reference averages over `N²`
individual pairs. Other implementations may sum over all
pairs then divide by `N` or use different logit-scale/bias
parameterizations. Compare those definitions before comparing values.

## Contract

- `2<=N<=64` pairs and `1<=D<=128` embedding dimensions.
  Input arrays have exactly `N*D` finite floating-point entries
  in flat row-major order and values in `[-1000,1000]`.
- The image and text embedding row norms must all be positive.
- `logit_scale` is finite and in `(0,100]`; additive
  `bias` is finite and in `[-20,20]`.
- Return one nonnegative finite scalar loss; no gradient,
  sampling, embeddings or learned model parameters are produced.
  Invalid shapes, zero rows, missing/nonfinite values and
  invalid hyperparameters are rejected.
- Python rejects booleans for numeric inputs. C writes to a
  caller-owned scalar result, unchanged if validation fails;
  it must not alias any input. Rust returns
  `Result<f64,&str>`; Python and R return a floating value.

## Complexity

Normalizing and comparing all pairs takes **O(N²D)** time
and **O(ND)** memory for normalized embedding rows.
Only one scalar accumulator is needed for the loss.

## Implementations

| Language | Directory |
| --- | --- |
| C | [siglip_pairwise_loss_c](siglip_pairwise_loss_c/) |
| Python | [siglip_pairwise_loss_python](siglip_pairwise_loss_python/) |
| Rust | [siglip_pairwise_loss_rust](siglip_pairwise_loss_rust/) |
| R | [siglip_pairwise_loss_r](siglip_pairwise_loss_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover aligned and mismatched embeddings, scaling invariance,
large logit scales, degenerate vectors, invalid shapes, nonfinite
parameters and out-of-range values.

## Limitations

Only the fixed diagonal match pattern is supported. Real
datasets can have several valid texts per image, so treating all
off-diagonal pairs as negatives can create **false negatives**.
The reference computes no gradient and uses no learned logit scale,
bias updates, preprocessing, or encoder model. This is an
educational implementation, not an optimized training kernel.

## References

- Zhai, X., Mustafa, B., Kolesnikov, A., & Beyer, L. (2023). *Sigmoid loss for language image pre-training*. Proceedings of the IEEE/CVF International Conference on Computer Vision, 11975–11986. https://openaccess.thecvf.com/content/ICCV2023/html/Zhai_Sigmoid_Loss_for_Language_Image_Pre-Training_ICCV_2023_paper.html
