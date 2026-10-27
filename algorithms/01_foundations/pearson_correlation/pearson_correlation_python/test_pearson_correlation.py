import unittest

from pearson_correlation import pearson_correlation


class PearsonCorrelationTests(unittest.TestCase):
    def test_reference_cases(self):
        self.assertAlmostEqual(pearson_correlation([1, 2, 3], [2, 4, 6]), 1)
        self.assertAlmostEqual(pearson_correlation([1, 2, 3], [6, 4, 2]), -1)
        self.assertAlmostEqual(pearson_correlation([1, 2, 3], [1, 0, 1]), 0)
        self.assertAlmostEqual(pearson_correlation([1, 2], [3, 4]), 1)

    def test_invalid(self):
        cases = [
            ([], []), ([1], [2]), ([1, 2], [1]), ([1, 2], [4, 4]),
            ([1, 2], [1, float("nan")]), ([1, 2], [1, float("inf")]),
            ([1, 2], [1, 1000001]), ([True, 2], [1, 2]),
            (["1", 2], [1, 2]), ([1, 2], [1, 2j]),
            (list(range(10001)), list(range(10001))),
        ]
        for x, y in cases:
            with self.subTest(x=str(x)[:30], y=str(y)[:30]):
                with self.assertRaises(ValueError):
                    pearson_correlation(x, y)


if __name__ == "__main__":
    unittest.main()
