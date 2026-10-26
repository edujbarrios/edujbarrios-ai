import unittest
from median_absolute_error import median_absolute_error as medae


class MedianAbsoluteErrorTests(unittest.TestCase):
    def test_known_values_and_extremes(self):
        self.assertEqual(medae([0, 10, 0], [0, 0, 0]), 0)
        self.assertEqual(medae([1, 2, 3, 4], [0, 0, 0, 0]), 2.5)
        self.assertEqual(medae([1, 2], [1, 2]), 0)
        self.assertEqual(medae([-1, -3, 4], [0, 0, 0]), 3)
        self.assertEqual(medae([5], [-5]), 10)
        self.assertEqual(medae([1e308, 1e308], [0, 0]), 1e308)

    def test_invalid(self):
        for a, b in (([], []), ([1], [1, 2]), ([True], [1]),
                     (["1"], [1]), ([float("nan")], [1]),
                     ([float("inf")], [1]), ([1e308], [-1e308]),
                     ([2**2000], [0]), (None, [1])):
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                medae(a, b)


if __name__ == "__main__":
    unittest.main()
