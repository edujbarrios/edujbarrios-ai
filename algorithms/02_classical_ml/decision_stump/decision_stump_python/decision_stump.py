"""Fit an exhaustive single-feature binary decision stump."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value, bool) or not isinstance(value, (int,float)):
        raise ValueError("features must be real and finite")
    try:
        result=float(value)
    except OverflowError as exc:
        raise ValueError("unrepresentable feature") from exc
    if not math.isfinite(result):
        raise ValueError("features must be finite")
    return result

def decision_stump_predict(feature: float, threshold: float,
                           positive_right: bool) -> int:
    """Predict an integer 0/1 label from fitted threshold and orientation."""
    x,t=_real(feature),_real(threshold)
    if not isinstance(positive_right,bool):
        raise ValueError("orientation must be a boolean")
    return int(x>t) if positive_right else int(x<=t)

def decision_stump(x: Sequence[float], labels: Sequence[int]
                   ) -> tuple[float,bool,int]:
    """Return (threshold, positive_right, classification_error_count)."""
    if not x or len(x)!=len(labels):
        raise ValueError("nonempty equal-length features and labels required")
    values=[_real(v) for v in x]
    if any(isinstance(y,bool) or not isinstance(y,int) or y not in (0,1)
           for y in labels):
        raise ValueError("binary integer labels required")
    best=None
    for threshold in values:
        for right in (True,False):
            errors=sum(((v>threshold) if right else (v<=threshold)) != bool(y)
                       for v,y in zip(values,labels))
            key=(errors,threshold,0 if right else 1)
            if best is None or key<best[0]:
                best=(key,(threshold,right,errors))
    return best[1]
