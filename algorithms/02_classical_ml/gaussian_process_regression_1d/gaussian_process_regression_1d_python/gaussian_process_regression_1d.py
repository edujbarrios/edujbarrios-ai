"""Small one-dimensional Gaussian process regression with RBF covariance."""

import math


def _number(value, name, minimum, maximum, strict_minimum=False):
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError(f"{name} must be a bounded finite real")
    try:
        result = float(value)
    except (ValueError, OverflowError):
        raise ValueError(f"{name} must be a bounded finite real") from None
    if (not math.isfinite(result) or result > maximum or
            (result <= minimum if strict_minimum else result < minimum)):
        raise ValueError(f"{name} must be a bounded finite real")
    return result


def _vector(values, name, allow_empty=False, limit=64):
    if (not isinstance(values, (list, tuple)) or len(values) > limit or
            (not allow_empty and not values)):
        raise ValueError(f"{name} must have {'0' if allow_empty else '1'}..{limit} values")
    return [_number(x, name, -1000, 1000) for x in values]


def _solve_lower(lower, vector):
    result = []
    for i, value in enumerate(vector):
        result.append((value - sum(lower[i][j] * result[j] for j in range(i)))
                      / lower[i][i])
    return result


def _solve_upper_from_lower(lower, vector):
    n = len(lower)
    result = [0.0] * n
    for i in range(n - 1, -1, -1):
        result[i] = (vector[i] - sum(lower[j][i] * result[j]
                                   for j in range(i + 1, n))) / lower[i][i]
    return result


def gaussian_process_regression_1d(x_train, y_train, x_test,
                                   length_scale=1.0, signal_variance=1.0,
                                   noise_variance=1e-2):
    """Return posterior latent means and variances at the requested points."""
    x = _vector(x_train, "x_train")
    y = _vector(y_train, "y_train")
    queries = _vector(x_test, "x_test", allow_empty=True, limit=1024)
    if len(x) != len(y):
        raise ValueError("training inputs and targets must have equal lengths")
    scale = _number(length_scale, "length_scale", 1e-4, 1e4)
    signal = _number(signal_variance, "signal_variance", 0.0, 1e6,
                     strict_minimum=True)
    noise = _number(noise_variance, "noise_variance", 0.0, 1e6)

    def kernel(left, right):
        return signal * math.exp(-0.5 * ((left - right) / scale) ** 2)

    n = len(x)
    jitter = 1e-9 * max(1.0, signal)
    lower = [[0.0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1):
            covariance = kernel(x[i], x[j])
            if i == j:
                covariance += noise + jitter
            residual = covariance - sum(lower[i][k] * lower[j][k]
                                        for k in range(j))
            if i == j:
                if not math.isfinite(residual) or residual <= 0:
                    raise ValueError("covariance matrix is not positive definite")
                lower[i][i] = math.sqrt(residual)
            else:
                lower[i][j] = residual / lower[j][j]

    alpha = _solve_upper_from_lower(lower, _solve_lower(lower, y))
    means, variances = [], []
    for query in queries:
        cross = [kernel(query, xi) for xi in x]
        mean = sum(a * k for a, k in zip(alpha, cross))
        triangular = _solve_lower(lower, cross)
        variance = signal - sum(value * value for value in triangular)
        if not math.isfinite(mean) or not math.isfinite(variance):
            raise ValueError("nonfinite posterior")
        means.append(mean)
        variances.append(max(0.0, variance))
    return {"means": means, "variances": variances}
