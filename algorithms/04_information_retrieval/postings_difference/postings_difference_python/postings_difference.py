"""Boolean AND-NOT difference of sorted unique document-ID lists."""
from typing import Sequence

MAX_ID=2**31-1

def _validate(values: Sequence[int]) -> None:
    if not isinstance(values,(list,tuple)):
        raise ValueError("postings must be sequences")
    last=-1
    for value in values:
        if (isinstance(value,bool) or not isinstance(value,int) or
            value<0 or value>MAX_ID or value<=last):
            raise ValueError("expected strictly increasing 31-bit document IDs")
        last=value

def postings_difference(included: Sequence[int],
                        excluded: Sequence[int]) -> list[int]:
    """Return sorted IDs in included but not excluded."""
    _validate(included)
    _validate(excluded)
    result=[]
    i=j=0
    while i<len(included) and j<len(excluded):
        if included[i]<excluded[j]:
            result.append(included[i])
            i+=1
        elif included[i]>excluded[j]:
            j+=1
        else:
            i+=1
            j+=1
    result.extend(included[i:])
    return result
