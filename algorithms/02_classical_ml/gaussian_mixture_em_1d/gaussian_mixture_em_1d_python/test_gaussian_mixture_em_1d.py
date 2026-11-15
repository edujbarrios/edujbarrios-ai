"""Unit tests for deterministic one-dimensional Gaussian mixture EM."""

import math
import unittest
from gaussian_mixture_em_1d import gaussian_mixture_em_1d as fit


class GaussianMixtureTests(unittest.TestCase):
    def test_two_well_separated_components(self):
        values = [-5.2, -5, -4.8, 4.8, 5, 5.2]
        result = fit(values)
        self.assertAlmostEqual(result["means"][0], -5, delta=0.25)
        self.assertAlmostEqual(result["means"][1], 5, delta=0.25)
        self.assertAlmostEqual(result["weights"][0], 0.5, delta=0.05)
        self.assertAlmostEqual(sum(result["weights"]), 1)
        self.assertTrue(all(v >= 1e-6 for v in result["variances"]))
        self.assertTrue(math.isfinite(result["log_likelihood"]))
        self.assertTrue(1 <= result["iterations"] <= 100)
        self.assertEqual(values, [-5.2, -5, -4.8, 4.8, 5, 5.2])

    def test_determinism_and_iteration_limit(self):
        data = [-2, -1, 1, 2]
        self.assertEqual(fit(data), fit(data))
        self.assertEqual(fit(data, max_iterations=1)["iterations"], 1)
        self.assertLess(fit(data)["means"][0], fit(data)["means"][1])
        self.assertEqual(len(fit([0, 1])["variances"]), 2)

    def test_invalid(self):
        for values in ([], [0], [1, 1], [True, 1], [math.inf, 1],
                       [math.nan, 1], [1_000_001, 1], ["1", 2], None):
            with self.subTest(values=values), self.assertRaises(ValueError):
                fit(values)
        for kwargs in ({"max_iterations": 0}, {"max_iterations": True},
                       {"max_iterations": 1001}, {"tolerance": -1},
                       {"tolerance": math.nan}, {"tolerance": True}):
            with self.subTest(kwargs=kwargs), self.assertRaises(ValueError):
                fit([0, 1], **kwargs)


if __name__ == "__main__":
    unittest.main()
