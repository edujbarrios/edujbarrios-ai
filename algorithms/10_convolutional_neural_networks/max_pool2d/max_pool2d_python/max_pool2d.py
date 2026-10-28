"""Non-overlapping 2x2 max pooling, stride 2 and no padding."""
import math


def max_pool2d(values, height, width):
    for dimension in (height, width):
        if isinstance(dimension,bool) or not isinstance(dimension,int) or not 2 <= dimension <= 32:
            raise ValueError("H/W must be integers in 2..32")
    if not isinstance(values,(list,tuple)) or len(values)!=height*width:
        raise ValueError("expected a flat H*W vector")
    numeric=[]
    for raw in values:
        if isinstance(raw,bool) or not isinstance(raw,(int,float)):
            raise ValueError("invalid activation")
        try:
            x=float(raw)
        except OverflowError as exc:
            raise ValueError("unrepresentable activation") from exc
        if not math.isfinite(x) or abs(x)>1000:
            raise ValueError("activation outside [-1000,1000]")
        numeric.append(x)
    out=[]
    for r in range(height//2):
        for c in range(width//2):
            at=(2*r)*width+2*c
            out.append(max(numeric[at],numeric[at+1],
                           numeric[at+width],numeric[at+width+1]))
    return out
