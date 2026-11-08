import unittest
from character_bigram_jaccard import character_bigram_jaccard as score


class CharacterBigramJaccardTests(unittest.TestCase):
    def test_sets_and_symmetry(self):
        self.assertAlmostEqual(score("banana", "bandana"), 0.6)
        self.assertAlmostEqual(score("bandana", "banana"), 0.6)
        self.assertEqual(score("aaaa", "aa"), 1.0)
        self.assertAlmostEqual(score("abcd", "cdef"), 0.2)
        self.assertEqual(score("abc", "xyz"), 0.0)
        self.assertAlmostEqual(score("abc", "abcd"), 2 / 3)
        self.assertEqual(score("AB", "ab"), 0.0)
        self.assertEqual(score("  ", "  "), 1.0)

    def test_empty_sets_and_limits(self):
        self.assertEqual(score("", ""), 1.0)
        self.assertEqual(score("a", ""), 1.0)
        self.assertEqual(score("a", "b"), 1.0)
        self.assertEqual(score("a", "ab"), 0.0)
        self.assertEqual(score("a" * 512, "aa"), 1.0)
        self.assertEqual(score("a" * 512, "b" * 512), 0.0)
        self.assertEqual(score("~!", "~!"), 1.0)

    def test_invalid(self):
        for a, b in [
            ("a" * 513, "a"), ("a", "b" * 513),
            ("é", "e"), ("a\tb", "a"), ("a\0b", "a"),
            ("a", "\n"), (None, "a"), ("a", 1), (b"ab", "ab"),
        ]:
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                score(a, b)


if __name__ == "__main__":
    unittest.main()
