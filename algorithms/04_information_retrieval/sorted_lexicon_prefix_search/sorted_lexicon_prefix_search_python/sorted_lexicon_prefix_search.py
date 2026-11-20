"""Prefix-completion indices in a sorted unique ASCII word dictionary."""


def _valid_word(word: str, allow_empty: bool = False) -> bool:
    return (isinstance(word, str) and (allow_empty or bool(word))
            and len(word) <= 512 and all("a" <= ch <= "z" for ch in word))


def sorted_lexicon_prefix_search(terms: list[str], prefix: str) -> list[int]:
    """Return zero-based indices of dictionary words beginning with prefix."""
    if not isinstance(terms, (list, tuple)) or not _valid_word(prefix, True):
        raise ValueError("invalid dictionary or prefix")
    previous = None
    for term in terms:
        if not _valid_word(term) or (previous is not None and previous >= term):
            raise ValueError("dictionary must be sorted, unique lowercase ASCII words")
        previous = term

    low, high = 0, len(terms)
    while low < high:
        mid = (low + high) // 2
        if terms[mid] < prefix:
            low = mid + 1
        else:
            high = mid

    result = []
    while low < len(terms) and terms[low].startswith(prefix):
        result.append(low)
        low += 1
    return result
