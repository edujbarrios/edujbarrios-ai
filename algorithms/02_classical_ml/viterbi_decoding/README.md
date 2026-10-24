# Viterbi Decoding for a Two-State Hidden Markov Model

## Why it belongs in AI

**Hidden Markov Models (HMMs)** model sequences by assuming an unobserved
state evolves according to a transition matrix, while each state
generates observable symbols. **Viterbi decoding** finds the single
most probable hidden-state path given the observations using dynamic
programming, rather than enumerating all possible paths. HMMs and the
Viterbi algorithm are classical methods for speech recognition,
sequence labeling and probabilistic modeling (Rabiner, 1989).

This reference fixes the state space to **{0,1}** and the observation
alphabet to **{0,1}** so the algorithm can be studied without
complicating the API with arbitrary tensor shapes.

## Mathematical recurrence

Inputs:
- `initial[2]`: probability of each initial state.
- `transition[2][2]`: `P(state_t=j | state_(t-1)=i)`.
- `emission[2][2]`: `P(observation=o | state=i)`.
- Nonempty observation sequence `o[0..T-1]` of binary symbols.

Use log probabilities to replace products by sums:

```text
delta[0,s] = log(initial[s]) + log(emission[s,o[0]])

delta[t,s] = log(emission[s,o[t]]) +
                 max_{p in {0,1}} (
                     delta[t-1,p] + log(transition[p,s])
                 )
backpointer[t,s] = argmax predecessor p
```

Backtrack from the highest-scoring final state to recover the best
zero-based sequence of hidden states.

**Tie-breaking:** choose predecessor state 0 for equal scores and final
state 0 when final scores tie. The result includes the best path and
its **log joint probability**, not posterior probabilities at each
time step.

## Example

```text
initial = [0.6,0.4]
transition = [[0.7,0.3],[0.4,0.6]]
emission = [[0.5,0.5],[0.1,0.9]]
observations = [0,1,0]

best path = [0,0,0]
best joint probability = 0.03675
log probability = log(0.03675)
```

## Shared contract

- A **nonempty** sequence (maximum 4096 symbols) of exactly binary
  integer observations, values 0 or 1.
- Initial, transition and emission probabilities must be finite
  and **strictly positive**, each at most 1. Each probability
  distribution row must sum to 1 within **absolute tolerance 1e-9**.
  This deliberately excludes impossible events (zero-probability
  transitions/emissions), which would require special -Inf path logic.
- Require `initial[0]+initial[1]≈1`, and each of the two rows in
  transition and emission to be normalized.
- Return a hidden-state sequence of the same length and the finite
  best-path log joint probability. Inputs are never modified.
- C/Rust use a **flat row-major 2×2 matrix** for transition/emission;
  Python accepts nested two-row arrays; R accepts two native 2×2
  matrices. C uses caller-owned `states[T]` and a separate
  `log_probability` scalar, which must not overlap inputs and
  remain unmodified on failure. Rust returns a `Result`, and
  Python/R raise errors.
- Validate all model parameters before decoding; reject malformed
  matrices, bad symbols, single/empty probability rows, NaNs/infinities,
  and nonfinite intermediate log scores.
- This is a **maximum joint path** computation, not forward-backward
  inference or the probability of the whole observation sequence.

## Complexity

O(T × 2²) = **O(T)** time for this fixed two-state model,
with O(T) memory for backpointers and returned path. More general
Viterbi with S states costs O(T×S²) time and O(T×S) storage.

## Implementations

| Language | Directory |
| --- | --- |
| C | [viterbi_decoding_c](viterbi_decoding_c/) |
| Python | [viterbi_decoding_python](viterbi_decoding_python/) |
| Rust | [viterbi_decoding_rust](viterbi_decoding_rust/) |
| R | [viterbi_decoding_r](viterbi_decoding_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the example sequence, repeated emissions favoring another
state, exact tied paths, a single symbol, invalid observation values,
negative/zero probabilities, nonnormalized rows and maximum length.

## Limitations

Exactly two states and two possible observations; no unknown
observations, continuous emissions, parameter estimation (Baum–Welch),
arbitrary-length streaming buffers, posterior marginals, learned
embeddings, or neural encoders. The 4096-symbol cap keeps the reference
testable and prevents unbounded allocations. No CUDA acceleration
without reproducible GPU tests.

## Reference

Rabiner, L. R. (1989). A tutorial on hidden Markov models and selected
applications in speech recognition. *Proceedings of the IEEE, 77*(2),
257–286. https://doi.org/10.1109/5.18626
