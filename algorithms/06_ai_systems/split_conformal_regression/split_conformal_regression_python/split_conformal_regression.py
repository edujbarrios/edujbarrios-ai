"""Distribution-free split conformal regression intervals from held-out errors."""

import math


def _finite_vector(values, name, allow_empty=False):
    if not isinstance(values, (list, tuple)) or (not allow_empty and not values):
        raise ValueError(f"{name} must be a {'possibly empty' if allow_empty else 'nonempty'} list or tuple")
    if len(values) > 100_000:
        raise ValueError(f"{name} exceeds the 100000-element teaching limit")
    result = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError(f"{name} must contain finite real numbers")
        try:
            converted = float(value)
        except (OverflowError, ValueError):
            raise ValueError(f"{name} must contain finite real numbers") from None
        if not math.isfinite(converted):
            raise ValueError(f"{name} must contain finite real numbers")
        result.append(converted)
    return result


def split_conformal_regression(calibration_targets, calibration_predictions,
                               test_predictions, alpha=0.1):
    """Return (lower, upper) intervals at miscoverage level alpha.

    The fixed predictor must not be fit on the held-out calibration examples.
    For alpha < 1/(n+1), a mathematically valid but uninformative
    (-inf, inf) interval is returned rather than dropping the guarantee.
    """
    if isinstance(alpha, bool) or not isinstance(alpha, (int, float)):
        raise ValueError("alpha must be a finite number strictly between 0 and 1")
    try:
        alpha = float(alpha)
    except (OverflowError, ValueError):
        raise ValueError("alpha must be a finite number strictly between 0 and 1") from None
    if not math.isfinite(alpha) or not 0 < alpha < 1:
        raise ValueError("alpha must be a finite number strictly between 0 and 1")

    targets = _finite_vector(calibration_targets, "calibration_targets")
    predictions = _finite_vector(calibration_predictions, "calibration_predictions")
    queries = _finite_vector(test_predictions, "test_predictions", allow_empty=True)
    if len(targets) != len(predictions):
        raise ValueError("calibration targets and predictions must be aligned")

    residuals = []
    for target, predicted in zip(targets, predictions):
        residual = abs(target - predicted)
        if not math.isfinite(residual):
            raise ValueError("calibration residuals overflowed")
        residuals.append(residual)
    rank = math.ceil((len(residuals) + 1) * (1 - alpha))
    radius = math.inf if rank > len(residuals) else sorted(residuals)[rank - 1]
    intervals = []
    for predicted in queries:
        if math.isinf(radius):
            intervals.append((-math.inf, math.inf))
            continue
        lower, upper = predicted - radius, predicted + radius
        if not (math.isfinite(lower) and math.isfinite(upper)):
            raise ValueError("prediction interval endpoints overflowed")
        intervals.append((lower, upper))
    return intervals
