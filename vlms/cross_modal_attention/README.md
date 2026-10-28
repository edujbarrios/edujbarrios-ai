# Text-to-Image Cross-Modal Attention

## Vision-language context

Attention can fuse information across modalities: a **text query**
uses learned image patch keys to decide how strongly to read
corresponding image value vectors. This reference implements one
text-query-to-image-key/value attention operation, a minimal building
block behind multimodal fusion mechanisms (Alayrac et al., 2022;
Vaswani et al., 2017).

## Definition

Given a text query `q` of dimension `D`, `P` visual patch
keys `K[p]` and patch values `V[p]`, both of dimension `D`:

```text
score[p] = dot(q, K[p]) / sqrt(D)
a[p] = exp(score[p]-max(score)) / sum_j exp(score[j]-max(score))
context[d] = sum_p a[p]*V[p,d]
```

The weights sum to approximately 1. The result is
`(attention_weights[P], context[D])`.

Example: `q=[1,0]`, visual keys `[[1,0],[0,1]]`,
values `[[10,0],[0,20]]`. Then
`a[0]=sigmoid(1/sqrt(2)) ≈ 0.6697615`,
`a[1]=1-a[0]`, and context is
`[10*a[0],20*a[1]]`. A zero query gives uniform weights.

## Contract

- `P=1..64` image patches and `D=1..64` features.
- A flat query vector of length `D`, and flat **row-major**
  key and value arrays each of length `P*D`.
- All entries must be finite real values in `[-1000,1000]`.
  A zero query and/or zero keys are allowed.
- Return `P` probability weights and a length-`D` context
  vector. No attention mask, bias or trainable projection is applied.
- C uses independent caller-owned `weights[P]` and
  `context[D]` buffers, left unchanged on validation failure.
  Python returns a pair of lists, Rust returns
  `Result<(Vec<f64>,Vec<f64>),&str>`, R returns a named list.
  Python rejects boolean dimensions and feature values.

## Complexity

Computing keys' scores and the visual context uses
**O(PD)** time, with **O(P+D)** scratch/output storage.
The reference uses max-subtracted softmax for stability.

## Implementations

| Language | Reference |
| --- | --- |
| C | [cross_modal_attention_c](cross_modal_attention_c/) |
| Python | [cross_modal_attention_python](cross_modal_attention_python/) |
| Rust | [cross_modal_attention_rust](cross_modal_attention_rust/) |
| R | [cross_modal_attention_r](cross_modal_attention_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover contrasting patch keys, zero queries, a single
patch, output normalization, shape errors and nonfinite data.

## Limitations

This is *single-head, single-query* forward attention on
precomputed vectors, not a complete VLM encoder/decoder.
It does not project input embeddings, use multiple heads,
process sequences of text queries, handle masks, or compute gradients.

## References

- Alayrac, J.-B., et al. (2022). *Flamingo: A visual language model for few-shot learning*. Advances in Neural Information Processing Systems, 35. https://arxiv.org/abs/2204.14198
- Vaswani, A., et al. (2017). *Attention is all you need*. Advances in Neural Information Processing Systems, 30. https://arxiv.org/abs/1706.03762
