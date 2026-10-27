"""Hardest nonmatching in-batch image/text pairs by cosine similarity."""
import math


def image_text_hard_negatives(images, texts, n, dimension):
    """Return image-to-text IDs followed by text-to-image IDs, zero-based."""
    for value, low, high in ((n,2,64),(dimension,1,128)):
        if isinstance(value, bool) or not isinstance(value, int) or not low <= value <= high:
            raise ValueError("invalid number of pairs or dimension")

    def normalize(flat):
        if not isinstance(flat, (list, tuple)) or len(flat) != n*dimension:
            raise ValueError("flat N*D embeddings required")
        values = []
        for value in flat:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("numeric embeddings required")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable embedding") from exc
            if not math.isfinite(number) or abs(number) > 1000:
                raise ValueError("embedding must be finite in [-1000,1000]")
            values.append(number)
        normalized = []
        for i in range(n):
            row = values[i*dimension:(i+1)*dimension]
            norm = math.sqrt(sum(x*x for x in row))
            if norm <= 0:
                raise ValueError("zero embedding vector")
            normalized.append([x/norm for x in row])
        return normalized

    visual, language = normalize(images), normalize(texts)
    scores = [[sum(a*b for a,b in zip(visual[i],language[j]))
               for j in range(n)] for i in range(n)]
    image_ids = []
    text_ids = []
    for i in range(n):
        candidates = [j for j in range(n) if j != i]
        image_ids.append(max(candidates,key=lambda j:(scores[i][j],-j)))
        text_ids.append(max(candidates,key=lambda j:(scores[j][i],-j)))
    return image_ids + text_ids
