"""Reference cases for finite-sample split conformal intervals."""

import math
import unittest
from split_conformal_regression import split_conformal_regression as intervals


class SplitConformalTests(unittest.TestCase):
    def test_finite_sample_rank(self):
        target = [1, 2, 3, 4]
        pred = [0, 2, 1, 4]  # Sorted residuals: [0, 0, 1, 2].
        self.assertEqual(intervals(target, pred, [0, 10], alpha=0.25),
                         [(-2, 2), (8, 12)])
        self.assertEqual(intervals(target, pred, [10], alpha=0.5), [(9, 11)])
        self.assertEqual(intervals(target, pred, [10], alpha=0.75), [(10, 10)])
        self.assertEqual(intervals(target, pred, [], alpha=0.5), [])
        self.assertEqual((target, pred), ([1, 2, 3, 4], [0, 2, 1, 4]))

    def test_infinite_quantile_when_calibration_is_too_small(self):
        self.assertEqual(intervals([1], [0], [3], alpha=0.5), [(2, 4)])
        self.assertEqual(intervals([1], [0], [3], alpha=0.1),
                         [(-math.inf, math.inf)])

    def test_invalid_and_overflow(self):
        invalid = [
            ([], [], [1], 0.5), ([1], [], [1], 0.5),
            ([1], [1], [math.nan], 0.5), ([True], [1], [1], 0.5),
            ([1], [1], [1], 0), ([1], [1], [1], 1),
            ([1], [1], [1], math.nan), ([1], [1], [1], True),
            ([1e308], [-1e308], [1], 0.5),
            ([1e308], [0], [1e308], 0.5),
        ]
        for args in invalid:
            with self.subTest(args=args), self.assertRaises(ValueError):
                intervals(*args[:3], alpha=args[3])


if __name__ == "__main__":
    unittest.main()
