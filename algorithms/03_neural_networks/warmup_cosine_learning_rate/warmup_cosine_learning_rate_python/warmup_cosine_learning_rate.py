"""A linear warmup followed by one cosine annealing period."""
import math

def warmup_cosine_learning_rate(peak_rate: float, minimum_rate: float,
                                warmup_steps: int, total_steps: int, step: int) -> float:
    """Return a scheduled scalar rate or raise ValueError for invalid arguments."""
    if any(isinstance(x, bool) or not isinstance(x, (int, float))
           for x in (peak_rate, minimum_rate)):
        raise ValueError("rates must be finite real numbers")
    try:
        peak = float(peak_rate)
        minimum = float(minimum_rate)
    except OverflowError as exc:
        raise ValueError("rates must be representable") from exc
    if not (math.isfinite(peak) and math.isfinite(minimum) and
            0 <= minimum <= peak <= 1_000_000):
        raise ValueError("invalid learning rate bounds")
    if (type(warmup_steps) is not int or type(total_steps) is not int or
            type(step) is not int or
            not 1 <= warmup_steps < total_steps <= 100_000 or
            not 0 <= step <= total_steps):
        raise ValueError("invalid warmup, total steps or current step")
    if step <= warmup_steps:
        return peak * (step / warmup_steps)
    if step == total_steps:
        return minimum
    progress = (step - warmup_steps) / (total_steps - warmup_steps)
    return minimum + (peak - minimum) * 0.5 * (1 + math.cos(math.pi * progress))
