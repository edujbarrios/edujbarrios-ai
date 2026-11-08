import unittest
from kmp_substring_search import kmp_substring_search as search


class KMPTests(unittest.TestCase):
    def test_matches(self):
        self.assertEqual(search("aaaa", "aa"), [0, 1, 2])
        self.assertEqual(search("ABABDABACDABABCABAB", "ABABCABAB"), [10])
        self.assertEqual(search("abcabc", "abc"), [0, 3])
        self.assertEqual(search("abc", "z"), [])
        self.assertEqual(search("abc", "ABC"), [])
        self.assertEqual(search("aba", "aba"), [0])
        self.assertEqual(search("!", "!"), [0])

    def test_empty_and_bounds(self):
        self.assertEqual(search("abc", ""), [0, 1, 2, 3])
        self.assertEqual(search("", ""), [0])
        self.assertEqual(search("", "a"), [])
        self.assertEqual(search("a" * 512, "a" * 512), [0])
        self.assertEqual(search("a" * 512, "aa"), list(range(511)))

    def test_invalid(self):
        for text, pattern in [
            ("a" * 513, "a"), ("a", "b" * 513),
            ("a\tb", "a"), ("a\nb", "a"), ("a\0", "a"),
            ("é", "e"), ("a", "\x01"), (None, "x"),
            ("a", 1), (b"abc", "a"),
        ]:
            with self.subTest(text=text, pattern=pattern), self.assertRaises(ValueError):
                search(text, pattern)


if __name__ == "__main__":
    unittest.main()
