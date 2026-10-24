"""All-pairs shortest distances for directed weighted graphs."""
import math
from typing import Sequence

def floyd_warshall(adjacency: Sequence[Sequence[float]]) -> list[list[float]]:
    """Return shortest-path distances; +inf denotes unreachable vertices."""
    if not isinstance(adjacency,(list,tuple)) or not 1<=len(adjacency)<=64:
        raise ValueError("expected a nonempty n by n graph with n<=64")
    n=len(adjacency)
    d=[]
    for row in adjacency:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("adjacency matrix must be square")
        converted=[]
        for weight in row:
            if isinstance(weight,bool) or not isinstance(weight,(int,float)):
                raise ValueError("edge weights must be real")
            try: value=float(weight)
            except OverflowError as exc: raise ValueError("invalid weight") from exc
            if (math.isnan(value) or value==-math.inf or
                (math.isfinite(value) and not -1e6<=value<=1e6)):
                raise ValueError("weight out of range or invalid")
            converted.append(value)
        d.append(converted)
    if any(d[i][i]!=0 for i in range(n)):
        raise ValueError("diagonal must equal zero")
    for k in range(n):
        for i in range(n):
            if math.isinf(d[i][k]):continue
            for j in range(n):
                if math.isinf(d[k][j]):continue
                via=d[i][k]+d[k][j]
                if not math.isfinite(via):
                    raise ValueError("shortest path overflow")
                if via<d[i][j]:d[i][j]=via
    if any(d[i][i]<0 for i in range(n)):
        raise ValueError("negative-weight cycle")
    return d
