"""Log-space Forward likelihood for a two-state binary-emission HMM."""
import math
from typing import Sequence

def _pair(values:Sequence[float])->list[float]:
    if not isinstance(values,(list,tuple)) or len(values)!=2:
        raise ValueError("expected two probabilities")
    result=[]
    for v in values:
        if isinstance(v,bool) or not isinstance(v,(int,float)):
            raise ValueError("finite probability required")
        try: x=float(v)
        except OverflowError as exc:raise ValueError("unrepresentable probability") from exc
        if not math.isfinite(x) or not 0<x<=1:
            raise ValueError("probabilities must be strictly positive and <=1")
        result.append(x)
    if abs(sum(result)-1)>1e-9:
        raise ValueError("probability pair must sum to one")
    return result

def _logsumexp(a:float,b:float)->float:
    high=max(a,b)
    low=min(a,b)
    return high+math.log1p(math.exp(low-high))

def hmm_forward_algorithm(
    observations:Sequence[int], initial:Sequence[float],
    transition:Sequence[Sequence[float]],
    emission:Sequence[Sequence[float]]
)->float:
    """Return the marginal log-probability of an observed binary sequence."""
    if (not isinstance(observations,(list,tuple)) or
        not 1<=len(observations)<=4096 or
        any(isinstance(x,bool) or not isinstance(x,int) or x not in (0,1)
            for x in observations)):
        raise ValueError("binary sequence length must be 1..4096")
    prior=_pair(initial)
    if (not isinstance(transition,(list,tuple)) or len(transition)!=2 or
        not isinstance(emission,(list,tuple)) or len(emission)!=2):
        raise ValueError("two-row transition and emission matrices required")
    a=[_pair(row) for row in transition]
    b=[_pair(row) for row in emission]
    prev=[math.log(prior[s])+math.log(b[s][observations[0]]) for s in range(2)]
    for obs in observations[1:]:
        nxt=[]
        for s in range(2):
            value=_logsumexp(prev[0]+math.log(a[0][s]),
                             prev[1]+math.log(a[1][s]))+math.log(b[s][obs])
            if not math.isfinite(value):
                raise ValueError("forward update overflow")
            nxt.append(value)
        prev=nxt
    result=_logsumexp(*prev)
    if not math.isfinite(result) or result>1e-12:
        raise ValueError("invalid observation log-likelihood")
    return min(0.0,result)
