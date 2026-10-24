# Top-p (Nucleus) Candidate Filtering

## Why it belongs in AI

Text generation often samples the next token from a model's
probability distribution. **Nucleus sampling**, or top-p sampling,
restricts the candidate pool to the smallest high-probability subset
whose cumulative mass reaches a specified threshold. This suppresses
the long tail of unlikely tokens while allowing a varying number of
candidate tokens across contexts (Holtzman et al., 2020).

This reference implements the **deterministic nucleus filtering
step only**: it returns the eligible token IDs. It does **not**
perform the subsequent random draw or renormalize returned weights.

## Mathematical definition

For a vocabulary of `n` tokens with probabilities `q[i]` that
sum to one, order their indices by **descending probability**, breaking
exact ties by **ascending original zero-based token index**.

Given a probability mass threshold `p ∈ (0,1]`, choose the
shortest prefix of this sorted order such that

```text
sum_{j=1..m} q[order[j]] >= p
```

Return the first `m` token indices in **sorted probability order**.
Always include at least one token. If a slightly unnormalized input
within the accepted summation tolerance never reaches p, return
the entire list; no probability mass is invented.

Example:

```text
probabilities = [0.4,0.3,0.2,0.1]

p=0.6 -> [0,1]          # mass 0.7
p=0.85 -> [0,1,2]       # mass 0.9
p=0.95 -> [0,1,2,3]     # mass 1.0

probabilities = [0.5,0.5,0.0]
p=0.5 -> [0]            # exact tie chooses index 0 first
p=0.75 -> [0,1]
p=1 -> [0,1]           # no need to include a zero-mass token
```

## Shared contract

- `probabilities` is a **nonempty** vector of length `1..4096`.
  Values must be finite reals in `[0,1]`, inclusive, and their
  sum must equal 1 within **absolute tolerance 1e-9**.
- Threshold `p` must be a finite real in **(0,1]**.
- Return one or more **distinct zero-based indices** ordered by
  descending probability and ascending index for exact ties.
  Zero-probability tokens are included only if the cumulative
  threshold was not reached earlier.
- Reject invalid probabilities, negative values, nonfinite numbers,
  invalid threshold or nonnormalized distributions. Validation
  examines **every** input value even if it would not be selected.
- C accepts an input array and caller-owned `size_t indices[n]`
  plus nonnull `size_t* count`; both output buffers are unchanged on
  failure and must not alias the input. Rust returns
  `Result<Vec<usize>,...>`, Python returns a list of integer IDs,
  and R returns a base integer vector.
- Probabilities are not logits or log-probabilities. Apply softmax
  externally to unnormalized model scores before calling this
  filtering primitive.

## Complexity

Sorting n tokens costs **O(n log n)** time and O(n) memory.
Selecting the shortest cumulative-probability prefix costs O(n)
worst case. This is a CPU reference; output size varies with p.

## Implementations

| Language | Directory |
| --- | --- |
| C | [top_p_nucleus_filtering_c](top_p_nucleus_filtering_c/) |
| Python | [top_p_nucleus_filtering_python](top_p_nucleus_filtering_python/) |
| Rust | [top_p_nucleus_filtering_rust](top_p_nucleus_filtering_rust/) |
| R | [top_p_nucleus_filtering_r](top_p_nucleus_filtering_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the worked examples, exact ties, zero-probability entries,
single-token vocabularies, cutoff endpoints, nonnormalized/negative
distributions, invalid threshold values, and nonfinite input.

## Limitations

No stochastic sampling, random seed, temperature rescaling, logits
processing, token repetition penalty, beam search, language model,
RAG chain, or next-token decoding loop. A selected subset must be
**renormalized** before a subsequent random draw; doing so is the
caller's responsibility. Tolerated probability-sum rounding can affect
the selected prefix extremely close to p=1. CUDA has not been tested.

## References

Holtzman, A., Buys, J., Du, L., Forbes, M., & Choi, Y. (2020).
The curious case of neural text degeneration. In *8th International
Conference on Learning Representations*. https://arxiv.org/abs/1904.09751
