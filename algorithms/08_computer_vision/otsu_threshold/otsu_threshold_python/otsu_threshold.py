"""Otsu threshold selection on a finite 8-bit grayscale pixel vector."""


def otsu_threshold(pixels):
    """Return the smallest maximizer of between-class variance."""
    if not isinstance(pixels, (list, tuple)) or not 1 <= len(pixels) <= 4096:
        raise ValueError("pixels must be a sequence with 1..4096 values")
    hist = [0] * 256
    total = 0
    for value in pixels:
        if isinstance(value, bool) or not isinstance(value, int) or not 0 <= value <= 255:
            raise ValueError("8-bit integer grayscale values required")
        hist[value] += 1
        total += value
    n = len(pixels)
    count_low = sum_low = 0
    best_score = -1.0
    threshold = 0
    for t in range(255):
        count_low += hist[t]
        sum_low += t * hist[t]
        if count_low == 0 or count_low == n:
            continue
        difference = total * count_low - n * sum_low
        score = difference * difference / (count_low * (n - count_low))
        if score > best_score:
            best_score = score
            threshold = t
    return threshold
