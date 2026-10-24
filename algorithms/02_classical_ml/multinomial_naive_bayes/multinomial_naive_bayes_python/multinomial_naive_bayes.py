"""Multinomial Naive Bayes with add-one smoothing for binary labels."""
import math
from typing import Sequence

MAX_COUNT=1_000_000
MAX_EXACT_SUM=2**53-1

def _count(value: int) -> int:
    if isinstance(value,bool) or not isinstance(value,int) or not 0<=value<=MAX_COUNT:
        raise ValueError("counts must be integers in 0..1,000,000")
    return value

def multinomial_naive_bayes(
    rows: Sequence[Sequence[int]], labels: Sequence[int]
) -> tuple[list[float],list[list[float]]]:
    """Return two log class priors and two rows of smoothed log token probabilities."""
    if not isinstance(rows,(list,tuple)) or not rows or not isinstance(rows[0],(list,tuple)) or not rows[0] or len(rows)!=len(labels):
        raise ValueError("nonempty aligned count matrix and labels required")
    d=len(rows[0])
    tokens=[[0]*d for _ in range(2)]
    members=[0,0]
    totals=[0,0]
    for row,label in zip(rows,labels):
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("count matrix must be rectangular")
        if isinstance(label,bool) or not isinstance(label,int) or label not in (0,1):
            raise ValueError("labels must be integer 0/1")
        members[label]+=1
        for j,value in enumerate(row):
            count=_count(value)
            tokens[label][j]+=count
            totals[label]+=count
            if tokens[label][j]>MAX_EXACT_SUM or totals[label]>MAX_EXACT_SUM:
                raise ValueError("token count exceeds exact integer range")
    if not all(members):
        raise ValueError("both classes required")
    priors=[math.log(m/len(rows)) for m in members]
    likelihoods=[[math.log((value+1)/(totals[c]+d)) for value in tokens[c]]
                 for c in range(2)]
    if not all(map(math.isfinite,priors+likelihoods[0]+likelihoods[1])):
        raise ValueError("nonfinite model")
    return priors,likelihoods

def multinomial_naive_bayes_predict(
    priors: Sequence[float], likelihoods: Sequence[Sequence[float]],
    counts: Sequence[int]
) -> int:
    """Return most likely class, breaking score ties toward 0."""
    if (not isinstance(priors,(list,tuple)) or len(priors)!=2 or
        not isinstance(likelihoods,(list,tuple)) or len(likelihoods)!=2 or
        not isinstance(counts,(list,tuple)) or not counts or
        any(not isinstance(row,(list,tuple)) or len(row)!=len(counts) for row in likelihoods)):
        raise ValueError("invalid fitted parameter shapes")
    def finite_real(value):
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("finite model parameters required")
        try: number=float(value)
        except OverflowError as exc: raise ValueError("unrepresentable parameter") from exc
        if not math.isfinite(number): raise ValueError("nonfinite parameter")
        return number
    p=[finite_real(x) for x in priors]
    w=[[finite_real(x) for x in row] for row in likelihoods]
    q=[_count(x) for x in counts]
    scores=p[:]
    for j,value in enumerate(q):
        for c in range(2):
            term=value*w[c][j]
            scores[c]+=term
            if not math.isfinite(term) or not math.isfinite(scores[c]):
                raise ValueError("nonfinite prediction score")
    return int(scores[1]>scores[0])
