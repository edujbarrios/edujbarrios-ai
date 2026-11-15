# Connectionist Temporal Classification (CTC) Loss

## Why it belongs in AI

Connectionist temporal classification enables speech recognition and
handwriting models to learn from **unsegmented** input frames and a shorter
target label sequence. Its forward algorithm sums the probabilities of
all frame-level paths that collapse to the given targets, without having
to enumerate alignments explicitly (Graves et al., 2006).

## Algorithm

Input is `T` time steps of categorical probabilities over a vocabulary
of size `V` including one **blank** token, plus target labels
`y[0..U-1]` that exclude blank. Adjacent repeated frame labels
collapse into one, then blanks are removed. For example, frame labels
`[a, a, blank, a]` collapse to `[a, a]`.

Construct the expanded target

```text
z = [blank, y0, blank, y1, ..., blank, y(U-1), blank]
```

with `S = 2U + 1` states. In log space, the forward score for
state `s` at time `t` sums paths staying at `s`, advancing from
`s-1`, and—when `z[s]` is nonblank and differs from
`z[s-2]`—skipping from `s-2`. Each transition adds
`log(probability[t][z[s]])`.

The total log-likelihood at the final frame sums states `S-1`
and (when `S>1`) `S-2`. Return its negative.
The sum is implemented with stable log-sum-exp; zero-probability
transitions have log score `-inf`.

## Contract

- `ctc_loss(probabilities, targets, blank_id=0)` accepts a
  list/tuple of **1..256** nonempty probability rows, each with the
  same vocabulary size `2..128`.
- Each probability must be a finite non-boolean real in `[0,1]`,
  and each row must sum to one within absolute tolerance `1e-9`.
- `targets` is a list/tuple of **0..256** zero-based integer token
  IDs in `[0,V)` excluding `blank_id`. `blank_id`
  is a valid zero-based integer token ID, not bool.
- Returns a nonnegative scalar negative log-likelihood in binary64.
  If the target is impossible (including too few frames to separate
  repeated target labels), returns `math.inf`.
- Invalid shape, probability rows, token IDs, or nonnumeric values
  raise `ValueError`. Inputs are never modified.

## Worked examples

```python
ctc_loss([[0.5, 0.5], [0.5, 0.5]], [1])
# -log(0.75) = 0.2876820724517809
ctc_loss([[0, 1], [1, 0], [0, 1]], [1, 1])
# 0.0: the only path is [1, blank, 1]
ctc_loss([[0.5, 0.5], [0.5, 0.5]], [1, 1])
# inf: cannot separate two identical labels in two frames
```

## Complexity

The `S=2U+1` forward states cost **O(T(U+1))** time.
Validation costs O(TV). The implementation stores validated row
copies, using **O(TV+U)** space; the forward recurrence itself
uses two O(U) rows.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [ctc_loss_python](ctc_loss_python/) |

Other language variants are not yet implemented.

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests compare the forward result with explicit enumeration of small
alignment paths, check repeated labels, empty targets, impossible
alignments, custom blanks, row validation, and input immutability.

## Limitations

This educational implementation accepts **probabilities rather than logits**
and does not include gradients, batch processing, blank label biases,
streaming state, or neural-network integration. For real networks,
log-softmax inputs and a specialized GPU implementation are usually
preferable. Very long or large-vocabulary cases exceed the defined limits.

## References

Graves, A., Fernández, S., Gomez, F., & Schmidhuber, J. (2006).
Connectionist temporal classification: Labelling unsegmented sequence
data with recurrent neural networks. In *Proceedings of the 23rd
International Conference on Machine Learning* (pp. 369–376).
https://doi.org/10.1145/1143844.1143891
