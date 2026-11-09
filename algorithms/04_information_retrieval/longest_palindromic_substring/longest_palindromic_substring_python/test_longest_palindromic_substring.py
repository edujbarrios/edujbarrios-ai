import unittest
from longest_palindromic_substring import longest_palindromic_substring as match


class PalindromeTests(unittest.TestCase):
    def test_examples_and_ties(self):
        self.assertEqual(match("babad"), (3, 0))
        self.assertEqual(match("cbbd"), (2, 1))
        self.assertEqual(match("aaaa"), (4, 0))
        self.assertEqual(match("abc"), (1, 0))
        self.assertEqual(match("racecar"), (7, 0))
        self.assertEqual(match("abacdfgdcaba"), (3, 0))
        self.assertEqual(match("a b a"), (5, 0))
        self.assertEqual(match("a!a"), (3, 0))
        self.assertEqual(match("abbaxabba"), (9, 0))

    def test_boundaries(self):
        self.assertEqual(match(""), (0, 0))
        self.assertEqual(match("z"), (1, 0))
        self.assertEqual(match("a" * 512), (512, 0))
        self.assertEqual(match("a" * 256 + "b" * 256), (256, 0))
        self.assertEqual(match("~"), (1, 0))

    def test_invalid(self):
        for value in ["a" * 513, "é", "a\tb", "a\nb", "a\0b", None, 22, b"aba"]:
            with self.subTest(value=value), self.assertRaises(ValueError):
                match(value)


if __name__ == "__main__":
    unittest.main()
