import math
import unittest

from sparsemax import sparsemax


class SparsemaxTests(unittest.TestCase):
    def test_sparse_support(self):
        self.assertEqual(sparsemax([3, 1, 0]), [1.0, 0.0, 0.0])
        self.assertEqual(sparsemax([0, 0, -1]), [0.5, 0.5, 0.0])
        self.assertEqual(sparsemax([100, 100, 99]), [0.5, 0.5, 0.0])

    def test_uniform_and_singleton(self):
        for value in sparsemax([1, 1, 1]):
            self.assertAlmostEqual(value, 1 / 3)
        self.assertEqual(sparsemax([1e308]), [1.0])
        self.assertEqual(sparsemax([1e308, -1e308]), [1.0, 0.0])

    def test_invariance(self):
        actual = sparsemax([2, 1, 0])
        shifted = sparsemax([102, 101, 100])
        for a, b in zip(actual, shifted):
            self.assertAlmostEqual(a, b)
        self.assertAlmostEqual(sum(actual), 1.0)
        self.assertTrue(all(math.isfinite(x) and x >= 0 for x in actual))

    def test_invalid(self):
        for values in ([], [math.nan], [math.inf], [-math.inf],
                       [True], ["1"], [10**400]):
            with self.subTest(values=values), self.assertRaises(ValueError):
                sparsemax(values)


if __name__ == "__main__":
    unittest.main()
