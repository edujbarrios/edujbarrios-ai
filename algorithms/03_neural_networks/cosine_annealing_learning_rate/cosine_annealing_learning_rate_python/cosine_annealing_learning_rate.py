"""One-period stateless cosine learning-rate annealing."""
import math


def cosine_annealing_learning_rate(max_rate, min_rate, step, total_steps):
    """Return the cosine-interpolated rate at a bounded training step."""
    if (isinstance(max_rate,bool) or isinstance(min_rate,bool) or
        not isinstance(max_rate,(int,float)) or
        not isinstance(min_rate,(int,float))):
        raise ValueError("numeric rate bounds required")
    try:
        high=float(max_rate)
        low=float(min_rate)
    except OverflowError as exc:
        raise ValueError("unrepresentable rate") from exc
    if (not math.isfinite(high) or not math.isfinite(low) or
        not 0<=low<=high<=1_000_000):
        raise ValueError("finite ordered rate bounds in 0..1000000 required")
    if (isinstance(step,bool) or isinstance(total_steps,bool) or
        not isinstance(step,int) or not isinstance(total_steps,int) or
        not 1<=total_steps<=100_000 or not 0<=step<=total_steps):
        raise ValueError("integer step in 0..total_steps, total_steps in 1..100000")
    fraction=(1+math.cos(math.pi*step/total_steps))/2
    return max(low,min(high,low+(high-low)*fraction))
