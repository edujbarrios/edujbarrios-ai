"""Stable CLIP-style classification from image and text embeddings."""
import math


def clip_zero_shot_classification(image, texts, classes, dimension, logit_scale):
    """Return probabilities in supplied text candidate order."""
    if (isinstance(classes, bool) or not isinstance(classes, int) or
            not 2 <= classes <= 64 or isinstance(dimension, bool) or
            not isinstance(dimension, int) or not 1 <= dimension <= 128):
        raise ValueError("invalid class count or embedding dimension")
    if (isinstance(logit_scale, bool) or
            not isinstance(logit_scale, (int, float))):
        raise ValueError("invalid logit scale")
    try:
        scale = float(logit_scale)
    except OverflowError as exc:
        raise ValueError("unrepresentable scale") from exc
    if not math.isfinite(scale) or not 0 < scale <= 100:
        raise ValueError("logit scale must be in (0,100]")

    def numeric(values, length):
        if not isinstance(values, (list, tuple)) or len(values) != length:
            raise ValueError("embedding length mismatch")
        result = []
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("finite numeric embeddings required")
            try:
                x = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable embedding") from exc
            if not math.isfinite(x) or abs(x) > 1000:
                raise ValueError("embedding value outside allowed range")
            result.append(x)
        return result

    visual = numeric(image, dimension)
    language = numeric(texts, classes * dimension)
    visual_norm = math.sqrt(sum(x*x for x in visual))
    if visual_norm <= 0:
        raise ValueError("zero image embedding")
    logits = []
    for k in range(classes):
        row = language[k*dimension:(k+1)*dimension]
        norm = math.sqrt(sum(x*x for x in row))
        if norm <= 0:
            raise ValueError("zero text embedding")
        dot = sum(x*y for x, y in zip(visual, row))
        logits.append(scale * dot / (visual_norm * norm))
    maximum = max(logits)
    weights = [math.exp(x - maximum) for x in logits]
    total = sum(weights)
    if not math.isfinite(total) or total <= 0:
        raise ValueError("nonfinite normalization")
    return [w/total for w in weights]
