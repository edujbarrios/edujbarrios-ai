import unittest
from spearman_rank_correlation import spearman_rank_correlation as correlate


class SpearmanTests(unittest.TestCase):
    def test_reference(self):
        self.assertAlmostEqual(correlate([1, 2, 3], [4, 5, 6]), 1)
        self.assertAlmostEqual(correlate([1, 2, 3], [6, 5, 4]), -1)
        self.assertAlmostEqual(correlate([1, 2, 3], [1, 3, 2]), 0.5)
        self.assertAlmostEqual(correlate([1, 1, 2, 3], [1, 2, 2, 3]), 5 / 6)
        self.assertAlmostEqual(correlate([1, 2], [2, 1]), -1)

    def test_invalid(self):
        cases = [
            ([], []), ([1], [1]), ([1, 2], [1]),
            ([1, 2, 3], [7, 7, 7]),
            ([1, 2, 3], [1, float("nan"), 3]),
            ([1, 2, 3], [1, float("inf"), 3]),
            ([1, 2, 3], [1, 1_000_001, 3]),
            ([1, True], [1, 2]), ([1, 2j], [1, 2]),
            (list(range(129)), list(range(129))),
        ]
        for x, y in cases:
            with self.subTest(length=len(x)):
                with self.assertRaises(ValueError):
                    correlate(x, y)


if __name__ == "__main__":
    unittest.main()
