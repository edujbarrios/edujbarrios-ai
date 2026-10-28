"""Channel-first global spatial average pooling."""
import math


def global_average_pool2d(values, channels, height, width):
    for size, limit in ((channels,16),(height,32),(width,32)):
        if isinstance(size,bool) or not isinstance(size,int) or not 1 <= size <= limit:
            raise ValueError("invalid CHW dimension")
    if not isinstance(values,(list,tuple)) or len(values)!=channels*height*width:
        raise ValueError("expected flat CHW feature map")
    numeric=[]
    for raw in values:
        if isinstance(raw,bool) or not isinstance(raw,(int,float)):
            raise ValueError("activation must be numeric")
        try:
            v=float(raw)
        except OverflowError as exc:
            raise ValueError("unrepresentable activation") from exc
        if not math.isfinite(v) or abs(v)>1000:
            raise ValueError("invalid activation")
        numeric.append(v)
    spatial=height*width
    return [sum(numeric[c*spatial:(c+1)*spatial])/spatial
            for c in range(channels)]
