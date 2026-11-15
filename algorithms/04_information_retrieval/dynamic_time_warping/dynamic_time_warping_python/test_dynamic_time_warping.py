import math
import unittest

from dynamic_time_warping import dynamic_time_warping as dtw


def reference(a, b):
    n, m = len(a), len(b)
    d = [[math.inf] * (m + 1) for _ in range(n + 1)]
    d[0][0] = 0
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            d[i][j] = abs(a[i - 1] - b[j - 1]) + min(
                d[i - 1][j], d[i][j - 1], d[i - 1][j - 1])
    return float(d[n][m])


class DTWTests(unittest.TestCase):
    def test_worked_examples(self):
        self.assertEqual(dtw([1, 2, 3], [1, 2, 2, 3]), 0.0)
        self.assertEqual(dtw([1, 3], [2, 4], window=0), 2.0)
        self.assertEqual(dtw([3], [2, 2, 2]), 3.0)
        self.assertEqual(dtw([-2, 0], [-2, 0]), 0.0)

    def test_matches_full_matrix_and_does_not_mutate(self):
        a, b = [4, 1, 5, 2], [1, 4, 2]
        copy_a, copy_b = a[:], b[:]
        self.assertEqual(dtw(a, b), reference(a, b))
        self.assertEqual(dtw(b, a), reference(b, a))
        self.assertEqual((a, b), (copy_a, copy_b))
        for n in range(1, 7):
            for m in range(1, 7):
                x = [(i * 5) % 7 - 3 for i in range(n)]
                y = [(j * 3) % 5 - 2 for j in range(m)]
                self.assertEqual(dtw(x, y), reference(x, y))
                self.assertGreaterEqual(
                    dtw(x, y, window=abs(n - m)), dtw(x, y))

    def test_invalid(self):
        for args in (
            ([], [1], None), ([1], [], None), ([True], [0], None),
            ([math.nan], [1], None), ([math.inf], [1], None),
            ([1_000_001], [1], None), (["1"], [1], None),
            ([1, 2], [2], 0), ([1], [2], -1),
            ([1], [2], True), ([1], [2], 1.0),
            ([1], [2], 513), (None, [2], None),
        ):
            with self.subTest(args=args), self.assertRaises(ValueError):
                dtw(*args)


if __name__ == "__main__":
    unittest.main()
