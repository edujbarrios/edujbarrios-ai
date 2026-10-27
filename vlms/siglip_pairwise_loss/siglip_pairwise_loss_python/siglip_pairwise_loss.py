"""Stable pairwise sigmoid contrastive loss over aligned image/text pairs."""
import math


def siglip_pairwise_loss(images, texts, n, dimension, logit_scale, bias):
    """Return mean diagonal-positive / off-diagonal-negative softplus loss."""
    for x, lo, hi in ((n, 2, 64), (dimension, 1, 128)):
        if isinstance(x, bool) or not isinstance(x, int) or not lo <= x <= hi:
            raise ValueError("invalid pair count or dimension")
    for name, value, lo, hi in (("scale", logit_scale, 0, 100),
                                ("bias", bias, -20, 20)):
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError(f"invalid {name}")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError(f"unrepresentable {name}") from exc
        if not math.isfinite(number) or not lo <= number <= hi or (name == "scale" and number == 0):
            raise ValueError(f"{name} outside supported range")

    def normalize(flat):
        if not isinstance(flat, (list, tuple)) or len(flat) != n*dimension:
            raise ValueError("embeddings must be flat N*D arrays")
        values = []
        for value in flat:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("numeric embeddings required")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable embedding") from exc
            if not math.isfinite(number) or abs(number) > 1000:
                raise ValueError("invalid embedding")
            values.append(number)
        result = []
        for i in range(n):
            row = values[i*dimension:(i+1)*dimension]
            norm = math.sqrt(sum(x*x for x in row))
            if norm == 0:
                raise ValueError("zero embedding vector")
            result.append([x/norm for x in row])
        return result

    visual, language = normalize(images), normalize(texts)

    def softplus(x):
        return max(0.0, x) + math.log1p(math.exp(-abs(x)))

    total = 0.0
    for i in range(n):
        for j in range(n):
            cosine = sum(a*b for a,b in zip(visual[i], language[j]))
            label = 1 if i == j else -1
            total += softplus(-label*(logit_scale*cosine+bias))
    return total/(n*n)
