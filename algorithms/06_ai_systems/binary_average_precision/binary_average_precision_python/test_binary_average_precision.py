import math
import unittest

from binary_average_precision import binary_average_precision as ap


class BinaryAveragePrecisionTests(unittest.TestCase):
    def test_known_ranking(self):
        labels, scores = [1, 0, 1, 0], [0.9, 0.8, 0.7, 0.1]
        self.assertAlmostEqual(ap(labels, scores), 5 / 6)
        self.assertEqual((labels, scores), ([1, 0, 1, 0],
                                           [0.9, 0.8, 0.7, 0.1]))
        self.assertEqual(ap([1], [0]), 1.0)
        self.assertEqual(ap([0], [0]), 0.0)
        self.assertEqual(ap([1, 1, 1], [3, 2, 1]), 1.0)

    def test_equal_scores_and_permutation_invariance(self):
        self.assertAlmostEqual(ap([1, 0, 1], [0.5, 0.5, 0.2]), 7 / 12)
        self.assertAlmostEqual(ap([0, 1, 1], [0.5, 0.5, 0.2]), 7 / 12)
        self.assertAlmostEqual(ap([1, 0, 0, 1], [3, 3, 3, 3]), 0.5)
        self.assertAlmostEqual(ap([0, 1, 1, 0], [3, 3, 3, 3]), 0.5)
        self.assertAlmostEqual(ap([0, 1], [0.8, 0.1]), 0.5)
        self.assertEqual(ap([1, 0], [1e6, -1e6]), 1.0)

    def test_invalid(self):
        for labels, scores in (
            ([], []), ([1], []), ([1, 0], [0.3]),
            ([True], [0.3]), ([2], [0.3]), (["1"], [0.3]),
            ([1], [True]), ([1], [math.nan]), ([1], [math.inf]),
            ([1], [1_000_001]), ([1], ["0.3"]), (None, [1]),
        ):
            with self.subTest(labels=labels, scores=scores):
                with self.assertRaises(ValueError):
                    ap(labels, scores)


if __name__ == "__main__":
    unittest.main()
