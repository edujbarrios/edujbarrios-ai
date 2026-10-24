"""Exact unit-cost edit distance on printable ASCII strings."""
def _validate(value: str) -> str:
    if (not isinstance(value,str) or len(value)>512 or
        any(not 32<=ord(ch)<=126 for ch in value)):
        raise ValueError("strings must be printable ASCII with length <=512")
    return value

def levenshtein_distance(a: str,b: str) -> int:
    """Return the minimum number of insertions, deletions or substitutions."""
    a,b=_validate(a),_validate(b)
    if len(b)>len(a):
        a,b=b,a
    prev=list(range(len(b)+1))
    for i,ch_a in enumerate(a,1):
        curr=[i]+[0]*len(b)
        for j,ch_b in enumerate(b,1):
            curr[j]=min(prev[j]+1,curr[j-1]+1,prev[j-1]+(ch_a!=ch_b))
        prev=curr
    return prev[-1]
