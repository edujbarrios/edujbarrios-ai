import unittest

from balanced_accuracy import balanced_accuracy


class BalancedAccuracyTests(unittest.TestCase):
    def test_scores(self):
        self.assertAlmostEqual(
            balanced_accuracy([0, 0, 0, 1, 1], [0, 0, 1, 1, 0]), 7 / 12
        )
        self.assertEqual(balanced_accuracy([0, 1], [0, 1]), 1.0)
        self.assertEqual(balanced_accuracy([0, 1], [1, 0]), 0.0)
        self.assertEqual(balanced_accuracy([0, 0, 1], [0, 0, 0]), 0.5)

    def test_invalid(self):
        for truth, pred in [
            ([], []), ([0], []), ([0, 0], [0, 1]),
            ([1, 1], [1, 1]), ([0, 1], [0, 2]),
            ([0, 1], [True, 1]), ([0.0, 1], [0, 1]),
        ]:
            with self.subTest(truth=truth, pred=pred), self.assertRaises(ValueError):
                balanced_accuracy(truth, pred)


if __name__ == "__main__":
    unittest.main()
