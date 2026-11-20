import math
import unittest
from precision_recall_curve import precision_recall_curve


class PrecisionRecallCurveTests(unittest.TestCase):
    def test_thresholds_and_ties(self):
        actual = precision_recall_curve([0.8, 0.8, 0.5, 0.2], [1, 0, 1, 0])
        expected = [(0.8, 0.5, 0.5), (0.5, 2 / 3, 1.0), (0.2, 0.5, 1.0)]
        self.assertEqual(len(actual), 3)
        for point, want in zip(actual, expected):
            for got, target in zip(point, want):
                self.assertTrue(math.isclose(got, target, rel_tol=1e-12))
        self.assertEqual(precision_recall_curve([0.4, 0.4], [1, 1]), [(0.4, 1.0, 1.0)])
        self.assertEqual(precision_recall_curve([1], [1]), [(1.0, 1.0, 1.0)])
        self.assertEqual(precision_recall_curve([0, 1], [1, 0]),
                         [(1.0, 0.0, 0.0), (0.0, 0.5, 1.0)])

    def test_invalid(self):
        examples = [
            ([], []), ([0.2], []), ([0.3], [0]), ([float("nan")], [1]),
            ([float("inf")], [1]), ([-0.1], [1]), ([1.1], [1]),
            ([0.5], [2]), ([0.5], [True]), ([True], [1]),
            ([0.5], [0.0]), (["0.5"], [1]),
        ]
        for scores, labels in examples:
            with self.subTest(scores=scores, labels=labels), self.assertRaises(ValueError):
                precision_recall_curve(scores, labels)


if __name__ == "__main__":
    unittest.main()
