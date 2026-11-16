import unittest

from aho_corasick_search import aho_corasick_search as find


def naive(text, patterns):
    return [(index, start)
            for start in range(len(text))
            for index, pattern in enumerate(patterns)
            if text.startswith(pattern, start)]


class AhoCorasickTests(unittest.TestCase):
    def test_classic_example(self):
        patterns = ["he", "she", "his", "hers"]
        self.assertEqual(find("ushers", patterns), [(1, 1), (0, 2), (3, 2)])
        self.assertEqual(patterns, ["he", "she", "his", "hers"])
        self.assertEqual(find("", patterns), [])

    def test_overlaps_unicode_and_duplicate_patterns(self):
        cases = [
            ("aaaa", ["a", "aa", "aaa", "a"]),
            ("猫猫abc猫", ["猫", "猫猫", "abc", "猫a"]),
            ("ababa", ["aba", "ba", "a", "bab"]),
            ("bbbb", ["bb", "b"]),
            ("", ["a", "b"]),
            ("abcdef", ["xyz"]),
        ]
        for text, patterns in cases:
            with self.subTest(text=text, patterns=patterns):
                self.assertEqual(find(text, patterns), naive(text, patterns))

    def test_comparison_with_exhaustive_matcher(self):
        alphabet = "abc"
        patterns = ["a", "ab", "aba", "b", "bc", "ca", "cabc", "aa", "a"]
        for length in range(12):
            text = "".join(alphabet[(i * i + 2 * i + length) % 3]
                           for i in range(length))
            self.assertEqual(find(text, patterns), naive(text, patterns))

    def test_invalid(self):
        for text, patterns in (
            (None, ["a"]), ("a", []), ("a", [""]),
            ("a", ["a", 42]), (5, ["a"]),
            ("a", None), ("a", ["a"] * 257),
            ("a", ["a" * 8193]), ("a" * 100_001, ["a"]),
        ):
            with self.subTest(text_type=type(text), patterns_type=type(patterns)):
                with self.assertRaises(ValueError):
                    find(text, patterns)


if __name__ == "__main__":
    unittest.main()
