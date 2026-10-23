# Binary Cross-Entropy

## Purpose

Binary cross-entropy evaluates probabilistic binary classifiers, including logistic regression and neural network outputs. This implementation accepts probabilities rather than logits and is intended as an understandable CPU reference.

For labels y in {0,1} and probabilities p in [0,1], the mean loss is -mean(y log(p) + (1-y) log(1-p)), evaluated with branches so 0 log(0) is not computed for the inactive class. Correctly predicted exact boundaries have zero loss; assigning probability zero to the true class is rejected as an infinite loss.

## Contract

Inputs have the same nonzero length; labels must be exactly 0 or 1 and probabilities finite in [0,1]. Return the finite mean loss, otherwise fail. C takes caller-owned arrays and an output pointer unchanged on failure. Python/R raise errors; Rust returns Result. Complexity O(n) time, O(1) auxiliary space. This is a probability-space reference, not a numerically stable logits-based loss or a training gradient.

## Implementations

- [C](binary_cross_entropy_c/)
- [Python](binary_cross_entropy_python/)
- [Rust](binary_cross_entropy_rust/)
- [R](binary_cross_entropy_r/)

## Tests

Run `python3 scripts/run_tests.py --strict` at the repository root. Tests cover exact and uncertain predictions, invalid labels, boundaries, and non-finite values.

## References

Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*. MIT Press. https://www.deeplearningbook.org/
