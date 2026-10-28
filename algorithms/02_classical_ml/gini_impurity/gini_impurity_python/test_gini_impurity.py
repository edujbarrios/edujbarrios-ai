import unittest
from gini_impurity import gini_impurity


class GiniImpurityTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(gini_impurity([5, 0]), 0.0)
        self.assertEqual(gini_impurity([1, 1]), 0.5)
        self.assertEqual(gini_impurity([3, 1]), 0.375)
        self.assertAlmostEqual(gini_impurity([1, 1, 1]), 2/3)
        self.assertEqual(gini_impurity([1000000, 1000000]), 0.5)
        self.assertEqual(gini_impurity([8]), 0.0)

    def test_invalid(self):
        cases = ([], [0], [0, 0], [-1, 3], [1.5, 2], [True, 1],
                 [float("nan"), 1], [float("inf"), 1],
                 ["2", 3], [0, 1000001], [1+0j, 2], [1]*129)
        for counts in cases:
            with self.subTest(counts=str(counts)[:35]):
                with self.assertRaises(ValueError):
                    gini_impurity(counts)


if __name__ == "__main__":
    unittest.main()
