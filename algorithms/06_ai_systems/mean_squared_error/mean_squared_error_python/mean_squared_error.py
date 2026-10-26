"""Mean squared error with validated finite numeric observations."""
import math
from typing import Sequence


def mean_squared_error(actual: Sequence[float], predicted: Sequence[float]) -> float:
    """Return mean squared residual, rejecting nonfinite intermediate results."""
    def validate(values: Sequence[float]) -> list[float]:
        if not isinstance(values, (list, tuple)) or not values:
            raise ValueError("nonempty real vector required")
        result = []
        for item in values:
            if isinstance(item, bool) or not isinstance(item, (int, float)):
                raise ValueError("finite real values required")
            try:
                value = float(item)
            except OverflowError as exc:
                raise ValueError("unrepresentable real value") from exc
            if not math.isfinite(value):
                raise ValueError("finite real values required")
            result.append(value)
        return result

    y = validate(actual)
    prediction = validate(predicted)
    if len(y) != len(prediction):
        raise ValueError("actual and predicted lengths must match")
    mean = 0.0
    for index, (a, b) in enumerate(zip(y, prediction), start=1):
        residual = a - b
        squared = residual * residual
        if not math.isfinite(squared):
            raise ValueError("squared residual overflow")
        mean += (squared - mean) / index
    if not math.isfinite(mean):
        raise ValueError("nonfinite mean squared error")
    return max(0.0, mean)
