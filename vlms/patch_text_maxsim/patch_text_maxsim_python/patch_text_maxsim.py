"""Text-token to visual-patch maximum cosine alignment and mean score."""
import math


def patch_text_maxsim(patches, n_patches, tokens, n_tokens, dimension):
    for v, limit in ((n_patches,32),(n_tokens,32),(dimension,64)):
        if isinstance(v,bool) or not isinstance(v,int) or not 1 <= v <= limit:
            raise ValueError("invalid patch/token count or dimension")

    def normalize(flat, count):
        if not isinstance(flat,(list,tuple)) or len(flat)!=count*dimension:
            raise ValueError("invalid row-major embeddings")
        values=[]
        for raw in flat:
            if isinstance(raw,bool) or not isinstance(raw,(int,float)):
                raise ValueError("numeric embeddings required")
            try:
                number=float(raw)
            except OverflowError as exc:
                raise ValueError("unrepresentable embedding") from exc
            if not math.isfinite(number) or abs(number)>1000:
                raise ValueError("invalid embedding element")
            values.append(number)
        result=[]
        for i in range(count):
            row=values[i*dimension:(i+1)*dimension]
            norm=math.sqrt(sum(v*v for v in row))
            if norm<=0:
                raise ValueError("zero embedding")
            result.append([v/norm for v in row])
        return result

    visual=normalize(patches,n_patches)
    language=normalize(tokens,n_tokens)
    selected=[]
    total=0.0
    for token in language:
        similarities=[sum(a*b for a,b in zip(token,patch)) for patch in visual]
        best=max(range(n_patches),key=lambda p:(similarities[p],-p))
        selected.append(best)
        total+=similarities[best]
    return (selected,total/n_tokens)
