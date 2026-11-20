# Inverted Dropout Forward Pass

## Why it belongs in AI

Dropout randomly suppresses activations during neural-network training
to discourage co-adaptation (Srivastava et al., 2014). Inverted dropout
scales the activations retained during training, so inference does not
need additional rescaling. The example here implements the deterministic
**forward transform given a binary mask**, making the numerical behavior
directly comparable across C, Python, Rust and R.

## Algorithm

Given finite activations x, a same-length binary mask m and a dropout
probability p with 0 <= p < 1, compute elementwise:

    y_i = 0                  if m_i = 0
    y_i = x_i / (1 - p)      if m_i = 1

During actual training the mask would usually be sampled independently
from Bernoulli(1-p). This standalone algorithm **does not sample a mask**
or provide backpropagation: callers provide one explicitly. If masks
are sampled with the stated probabilities, E[y_i] = x_i.

Example: activations [2,-4,0], mask [1,0,1], p=0.5
produce [4,0,0]. The zero mask produces exactly zero even for a large
but finite activation.

## Contract

- Input: equal-length vectors of finite real activations and binary
  integer mask values 0 or 1, plus a finite dropout probability in [0,1).
- Output: a real vector of the same length; empty vectors are allowed.
- Invalid: mismatched lengths, non-finite activations, nonbinary mask,
  invalid p, or any non-finite *retained* result from scaling.
  Masked values are still required to be finite but are never divided.
- All languages compute in double precision. C uses caller-owned,
  nonoverlapping output of length n and does not change it on failure;
  a nonnull pointer is required even when n=0.
- A fixed input mask makes the operation deterministic; no seed is used.

## Complexity

O(n) time and O(n) returned output memory. C uses O(1) auxiliary
memory in addition to its caller-owned output, validating in a full
first pass before writing any result.

## Implementations

| Language | Directory |
| --- | --- |
| C | [inverted_dropout_c](inverted_dropout_c/) |
| Python | [inverted_dropout_python](inverted_dropout_python/) |
| Rust | [inverted_dropout_rust](inverted_dropout_rust/) |
| R | [inverted_dropout_r](inverted_dropout_r/) |

Run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

Mask generation and inference/training mode selection are outside this
function. A fixed mask is not a statistical regularizer by itself.
Probabilities very close to one may overflow the retained scaling even
with finite inputs; this is reported as an error instead of returning Inf.
The implementation does not support tensors, broadcasting, batch masks
or neural-network gradient calculations.

## References

Srivastava, N., Hinton, G., Krizhevsky, A., Sutskever, I., &
Salakhutdinov, R. (2014). Dropout: A simple way to prevent neural
networks from overfitting. *Journal of Machine Learning Research,
15*(56), 1929–1958. https://jmlr.org/papers/v15/srivastava14a.html
