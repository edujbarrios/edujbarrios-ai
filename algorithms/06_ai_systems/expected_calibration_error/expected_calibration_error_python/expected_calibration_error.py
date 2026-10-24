"""Binary expected calibration error using equal-width top-class confidence bins."""
import math
from typing import Sequence

def expected_calibration_error(probabilities:Sequence[float],
                               labels:Sequence[int],bins:int)->float:
    """Return the weighted confidence/accuracy gap over occupied bins."""
    if (not isinstance(probabilities,(list,tuple)) or
        not isinstance(labels,(list,tuple)) or not probabilities or
        len(probabilities)!=len(labels)):
        raise ValueError("nonempty aligned probabilities and labels required")
    if isinstance(bins,bool) or not isinstance(bins,int) or not 1<=bins<=100:
        raise ValueError("bin count must be integer 1..100")
    counts=[0]*bins
    confidence_sums=[0.0]*bins
    correct_sums=[0.0]*bins
    for p,y in zip(probabilities,labels):
        if isinstance(p,bool) or not isinstance(p,(int,float)):
            raise ValueError("finite probability required")
        try:prob=float(p)
        except OverflowError as exc:raise ValueError("unrepresentable probability") from exc
        if (not math.isfinite(prob) or not 0<=prob<=1 or
            isinstance(y,bool) or not isinstance(y,int) or y not in (0,1)):
            raise ValueError("invalid probability or binary label")
        prediction=int(prob>=.5)
        confidence=max(prob,1-prob)
        slot=min(bins-1,int(math.floor(confidence*bins)))
        counts[slot]+=1
        confidence_sums[slot]+=confidence
        correct_sums[slot]+=int(prediction==y)
        if not all(map(math.isfinite,(confidence_sums[slot],correct_sums[slot]))):
            raise ValueError("ECE bin overflow")
    score=0.0
    for count,conf_sum,correct_sum in zip(counts,confidence_sums,correct_sums):
        if count:
            score+=(count/len(probabilities))*abs(correct_sum/count-conf_sum/count)
    if not math.isfinite(score) or not 0<=score<=1+1e-12:
        raise ValueError("invalid ECE")
    return min(1.0,score)
