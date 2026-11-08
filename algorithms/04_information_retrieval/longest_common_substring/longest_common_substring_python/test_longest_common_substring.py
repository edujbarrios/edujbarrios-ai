import unittest
from longest_common_substring import longest_common_substring as match


class LongestCommonSubstringTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(match("banana", "ananas"), (5, 1, 0))
        self.assertEqual(match("abcdef", "zabcf"), (3, 0, 1))
        self.assertEqual(match("ababa", "babab"), (4, 0, 1))
        self.assertEqual(match("abcabc", "abc"), (3, 0, 0))
        self.assertEqual(match("aa", "baaa"), (2, 0, 1))
        self.assertEqual(match("ABC", "abc"), (0, 0, 0))
        self.assertEqual(match("x-y", "z-y"), (2, 1, 1))

    def test_bounds(self):
        self.assertEqual(match("", ""), (0, 0, 0))
        self.assertEqual(match("abc", ""), (0, 0, 0))
        self.assertEqual(match("", "abc"), (0, 0, 0))
        self.assertEqual(match("a" * 512, "a" * 512), (512, 0, 0))
        self.assertEqual(match("a" * 512, "a" * 511 + "b"), (511, 0, 0))
        self.assertEqual(match("A", "A"), (1, 0, 0))

    def test_invalid(self):
        for a, b in [
            ("a" * 513, "a"), ("a", "a" * 513),
            ("a\tb", "a"), ("a", "\n"), ("a\0b", "a"),
            ("é", "e"), (None, "a"), ("a", 10), (b"abc", "a"),
        ]:
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                match(a, b)


if __name__ == "__main__":
    unittest.main()
