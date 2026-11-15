# Connectionist Temporal Classification (CTC) Greedy Decoding

## Why it belongs in AI

CTC models emit a categorical distribution per input frame without
requiring an alignment between frames and target labels. Greedy
decoding is a deterministic, inexpensive baseline for speech and
handwriting recognition (Graves et al., 2006).

## Algorithm

1. For each time step, choose the token with the highest probability;
   ties go to the smallest zero-based token ID.
2. **Collapse consecutive repeated IDs first**.
3. **Remove the blank token afterward**.

The order of steps 2 and 3 matters: `[A, blank, A]` becomes
`[A, A]`, whereas `[A, A]` becomes `[A]`.

The frame-level best-path log probability is the sum of the log
probabilities of the chosen token at each frame.

## Contract

- `ctc_greedy_decoding(probabilities, blank_id=0)` accepts
  `1..256` time steps, each a list/tuple of `2..128`
  probabilities over the same vocabulary size.
- Probabilities must be finite, non-boolean reals in `[0,1]`;
  each row must sum to 1 within `1e-9` absolute tolerance.
- `blank_id` must be a valid zero-based integer token ID
  (not bool). Invalid input raises `ValueError`.
- Returns a dictionary containing `token_ids` (collapsed labels),
  `alignment` (one selected token ID per frame), and
  `path_log_probability` (a finite scalar <= 0).
- Inputs are not mutated. Probabilities are evaluated with binary64.

## Worked example

```python
ctc_greedy_decoding([[0.1, 0.9], [0.1, 0.9],
                     [0.9, 0.1], [0.1, 0.9]])
# token_ids == [1, 1], alignment == [1, 1, 0, 1]
```

## Complexity

**O(T V)** time for `T` frames and vocabulary size `V`,
with **O(T)** extra space for the alignment and decoded labels.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [ctc_greedy_decoding_python](ctc_greedy_decoding_python/) |

Other languages are not yet implemented.

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover repeated labels, blank-separated repeats, custom blanks,
deterministic ties, path log probability, invalid rows and input
immutability.

## Limitations

This chooses the **most probable path**, not necessarily the **most
probable collapsed label sequence** (CTC prefix beam search can
perform better). It does not combine probabilities of multiple paths,
use a language model, normalize text, or produce timestamps or word
boundaries. It is a forward decoding operation, not a CTC training loss.

## References

Graves, A., Fernández, S., Gomez, F., & Schmidhuber, J. (2006).
Connectionist temporal classification: Labelling unsegmented sequence
data with recurrent neural networks. In *Proceedings of the 23rd
International Conference on Machine Learning* (pp. 369–376).
https://doi.org/10.1145/1143844.1143891
