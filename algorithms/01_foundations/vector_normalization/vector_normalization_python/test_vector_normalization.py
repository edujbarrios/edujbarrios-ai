import math
import unittest

from vector_normalization import vector_normalization


class VectorNormalizationTests(unittest.TestCase):
    def test_three_four_five(self):
        a, b = vector_normalization([3, 4])
        self.assertAlmostEqual(a, 0.6)
        self.assertAlmostEqual(b, 0.8)

    def test_negative_and_unit_norm(self):
        x = vector_normalization([-3, 4])
        self.assertAlmostEqual(x[0], -0.6)
        self.assertAlmostEqual(math.hypot(*x), 1.0)

    def test_extreme_magnitudes(self):
        self.assertEqual(vector_normalization([1e-308]), [1.0])
        self.assertAlmostEqual(math.hypot(*vector_normalization([1e308, 1e308])), 1.0)

    def test_invalid(self):
        for values in ([], [0, 0], [float("nan")], [float("inf")],
                       [True], ["1"]):
            with self.subTest(values=values), self.assertRaises(ValueError):
                vector_normalization(values)


if __name__ == "__main__":
    unittest.main()
