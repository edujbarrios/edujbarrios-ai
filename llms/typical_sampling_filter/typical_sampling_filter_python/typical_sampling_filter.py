"""Filter tokens by local typicality of their surprisal."""
import math


def typical_sampling_filter(logits, p):
    if not isinstance(logits,(list,tuple)) or not 1 <= len(logits) <= 4096:
        raise ValueError("invalid vocabulary size")
    if isinstance(p,bool) or not isinstance(p,(int,float)):
        raise ValueError("invalid typical mass")
    try:
        threshold=float(p)
    except OverflowError as exc:
        raise ValueError("unrepresentable threshold") from exc
    if not math.isfinite(threshold) or not 0 < threshold <= 1:
        raise ValueError("typical mass must be in (0,1]")
    scores=[]
    for raw in logits:
        if isinstance(raw,bool) or not isinstance(raw,(int,float)):
            raise ValueError("numeric logits required")
        try:
            score=float(raw)
        except OverflowError as exc:
            raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(score) or abs(score)>1000:
            raise ValueError("nonfinite or out-of-range logit")
        scores.append(score)
    m=max(scores)
    w=[math.exp(s-m) for s in scores]
    total=sum(w)
    logz=math.log(total)
    surprise=[logz-(s-m) for s in scores]
    entropy=sum((wi/total)*si for wi,si in zip(w,surprise))
    order=sorted(range(len(scores)), key=lambda i:(abs(surprise[i]-entropy),i))
    keep=[False]*len(scores)
    mass=0.0
    for i in order:
        keep[i]=True
        mass+=w[i]/total
        if threshold<1 and mass>=threshold:
            break
    return keep
