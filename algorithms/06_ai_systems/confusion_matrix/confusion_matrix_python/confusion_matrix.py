"""Binary confusion matrix in canonical (TN, FP, FN, TP) order."""
from typing import Sequence

def confusion_matrix(actual: Sequence[int], predicted: Sequence[int]
                     ) -> tuple[int,int,int,int]:
    """Count each binary classification outcome with full-input validation."""
    if len(actual)!=len(predicted) or not actual:
        raise ValueError("nonempty, equal-length label arrays required")
    counts=[0,0,0,0]
    for a,p in zip(actual,predicted):
        if isinstance(a,bool) or isinstance(p,bool) or not isinstance(a,int) or not isinstance(p,int) or a not in (0,1) or p not in (0,1):
            raise ValueError("actual and predicted must contain integer 0/1 labels")
        counts[a*2+p]+=1
    return tuple(counts)
