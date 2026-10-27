"""Bidirectional matched-pair image-text retrieval recall at K."""
import math


def image_text_recall_at_k(images, texts, n, dimension, k):
    """Return (image_to_text, text_to_image) recall fractions."""
    for value, low, high in ((n, 2, 64), (dimension, 1, 128)):
        if isinstance(value, bool) or not isinstance(value, int) or not low <= value <= high:
            raise ValueError("invalid embedding dimensions")
    if isinstance(k, bool) or not isinstance(k, int) or not 1 <= k <= n:
        raise ValueError("k must be an integer in [1,n]")

    def normalized(flat):
        if not isinstance(flat, (list, tuple)) or len(flat) != n * dimension:
            raise ValueError("embeddings must be flat, row-major N*D arrays")
        values = []
        for value in flat:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("numeric finite embeddings required")
            try:
                num = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable embedding value") from exc
            if not math.isfinite(num) or abs(num) > 1000:
                raise ValueError("embedding out of permitted range")
            values.append(num)
        out = []
        for i in range(n):
            row = values[i*dimension:(i+1)*dimension]
            norm = math.sqrt(sum(x*x for x in row))
            if norm <= 0:
                raise ValueError("embedding norm must be positive")
            out.append([x/norm for x in row])
        return out

    visual, language = normalized(images), normalized(texts)
    scores = [[sum(x*y for x,y in zip(visual[i],language[j]))
               for j in range(n)] for i in range(n)]
    image_hits = text_hits = 0
    for i in range(n):
        correct = scores[i][i]
        rank_row = sum(s > correct or (s == correct and j < i)
                       for j,s in enumerate(scores[i]))
        rank_column = sum(scores[j][i] > correct or
                          (scores[j][i] == correct and j < i)
                          for j in range(n))
        image_hits += rank_row < k
        text_hits += rank_column < k
    return (image_hits/n, text_hits/n)
