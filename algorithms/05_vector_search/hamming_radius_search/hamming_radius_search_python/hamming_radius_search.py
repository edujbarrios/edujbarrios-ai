"""Hamming-radius filtering over 30-bit packed signatures."""
from typing import Sequence

MAX_SIGNATURE=(1<<30)-1

def _signature(value: int) -> int:
    if isinstance(value,bool) or not isinstance(value,int) or not 0<=value<=MAX_SIGNATURE:
        raise ValueError("signatures must be integers in 0..2^30-1")
    return value

def hamming_radius_search(query: int, signatures: Sequence[int],
                          radius: int) -> list[int]:
    """Return all matching zero-based indices, ordered by distance then index."""
    _signature(query)
    if (isinstance(radius,bool) or not isinstance(radius,int) or
        not 0<=radius<=30):
        raise ValueError("radius must be an integer in 0..30")
    if not isinstance(signatures,(list,tuple)):
        raise ValueError("signatures must be a sequence")
    checked=[_signature(value) for value in signatures]
    matches=[((query^value).bit_count(),i)
             for i,value in enumerate(checked)
             if (query^value).bit_count()<=radius]
    matches.sort()
    return [index for _,index in matches]
