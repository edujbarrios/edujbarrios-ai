"""Single text-query cross-attention over visual patch key/value rows."""
import math


def cross_modal_attention(query, keys, values, patches, dimension):
    for size in (patches,dimension):
        if isinstance(size,bool) or not isinstance(size,int) or not 1 <= size <= 64:
            raise ValueError("patches and dimension must be integers in 1..64")
    def checked(items, expected):
        if not isinstance(items,(list,tuple)) or len(items)!=expected:
            raise ValueError("incorrect flat vector length")
        result=[]
        for v in items:
            if isinstance(v,bool) or not isinstance(v,(int,float)):
                raise ValueError("numeric values required")
            try:
                number=float(v)
            except OverflowError as exc:
                raise ValueError("unrepresentable value") from exc
            if not math.isfinite(number) or abs(number)>1000:
                raise ValueError("nonfinite or out-of-range value")
            result.append(number)
        return result
    q=checked(query,dimension)
    k=checked(keys,patches*dimension)
    v=checked(values,patches*dimension)
    scores=[sum(q[d]*k[p*dimension+d] for d in range(dimension)) /
            math.sqrt(dimension) for p in range(patches)]
    maximum=max(scores)
    exp_scores=[math.exp(s-maximum) for s in scores]
    total=sum(exp_scores)
    weights=[x/total for x in exp_scores]
    context=[sum(weights[p]*v[p*dimension+d] for p in range(patches))
             for d in range(dimension)]
    return (weights,context)
