"""Token-level language-model perplexity from natural log probabilities."""
import math
from typing import Sequence

def perplexity(log_probabilities:Sequence[float])->float:
    """Return exp(mean negative log probability), requiring a finite result."""
    if (not isinstance(log_probabilities,(list,tuple)) or
        not 1<=len(log_probabilities)<=4096):
        raise ValueError("nonempty log-probability vector length 1..4096 required")
    n=len(log_probabilities)
    mean_nll=0.0
    for value in log_probabilities:
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("finite natural log probabilities required")
        try: logp=float(value)
        except OverflowError as exc:raise ValueError("unrepresentable log probability") from exc
        if not math.isfinite(logp) or logp>0:
            raise ValueError("log probabilities must be finite and nonpositive")
        contribution=(-logp)/n
        mean_nll+=contribution
        if not math.isfinite(contribution) or not math.isfinite(mean_nll):
            raise ValueError("negative log-likelihood overflow")
    try: result=math.exp(mean_nll)
    except OverflowError as exc:raise ValueError("perplexity exponential overflow") from exc
    if not math.isfinite(result) or result<1:
        raise ValueError("invalid finite perplexity")
    return result
