# Deterministic Beam Search for a Markov Token Model

## Why it belongs in AI

**Beam search** is a practical decoding algorithm used in sequence
generation and speech recognition. Instead of tracking only a single
greedy hypothesis or enumerating all possible token sequences, it
keeps a fixed number of the best partial hypotheses after each
decoding step (Sutskever et al., 2014).

This reference demonstrates the mechanics using an explicit
**first-order Markov token model**. Real neural decoders compute
context-dependent next-token probabilities; here the probabilities
depend only on the previous token. The beam search logic is the
point of the exercise, not a claim that this is a Transformer.

## Algorithm

For vocabulary size `V`, initial distribution `p0[V]`,
row-stochastic transition probabilities `P[V,V]`, requested
sequence length `T`, and beam width `B`:

```text
beam = [([], log_score=0)]
for t in 0..T-1:
    candidates = []
    for prefix in beam:
        for token in 0..V-1:
            prob = p0[token] if t==0 else P[prefix.last, token]
            candidates.append(prefix + [token],
                              prefix.score + ln(prob))
    beam = highest-scoring B candidates

return the highest-scoring sequence and its log joint probability
```

Sort by **descending log probability**, then **ascending
lexicographic token sequence** for exact ties. Both beam filtering
and final selection use this rule. The returned path is not
guaranteed globally optimal if beam width is insufficient.

Example:

```text
p0 = [0.6,0.4]
P = [[0.9,0.1],[0.2,0.8]]
T=3, B=2

best sequence = [0,0,0]
joint probability = 0.6 * 0.9 * 0.9 = 0.486
log joint probability = ln(0.486)
```

## Shared contract

- Vocabulary **1..8** tokens; `p0` has V strictly positive,
  finite probabilities. P is a V×V matrix of strictly positive,
  finite probabilities. All entries are in (0,1], and every
  probability row sums to one within absolute tolerance 1e-9.
- Sequence length `T∈[1,8]`, beam width `B∈[1,8]`, both
  integers. Beam width can exceed V; at most existing distinct
  hypotheses are retained.
- Return a length-T **zero-based token index sequence** and its
  finite natural-log joint probability. All generated token IDs
  belong to `[0,V-1]`. Ties break lexicographically.
- C/Rust accept flat **row-major** P; Python nested rows;
  R a native numeric matrix. In C, caller owns `uint8_t output[T]`
  and `double* log_probability`, both unchanged on error.
  Rust returns `Result<(Vec<u8>,f64),...>`; Python returns a
  tuple of token list and score; R returns a named list.
- Reject shapes, unnormalized rows, nonpositive or nonfinite
  probabilities, invalid T/B, numeric overflow or other invalid
  input. No random number generator is used.
- Positive-only probabilities avoid handling impossible
  `-Inf` candidate scores in this small reference.

## Complexity

Each step expands at most B paths by V tokens, sorting at most
B×V sequences. Comparisons cost O(T) for lexicographic ties.
Worst-case time **O(T² B V log(BV))**, space **O(B V T)** for
the candidate paths and copies. V, B and T are capped at 8.

## Implementations

| Language | Directory |
| --- | --- |
| C | [markov_beam_search_c](markov_beam_search_c/) |
| Python | [markov_beam_search_python](markov_beam_search_python/) |
| Rust | [markov_beam_search_rust](markov_beam_search_rust/) |
| R | [markov_beam_search_r](markov_beam_search_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the known sequence, uniform tie order, one-token
vocabularies, beam width one, invalid distributions and bounds.

## Limitations

Fixed-length generation only: no end-of-sequence token, length
penalty, repetition penalty, coverage penalty, language model,
context-dependent neural decoder, stochastic sampling, or
CUDA implementation. Beam search may prefer short or repetitive
sequences in real systems; no quality claims are made.

## Reference

Sutskever, I., Vinyals, O., & Le, Q. V. (2014). Sequence to
sequence learning with neural networks. In *Advances in neural
information processing systems* (Vol. 27).
https://arxiv.org/abs/1409.3215
