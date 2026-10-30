# Large Language Models (LLMs)

Algorithms used to prepare tokens, decode language models, manage context and
support efficient autoregressive inference. These educational implementations
focus on observable numerical or discrete behavior, not full pretrained models.

## Implemented algorithms (12)

| Algorithm | What it does |
| --- | --- |
| [`greedy_token_selection`](greedy_token_selection/README.md) | Chooses the highest-logit next token with deterministic tie-breaking. |
| [`logit_bias_adjustment`](logit_bias_adjustment/README.md) | Adds sparse per-token logit biases before decoding. |
| [`min_p_logit_filter`](min_p_logit_filter/README.md) | Keeps tokens above a probability threshold relative to the strongest token. |
| [`no_repeat_ngram`](no_repeat_ngram/README.md) | Masks next tokens that would recreate a previously seen n-gram. |
| [`repetition_penalty`](repetition_penalty/README.md) | Applies a sign-aware penalty to logits for generated token IDs. |
| [`rms_normalization`](rms_normalization/README.md) | Normalizes token features by their root mean square without mean subtraction. |
| [`token_frequency_penalty`](token_frequency_penalty/README.md) | Subtracts a per-occurrence penalty from next-token logits. |
| [`token_presence_penalty`](token_presence_penalty/README.md) | Penalizes each previously observed token once, regardless of frequency. |
| [`typical_sampling_filter`](typical_sampling_filter/README.md) | Retains an entropy-typical probability-mass prefix. |
| [`top_k_logit_filter`](top_k_logit_filter/README.md) | Keeps exactly k next-token candidates with stable index tie breaks. |
| [`top_p_logit_filter`](top_p_logit_filter/README.md) | Keeps the smallest nucleus whose softmax mass reaches p. |
| [`temperature_softmax`](temperature_softmax/README.md) | Turns logits into probabilities using adjustable temperature. |

[← Repository home](../README.md)
