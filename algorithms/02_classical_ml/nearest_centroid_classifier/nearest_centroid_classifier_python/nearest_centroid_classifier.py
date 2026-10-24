"""Binary nearest-centroid classifier trained from finite feature rows."""
import math
from typing import Sequence

def _real(value:float)->float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real features required")
    try:v=float(value)
    except OverflowError as exc:raise ValueError("unrepresentable coordinate") from exc
    if not math.isfinite(v):raise ValueError("nonfinite feature")
    return v

def nearest_centroid_classifier(rows:Sequence[Sequence[float]],
                                 labels:Sequence[int])->list[list[float]]:
    """Return two class-major mean centroid rows."""
    if (not isinstance(rows,(list,tuple)) or not 2<=len(rows)<=4096 or
        not isinstance(rows[0],(list,tuple)) or not 1<=len(rows[0])<=64 or
        not isinstance(labels,(list,tuple)) or len(rows)!=len(labels)):
        raise ValueError("aligned n=2..4096 by d=1..64 data required")
    d=len(rows[0])
    sums=[[0.0]*d for _ in range(2)]
    members=[0,0]
    for row,label in zip(rows,labels):
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular feature matrix required")
        if isinstance(label,bool) or not isinstance(label,int) or label not in (0,1):
            raise ValueError("integer binary label required")
        members[label]+=1
        for j,value in enumerate(row):
            sums[label][j]+=_real(value)
            if not math.isfinite(sums[label][j]):
                raise ValueError("feature sum overflow")
    if not all(members):raise ValueError("both classes required")
    result=[[total/members[c] for total in sums[c]] for c in range(2)]
    if not all(math.isfinite(x) for row in result for x in row):
        raise ValueError("invalid centroid")
    return result

def nearest_centroid_classifier_predict(centroids:Sequence[Sequence[float]],
                                         query:Sequence[float])->int:
    """Return nearest centroid class, breaking ties toward label zero."""
    if (not isinstance(centroids,(list,tuple)) or len(centroids)!=2 or
        not isinstance(query,(list,tuple)) or not 1<=len(query)<=64 or
        any(not isinstance(row,(list,tuple)) or len(row)!=len(query)
            for row in centroids)):
        raise ValueError("two aligned centroid rows and nonempty query required")
    q=[_real(x) for x in query]
    distances=[]
    for row in centroids:
        distance=0.0
        for a,b in zip(q,row):
            delta=a-_real(b)
            if not math.isfinite(delta):raise ValueError("difference overflow")
            distance=math.hypot(distance,delta)
            if not math.isfinite(distance):raise ValueError("distance overflow")
        distances.append(distance)
    return int(distances[1]<distances[0])
