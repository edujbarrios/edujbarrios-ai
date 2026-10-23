import unittest

from euclidean_distance import euclidean_distance


class EuclideanDistanceTests(unittest.TestCase):
    def test_three_four_five(self):
        self.assertEqual(euclidean_distance([0, 0], [3, 4]), 5.0)

    def test_same_vector_and_empty(self):
        self.assertEqual(euclidean_distance([1, 2], [1, 2]), 0.0)
        self.assertEqual(euclidean_distance([], []), 0.0)

    def test_stable_large_numbers(self):
        self.assertAlmostEqual(euclidean_distance([1e154], [0]) / 1e154, 1.0)

    def test_mismatched(self):
        with self.assertRaises(ValueError):
            euclidean_distance([1], [])

    def test_invalid_values(self):
        for invalid in (float("nan"), float("inf"), True, "1"):
            with self.subTest(value=invalid), self.assertRaises(ValueError):
                euclidean_distance([invalid], [0])

    def test_overflow(self):
        with self.assertRaises(ValueError):
            euclidean_distance([1e308], [-1e308])


if __name__ == "__main__":
    unittest.main()
