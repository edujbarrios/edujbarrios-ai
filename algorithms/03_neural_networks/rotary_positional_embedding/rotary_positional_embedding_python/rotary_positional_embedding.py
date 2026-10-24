"""Adjacent-pair rotary positional embedding for query and key vectors."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real vector components required")
    try:
        number=float(value)
    except OverflowError as exc:
        raise ValueError("unrepresentable vector component") from exc
    if not math.isfinite(number):
        raise ValueError("finite vector components required")
    return number

def rotary_positional_embedding(query: Sequence[float], key: Sequence[float],
                                query_position: int, key_position: int
                                ) -> tuple[list[float],list[float]]:
    """Rotate consecutive coordinate pairs by their RoPE phase."""
    if (not isinstance(query,(list,tuple)) or
        not isinstance(key,(list,tuple)) or len(query)!=len(key) or
        not 2<=len(query)<=1024 or len(query)%2 != 0):
        raise ValueError("matching even dimensions in 2..1024 required")
    for value in (query_position,key_position):
        if isinstance(value,bool) or not isinstance(value,int) or not 0<=value<=1000000:
            raise ValueError("positions must be integers in 0..1,000,000")
    q=[_real(v) for v in query]
    k=[_real(v) for v in key]
    qr=[]
    kr=[]
    for j in range(0,len(q),2):
        frequency=10000.0**(j/len(q))
        angles=(query_position/frequency,key_position/frequency)
        for v,angle,result in ((q,angles[0],qr),(k,angles[1],kr)):
            c,s=math.cos(angle),math.sin(angle)
            a=v[j]*c-v[j+1]*s
            b=v[j]*s+v[j+1]*c
            if not math.isfinite(a) or not math.isfinite(b):
                raise ValueError("rotary coordinate overflow")
            result.extend((a,b))
    return qr,kr
