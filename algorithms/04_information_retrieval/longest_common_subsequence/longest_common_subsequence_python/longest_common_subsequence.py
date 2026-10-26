"""Memory-efficient longest common subsequence length for int32 token IDs."""


def longest_common_subsequence(first: list[int], second: list[int]) -> int:
    """Return the LCS length, retaining order and duplicate occurrences."""
    def check(data: list[int]) -> list[int]:
        if not isinstance(data, (tuple, list)):
            raise ValueError("token sequence must be a list or tuple")
        for token in data:
            if isinstance(token, bool) or not isinstance(token, int) or \
                    not -(2**31) <= token <= 2**31 - 1:
                raise ValueError("signed 32-bit integer token IDs required")
        return data

    a, b = check(first), check(second)
    if len(a) < len(b):
        a, b = b, a
    previous = [0] * (len(b) + 1)
    current = [0] * (len(b) + 1)
    for item in a:
        for j, other in enumerate(b, start=1):
            current[j] = (previous[j - 1] + 1 if item == other
                          else max(previous[j], current[j - 1]))
        previous, current = current, previous
    return previous[-1]
