import math
import unittest

from mean_absolute_error import mean_absolute_error as mae


class MeanAbsoluteErrorTests(unittest.TestCase):
    def test_scores(self):
        self.assertAlmostEqual(mae([2, -1, 5], [1, 1, 3]), 5/3)
        self.assertEqual(mae([2, -1], [2, -1]), 0.0)
        self.assertEqual(mae([2], [0]), 2.0)
        self.assertEqual(mae([1e308, 1e308], [0, 0]), 1e308)

    def test_invalid(self):
        for truth, pred in [
            ([], []), ([1], []), ([math.nan], [0]),
            ([math.inf], [0]), ([True], [0]),
            (["1"], [0]), ([1e308], [-1e308]),
        ]:
            with self.subTest(truth=truth, pred=pred), self.assertRaises(ValueError):
                mae(truth, pred)


if __name__ == "__main__":
    unittest.main()
