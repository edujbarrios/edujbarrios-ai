# Multinomial Naive Bayes for Binary Count Features

## AI motivation

Multinomial Naive Bayes is a foundational **probabilistic text classifier**.
Given document term-frequency vectors, it estimates class-specific token
probabilities, uses a conditional-independence model, and combines
log-probabilities to avoid multiplying many small numbers. It is useful
for teaching spam detection, document categorization, and bag-of-words
classification (Manning et al., 2008).

Unlike the repository's Gaussian Naive Bayes, **features here are
nonnegative integer counts**, not continuous normally distributed values.

## Training and inference

Given `n` documents with `d` vocabulary dimensions, matrix `X[n,d]`,
and labels `y ∈ {0,1}`, count the training documents in each class
(`N_c`) and token occurrences (`T_cj`). Require both classes.

With **add-one Laplace smoothing**, calculate:

```text
log_prior[c] = log(N_c/n)
log_likelihood[c,j] = log((T_cj+1)/(sum_j T_cj+d))

score[c | x] = log_prior[c] + sum_j x[j]*log_likelihood[c,j]
prediction(x) = 1 if score[1|x] > score[0|x], else 0
```

The prior is **not smoothed**. The denominator of each token
likelihood includes the vocabulary size `d`. Scores are
**unnormalized log joint probabilities**, not posterior probabilities.
Ties predict class 0.

Example training data:

```text
X = [[3,0], [2,0], [0,3], [0,2]]
y = [0,0,1,1]

log_prior = [ln(1/2),ln(1/2)]
log_likelihood[0] = [ln(6/7),ln(1/7)]
log_likelihood[1] = [ln(1/7),ln(6/7)]

predict([2,0]) -> 0
predict([0,2]) -> 1
predict([0,0]) -> 0    # prior tie
```

## Shared contract

- Training input is a **nonempty** `n×d` matrix, `d>=1`, of
  **integer counts in [0,1,000,000]**. Every row has a binary class
  label 0 or 1; **both classes must occur**.
- Any input query is exactly `d` nonnegative integer counts in the
  same range, including all-zero queries.
- Training returns two `log_prior` values and a 2×d matrix of
  `log_likelihood` values. Inference returns a binary label with
  the class-0 tie convention.
- Reject nonbinary labels, negative, fractional, nonfinite or
  out-of-range counts, shape mismatches and nonfinite intermediates.
- To keep counts exact when converted to double, reject accumulated
  per-class token counts above `2^53-1`. This is an educational
  numerical safety bound, not a corpus capacity claim.
- C uses caller-allocated **row-major** counts `X[n*d]`, a two-value
  `log_priors[2]`, and `log_likelihoods[2*d]` (class-major),
  preserving both output buffers on failure; inference takes the
  same fitted arrays and a `uint32_t query[d]`.
  Rust also uses flat row-major count arrays. Python takes nested
  integer lists and returns `(priors,likelihoods)`.
  R takes a base numeric count matrix and returns a named list.
- In C the caller supplies correctly sized, nonoverlapping buffers.
  Python/R raise errors; Rust returns `Result`.

## Complexity

Fit in **O(n*d)** time and **O(d)** auxiliary memory (besides
output). Prediction takes O(d) time and O(1) extra space.
Python copies input validation data (O(n*d) space).

## Implementations

| Language | Directory |
| --- | --- |
| C | [multinomial_naive_bayes_c](multinomial_naive_bayes_c/) |
| Python | [multinomial_naive_bayes_python](multinomial_naive_bayes_python/) |
| Rust | [multinomial_naive_bayes_rust](multinomial_naive_bayes_rust/) |
| R | [multinomial_naive_bayes_r](multinomial_naive_bayes_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests check exact log probabilities on a small corpus, classification,
all-zero queries, empty input, a missing class, bad counts, bad labels,
incorrect query dimensions, and nonfinite fitted parameters.

## Limitations

Two classes only; fixed vocabulary and add-one smoothing. No token
extraction, text normalization, unknown-word model, vocabulary fitting,
multiclass expansion, class-weighted priors, posterior normalization
or out-of-core training. The multinomial independence assumption is
often only an approximation. Training and prediction are CPU-only,
without unverified CUDA acceleration.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
