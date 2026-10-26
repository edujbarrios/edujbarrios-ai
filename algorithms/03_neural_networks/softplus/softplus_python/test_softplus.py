import math
import unittest
from softplus import softplus


class SoftplusTests(unittest.TestCase):
    def test_reference_and_extreme_values(self):
        v, d = softplus([0, 1, -1, 1000, -1000])
        self.assertAlmostEqual(v[0], math.log(2))
        self.assertEqual(d[0], 0.5)
        self.assertAlmostEqual(v[1], math.log1p(math.exp(1)))
        self.assertAlmostEqual(v[2], math.log1p(math.exp(-1)))
        self.assertAlmostEqual(d[1], 1 / (1 + math.exp(-1)))
        self.assertAlmostEqual(d[2], 1 / (1 + math.exp(1)))
        self.assertEqual(v[3:], [1000, 0])
        self.assertEqual(d[3:], [1, 0])

    def test_invalid(self):
        for x in ([], [True], [float("nan")], [float("inf")],
                  [1, float("-inf")], ["invalid"], None):
            with self.subTest(x=x), self.assertRaises(ValueError):
                softplus(x)


if __name__ == "__main__":
    unittest.main()
