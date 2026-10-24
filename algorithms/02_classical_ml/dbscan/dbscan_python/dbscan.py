"""Deterministic Euclidean DBSCAN with inclusive, self-counted neighborhoods."""
import math
from typing import Sequence

def dbscan(points:Sequence[Sequence[float]],epsilon:float,
           min_samples:int)->list[int]:
    """Return cluster labels starting at 0, or -1 for noise."""
    if (not isinstance(points,(list,tuple)) or not 1<=len(points)<=1024 or
        not isinstance(points[0],(list,tuple)) or not 1<=len(points[0])<=64):
        raise ValueError("expected n=1..1024 by d=1..64 numeric matrix")
    n=len(points);d=len(points[0])
    if (isinstance(epsilon,bool) or not isinstance(epsilon,(int,float)) or
        isinstance(min_samples,bool) or not isinstance(min_samples,int) or
        not 1<=min_samples<=n):
        raise ValueError("invalid epsilon or min_samples")
    try: radius=float(epsilon)
    except OverflowError as exc:raise ValueError("invalid radius") from exc
    if not math.isfinite(radius) or radius<=0:
        raise ValueError("epsilon must be finite and positive")
    data=[]
    for row in points:
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular matrix required")
        values=[]
        for x in row:
            if isinstance(x,bool) or not isinstance(x,(int,float)):
                raise ValueError("finite numeric coordinates required")
            try: value=float(x)
            except OverflowError as exc:raise ValueError("invalid coordinate") from exc
            if not math.isfinite(value):raise ValueError("nonfinite coordinate")
            values.append(value)
        data.append(values)
    neighbors=[[] for _ in range(n)]
    for i in range(n):
        for j in range(i,n):
            distance=0.0
            for a,b in zip(data[i],data[j]):
                delta=a-b
                if not math.isfinite(delta):raise ValueError("difference overflow")
                distance=math.hypot(distance,delta)
                if not math.isfinite(distance):raise ValueError("distance overflow")
            if distance<=radius:
                neighbors[i].append(j)
                if j!=i:neighbors[j].append(i)
    for row in neighbors:row.sort()
    visited=[False]*n
    labels=[-1]*n
    cluster=0
    for i in range(n):
        if visited[i]:continue
        visited[i]=True
        if len(neighbors[i])<min_samples:continue
        labels[i]=cluster
        queued=[False]*n
        queue=[]
        for j in neighbors[i]:
            if j!=i:queue.append(j);queued[j]=True
        front=0
        while front<len(queue):
            j=queue[front];front+=1
            if not visited[j]:
                visited[j]=True
                if len(neighbors[j])>=min_samples:
                    for h in neighbors[j]:
                        if not visited[h] and not queued[h]:
                            queue.append(h);queued[h]=True
            if labels[j]==-1:labels[j]=cluster
        cluster+=1
    return labels
