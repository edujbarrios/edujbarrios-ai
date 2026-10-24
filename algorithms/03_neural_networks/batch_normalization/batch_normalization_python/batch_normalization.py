"""Training-mode batch normalization across rows of dense feature data."""
import math
from typing import Sequence

def _real(v:float)->float:
    if isinstance(v,bool) or not isinstance(v,(int,float)):
        raise ValueError("finite real value required")
    try: x=float(v)
    except OverflowError as exc:raise ValueError("unrepresentable feature") from exc
    if not math.isfinite(x) or abs(x)>1e6:
        raise ValueError("numeric input outside bounded range")
    return x

def batch_normalization(
    rows:Sequence[Sequence[float]],
    gamma:Sequence[float],beta:Sequence[float]
)->list[list[float]]:
    """Apply featurewise normalized/affine transform using current batch moments."""
    if not isinstance(rows,(list,tuple)) or not 1<=len(rows)<=512 or        not isinstance(rows[0],(list,tuple)) or not 1<=len(rows[0])<=32:
        raise ValueError("batch must have 1..512 rows and 1..32 columns")
    n=len(rows);d=len(rows[0])
    if (not isinstance(gamma,(list,tuple)) or
        not isinstance(beta,(list,tuple)) or
        len(gamma)!=d or len(beta)!=d):
        raise ValueError("gamma/beta must match feature count")
    g=[_real(x) for x in gamma]
    b=[_real(x) for x in beta]
    data=[]
    for row in rows:
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular matrix required")
        data.append([_real(x) for x in row])
    means=[sum(data[i][j]/n for i in range(n)) for j in range(d)]
    variances=[sum((data[i][j]-means[j])**2/n for i in range(n)) for j in range(d)]
    scales=[g[j]/math.sqrt(variances[j]+1e-5) for j in range(d)]
    if not all(map(math.isfinite,means+variances+scales)):
        raise ValueError("nonfinite moments or normalization")
    out=[[(data[i][j]-means[j])*scales[j]+b[j] for j in range(d)]
         for i in range(n)]
    if not all(math.isfinite(x) for row in out for x in row):
        raise ValueError("nonfinite normalized output")
    return out
