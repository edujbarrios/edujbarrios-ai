# Large Language Models (LLMs)

Algorithms used to prepare tokens, decode language models, manage context and
support efficient autoregressive inference. These educational implementations
focus on observable numerical or discrete behavior, not full pretrained models.

## Implemented algorithms (4)

| Algorithm | What it does |
| --- | --- |
| [`no_repeat_ngram`](no_repeat_ngram/README.md) | Masks next tokens that would recreate a previously seen n-gram. |
| [`repetition_penalty`](repetition_penalty/README.md) | Applies a sign-aware penalty to logits for generated token IDs. |
| [`top_k_logit_filter`](top_k_logit_filter/README.md) | Keeps exactly k next-token candidates with stable index tie breaks. |
| [`top_p_logit_filter`](top_p_logit_filter/README.md) | Keeps the smallest nucleus whose softmax mass reaches p. |

[← Repository home](../README.md)
