"""Deterministic Maximal Marginal Relevance reranking."""
import math
from typing import Sequence

def _real(value: float,low: float,high: float) -> float:
    if isinstance(value,bool) or not isinstance(value,(float,int)):
        raise ValueError("finite real scores required")
    try: result=float(value)
    except OverflowError as exc: raise ValueError("unrepresentable score") from exc
    if not math.isfinite(result) or not low<=result<=high:
        raise ValueError("score out of finite allowed range")
    return result

def maximal_marginal_relevance(
    relevance: Sequence[float],
    similarities: Sequence[Sequence[float]],
    tradeoff: float,k:int
) -> list[int]:
    """Return k distinct zero-based indices in greedy MMR selection order."""
    if not isinstance(relevance,(list,tuple)) or not relevance:
        raise ValueError("nonempty relevance required")
    n=len(relevance)
    if isinstance(k,bool) or not isinstance(k,int) or not 1<=k<=n:
        raise ValueError("k must be an integer in 1..n")
    lam=_real(tradeoff,0,1)
    r=[_real(x,0,1) for x in relevance]
    if not isinstance(similarities,(list,tuple)) or len(similarities)!=n:
        raise ValueError("similarity matrix must be n by n")
    matrix=[]
    for row in similarities:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("similarity matrix must be n by n")
        matrix.append([_real(x,-1,1) for x in row])
    used=set()
    nearest=[-math.inf]*n
    selected=[]
    for step in range(k):
        best=None
        for i in range(n):
            if i in used:
                continue
            score=lam*r[i] - (0 if step==0 else (1-lam)*nearest[i])
            if not math.isfinite(score):
                raise ValueError("MMR score overflow")
            if best is None or score>best[0]:
                best=(score,i)
        chosen=best[1]
        selected.append(chosen)
        used.add(chosen)
        for i in range(n):
            if i not in used:
                nearest[i]=max(nearest[i],matrix[i][chosen])
    return selected
