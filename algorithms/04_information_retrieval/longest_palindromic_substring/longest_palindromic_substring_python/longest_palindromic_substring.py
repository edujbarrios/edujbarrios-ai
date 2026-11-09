"""Longest palindromic substring over case-sensitive printable ASCII."""


def longest_palindromic_substring(text: str) -> tuple[int, int]:
    """Return (length, first start) for the longest contiguous palindrome."""
    if (not isinstance(text, str) or len(text) > 512
            or any(not 32 <= ord(ch) <= 126 for ch in text)):
        raise ValueError("expected printable ASCII string of length <=512")
    n = len(text)
    best_length = 0
    best_start = 0
    for center in range(n):
        for left, right in ((center, center), (center, center + 1)):
            while left >= 0 and right < n and text[left] == text[right]:
                length = right - left + 1
                if length > best_length or (
                    length == best_length and left < best_start
                ):
                    best_length, best_start = length, left
                left -= 1
                right += 1
    return best_length, best_start
