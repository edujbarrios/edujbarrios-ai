import math
import unittest
from ivf_flat_search import ivf_flat_search as search


def exhaustive(points, query, k):
    return sorted(((i, sum((a - b) ** 2 for a, b in zip(point, query)))
                   for i, point in enumerate(points)),
                  key=lambda pair: (pair[1], pair[0]))[:k]


class IVFFlatTests(unittest.TestCase):
    def test_one_cell_and_full_scan(self):
        points = [[0], [1], [9], [10]]
        before = [row[:] for row in points]
        one_cell = search(points, [9.4], 2, 1, 3)
        self.assertEqual([index for index, _ in one_cell], [2, 3])
        self.assertEqual(search(points, [9.4], 2, 2, 3),
                         exhaustive(points, [9.4], 3))
        self.assertEqual(points, before)

    def test_full_probe_is_exact_with_duplicates_and_ties(self):
        points = [[(i * 7) % 17, (i * 11) % 13] for i in range(60)]
        points.extend([[1, 1], [1, 1]])
        for query in ([0, 0], [8, 3], [10, -3]):
            for size in (1, 7, len(points)):
                with self.subTest(query=query, size=size):
                    self.assertEqual(search(points, query, 5, 5, size),
                                     exhaustive(points, query, size))
        self.assertEqual(search([[0], [0], [1]], [0], 2, 2, 3),
                         [(0, 0.0), (1, 0.0), (2, 1.0)])
        self.assertEqual(search([[2]], [3], 1, 1, 1), [(0, 1.0)])

    def test_invalid(self):
        valid_points = [[0], [1]]
        for args in (
            ([], [0], 1, 1, 1), (valid_points, [0], 0, 1, 1),
            (valid_points, [0], 1, 0, 1), (valid_points, [0], 1, 2, 1),
            (valid_points, [0], 1, 1, 3), (valid_points, [0], True, 1, 1),
            (valid_points, [0], 1, 1, True),
            ([[0], [0]], [0], 2, 1, 1),
            ([[0], [1, 2]], [0], 1, 1, 1),
            ([[math.inf]], [0], 1, 1, 1),
            ([[True]], [0], 1, 1, 1),
            ([[0]], [math.nan], 1, 1, 1),
            (None, [0], 1, 1, 1),
        ):
            with self.subTest(args=args), self.assertRaises(ValueError):
                search(*args)


if __name__ == "__main__":
    unittest.main()
