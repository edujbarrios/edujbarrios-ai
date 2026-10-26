import math
import unittest
from hellinger_distance import hellinger_distance


class HellingerTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(hellinger_distance([0.5, 0.5], [0.5, 0.5]), 0)
        self.assertEqual(hellinger_distance([1, 0], [0, 1]), 1)
        expected = math.sqrt(1 - 1 / math.sqrt(2))
        self.assertAlmostEqual(hellinger_distance([1, 0], [0.5, 0.5]), expected)
        self.assertAlmostEqual(hellinger_distance([0.5, 0.5], [1, 0]), expected)
        self.assertEqual(hellinger_distance([1], [1]), 0)

    def test_invalid(self):
        for p, q in (([], []), ([1], []), ([1], [0, 1]),
                     ([0.2, 0.2], [0.5, 0.5]), ([1.1, -0.1], [1, 0]),
                     ([True], [1]), ([float("nan")], [1]),
                     ([float("inf")], [1]), (["1"], [1])):
            with self.subTest(p=p, q=q), self.assertRaises(ValueError):
                hellinger_distance(p, q)


if __name__ == "__main__":
    unittest.main()
