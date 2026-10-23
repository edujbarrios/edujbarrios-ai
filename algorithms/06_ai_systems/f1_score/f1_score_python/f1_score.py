"""Binary F1 score for aligned hard-label predictions."""
from typing import Sequence

def f1_score(actual: Sequence[int], predicted: Sequence[int]) -> float:
    """Return binary F1, or zero if there are no true or predicted positives."""
    if not actual or len(actual)!=len(predicted):
        raise ValueError("nonempty aligned arrays required")
    tp=fp=fn=0
    for y,p in zip(actual,predicted):
        if isinstance(y,bool) or isinstance(p,bool) or not isinstance(y,int) or not isinstance(p,int) or y not in (0,1) or p not in (0,1):
            raise ValueError("integer labels must be 0 or 1")
        if y==1 and p==1:
            tp+=1
        elif y==0 and p==1:
            fp+=1
        elif y==1 and p==0:
            fn+=1
    denominator=2*tp+fp+fn
    return 2*tp/denominator if denominator else 0.0
