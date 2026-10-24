"""Two-hidden-state Viterbi decoding in log probability space."""
import math
from typing import Sequence

def _probabilities(values: Sequence[float]) -> list[float]:
    if not isinstance(values,(list,tuple)) or len(values)!=2:
        raise ValueError("probabilities must contain exactly two values")
    result=[]
    for value in values:
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("probabilities must be finite reals")
        try: x=float(value)
        except OverflowError as exc: raise ValueError("unrepresentable probability") from exc
        if not math.isfinite(x) or not 0<x<=1:
            raise ValueError("probabilities must be strictly positive and <=1")
        result.append(x)
    if abs(sum(result)-1)>1e-9:
        raise ValueError("probability row must sum to one")
    return result

def viterbi_decoding(
    observations: Sequence[int],initial: Sequence[float],
    transition: Sequence[Sequence[float]],
    emission: Sequence[Sequence[float]]
) -> tuple[list[int],float]:
    """Return the most probable state sequence and its joint log probability."""
    if not isinstance(observations,(list,tuple)) or not 1<=len(observations)<=4096:
        raise ValueError("observation sequence length must be 1..4096")
    if any(isinstance(o,bool) or not isinstance(o,int) or o not in (0,1) for o in observations):
        raise ValueError("observations must be integer binary symbols")
    p=_probabilities(initial)
    if (not isinstance(transition,(list,tuple)) or len(transition)!=2 or
        not isinstance(emission,(list,tuple)) or len(emission)!=2):
        raise ValueError("two-row transition and emission matrices required")
    a=[_probabilities(row) for row in transition]
    b=[_probabilities(row) for row in emission]
    prev=[math.log(p[s])+math.log(b[s][observations[0]]) for s in range(2)]
    back=[[0,0] for _ in observations]
    for t in range(1,len(observations)):
        nxt=[]
        for s in range(2):
            c0=prev[0]+math.log(a[0][s])
            c1=prev[1]+math.log(a[1][s])
            predecessor=int(c1>c0)
            back[t][s]=predecessor
            score=(c1 if predecessor else c0)+math.log(b[s][observations[t]])
            if not math.isfinite(score):
                raise ValueError("nonfinite path score")
            nxt.append(score)
        prev=nxt
    state=int(prev[1]>prev[0])
    probability=prev[state]
    if not math.isfinite(probability):
        raise ValueError("nonfinite path probability")
    path=[0]*len(observations)
    for t in range(len(observations)-1,-1,-1):
        path[t]=state
        if t>0: state=back[t][state]
    return path,probability
