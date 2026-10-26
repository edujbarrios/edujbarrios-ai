import math
import unittest
from minkowski_distance import minkowski_distance as md


class MinkowskiTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(md([0, 0], [3, 4], 1), 7)
        self.assertAlmostEqual(md([0, 0], [3, 4], 2), 5)
        self.assertAlmostEqual(md([0, 0], [3, 4], 3), 91 ** (1 / 3))
        self.assertEqual(md([1], [1]), 0)
        self.assertAlmostEqual(md([0, 0], [3, 4], 1000), 4, places=2)
        self.assertAlmostEqual(md([1e150, 1e150], [0, 0]) / 1e150,
                               math.sqrt(2))

    def test_invalid(self):
        for a, b, p in (([], [], 2), ([1], [1, 2], 2),
                        ([True], [0], 2), ([float("nan")], [0], 2),
                        ([float("inf")], [0], 2), ([1], [0], 0),
                        ([1], [0], float("inf")), ([1], [0], True),
                        ([1e308], [-1e308], 2), (None, [0], 2)):
            with self.subTest(a=a, b=b, p=p), self.assertRaises(ValueError):
                md(a, b, p)


if __name__ == "__main__":
    unittest.main()
