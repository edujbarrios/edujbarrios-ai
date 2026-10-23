import unittest

from dot_product import dot_product


class DotProductTests(unittest.TestCase):
    def test_positive(self):
        self.assertEqual(dot_product([1, 2, 3], [4, 5, 6]), 32.0)

    def test_negative_and_fractions(self):
        self.assertEqual(dot_product([-1, 2, -3], [4, -5, 6]), -32.0)
        self.assertEqual(dot_product([0.5, -1.5], [2, 4]), -5.0)

    def test_empty(self):
        self.assertEqual(dot_product([], []), 0.0)

    def test_mismatched(self):
        with self.assertRaises(ValueError):
            dot_product([1], [])

    def test_invalid_values(self):
        for invalid in (float("nan"), float("inf"), "1", True):
            with self.subTest(value=invalid), self.assertRaises(ValueError):
                dot_product([invalid], [1.0])

    def test_overflow(self):
        with self.assertRaises(ValueError):
            dot_product([1e308], [1e308])


if __name__ == "__main__":
    unittest.main()
