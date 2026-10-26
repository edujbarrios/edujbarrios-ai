import unittest
from canberra_distance import canberra_distance as distance


class CanberraDistanceTests(unittest.TestCase):
    def test_reference(self):
        self.assertAlmostEqual(distance([1, 2], [2, 4]), 2 / 3)
        self.assertEqual(distance([0, 0], [0, 0]), 0.0)
        self.assertEqual(distance([1, -1], [-1, 1]), 2.0)
        self.assertEqual(distance([0, 1], [1, 0]), 2.0)
        self.assertEqual(distance([1e308, -1e308], [-1e308, 1e308]), 2.0)
        self.assertAlmostEqual(distance([2, 4], [1, 2]), 2 / 3)

    def test_invalid(self):
        for x, y in (([], []), ([1], [1, 2]), ([True], [1]),
                     ([float("nan")], [1]), ([float("inf")], [1]),
                     (["1"], [1]), (None, [1]), ([2**2000], [0])):
            with self.subTest(x=x, y=y), self.assertRaises(ValueError):
                distance(x, y)


if __name__ == "__main__":
    unittest.main()
