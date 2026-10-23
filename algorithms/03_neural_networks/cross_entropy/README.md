# Multiclass Cross-Entropy from Logits

**Stage:** Neural Networks · **Languages:** C, Python, Rust, R.

## Why it belongs here

Cross-entropy measures how surprising the correct class is under a
classification model. It is a standard learning objective for multiclass
neural networks. Implementing it directly from logits using the
log-sum-exp identity avoids the loss of precision that would occur by
computing softmax and then taking a logarithm (Goodfellow et al., 2016).

## Mathematical definition

For finite logits `z` and **zero-based** correct class `t`:

```text
loss(z, t) = log(sum(exp(z[i]))) - z[t]
m = max(z)
loss(z, t) = log(sum(exp(z[i] - m))) - (z[t] - m)
```

Examples: `[0,0]`, target 0 → `log(2)`; `[1000,0]`,
target 0 → approximately 0; target 1 → approximately 1000.

## Shared contract

- Input: one nonempty sequence of finite real logits and an integer target
  index in `0..n-1`. **R also uses a zero-based target index**.
- Output: one finite, nonnegative loss. Tiny negative results due to
  rounding are clamped to zero.
- Empty logits, invalid indices, NaN/infinity, unrepresentable differences
  or sum: errors. No class weights, label smoothing or batches.
- C API accepts caller-owned input of length `n` and writes one double
  on success only. Python/R raise errors. Rust returns `Result<f64,...>`.
- The algorithm calculates a stable scalar loss, not calibrated
  probabilities and not the derivative of the loss.

## Complexity

**O(n)** time. O(1) auxiliary memory in C, Python and Rust; R may allocate
O(n) intermediate arrays to subtract the maximum and exponentiate.

## Implementations

| Language | Directory |
| --- | --- |
| C | [cross_entropy_c](cross_entropy_c/) |
| Python | [cross_entropy_python](cross_entropy_python/) |
| Rust | [cross_entropy_rust](cross_entropy_rust/) |
| R | [cross_entropy_r](cross_entropy_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Cases cover uniform logits, highly confident correct and incorrect
predictions, integer bounds, empty data and non-finite values.

## Limitations

Single classification example, no class weights, batch reductions,
label smoothing, autodiff, GPU support or guarantees on highly unbalanced
real-world datasets. The index API uses zero-based labels in every language.

## References

Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*.
MIT Press. https://www.deeplearningbook.org/
