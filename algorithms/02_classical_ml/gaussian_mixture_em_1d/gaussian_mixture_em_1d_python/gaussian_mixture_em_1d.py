"""Deterministic two-component one-dimensional Gaussian mixture via EM."""

import math


_MIN_VARIANCE = 1e-6


def _expectation(data, weights, means, variances):
    """Compute first-component responsibilities and mixture log-likelihood."""
    responsibilities = []
    total_log_likelihood = 0.0
    for x in data:
        scores = []
        for weight, mean, variance in zip(weights, means, variances):
            scores.append(math.log(weight) - 0.5 *
                          (math.log(2 * math.pi * variance) +
                           (x - mean) ** 2 / variance))
        maximum = max(scores)
        e0, e1 = math.exp(scores[0] - maximum), math.exp(scores[1] - maximum)
        denominator = e0 + e1
        responsibilities.append(e0 / denominator)
        total_log_likelihood += maximum + math.log(denominator)
    if not math.isfinite(total_log_likelihood):
        raise ValueError("mixture log-likelihood is non-finite")
    return responsibilities, total_log_likelihood


def gaussian_mixture_em_1d(values, max_iterations=100, tolerance=1e-8):
    """Fit two Gaussian components; return weights, means, variances and fit data."""
    if not isinstance(values, (list, tuple)) or not 2 <= len(values) <= 4096:
        raise ValueError("expected 2..4096 finite observations")
    data = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("observations must be bounded finite numbers")
        try:
            x = float(value)
        except (ValueError, OverflowError):
            raise ValueError("observations must be bounded finite numbers") from None
        if not math.isfinite(x) or abs(x) > 1_000_000:
            raise ValueError("observations must be bounded finite numbers")
        data.append(x)
    if min(data) == max(data):
        raise ValueError("at least two distinct observations are required")
    if isinstance(max_iterations, bool) or not isinstance(max_iterations, int) or \
            not 1 <= max_iterations <= 1000:
        raise ValueError("max_iterations must be an integer from 1 to 1000")
    if isinstance(tolerance, bool) or not isinstance(tolerance, (int, float)) or \
            not math.isfinite(tolerance) or tolerance < 0:
        raise ValueError("tolerance must be a finite nonnegative real number")

    means = [min(data), max(data)]
    midpoint = sum(data) / len(data)
    initial_variance = max(sum((x - midpoint) ** 2 for x in data) / len(data),
                           _MIN_VARIANCE)
    variances = [initial_variance, initial_variance]
    weights = [0.5, 0.5]
    responsibilities, old_ll = _expectation(data, weights, means, variances)
    iterations = 0
    for step in range(max_iterations):
        mass0 = sum(responsibilities)
        mass1 = len(data) - mass0
        if min(mass0, mass1) <= 1e-12:
            break  # Avoid division by zero if a component has collapsed.
        new_means = [sum(r * x for r, x in zip(responsibilities, data)) / mass0,
                     sum((1 - r) * x for r, x in zip(responsibilities, data)) / mass1]
        new_variances = [
            max(sum(r * (x - new_means[0]) ** 2 for r, x in
                    zip(responsibilities, data)) / mass0, _MIN_VARIANCE),
            max(sum((1 - r) * (x - new_means[1]) ** 2 for r, x in
                    zip(responsibilities, data)) / mass1, _MIN_VARIANCE),
        ]
        new_weights = [mass0 / len(data), mass1 / len(data)]
        next_responsibilities, next_ll = _expectation(
            data, new_weights, new_means, new_variances)
        weights, means, variances = new_weights, new_means, new_variances
        iterations = step + 1
        if abs(next_ll - old_ll) <= tolerance:
            old_ll = next_ll
            break
        responsibilities, old_ll = next_responsibilities, next_ll

    components = sorted(zip(means, weights, variances))
    return {
        "means": [component[0] for component in components],
        "weights": [component[1] for component in components],
        "variances": [component[2] for component in components],
        "log_likelihood": old_ll,
        "iterations": iterations,
    }
