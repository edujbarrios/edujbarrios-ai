"""Binary ROC AUC from average tied ranks and Mann-Whitney U."""
import math
from typing import Sequence

def roc_auc(scores: Sequence[float], labels: Sequence[int]) -> float:
    """Return AUC if both classes are present, otherwise raise ValueError."""
    if (not isinstance(scores,(list,tuple)) or
        not isinstance(labels,(list,tuple)) or
        len(scores)!=len(labels) or len(scores)<2):
        raise ValueError("aligned scores and labels with both classes required")
    pairs=[]
    for score,label in zip(scores,labels):
        if isinstance(score,bool) or not isinstance(score,(int,float)):
            raise ValueError("scores must be finite real numbers")
        try: s=float(score)
        except OverflowError as exc: raise ValueError("unrepresentable score") from exc
        if not math.isfinite(s) or isinstance(label,bool) or not isinstance(label,int) or label not in (0,1):
            raise ValueError("scores must be finite and labels binary integers")
        pairs.append((s,label))
    positives=sum(label for _,label in pairs)
    if positives==0 or positives==len(pairs):
        raise ValueError("both classes required for ROC AUC")
    pairs.sort(key=lambda item:item[0])
    positive_ranks=0.0
    start=0
    while start<len(pairs):
        end=start+1
        count=pairs[start][1]
        while end<len(pairs) and pairs[end][0]==pairs[start][0]:
            count+=pairs[end][1]
            end+=1
        positive_ranks+=count*((start+1+end)/2)
        start=end
    n=len(pairs)
    statistic=positive_ranks-positives*(positives+1)/2
    result=statistic/(positives*(n-positives))
    if not math.isfinite(result) or not -1e-12<=result<=1+1e-12:
        raise ValueError("nonfinite ROC AUC")
    return max(0.0,min(1.0,result))
