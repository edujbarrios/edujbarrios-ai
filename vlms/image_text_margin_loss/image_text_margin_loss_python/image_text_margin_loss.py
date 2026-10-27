"""Bidirectional max-margin ranking loss over cosine image-text embeddings."""
import math


def image_text_margin_loss(images, texts, n, dimension, margin):
    """Mean of hardest image-to-text and text-to-image hinge losses."""
    for v, lo, hi in ((n,2,64),(dimension,1,128)):
        if isinstance(v, bool) or not isinstance(v, int) or not lo <= v <= hi:
            raise ValueError("invalid number of pairs or dimension")
    if isinstance(margin, bool) or not isinstance(margin, (int, float)):
        raise ValueError("margin must be a number")
    try:
        m = float(margin)
    except OverflowError as exc:
        raise ValueError("unrepresentable margin") from exc
    if not math.isfinite(m) or not 0 <= m <= 2:
        raise ValueError("margin must be finite within [0,2]")

    def normalize(flat):
        if not isinstance(flat, (list, tuple)) or len(flat) != n*dimension:
            raise ValueError("embeddings must be flat row-major N*D vectors")
        values = []
        for v in flat:
            if isinstance(v, bool) or not isinstance(v, (int, float)):
                raise ValueError("real finite embedding values required")
            try:
                value = float(v)
            except OverflowError as exc:
                raise ValueError("unrepresentable embedding") from exc
            if not math.isfinite(value) or abs(value) > 1000:
                raise ValueError("embedding outside [-1000,1000]")
            values.append(value)
        rows = []
        for i in range(n):
            row = values[i*dimension:(i+1)*dimension]
            norm = math.sqrt(sum(x*x for x in row))
            if norm <= 0:
                raise ValueError("zero embedding row")
            rows.append([x/norm for x in row])
        return rows

    visual, language = normalize(images), normalize(texts)
    scores = [[sum(a*b for a,b in zip(visual[i],language[j]))
               for j in range(n)] for i in range(n)]
    total = 0.0
    for i in range(n):
        positive = scores[i][i]
        hardest_row = max(scores[i][j] for j in range(n) if j != i)
        hardest_col = max(scores[j][i] for j in range(n) if j != i)
        total += max(0.0, m-positive+hardest_row)
        total += max(0.0, m-positive+hardest_col)
    return total/(2*n)
