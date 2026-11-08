import unittest
from rabin_karp_substring_search import rabin_karp_substring_search as search


class RabinKarpTests(unittest.TestCase):
    def test_matches(self):
        self.assertEqual(search("aaaa", "aa"), [0, 1, 2])
        self.assertEqual(search("abababa", "aba"), [0, 2, 4])
        self.assertEqual(search("ABABDABACDABABCABAB", "ABABCABAB"), [10])
        self.assertEqual(search("abcabc", "abc"), [0, 3])
        self.assertEqual(search("abc", "ABC"), [])
        self.assertEqual(search("abc", "d"), [])
        self.assertEqual(search("ab", "abc"), [])

    def test_boundaries(self):
        self.assertEqual(search("abc", ""), [0, 1, 2, 3])
        self.assertEqual(search("", ""), [0])
        self.assertEqual(search("", "a"), [])
        self.assertEqual(search("a" * 512, "aa"), list(range(511)))
        self.assertEqual(search("b" + "a" * 511, "b" + "a" * 511), [0])

    def test_invalid(self):
        for text, pattern in [
            ("a" * 513, "a"), ("a", "b" * 513),
            ("é", "e"), ("a\tb", "a"), ("a\0b", "a"),
            ("a", "\n"), (None, "a"), ("a", 1),
        ]:
            with self.subTest(text=text, pattern=pattern), self.assertRaises(ValueError):
                search(text, pattern)


if __name__ == "__main__":
    unittest.main()
