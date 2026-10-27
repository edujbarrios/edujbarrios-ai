"""Boolean next-token mask for no-repeat n-gram language-model decoding."""


def no_repeat_ngram(history, vocabulary_size, n):
    """Return a mask indexed by token ID; True means forbidden."""
    if (isinstance(vocabulary_size, bool) or
            not isinstance(vocabulary_size, int) or
            not 1 <= vocabulary_size <= 4096):
        raise ValueError("vocabulary_size must be an integer in 1..4096")
    if isinstance(n, bool) or not isinstance(n, int) or not 1 <= n <= 32:
        raise ValueError("n must be an integer in 1..32")
    if not isinstance(history, (list, tuple)) or len(history) > 16384:
        raise ValueError("history must be a sequence of at most 16384 tokens")
    for token in history:
        if (isinstance(token, bool) or not isinstance(token, int) or
                not 0 <= token < vocabulary_size):
            raise ValueError("token ID outside vocabulary")
    blocked = [False] * vocabulary_size
    length = len(history)
    if length < n:
        return blocked
    suffix = history[length - n + 1:] if n > 1 else history[0:0]
    for index in range(length - n + 1):
        if history[index:index + n - 1] == suffix:
            blocked[history[index + n - 1]] = True
    return blocked
