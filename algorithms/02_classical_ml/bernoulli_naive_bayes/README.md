# Bernoulli Naive Bayes

## Why it belongs in this repository

Bernoulli Naive Bayes is a lightweight probabilistic classifier for binary attributes such as whether a term is present in a document. Unlike multinomial Naive Bayes, it models **both the presence and absence** of each feature. It complements the Gaussian and multinomial variants already in the Classical ML collection (Bishop, 2006).

## Algorithm

Given n samples with d binary features and class labels 0 or 1, let n_c denote the number of training samples in class c. Every class must appear. Compute the empirical class prior n_c/n and Laplace-smoothed Bernoulli parameters:

    p[c,j] = (number of class-c samples with feature j equal to 1 + 1) / (n_c + 2)

For a binary query q, compute in log space:

    score[c] = ln(n_c/n) + sum_j (q[j]*ln(p[c,j]) + (1-q[j])*ln(1-p[c,j]))

Return class 1 if score[1] > score[0], otherwise class 0 (exact ties select 0). Also return the posterior probability of class 1 using the stable two-class softmax

    probability_1 = 1 / (1 + exp(score[0]-score[1])).

This implementation fits and predicts for **one query**, rather than storing a reusable model.

## Contract

- Flat row-major training feature vector of length n*d, n labels, and d query features, all with integer values **0 or 1**.
- 2 <= n <= 128; 1 <= d <= 16; both label classes must appear. The number of training entries and labels must exactly match the declared dimensions.
- Output: predicted label in {0,1}, probability of class 1 in [0,1].
- C accepts uint8_t arrays, requires valid output pointers, and preserves both outputs on failure. Rust returns `Option<(u8, f64)>`; Python returns `(int, float)`; R returns `list(label, probability_1)`.
- Python accepts lists/tuples of exact integers and rejects booleans. R accepts one-dimensional numeric vectors of whole finite binary values and integer-valued scalar n and d. All malformed inputs are rejected.
- Inputs are not modified. The specified bounds avoid extreme numerical ranges for the smoothed log likelihoods.

## Complexity

O(n*d) time and O(d) auxiliary space to accumulate conditional feature counts for both classes. Predictions use O(d) time.

## Implementations

| Language | Directory |
| --- | --- |
| C | [bernoulli_naive_bayes_c](bernoulli_naive_bayes_c/) |
| Python | [bernoulli_naive_bayes_python](bernoulli_naive_bayes_python/) |
| Rust | [bernoulli_naive_bayes_rust](bernoulli_naive_bayes_rust/) |
| R | [bernoulli_naive_bayes_r](bernoulli_naive_bayes_r/) |

## Examples and tests

Training rows: `[[1,0],[1,1],[0,1],[0,0]]` with labels `[0,0,1,1]`.

- Query `[1,0]` yields label 0, probability_1 = 0.25.
- Query `[0,1]` yields label 1, probability_1 = 0.75.
- Equal evidence selects class 0 at probability_1 = 0.5.

Tests cover class predictions, probabilistic outputs, one feature, uneven class priors, exact ties, 128x16 bounds and invalid input shapes, labels and feature values.

From the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This reference supports exactly two classes, fixed unit Laplace smoothing, finite binary inputs, and a single query per call. It assumes conditional independence, does not tokenize text, and does not learn feature weights or tune smoothing. The resulting probabilities may be poorly calibrated.

## References

Bishop, C. M. (2006). *Pattern recognition and machine learning*. Springer.
