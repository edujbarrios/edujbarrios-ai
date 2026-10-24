"""Mean Euclidean silhouette coefficient for a finite labeled partition."""
import math
from typing import Sequence

def silhouette_score(points:Sequence[Sequence[float]],
                     labels:Sequence[int])->float:
    """Compute mean silhouette; singletons and zero-over-zero map to zero."""
    if (not isinstance(points,(list,tuple)) or not 2<=len(points)<=128 or
        not isinstance(points[0],(list,tuple)) or
        not 1<=len(points[0])<=32 or
        not isinstance(labels,(list,tuple)) or len(labels)!=len(points)):
        raise ValueError("aligned n=2..128 points and labels required")
    n=len(points);d=len(points[0])
    counts=[0]*n
    for label in labels:
        if isinstance(label,bool) or not isinstance(label,int) or not 0<=label<n:
            raise ValueError("cluster labels must be integer IDs in 0..n-1")
        counts[label]+=1
    clusters=sum(c>0 for c in counts)
    if not 2<=clusters<n:
        raise ValueError("require between two and n-1 nonempty clusters")
    data=[]
    for row in points:
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular data required")
        values=[]
        for value in row:
            if isinstance(value,bool) or not isinstance(value,(int,float)):
                raise ValueError("finite real data required")
            try:x=float(value)
            except OverflowError as exc:raise ValueError("unrepresentable coordinate") from exc
            if not math.isfinite(x):raise ValueError("nonfinite coordinate")
            values.append(x)
        data.append(values)
    total=0.0
    for i in range(n):
        if counts[labels[i]]==1:
            continue
        sums=[0.0]*n
        for j in range(n):
            if i==j:continue
            distance=0.0
            for a,b in zip(data[i],data[j]):
                delta=a-b
                if not math.isfinite(delta):raise ValueError("coordinate difference overflow")
                distance=math.hypot(distance,delta)
                if not math.isfinite(distance):raise ValueError("distance overflow")
            sums[labels[j]]+=distance
            if not math.isfinite(sums[labels[j]]):raise ValueError("distance sum overflow")
        a=sums[labels[i]]/(counts[labels[i]]-1)
        b=min(sums[c]/counts[c] for c in range(n) if c!=labels[i] and counts[c])
        denom=max(a,b)
        total+=(b-a)/denom if denom else 0.0
        if not math.isfinite(total):raise ValueError("silhouette mean overflow")
    result=total/n
    if not math.isfinite(result) or not -1-1e-12<=result<=1+1e-12:
        raise ValueError("invalid silhouette")
    return max(-1.0,min(1.0,result))
