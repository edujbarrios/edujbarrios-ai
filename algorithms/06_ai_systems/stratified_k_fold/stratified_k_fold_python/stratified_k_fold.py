"""Deterministic stratified K-fold assignments for binary class labels."""
from typing import Sequence

def stratified_k_fold(labels:Sequence[int],k:int)->list[int]:
    """Return zero-based fold IDs, preserving each class in every fold."""
    if not isinstance(labels,(list,tuple)) or not 2<=len(labels)<=4096:
        raise ValueError("binary label vector must have length 2..4096")
    n=len(labels)
    if isinstance(k,bool) or not isinstance(k,int) or not 2<=k<=n:
        raise ValueError("integer k must be in 2..n")
    counts=[0,0]
    for label in labels:
        if isinstance(label,bool) or not isinstance(label,int) or label not in (0,1):
            raise ValueError("labels must be binary integers")
        counts[label]+=1
    if any(value<k for value in counts):
        raise ValueError("each class needs at least k examples")
    folds=[-1]*n
    counter=0
    for label in (0,1):
        for i,y in enumerate(labels):
            if y==label:
                folds[i]=counter%k
                counter+=1
    return folds
