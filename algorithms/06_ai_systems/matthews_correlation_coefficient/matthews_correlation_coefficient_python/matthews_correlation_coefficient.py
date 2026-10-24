"""Binary Matthews correlation coefficient from hard labels."""
import math
from typing import Sequence

def matthews_correlation_coefficient(truth:Sequence[int],predictions:Sequence[int])->float:
    """Return MCC in [-1,1], or 0 for zero-denominator classification."""
    if (not isinstance(truth,(list,tuple)) or not truth or
        not isinstance(predictions,(list,tuple)) or len(truth)!=len(predictions)):
        raise ValueError("nonempty aligned binary vectors required")
    tp=tn=fp=fn=0
    for y,p in zip(truth,predictions):
        if (isinstance(y,bool) or isinstance(p,bool) or
            not isinstance(y,int) or not isinstance(p,int) or
            y not in (0,1) or p not in (0,1)):
            raise ValueError("integer 0/1 labels required")
        if y==1 and p==1:tp+=1
        elif y==0 and p==0:tn+=1
        elif y==0:fp+=1
        else:fn+=1
    n=len(truth)
    t,f,wrong,miss=(v/n for v in (tp,tn,fp,fn))
    denom=(t+wrong)*(t+miss)*(f+wrong)*(f+miss)
    score=(t*f-wrong*miss)/math.sqrt(denom) if denom else 0.0
    if not math.isfinite(score) or not -1-1e-12<=score<=1+1e-12:
        raise ValueError("MCC arithmetic overflow")
    return max(-1.0,min(1.0,score))
