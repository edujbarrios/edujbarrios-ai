# CLIP-Style Zero-Shot Image Classification

## Why it belongs here

Contrastively trained vision–language models can classify an image
without a task-specific classification head by comparing its embedding
with a collection of candidate text prompts and normalizing the
similarities (Radford et al., 2021). This algorithm implements that
**scoring operation**, assuming the image and text encoder outputs
are already available.

## Mathematical definition

Let `I` be one nonzero image embedding of length `D`, and
`T[k]` be nonzero text embeddings for `K` candidate classes.
Normalize the image and every text row to unit L2 length:

```text
s[k] = logit_scale * dot(I / ||I||, T[k] / ||T[k]||)
p[k] = exp(s[k] - max(s)) / sum_j exp(s[j] - max(s))
```

This numerically stable softmax yields `K` nonnegative probabilities
summing to one. For `I=[1,0]`, `T=[[1,0],[0,1]]`, and
`logit_scale=1`, output is approximately `[0.73105858,0.26894142]`.

## Contract

- Embedding dimension `1<=D<=128` and number of text candidates
  `2<=K<=64`; image has `D` entries and text embeddings contain
  exactly `K*D` flat **row-major** entries.
- Every embedding value must be finite and within `[-1000,1000]`.
  The image and every text row must have nonzero L2 norm.
- `logit_scale` must be finite and in `(0,100]`.
  This is a *multiplier*, not a temperature.
- Class probabilities follow candidate text order. No class IDs,
  labels or argmax are returned. Identical text vectors receive
  equal probability, even if their labels could be different.
- Python rejects boolean entries/dimensions. Invalid lengths,
  dimensions, zero norms and nonfinite values fail.
- C writes to a distinct caller-owned `K`-element output and
  leaves it unchanged upon failure (input/output must not alias).
  Rust returns `Result<Vec<f64>,&str>`, Python returns a list,
  R a numeric vector. The probabilities sum to approximately 1
  within floating-point tolerance.

## Complexity

**O(KD)** time and **O(K)** auxiliary space (similarity logits and
returned output). C uses bounded fixed-size scratch; no library
dependencies or pretrained models are required.

## Implementations

| Language | Folder |
| --- | --- |
| C | [clip_zero_shot_classification_c](clip_zero_shot_classification_c/) |
| Python | [clip_zero_shot_classification_python](clip_zero_shot_classification_python/) |
| Rust | [clip_zero_shot_classification_rust](clip_zero_shot_classification_rust/) |
| R | [clip_zero_shot_classification_r](clip_zero_shot_classification_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover aligned and reversed text candidates, scale invariance,
identical classes, large logit scales, shape mismatch, zero embeddings
and invalid parameters.

## Limitations

This algorithm is not a complete CLIP model: it has no visual or
language encoder, prompt-template aggregation, calibration, class
label mapping or training. Returned values are normalized over the
**supplied candidate prompts**, not calibrated real-world confidence.
Class ordering and prompt choice affect results.

## References

- Radford, A., Kim, J. W., Hallacy, C., Ramesh, A., Goh, G.,
  Agarwal, S., Sastry, G., Askell, A., Mishkin, P., Clark, J.,
  Krueger, G., & Sutskever, I. (2021).
  Learning transferable visual models from natural language supervision.
  *Proceedings of the 38th International Conference on Machine
  Learning, 139*, 8748–8763.
  https://proceedings.mlr.press/v139/radford21a.html
