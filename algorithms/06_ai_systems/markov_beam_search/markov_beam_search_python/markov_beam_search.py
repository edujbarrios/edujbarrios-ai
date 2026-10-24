"""Deterministic bounded beam search over a first-order Markov token model."""
import math
from typing import Sequence

def _distribution(row:Sequence[float],n:int)->list[float]:
    if not isinstance(row,(list,tuple)) or len(row)!=n:
        raise ValueError("probability row of correct vocabulary width required")
    checked=[]
    for value in row:
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("finite positive probability required")
        try:p=float(value)
        except OverflowError as exc:raise ValueError("unrepresentable probability") from exc
        if not math.isfinite(p) or not 0<p<=1:
            raise ValueError("probabilities must be in (0,1]")
        checked.append(p)
    if abs(sum(checked)-1)>1e-9:
        raise ValueError("probabilities must sum to one")
    return checked

def markov_beam_search(initial:Sequence[float],
                       transition:Sequence[Sequence[float]],
                       length:int,width:int)->tuple[list[int],float]:
    """Return (best token path, log joint probability) of retained hypotheses."""
    if not isinstance(initial,(list,tuple)) or not 1<=len(initial)<=8:
        raise ValueError("vocabulary size must be 1..8")
    if (isinstance(length,bool) or not isinstance(length,int) or
        not 1<=length<=8 or isinstance(width,bool) or
        not isinstance(width,int) or not 1<=width<=8):
        raise ValueError("length and beam width must be 1..8")
    n=len(initial)
    p=_distribution(initial,n)
    if not isinstance(transition,(list,tuple)) or len(transition)!=n:
        raise ValueError("n by n transition matrix required")
    matrix=[_distribution(row,n) for row in transition]
    beam=[(0.0,())]
    for step in range(length):
        candidates=[]
        for score,path in beam:
            for token in range(n):
                probability=p[token] if step==0 else matrix[path[-1]][token]
                next_score=score+math.log(probability)
                if not math.isfinite(next_score):
                    raise ValueError("beam score overflow")
                candidates.append((next_score,path+(token,)))
        candidates.sort(key=lambda item:(-item[0],item[1]))
        beam=candidates[:width]
    score,path=beam[0]
    return list(path),score
