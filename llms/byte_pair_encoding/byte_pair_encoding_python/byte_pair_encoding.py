"""Deterministic UTF-8 byte-level pair merging without external dependencies."""

from collections import Counter


_MAX_BYTES = 100_000
_MAX_MERGES = 256


def _bytes(text, name):
    if not isinstance(text, str):
        raise ValueError(f"{name} must be text")
    try:
        data = text.encode("utf-8")
    except UnicodeError:
        raise ValueError(f"{name} must contain valid Unicode") from None
    if len(data) > _MAX_BYTES:
        raise ValueError(f"{name} must not exceed {_MAX_BYTES} bytes")
    return list(data)


def _replace_pair(tokens, pair, new_id):
    output = []
    index = 0
    while index < len(tokens):
        if index + 1 < len(tokens) and tokens[index] == pair[0] and tokens[index + 1] == pair[1]:
            output.append(new_id)
            index += 2
        else:
            output.append(tokens[index])
            index += 1
    return output


def train_byte_pair_encoding(corpus, max_merges, min_frequency=2):
    """Learn deterministic merges from independent UTF-8 corpus documents."""
    if not isinstance(corpus, (list, tuple)) or not 1 <= len(corpus) <= 1000:
        raise ValueError("corpus must contain 1..1000 text documents")
    if (isinstance(max_merges, bool) or not isinstance(max_merges, int)
            or not 0 <= max_merges <= _MAX_MERGES):
        raise ValueError("max_merges must be an integer in 0..256")
    if (isinstance(min_frequency, bool) or not isinstance(min_frequency, int)
            or not 2 <= min_frequency <= _MAX_BYTES):
        raise ValueError("min_frequency must be an integer in 2..100000")

    documents = [_bytes(text, "document") for text in corpus]
    if not 1 <= sum(map(len, documents)) <= _MAX_BYTES:
        raise ValueError("corpus must contain 1..100000 total UTF-8 bytes")
    merges = []
    for _ in range(max_merges):
        counts = Counter()
        for doc in documents:
            counts.update(zip(doc, doc[1:]))
        if not counts:
            break
        pair, count = min(counts.items(), key=lambda entry: (-entry[1], entry[0]))
        if count < min_frequency:
            break
        new_id = 256 + len(merges)
        documents = [_replace_pair(doc, pair, new_id) for doc in documents]
        merges.append([pair[0], pair[1]])
    return {"merges": merges, "token_ids": documents}


def apply_byte_pair_merges(text, merges):
    """Encode new text by applying previously learned merges in order."""
    tokens = _bytes(text, "text")
    if not isinstance(merges, (list, tuple)) or len(merges) > _MAX_MERGES:
        raise ValueError("merges must be a list of at most 256 pairs")
    seen = set()
    for index, pair in enumerate(merges):
        if not isinstance(pair, (list, tuple)) or len(pair) != 2:
            raise ValueError("each merge must contain two token IDs")
        left, right = pair
        if any(isinstance(v, bool) or not isinstance(v, int)
               or not 0 <= v < 256 + index for v in pair):
            raise ValueError("merge token IDs must refer to existing tokens")
        if (left, right) in seen:
            raise ValueError("duplicate merge pair")
        seen.add((left, right))
        tokens = _replace_pair(tokens, (left, right), 256 + index)
    return tokens
