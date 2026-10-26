import unittest
from mean_squared_error import mean_squared_error as mse


class MeanSquaredErrorTests(unittest.TestCase):
    def test_fixtures(self):
        self.assertEqual(mse([3, 0], [1, 2]), 4)
        self.assertEqual(mse([1, 2], [1, 2]), 0)
        self.assertEqual(mse([1, 2], [2, 3]), 1)
        self.assertEqual(mse([-3, 3], [0, 0]), 9)
        self.assertEqual(mse([5], [-5]), 100)
        self.assertEqual(mse([1e154, 1e154], [0, 0]), 1e308)

    def test_invalid(self):
        pairs = (([], []), ([1], [1, 2]), ([True], [1]),
                 (["1"], [1]), ([float("nan")], [0]),
                 ([float("inf")], [0]), ([1e308], [0]), (None, [1]))
        for a, b in pairs:
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                mse(a, b)


if __name__ == "__main__":
    unittest.main()
