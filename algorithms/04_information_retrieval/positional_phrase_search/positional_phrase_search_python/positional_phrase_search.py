"""Linear two-term phrase intersection of positional postings lists."""
from typing import Sequence

LIMIT=1_000_000_000

def _validate(postings:Sequence[tuple[int,int]])->list[tuple[int,int]]:
    if not isinstance(postings,(list,tuple)) or len(postings)>4096:
        raise ValueError("postings must be a list of at most 4096 pairs")
    result=[]
    for pair in postings:
        if not isinstance(pair,(list,tuple)) or len(pair)!=2:
            raise ValueError("each posting must be a pair")
        a,b=pair
        if (isinstance(a,bool) or isinstance(b,bool) or
            not isinstance(a,int) or not isinstance(b,int) or
            not 0<=a<=LIMIT or not 0<=b<=LIMIT):
            raise ValueError("doc and token position must be bounded integers")
        record=(a,b)
        if result and record<=result[-1]:
            raise ValueError("postings must be strictly lexicographically sorted")
        result.append(record)
    return result

def positional_phrase_search(first:Sequence[tuple[int,int]],
                             second:Sequence[tuple[int,int]])->list[int]:
    """Return sorted unique document IDs with adjacent ordered term positions."""
    left=_validate(first)
    right=_validate(second)
    a=b=0
    matches=[]
    while a<len(left) and b<len(right):
        da=left[a][0]
        db=right[b][0]
        if da<db:
            while a<len(left) and left[a][0]==da:a+=1
        elif db<da:
            while b<len(right) and right[b][0]==db:b+=1
        else:
            end_a=a
            while end_a<len(left) and left[end_a][0]==da:end_a+=1
            end_b=b
            while end_b<len(right) and right[end_b][0]==db:end_b+=1
            pa,pb=a,b
            while pa<end_a and pb<end_b:
                wanted=left[pa][1]+1
                observed=right[pb][1]
                if wanted==observed:
                    matches.append(da)
                    break
                if wanted<observed:pa+=1
                else:pb+=1
            a=end_a
            b=end_b
    return matches
