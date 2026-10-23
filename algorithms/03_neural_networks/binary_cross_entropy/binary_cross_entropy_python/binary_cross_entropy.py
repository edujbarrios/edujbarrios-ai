"""Mean binary cross-entropy on finite probabilities."""
import math
def binary_cross_entropy(labels, probabilities):
    if len(labels)!=len(probabilities) or not labels:
        raise ValueError("nonempty equal-length inputs required")
    total=0.0
    for y,p in zip(labels,probabilities):
        if isinstance(y,bool) or isinstance(p,bool) or not isinstance(y,(int,float)) or not isinstance(p,(int,float)):
            raise ValueError("real numbers required")
        if not math.isfinite(y) or not math.isfinite(p) or y not in (0,1) or not 0<=p<=1:
            raise ValueError("invalid label or probability")
        if (y==1 and p==0) or (y==0 and p==1):
            raise ValueError("infinite loss")
        term=-math.log(p) if y==1 else -math.log1p(-p)
        total+=term
        if not math.isfinite(total):
            raise ValueError("loss overflow")
    return total/len(labels)
