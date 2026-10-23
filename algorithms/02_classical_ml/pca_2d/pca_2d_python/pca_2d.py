"""First principal component of a two-dimensional numeric data matrix."""
import math
from typing import Sequence

def pca_2d(points: Sequence[Sequence[float]]) -> tuple[list[float],list[float],list[float]]:
    """Return (mean [2], unit principal direction [2], projections [n])."""
    if not isinstance(points,(list,tuple)) or len(points)<2:
        raise ValueError("at least two sample rows required")
    rows=[]
    for row in points:
        if not isinstance(row,(list,tuple)) or len(row)!=2:
            raise ValueError("every observation must have two real features")
        converted=[]
        for value in row:
            if isinstance(value,bool) or not isinstance(value,(int,float)):
                raise ValueError("real numeric values required")
            try: number=float(value)
            except OverflowError as exc: raise ValueError("unrepresentable value") from exc
            if not math.isfinite(number):
                raise ValueError("finite numeric values required")
            converted.append(number)
        rows.append(converted)
    mx=my=0.0
    for i,(x,y) in enumerate(rows,1):
        dx,dy=x-mx,y-my
        if not math.isfinite(dx) or not math.isfinite(dy):
            raise ValueError("mean overflow")
        mx+=dx/i;my+=dy/i
        if not math.isfinite(mx) or not math.isfinite(my):
            raise ValueError("mean overflow")
    a=b=c=0.0
    for x,y in rows:
        dx,dy=x-mx,y-my
        xx,xy,yy=dx*dx,dx*dy,dy*dy
        if not all(map(math.isfinite,(xx,xy,yy))):
            raise ValueError("covariance overflow")
        a+=xx;b+=xy;c+=yy
        if not all(map(math.isfinite,(a,b,c))):
            raise ValueError("covariance overflow")
    a/=len(rows);b/=len(rows);c/=len(rows)
    if a==0.0 and c==0.0:
        raise ValueError("PCA requires positive variance")
    if b==0.0:
        vx,vy=(1.0,0.0) if a>=c else (0.0,1.0)
    else:
        delta=a-c
        twice_b=2.0*b
        disc=math.hypot(delta,twice_b)
        vx,vy=delta+disc,twice_b
        length=math.hypot(vx,vy)
        if not all(map(math.isfinite,(delta,twice_b,disc,vx,length))) or length==0:
            raise ValueError("eigenvector overflow")
        vx/=length;vy/=length
        if vx<0.0 or (vx==0.0 and vy<0.0):
            vx,vy=-vx,-vy
    projected=[]
    for x,y in rows:
        dx,dy=x-mx,y-my
        px,py=dx*vx,dy*vy
        result=px+py
        if not all(map(math.isfinite,(dx,dy,px,py,result))):
            raise ValueError("projection overflow")
        projected.append(result)
    return [mx,my],[vx,vy],projected
