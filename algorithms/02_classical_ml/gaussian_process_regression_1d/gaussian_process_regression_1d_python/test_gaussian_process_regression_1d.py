import math
import unittest

from gaussian_process_regression_1d import gaussian_process_regression_1d as gp


class GaussianProcessTests(unittest.TestCase):
    def test_one_observation_analytic_posterior(self):
        result = gp([0], [2], [0, 100], noise_variance=0.25)
        self.assertAlmostEqual(result["means"][0], 1.6, places=7)
        self.assertAlmostEqual(result["variances"][0], 0.2, places=7)
        self.assertAlmostEqual(result["means"][1], 0, places=10)
        self.assertAlmostEqual(result["variances"][1], 1, places=10)

    def test_duplicates_noise_and_immutability(self):
        x, y = [0, 0, 1], [2, 2, 3]
        output = gp(x, y, [0, 1, 2], noise_variance=0)
        self.assertEqual(len(output["means"]), 3)
        self.assertTrue(all(math.isfinite(v) and 0 <= v <= 1.000001
                            for v in output["variances"]))
        self.assertEqual((x, y), ([0, 0, 1], [2, 2, 3]))
        high_noise = gp([0], [2], [0], noise_variance=10)
        low_noise = gp([0], [2], [0], noise_variance=0.1)
        self.assertGreater(high_noise["variances"][0], low_noise["variances"][0])
        self.assertEqual(gp([0], [1], []), {"means": [], "variances": []})

    def test_invalid(self):
        for args in (
            ([], [], [1]), ([0], [], [0]), ([0], [1], [math.nan]),
            ([True], [1], [0]), ([1e4], [1], [0]),
            ([0], [math.inf], [0]), (None, [1], [0]),
        ):
            with self.subTest(args=args), self.assertRaises(ValueError):
                gp(*args)
        valid = ([0], [1], [0])
        for kwargs in (
            {"length_scale": 0}, {"length_scale": True},
            {"signal_variance": 0}, {"signal_variance": math.nan},
            {"noise_variance": -1}, {"noise_variance": math.inf},
        ):
            with self.subTest(kwargs=kwargs), self.assertRaises(ValueError):
                gp(*valid, **kwargs)


if __name__ == "__main__":
    unittest.main()
