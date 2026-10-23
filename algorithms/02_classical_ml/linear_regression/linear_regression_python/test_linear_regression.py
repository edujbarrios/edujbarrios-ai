import unittest
from linear_regression import linear_regression


class LinearRegressionTests(unittest.TestCase):
    def test_exact_line(self):
        self.assertEqual(linear_regression([0, 1, 2, 3], [1, 3, 5, 7]), (2.0, 1.0))

    def test_horizontal_line(self):
        self.assertEqual(linear_regression([0, 1, 2, 3], [3, 3, 3, 3]), (0.0, 3.0))

    def test_invalid(self):
        for x, y in (([], []), ([1], [2]), ([1, 2], [1]),
                     ([2, 2, 2], [1, 2, 3]),
                     ([float("nan"), 1], [1, 2]),
                     ([1e308, -1e308], [1e308, -1e308]),
                     ([True, 2], [1, 2])):
            with self.subTest(x=x, y=y), self.assertRaises(ValueError):
                linear_regression(x, y)


if __name__ == "__main__":
    unittest.main()
