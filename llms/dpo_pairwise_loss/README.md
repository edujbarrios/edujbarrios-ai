# Direct Preference Optimization (DPO) Pairwise Loss

## Why it belongs in AI

Direct Preference Optimization turns preference comparisons between a
preferred (`chosen`) response and a dispreferred (`rejected`) response
into an objective for fine-tuning language models. It uses a fixed
reference model, avoiding an explicit learned reward model in the
basic formulation (Rafailov et al., 2023).

## Algorithm

For each prompt-response preference pair, obtain the sequence log
probabilities of the chosen and rejected responses from the current
policy and reference model. Define

```text
policy_gap    = log_policy(chosen) - log_policy(rejected)
reference_gap = log_reference(chosen) - log_reference(rejected)
margin        = beta * (policy_gap - reference_gap)
loss          = -log(sigmoid(margin))
              = log(1 + exp(-margin))
```

This implementation evaluates the last expression using a stable
softplus branch to avoid overflow at large negative margins. The
returned mean is the arithmetic mean across preference pairs.
A positive margin means the policy prefers the chosen response
*more strongly relative to the reference*.

## Contract

- `dpo_pairwise_loss(policy_chosen, policy_rejected,
  reference_chosen, reference_rejected, beta=0.1)`
  accepts four lists/tuples of the same length **1..4096**.
- Every sequence log probability must be a finite, non-boolean real in
  `[-1_000_000, 0]`. These values are log probabilities of
  complete responses, **not token logits** or raw probabilities.
- `beta` must be a finite, non-boolean real in `(0, 100]`.
  Invalid shapes, types, lengths, and bounds raise `ValueError`.
- Returns `{"losses": [...], "mean_loss": float,
  "preference_accuracy": float}`, where accuracy is the
  fraction of pairs with strictly positive margins. Zero margins
  do not count as wins.
- Inputs are never modified; arithmetic uses Python binary64.
  `beta` controls preference strength; no smoothing or clipping
  of the margin is applied.

## Worked example

```python
dpo_pairwise_loss([-0.1], [-1.0], [-0.4], [-0.6], beta=2.0)
# margin = 2 * ((-0.1 + 1.0) - (-0.4 + 0.6)) = 1.4
# mean_loss = log(1 + exp(-1.4)) ~= 0.2204174099
# preference_accuracy = 1.0
```

If both policy and reference have equal chosen/rejected gaps,
the margin is zero and the loss is `log(2)`.

## Complexity

For `n` preference pairs: **O(n)** time and **O(n)** storage
for returned individual losses; scalar accumulation uses O(1)
additional space.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [dpo_pairwise_loss_python](dpo_pairwise_loss_python/) |

Other languages are not yet implemented.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover known margins, zero-gap ties, stable extreme margins,
multiple-pair averaging, strict invalid-input validation, and
immutability.

## Limitations

This is **only the loss evaluation**, not training or backpropagation.
The caller must compute correctly normalized *response-sequence*
log probabilities under both the policy and the frozen reference,
with consistent tokenization and prompt conditioning. It does not
implement gradient computation, label smoothing, length normalization,
multi-response rankings, or an optimizer. Accuracy is a diagnostic,
not an estimate of human preference quality.

## References

Rafailov, R., Sharma, A., Mitchell, E., Manning, C. D., Ermon, S.,
& Finn, C. (2023). Direct preference optimization: Your language model
is secretly a reward model. In *Advances in Neural Information
Processing Systems* (Vol. 36). https://arxiv.org/abs/2305.18290
