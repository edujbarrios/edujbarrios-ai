"""Valid 2D CNN cross-correlation with row-major single-channel data."""
import math


def valid_conv2d(image, height, width, kernel, kh, kw):
    for size, cap in ((height,32),(width,32),(kh,8),(kw,8)):
        if isinstance(size,bool) or not isinstance(size,int) or not 1 <= size <= cap:
            raise ValueError("invalid positive dimension")
    if kh>height or kw>width:
        raise ValueError("kernel exceeds input")
    def checked(seq, length):
        if not isinstance(seq,(list,tuple)) or len(seq)!=length:
            raise ValueError("invalid flat vector length")
        result=[]
        for item in seq:
            if isinstance(item,bool) or not isinstance(item,(int,float)):
                raise ValueError("values must be numeric")
            try:
                v=float(item)
            except OverflowError as exc:
                raise ValueError("unrepresentable numeric value") from exc
            if not math.isfinite(v) or abs(v)>1000:
                raise ValueError("nonfinite or out-of-range value")
            result.append(v)
        return result
    a=checked(image,height*width)
    k=checked(kernel,kh*kw)
    oh,ow=height-kh+1,width-kw+1
    return [sum(a[(r+y)*width+c+x]*k[y*kw+x]
                for y in range(kh) for x in range(kw))
            for r in range(oh) for c in range(ow)]
