import math
import unittest

from kalman_filter_1d import kalman_filter_1d as filter1d


class KalmanFilterTests(unittest.TestCase):
    def test_known_updates_and_immutability(self):
        samples = [1, 2]
        result = filter1d(samples, 0, 1, initial_mean=0, initial_variance=1)
        self.assertEqual(result["means"], [0.5, 1.0])
        self.assertAlmostEqual(result["variances"][0], 0.5)
        self.assertAlmostEqual(result["variances"][1], 1/3)
        self.assertAlmostEqual(result["gains"][0], 0.5)
        self.assertAlmostEqual(result["gains"][1], 1/3)
        self.assertEqual(samples, [1, 2])

    def test_missing_and_zero_uncertainty(self):
        expected = filter1d([1, 2], 0, 1)
        missing = filter1d([1, None, 2], 0, 1)
        self.assertEqual(missing["means"], [expected["means"][0],
                                           expected["means"][0],
                                           expected["means"][1]])
        self.assertEqual(missing["gains"][1], 0)
        no_uncertainty = filter1d([3, 4], 0, 1, initial_mean=2, initial_variance=0)
        self.assertEqual(no_uncertainty["means"], [2, 2])
        self.assertEqual(no_uncertainty["variances"], [0, 0])
        self.assertEqual(filter1d([1, None], 1, 1),
                         filter1d([1, None], 1, 1))

    def test_invalid_inputs(self):
        for args in (
            ([], 0, 1), ([True], 0, 1), ([math.nan], 0, 1),
            ([math.inf], 0, 1), ([1_000_001], 0, 1),
            (["1"], 0, 1), (None, 0, 1),
            ([1], -1, 1), ([1], 1, 0), ([1], 1, math.inf),
            ([1], True, 1),
        ):
            with self.subTest(args=args), self.assertRaises(ValueError):
                filter1d(*args)
        for kwargs in (
            {"initial_mean": math.nan}, {"initial_mean": True},
            {"initial_variance": -1}, {"initial_variance": True},
        ):
            with self.subTest(kwargs=kwargs), self.assertRaises(ValueError):
                filter1d([1], 0, 1, **kwargs)


if __name__ == "__main__":
    unittest.main()
