"""Population z-score standardization using per-column Welford moments."""
import math
from typing import Sequence

def z_score_standardization(rows: Sequence[Sequence[float]]
                            ) -> tuple[list[float],list[float],list[list[float]]]:
    """Return (column means, population standard deviations, standardized rows)."""
    if not isinstance(rows,(list,tuple)) or not rows or not isinstance(rows[0],(list,tuple)) or not rows[0]:
        raise ValueError("nonempty numeric matrix required")
    d=len(rows[0])
    matrix=[]
    for row in rows:
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular matrix required")
        converted=[]
        for value in row:
            if isinstance(value,bool) or not isinstance(value,(int,float)):
                raise ValueError("finite numeric values required")
            try: v=float(value)
            except OverflowError as exc: raise ValueError("unrepresentable value") from exc
            if not math.isfinite(v):
                raise ValueError("finite numeric values required")
            converted.append(v)
        matrix.append(converted)
    means=[0.0]*d
    m2=[0.0]*d
    for i,row in enumerate(matrix,1):
        for j,value in enumerate(row):
            delta=value-means[j]
            if not math.isfinite(delta):
                raise ValueError("mean update overflow")
            means[j]+=delta/i
            delta2=value-means[j]
            term=delta*delta2
            if not all(map(math.isfinite,(means[j],delta2,term))):
                raise ValueError("variance update overflow")
            m2[j]+=term
            if not math.isfinite(m2[j]):
                raise ValueError("variance sum overflow")
    stds=[]
    for value in m2:
        variance=value/len(matrix)
        if not math.isfinite(variance) or variance<0:
            raise ValueError("invalid population variance")
        stds.append(math.sqrt(variance))
    normalized=[]
    for row in matrix:
        result=[]
        for j,value in enumerate(row):
            if stds[j]==0.0:
                result.append(0.0)
            else:
                delta=value-means[j]
                z=delta/stds[j]
                if not math.isfinite(delta) or not math.isfinite(z):
                    raise ValueError("standardized output overflow")
                result.append(z)
        normalized.append(result)
    return means,stds,normalized
