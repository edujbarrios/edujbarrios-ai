import math
import unittest
from gaussian_kde_1d import gaussian_kde_1d as kde


class GaussianKDETests(unittest.TestCase):
    def test_examples(self):
        s = math.sqrt(2 * math.pi)
        self.assertAlmostEqual(kde([0], 0, 1), 1 / s)
        self.assertAlmostEqual(kde([-1, 1], 0, 1), math.exp(-.5) / s)
        self.assertAlmostEqual(kde([1, 1], 1, 2), 1 / (2 * s))
        self.assertEqual(kde([0], 1000, 0.001), 0)
        self.assertAlmostEqual(kde([0] * 512, 0, 1), 1 / s)
        self.assertAlmostEqual(kde([0], -1000, 1000), math.exp(-.5) / (1000 * s))
        source = [0, 1]
        kde(source, 0, 1)
        self.assertEqual(source, [0, 1])

    def test_invalid(self):
        cases = [
            ([], 0, 1), ([0]*513, 0, 1), (None, 0, 1),
            ([True], 0, 1), ([float("nan")], 0, 1),
            ([float("inf")], 0, 1), ([1001], 0, 1),
            ([0], True, 1), ([0], float("nan"), 1),
            ([0], 1001, 1), ([0], 0, True),
            ([0], 0, 0), ([0], 0, 0.0001),
            ([0], 0, 1001), ([0], 0, float("inf")),
        ]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                kde(*args)


if __name__ == "__main__":
    unittest.main()
