"""Shannon entropy in bits for bounded integer classification counts."""
import math


def entropy_impurity(counts):
    """Compute -sum(p*log2(p)); ignore zero-count categories."""
    if not isinstance(counts,(list,tuple)) or not 1<=len(counts)<=128:
        raise ValueError("expected 1..128 class counts")
    if any(isinstance(c,bool) or not isinstance(c,int) or
           not 0<=c<=1_000_000 for c in counts):
        raise ValueError("nonnegative integer counts <= 1000000 required")
    total=sum(counts)
    if total==0:
        raise ValueError("at least one positive count required")
    return max(0.0,-sum((c/total)*math.log2(c/total) for c in counts if c))
