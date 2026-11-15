"""Exactness, stable ties, duplicates, and input validation for k-d tree search."""

import math
import unittest
from kd_tree_knn import kd_tree_knn as neighbors


def brute_force(points, query, k):
    return sorted(((i, sum((a - b) ** 2 for a, b in zip(point, query)))
                   for i, point in enumerate(points)),
                  key=lambda item: (item[1], item[0]))[:k]


class KDTreeTests(unittest.TestCase):
    def test_example_and_ties(self):
        self.assertEqual(neighbors([[0, 0], [3, 4], [1, 0]], [0, 0], 2),
                         [(0, 0.0), (2, 1.0)])
        points = [[1, 0], [-1, 0], [0, 1], [0, -1], [0, 0]]
        self.assertEqual(neighbors(points, [0, 0], 4),
                         [(4, 0.0), (0, 1.0), (1, 1.0), (2, 1.0)])
        self.assertEqual(neighbors([[1], [1], [1]], [1], 2),
                         [(0, 0.0), (1, 0.0)])
        self.assertEqual(neighbors([[9]], [3], 1), [(0, 36.0)])

    def test_matches_exhaustive_search(self):
        points = [[(i * 11) % 17 - 8, (i * 7) % 13 - 6, (i * 5) % 19 - 9]
                  for i in range(100)]
        baseline = [row[:] for row in points]
        for query in ([0, 0, 0], [1, 2, 3], [10, -9, 2]):
            for k in (1, 5, 37, 100):
                with self.subTest(query=query, k=k):
                    self.assertEqual(neighbors(points, query, k),
                                     brute_force(points, query, k))
        self.assertEqual(points, baseline)

    def test_invalid(self):
        inputs = [([], [0], 1), ([[0]], [0], 0),
                  ([[0]], [0], True), ([[0]], [0], 2),
                  ([[0, 1], [1]], [0, 0], 1),
                  ([[0]], [0, 0], 1), ([[True]], [0], 1),
                  ([[math.inf]], [0], 1), ([[0]], [math.nan], 1),
                  ([[1_000_001]], [0], 1), ([[0]], [], 1),
                  ([[0]], [0], 1.0), (None, [0], 1)]
        for args in inputs:
            with self.subTest(args=args), self.assertRaises(ValueError):
                neighbors(*args)


if __name__ == "__main__":
    unittest.main()
