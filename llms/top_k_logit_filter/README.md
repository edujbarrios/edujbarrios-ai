# Top-K Logit Filtering

## Use in LLMs

Autoregressive language-model decoding can restrict next-token candidates
to the `k` highest-scoring vocabulary entries before sampling. This
educational reference outputs a **boolean keep mask**, rather than sampling
or modifying logits. The deterministic tie policy ensures reproducibility
across programming languages (Fan et al., 2018; Hugging Face, n.d.).

## Algorithm

Given logits `s[0],...,s[V-1]` and integer `k`, retain exactly `k`
tokens with the smallest priority tuple `(-s[i], i)`: highest logit
first, with the smaller original token ID winning ties.

Example: `logits=[2,5,5,-1]`, `k=2` returns
`[false,true,true,false]`. With `k=1`, token 1 is kept and token 2
is discarded. This is **not** top-p/nucleus sampling.

## Contract

- Logits are `1..4096` finite real values within `[-1000,1000]`.
- `k` is an integer in `[1,V]`; exactly `k` mask entries are true.
- Invalid lengths, nonfinite values, out-of-range logits, and invalid
  `k` values are rejected. Python rejects boolean values as numbers.
- C receives a separate caller-owned V-element boolean output buffer,
  which stays unchanged on failure and must not overlap input logits.
  Python returns `list[bool]`; Rust returns `Result<Vec<bool>,&str>`;
  R returns a logical vector.
- A special `k=V` case keeps every token. Probability normalization,
  softmax and any subsequent sampling step are out of scope.

## Complexity

C ranks all pairs in **O(V²)** time with **O(1)** scratch beyond the
caller-provided mask. Python/R/Rust sort token indices in
**O(V log V)** time and **O(V)** auxiliary memory.

## Implementations and testing

| Language | Directory |
| --- | --- |
| C | [top_k_logit_filter_c](top_k_logit_filter_c/) |
| Python | [top_k_logit_filter_python](top_k_logit_filter_python/) |
| Rust | [top_k_logit_filter_rust](top_k_logit_filter_rust/) |
| R | [top_k_logit_filter_r](top_k_logit_filter_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover ties, negative scores, `k=1`, `k=V`, one-token
vocabularies, and malformed/nonfinite input.

## Limitations

The mask is **not** a probability distribution. Applying a softmax
only to retained logits is the caller's responsibility. Hard top-k
selection may suppress semantically relevant low-scoring tokens,
and it does not prevent repeated text.

## References

- Fan, A., Lewis, M., & Dauphin, Y. (2018). *Hierarchical neural story generation*. Proceedings of the 56th Annual Meeting of the Association for Computational Linguistics (Volume 1: Long Papers), 889–898. https://doi.org/10.18653/v1/P18-1082
- Hugging Face. (n.d.). *Generation*. https://huggingface.co/docs/transformers/main_classes/text_generation
