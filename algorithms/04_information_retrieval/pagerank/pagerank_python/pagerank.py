"""PageRank with 100 fixed damped iterations on a weighted directed graph."""
import math
from typing import Sequence

def pagerank(weights:Sequence[Sequence[float]])->list[float]:
    """Return stationary-link-importance approximation in original node order."""
    if not isinstance(weights,(list,tuple)) or not 1<=len(weights)<=64:
        raise ValueError("expected 1..64 rows of directed edge weights")
    n=len(weights)
    matrix=[]
    out=[]
    for row in weights:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("weighted adjacency must be square")
        converted=[]
        total=0.0
        for weight in row:
            if isinstance(weight,bool) or not isinstance(weight,(int,float)):
                raise ValueError("finite real weights required")
            try: value=float(weight)
            except OverflowError as exc:raise ValueError("unrepresentable edge") from exc
            if not math.isfinite(value) or not 0<=value<=1_000_000:
                raise ValueError("weight outside allowed range")
            converted.append(value)
            total+=value
        if not math.isfinite(total): raise ValueError("row sum overflow")
        matrix.append(converted)
        out.append(total)
    rank=[1.0/n]*n
    for _ in range(100):
        next_rank=[.15/n]*n
        for i in range(n):
            if out[i]==0:
                share=.85*rank[i]/n
                for j in range(n):next_rank[j]+=share
            else:
                factor=.85*rank[i]/out[i]
                for j in range(n):
                    next_rank[j]+=factor*matrix[i][j]
        if not all(math.isfinite(v) and v>=0 for v in next_rank):
            raise ValueError("PageRank numeric overflow")
        rank=next_rank
    return rank
