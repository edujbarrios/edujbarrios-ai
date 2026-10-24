"""Reusable exact-bucket index for 30-bit locality-sensitive signatures."""
from typing import Sequence

MAX_SIGNATURE=(1<<30)-1

def _signature(value: int) -> int:
    if isinstance(value,bool) or not isinstance(value,int) or not 0<=value<=MAX_SIGNATURE:
        raise ValueError("signatures must be unsigned 30-bit integers")
    return value

class LshBucketIndex:
    """Static mapping from signature to original zero-based document indices."""
    def __init__(self, signatures: Sequence[int]) -> None:
        if not isinstance(signatures,(list,tuple)) or not signatures:
            raise ValueError("nonempty corpus signatures required")
        buckets: dict[int,list[int]]={}
        for i,value in enumerate(signatures):
            key=_signature(value)
            buckets.setdefault(key,[]).append(i)
        self._buckets=buckets

    def query(self, signature: int) -> list[int]:
        """Return source indices in insertion order; [] for absent buckets."""
        return list(self._buckets.get(_signature(signature),()))

def build_lsh_bucket_index(signatures: Sequence[int]) -> LshBucketIndex:
    """Construct an immutable-in-use bucket lookup object."""
    return LshBucketIndex(signatures)
