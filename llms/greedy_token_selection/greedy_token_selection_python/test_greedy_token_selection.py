import math
import unittest

from greedy_token_selection import greedy_token_selection as greedy


class GreedySelectionTests(unittest.TestCase):
    def test_ties_and_negatives(self):
        self.assertEqual(greedy([2, 5, 5, -1]), 1)
        self.assertEqual(greedy([-4, -2, -3]), 1)
        self.assertEqual(greedy([1, 1, 1]), 0)

    def test_boundaries(self):
        self.assertEqual(greedy([-1e308]), 0)
        self.assertEqual(greedy([-1e308, 1e308]), 1)

    def test_invalid(self):
        for value in ([], [1, math.nan], [math.inf], [-math.inf],
                      [True], ["1"], [10**400]):
            with self.subTest(value=value), self.assertRaises(ValueError):
                greedy(value)


if __name__ == "__main__":
    unittest.main()
