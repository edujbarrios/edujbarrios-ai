import math
import unittest
from exponential_moving_average import exponential_moving_average as ema


class ExponentialMovingAverageTests(unittest.TestCase):
    def test_values(self):
        self.assertEqual(ema([10, 20, 10], .5), [10, 15, 12.5])
        self.assertEqual(ema([10, 20, 10], 1), [10, 20, 10])
        self.assertEqual(ema([7, 7, 7], .1), [7, 7, 7])
        self.assertEqual(ema([1e308, -1e308], .5), [1e308, 0])

    def test_invalid(self):
        for data, alpha in [
            ([], .5), ([1, math.nan], .5), ([1, math.inf], .5),
            ([1, True], .5), (["1"], .5),
            ([1], 0), ([1], -1), ([1], 1.1),
            ([1], math.nan), ([1], True),
        ]:
            with self.subTest(data=data, alpha=alpha), self.assertRaises(ValueError):
                ema(data, alpha)


if __name__ == "__main__":
    unittest.main()
