"""Two-pointer union of unique sorted integer document-ID postings."""
from typing import Sequence

MAX_ID=2**31-1

def _validate(values: Sequence[int]) -> None:
    if not isinstance(values,(list,tuple)):
        raise ValueError("postings must be sequences")
    last=-1
    for value in values:
        if (isinstance(value,bool) or not isinstance(value,int) or
            not 0<=value<=MAX_ID or value<=last):
            raise ValueError("IDs must be strictly increasing 31-bit nonnegative integers")
        last=value

def postings_union(left: Sequence[int],right: Sequence[int]) -> list[int]:
    """Return sorted zero-based document IDs present in either list."""
    _validate(left)
    _validate(right)
    result=[]
    i=j=0
    while i<len(left) and j<len(right):
        if left[i]<right[j]:
            result.append(left[i]);i+=1
        elif left[i]>right[j]:
            result.append(right[j]);j+=1
        else:
            result.append(left[i]);i+=1;j+=1
    result.extend(left[i:])
    result.extend(right[j:])
    return result
