# Patch–Text Late Interaction (MaxSim)

## VLM context

Late interaction compares token-level text embeddings with visual
patch embeddings instead of compressing both modalities to one
global vector. This is an educational **cross-modal MaxSim**
primitive related to late-interaction retrieval architectures
(Khattab & Zaharia, 2020; Faysse et al., 2024).

## Algorithm

For `P` visual patch vectors and `T` text token vectors,
all of dimension `D`, normalize every row to unit L2 norm.
For each text token `t`:

```text
sim(t,p) = dot(normalize(text[t]), normalize(patch[p]))
match[t] = argmax_p sim(t,p)
score = (1/T) * sum_t sim(t,match[t])
```

Equal cosine similarities choose the **lowest zero-based patch ID**.
Return both `match[T]` and the scalar average score, which
lies approximately within `[-1,1]`.

Example with patches `[1,0, 0,1]` and text tokens
`[1,0, 0.6,0.8]` (P=T=D=2) returns matches
`[0,1]` and score `0.9`. A token whose best available
patch is opposite still produces a negative cosine score.

## Contract

- `P,T=1..32`; `D=1..64`.
- Patches and text tokens are flat **row-major** arrays of
  lengths `P*D` and `T*D` respectively.
- Every element finite and in `[-1000,1000]`, and every
  row must have positive L2 norm.
- Invalid lengths, nonfinite elements and zero vectors are rejected.
  Python rejects boolean numeric values.
- C writes to independent caller-owned `match[T]` and
  `score` buffers and leaves both unchanged on validation failure.
  Rust returns `Result<(Vec<usize>,f64),&str>`, Python
  returns a pair and R returns a named list with indices and score.
- All indices in every language are **zero-based**.

## Complexity

L2 normalization takes O((P+T)D); all text-to-patch comparisons
take **O(TPD)** time. The reference uses **O((P+T)D+T)**
storage including normalized vectors, matches and output.

## Implementations

| Language | Folder |
| --- | --- |
| C | [patch_text_maxsim_c](patch_text_maxsim_c/) |
| Python | [patch_text_maxsim_python](patch_text_maxsim_python/) |
| Rust | [patch_text_maxsim_rust](patch_text_maxsim_rust/) |
| R | [patch_text_maxsim_r](patch_text_maxsim_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover independent token matches, tied patches, a single
patch, negative cosine matches, scaled vectors, invalid shapes
and zero/nonfinite embeddings.

## Limitations

This is a one-direction *text-to-patches* score, without learned
projections, document-level ranking, retrieval index, gradients,
batching, or token masking. A high score is not proof of a
semantically correct image-caption pair. Real late-interaction
retrieval systems may sum instead of average over text tokens.

## References

- Faysse, M., et al. (2024). *ColPali: Efficient document retrieval with vision language models* [Preprint]. arXiv. https://arxiv.org/abs/2407.01449
- Khattab, O., & Zaharia, M. (2020). *ColBERT: Efficient and effective passage search via contextualized late interaction over BERT*. Proceedings of SIGIR. https://arxiv.org/abs/2004.12832
