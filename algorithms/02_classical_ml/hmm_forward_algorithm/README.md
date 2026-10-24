# Two-State HMM Forward Algorithm (Log Space)

## Why it belongs in AI

A **Hidden Markov Model (HMM)** assigns probabilities to sequences
of observations by summing over *all possible hidden-state paths*.
The **Forward algorithm** performs this summation efficiently with
dynamic programming. It supports probabilistic sequence analysis,
speech recognition, model comparison and HMM training (Rabiner, 1989).

Unlike this repository's **Viterbi decoding**, which selects the
*single most probable hidden-state path*, the Forward algorithm
calculates **the total probability of the observed sequence**.

## Mathematics

For two hidden states `s∈{0,1}` and observations `o∈{0,1}`,
let `π[s]` be the initial distribution,
`A[p,s]` the state-transition probabilities, and
`B[s,o]` the emission probabilities.

Use log-space dynamic programming to avoid multiplying many small
probabilities. Let `LSE(a,b) = max(a,b) + ln(exp(a-max)+exp(b-max))`.

```text
log_alpha[0,s] = ln(π[s]) + ln(B[s,observations[0]])

log_alpha[t,s] = ln(B[s,observations[t]]) +
    LSE(log_alpha[t-1,0] + ln(A[0,s]),
        log_alpha[t-1,1] + ln(A[1,s]))

log P(observations) =
    LSE(log_alpha[T-1,0], log_alpha[T-1,1])
```

The log-sum-exp function is evaluated stably as
`m + ln(1 + exp(min(a,b)-m))` for `m=max(a,b)`.
This does not confuse the **sum over paths** with the **maximum
over paths** used by Viterbi.

### Example

```text
π = [0.6,0.4]
A = [[0.7,0.3],[0.4,0.6]]
B = [[0.5,0.5],[0.1,0.9]]
observations = [0,1,0]

P(observations) = 0.069616
log_likelihood = ln(0.069616) ≈ -2.664
```

The Viterbi best-path *joint* probability for this same example
is 0.03675, which is smaller because the Forward algorithm sums
over all hidden paths.

## Shared contract

- Input observation sequence length **1..4096**, with exact binary
  integer symbols 0 or 1.
- Initial distribution `π[2]`, transition `A[2,2]`, and
  emission `B[2,2]` consist of finite probabilities **strictly
  greater than 0** and at most 1. The initial pair and each
  transition/emission row must sum to 1 within **absolute 1e-9**.
- Require positive probabilities, rather than representing impossible
  events by `-Inf`. This matches the companion Viterbi reference
  and ensures every log is finite.
- Return a single **finite log-likelihood** of the observations.
  It is generally negative or zero and need not be convertible back
  to a representable ordinary probability for long sequences.
- Reject malformed matrices, invalid symbols, bad normalization,
  NaNs/infinities and unrepresentable intermediate arithmetic.
- C/Rust matrices are flat **row-major** arrays; R receives native
  2×2 numeric matrices; Python nested 2×2 lists or tuples.
  C writes caller-owned `double* log_likelihood` only on success;
  all buffers must be properly sized and nonoverlapping.
  Rust returns `Result<f64,...>`; Python/R raise errors.
- No hidden-state sequence is returned, since this operation
  **marginalizes** hidden states instead of decoding one path.

## Complexity

**O(T × 2²) = O(T)** time for this fixed two-state model,
and **O(1)** auxiliary memory. The general S-state Forward
algorithm takes O(T×S²) time and O(S) rolling memory.

## Implementations

| Language | Directory |
| --- | --- |
| C | [hmm_forward_algorithm_c](hmm_forward_algorithm_c/) |
| Python | [hmm_forward_algorithm_python](hmm_forward_algorithm_python/) |
| Rust | [hmm_forward_algorithm_rust](hmm_forward_algorithm_rust/) |
| R | [hmm_forward_algorithm_r](hmm_forward_algorithm_r/) |

Run tests:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the known observation probability, single-observation
likelihood, uniform models, longer sequences without underflow,
non-normalized distributions, invalid symbols and missing events.

## Limitations

Exactly two hidden states and two discrete observation symbols;
no continuous emission density, parameter fitting (Baum–Welch),
posterior marginals (forward–backward), Viterbi backpointers, custom
prior smoothing, batching, missing observations or GPU acceleration.
Positive-only probability rows are a deliberate simplification.

## References

Rabiner, L. R. (1989). A tutorial on hidden Markov models and selected
applications in speech recognition. *Proceedings of the IEEE, 77*(2),
257–286. https://doi.org/10.1109/5.18626
