import unittest
from jaro_winkler_similarity import jaro_winkler_similarity as score


class JaroWinklerTests(unittest.TestCase):
    def test_reference_cases(self):
        self.assertAlmostEqual(score("MARTHA", "MARHTA"), 0.9611111111111111)
        self.assertAlmostEqual(score("DIXON", "DICKSONX"), 0.8133333333333334)
        self.assertAlmostEqual(score("JELLYFISH", "SMELLYFISH"), 0.8962962962962963)
        self.assertAlmostEqual(score("ABCD", "ACBD"), 0.925)
        self.assertAlmostEqual(score("abc", "axz"), 5 / 9)

    def test_symmetry_and_boundaries(self):
        self.assertEqual(score("", ""), 1.0)
        self.assertEqual(score("", "abc"), 0.0)
        self.assertEqual(score("abc", ""), 0.0)
        self.assertEqual(score("x", "x"), 1.0)
        self.assertEqual(score("x", "y"), 0.0)
        self.assertEqual(score("a" * 512, "a" * 512), 1.0)
        self.assertEqual(score("A", "a"), 0.0)
        for a, b in [("MARTHA", "MARHTA"), ("ABCD", "ACBD"), ("abc", "axz")]:
            self.assertAlmostEqual(score(a, b), score(b, a))

    def test_invalid(self):
        for a, b in [
            ("a" * 513, "a"), ("a", "b" * 513),
            ("a\tb", "a"), ("a\nb", "a"), ("a\0", "a"),
            ("é", "e"), ("a", "\x01"),
            (None, "a"), (1, "a"), ("a", b"a"),
        ]:
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                score(a, b)


if __name__ == "__main__":
    unittest.main()
