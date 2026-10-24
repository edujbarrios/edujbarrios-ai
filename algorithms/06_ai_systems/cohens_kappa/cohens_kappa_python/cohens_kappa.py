"""Unweighted binary Cohen's kappa chance-corrected agreement."""
import math
from typing import Sequence

def cohens_kappa(first:Sequence[int],second:Sequence[int])->float:
    """Return agreement beyond chance, rejecting p_e=1 degeneracy."""
    if (not isinstance(first,(list,tuple)) or
        not isinstance(second,(list,tuple)) or not first or
        len(first)!=len(second)):
        raise ValueError("nonempty aligned binary label vectors required")
    zeros_a=zeros_b=matches=0
    for a,b in zip(first,second):
        if (isinstance(a,bool) or isinstance(b,bool) or
            not isinstance(a,int) or not isinstance(b,int) or
            a not in (0,1) or b not in (0,1)):
            raise ValueError("labels must be binary integers")
        zeros_a+=int(a==0)
        zeros_b+=int(b==0)
        matches+=int(a==b)
    n=len(first)
    pa=zeros_a/n
    pb=zeros_b/n
    observed=matches/n
    expected=pa*pb+(1-pa)*(1-pb)
    denominator=1-expected
    if not denominator>0:
        raise ValueError("undefined kappa when expected agreement is 1")
    value=(observed-expected)/denominator
    if not math.isfinite(value) or not -1-1e-12<=value<=1+1e-12:
        raise ValueError("invalid kappa")
    return max(-1.0,min(1.0,value))
