"""Binary multivariate Gaussian naive Bayes with diagonal class covariance."""
import math
from typing import Sequence

FLOOR = 1e-9
TWO_PI = 2.0 * math.pi

def _real(value: float) -> float:
    if isinstance(value, bool) or not isinstance(value, (int,float)):
        raise ValueError("finite real values required")
    try:
        number = float(value)
    except OverflowError as exc:
        raise ValueError("unrepresentable value") from exc
    if not math.isfinite(number):
        raise ValueError("finite real values required")
    return number

def gaussian_naive_bayes(samples: Sequence[Sequence[float]], labels: Sequence[int],
                         query: Sequence[float]) -> tuple[int,float]:
    """Fit two diagonal Gaussian classes and return (label, P(label=1))."""
    if not isinstance(samples,(list,tuple)) or len(samples)<2 or len(labels)!=len(samples):
        raise ValueError("at least two aligned observations required")
    if not isinstance(query,(list,tuple)) or not query:
        raise ValueError("query must be nonempty")
    d = len(query)
    q = [_real(v) for v in query]
    count = [0,0]
    means = [[0.0]*d for _ in range(2)]
    points = []
    for row,label in zip(samples,labels):
        if isinstance(label,bool) or not isinstance(label,int) or label not in (0,1):
            raise ValueError("binary integer labels required")
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular samples must match query dimension")
        values = [_real(v) for v in row]
        points.append((values,label))
        count[label]+=1
        for j,x in enumerate(values):
            delta = x-means[label][j]
            if not math.isfinite(delta):
                raise ValueError("mean overflow")
            means[label][j]+=delta/count[label]
            if not math.isfinite(means[label][j]):
                raise ValueError("mean overflow")
    if not all(count):
        raise ValueError("both classes must be present")
    variances = [[0.0]*d for _ in range(2)]
    for row,label in points:
        for j,x in enumerate(row):
            delta = x-means[label][j]
            square = delta*delta
            variances[label][j]+=square
            if not all(math.isfinite(v) for v in (delta,square,variances[label][j])):
                raise ValueError("variance overflow")
    scores = []
    for label in (0,1):
        score = math.log(count[label]/len(samples))
        for j,x in enumerate(q):
            variance = max(variances[label][j]/count[label],FLOOR)
            delta = x-means[label][j]
            square = delta*delta
            term = math.log(TWO_PI*variance)+square/variance
            if not all(math.isfinite(v) for v in (variance,delta,square,term)):
                raise ValueError("likelihood overflow")
            score-=0.5*term
            if not math.isfinite(score):
                raise ValueError("likelihood overflow")
        scores.append(score)
    log_odds = scores[1]-scores[0]
    if not math.isfinite(log_odds):
        raise ValueError("log odds overflow")
    e = math.exp(-abs(log_odds))
    probability = 1/(1+e) if log_odds>=0 else e/(1+e)
    return (int(scores[1]>scores[0]),probability)
