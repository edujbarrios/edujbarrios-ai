"""Laplace-smoothed two-class Bernoulli Naive Bayes from scratch."""
import math


def bernoulli_naive_bayes(samples: list[int], labels: list[int],
                          n: int, d: int, query: list[int]) -> tuple[int, float]:
    """Fit binary-feature likelihoods and predict (label, probability of class 1)."""
    if any(isinstance(x, bool) or not isinstance(x, int)
           for x in (n, d)) or not (2 <= n <= 128 and 1 <= d <= 16):
        raise ValueError("n in 2..128 and d in 1..16 required")

    def validate(values: list[int], length: int) -> None:
        if not isinstance(values, (list, tuple)) or len(values) != length or any(
                isinstance(v, bool) or not isinstance(v, int) or v not in (0, 1)
                for v in values):
            raise ValueError("binary integer vector with exact length required")

    validate(samples, n * d)
    validate(labels, n)
    validate(query, d)
    counts = [0, 0]
    ones = [[0] * d for _ in range(2)]
    for i, label in enumerate(labels):
        counts[label] += 1
        for j in range(d):
            ones[label][j] += samples[i * d + j]
    if not all(counts):
        raise ValueError("both classes must occur")

    scores = [math.log(counts[c] / n) for c in range(2)]
    for c in range(2):
        for j, bit in enumerate(query):
            p = (ones[c][j] + 1) / (counts[c] + 2)
            scores[c] += math.log(p) if bit else math.log1p(-p)
    delta = scores[0] - scores[1]
    posterior = (math.exp(-delta) / (1 + math.exp(-delta))
                 if delta >= 0 else 1 / (1 + math.exp(delta)))
    return (int(scores[1] > scores[0]), posterior)
