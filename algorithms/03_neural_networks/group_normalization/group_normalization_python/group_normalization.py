"""Forward GroupNorm on one flat channel-major example."""
import math


def group_normalization(x, channels, spatial_size, groups, gamma, beta, epsilon):
    """Normalize within channel groups; apply channel-wise affine parameters."""
    if (type(channels) is not int or type(spatial_size) is not int
            or type(groups) is not int or not 1 <= channels <= 32
            or not 1 <= spatial_size <= 64 or not 1 <= groups <= channels
            or channels % groups != 0):
        raise ValueError("channels 1..32, spatial 1..64, groups dividing channels")
    if isinstance(epsilon, bool) or not isinstance(epsilon, (int, float)):
        raise ValueError("epsilon must be a finite real scalar")
    try:
        eps = float(epsilon)
    except OverflowError as exc:
        raise ValueError("epsilon outside finite range") from exc
    if not math.isfinite(eps) or not 0.0 < eps <= 1.0:
        raise ValueError("epsilon must be in (0,1]")
    if (not isinstance(x, (list, tuple)) or len(x) != channels*spatial_size
            or not isinstance(gamma, (list, tuple)) or len(gamma) != channels
            or not isinstance(beta, (list, tuple)) or len(beta) != channels):
        raise ValueError("expected channel-major X, gamma, beta")

    def checked(values):
        result = []
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("expected finite real tensor values")
            try:
                f = float(value)
            except OverflowError as exc:
                raise ValueError("tensor value unrepresentable") from exc
            if not math.isfinite(f) or abs(f) > 1000.0:
                raise ValueError("tensor values must be in [-1000,1000]")
            result.append(f)
        return result

    values, gain, bias = checked(x), checked(gamma), checked(beta)
    per_group = (channels//groups)*spatial_size
    out = [0.0]*(channels*spatial_size)
    for g in range(groups):
        start = g*per_group
        stop = start+per_group
        mean = math.fsum(v/per_group for v in values[start:stop])
        variance = math.fsum(((v-mean)**2)/per_group for v in values[start:stop])
        denom = math.sqrt(variance+eps)
        for i in range(start,stop):
            c = i//spatial_size
            y = gain[c]*((values[i]-mean)/denom)+bias[c]
            if not math.isfinite(y):
                raise ValueError("nonfinite output")
            out[i] = y
    return out
