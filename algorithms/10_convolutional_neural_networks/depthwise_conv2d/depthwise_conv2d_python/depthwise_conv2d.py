"""Depthwise same-padded 2D cross-correlation on CHW tensors."""
import math


def depthwise_conv2d(values, channels, height, width, kernels, kh, kw):
    for v, limit in ((channels,8),(height,16),(width,16),(kh,5),(kw,5)):
        if isinstance(v,bool) or not isinstance(v,int) or not 1 <= v <= limit:
            raise ValueError("invalid dimension")
    if kh%2==0 or kw%2==0:
        raise ValueError("kernel dimensions must be odd")
    def checked(seq, expected):
        if not isinstance(seq,(list,tuple)) or len(seq)!=expected:
            raise ValueError("incorrect flattened shape")
        out=[]
        for v in seq:
            if isinstance(v,bool) or not isinstance(v,(int,float)):
                raise ValueError("numeric tensor required")
            try:
                n=float(v)
            except OverflowError as exc:
                raise ValueError("unrepresentable value") from exc
            if not math.isfinite(n) or abs(n)>1000:
                raise ValueError("invalid tensor element")
            out.append(n)
        return out
    x=checked(values,channels*height*width)
    k=checked(kernels,channels*kh*kw)
    output=[]
    for c in range(channels):
        for r in range(height):
            for s in range(width):
                total=0.0
                for i in range(kh):
                    for j in range(kw):
                        y=r+i-kh//2
                        col=s+j-kw//2
                        if 0 <= y < height and 0 <= col < width:
                            total+=x[c*height*width+y*width+col]*k[c*kh*kw+i*kw+j]
                output.append(total)
    return output
