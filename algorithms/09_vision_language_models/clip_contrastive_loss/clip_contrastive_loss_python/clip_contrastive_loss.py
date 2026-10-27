"""Stable bidirectional CLIP-style paired image-text contrastive loss."""
import math


def clip_contrastive_loss(images, texts, n, dimension, logit_scale):
    """Return the symmetric cross-entropy loss on matched embedding rows."""
    if (isinstance(n, bool) or not isinstance(n, int) or not 2 <= n <= 64 or
            isinstance(dimension, bool) or not isinstance(dimension, int) or
            not 1 <= dimension <= 128):
        raise ValueError("N must be 2..64 and dimension 1..128")
    if (isinstance(logit_scale, bool) or
            not isinstance(logit_scale, (int, float))):
        raise ValueError("invalid logit scale")
    try:
        scale = float(logit_scale)
    except OverflowError as exc:
        raise ValueError("unrepresentable logit scale") from exc
    if not math.isfinite(scale) or not 0 < scale <= 100:
        raise ValueError("logit scale outside (0,100]")

    def normalized(values):
        if not isinstance(values, (list, tuple)) or len(values) != n * dimension:
            raise ValueError("flat row-major embeddings must have N*D entries")
        result = []
        for i in range(n):
            row = []
            for value in values[i * dimension:(i + 1) * dimension]:
                if isinstance(value, bool) or not isinstance(value, (int, float)):
                    raise ValueError("numeric finite embeddings required")
                try:
                    number = float(value)
                except OverflowError as exc:
                    raise ValueError("unrepresentable embedding") from exc
                if not math.isfinite(number) or abs(number) > 1000:
                    raise ValueError("embedding value outside allowed range")
                row.append(number)
            magnitude = math.sqrt(sum(x*x for x in row))
            if magnitude <= 0:
                raise ValueError("zero embedding vector")
            result.append([x / magnitude for x in row])
        return result

    visual, language = normalized(images), normalized(texts)
    logits = [[scale * sum(a*b for a, b in zip(visual[i], language[j]))
               for j in range(n)] for i in range(n)]

    def logsumexp(values):
        maximum = max(values)
        return maximum + math.log(sum(math.exp(v - maximum) for v in values))

    total = 0.0
    for i in range(n):
        total += logsumexp(logits[i]) - logits[i][i]
        total += logsumexp([logits[j][i] for j in range(n)]) - logits[i][i]
    loss = total / (2*n)
    if not math.isfinite(loss):
        raise ValueError("nonfinite contrastive loss")
    return max(0.0, loss)
