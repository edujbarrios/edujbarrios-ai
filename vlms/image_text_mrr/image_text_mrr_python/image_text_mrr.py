"""Bidirectional image/text cosine mean reciprocal rank."""
import math


def image_text_mrr(images, texts, n, dimension):
    """Return (image_to_text_MRR, text_to_image_MRR) for diagonal matches."""
    for value, low, high in ((n,2,64),(dimension,1,128)):
        if isinstance(value, bool) or not isinstance(value, int) or not low <= value <= high:
            raise ValueError("invalid N or dimension")

    def normalize(flat):
        if not isinstance(flat, (list, tuple)) or len(flat)!=n*dimension:
            raise ValueError("expected N*D row-major embeddings")
        values = []
        for raw in flat:
            if isinstance(raw, bool) or not isinstance(raw, (int,float)):
                raise ValueError("embeddings must be numeric")
            try:
                x = float(raw)
            except OverflowError as exc:
                raise ValueError("unrepresentable embedding") from exc
            if not math.isfinite(x) or abs(x)>1000:
                raise ValueError("nonfinite or out-of-range embedding")
            values.append(x)
        rows=[]
        for i in range(n):
            row=values[i*dimension:(i+1)*dimension]
            norm=math.sqrt(sum(x*x for x in row))
            if norm<=0:
                raise ValueError("zero-length vector")
            rows.append([x/norm for x in row])
        return rows

    visual, language = normalize(images), normalize(texts)
    scores = [[sum(a*b for a,b in zip(visual[i],language[j]))
               for j in range(n)] for i in range(n)]
    image_total=text_total=0.0
    for i in range(n):
        diagonal=scores[i][i]
        row_rank=1+sum(s>diagonal or (s==diagonal and j<i)
                       for j,s in enumerate(scores[i]))
        col_rank=1+sum(scores[j][i]>diagonal or
                       (scores[j][i]==diagonal and j<i) for j in range(n))
        image_total+=1/row_rank
        text_total+=1/col_rank
    return (image_total/n,text_total/n)
