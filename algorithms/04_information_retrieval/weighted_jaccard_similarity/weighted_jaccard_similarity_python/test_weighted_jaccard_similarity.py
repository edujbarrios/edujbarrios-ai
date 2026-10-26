import unittest
from weighted_jaccard_similarity import weighted_jaccard_similarity as wj


class WeightedJaccardTests(unittest.TestCase):
    def test_fixtures(self):
        self.assertAlmostEqual(wj([1, 2, 0], [1, 1, 3]), 1 / 3)
        self.assertEqual(wj([0, 0], [0, 0]), 1)
        self.assertEqual(wj([0, 1], [1, 0]), 0)
        self.assertEqual(wj([1, 2], [1, 2]), 1)
        self.assertAlmostEqual(wj([0.2, 0.8], [0.5, 0.5]), 7 / 13)
        self.assertEqual(wj([1e308, 1e308], [1e308, 0]), 0.5)
        self.assertEqual(wj([1], [0]), 0)

    def test_invalid(self):
        for a, b in (([], []), ([1], [1, 2]), ([-1], [0]),
                     ([float("nan")], [1]), ([float("inf")], [1]),
                     ([True], [1]), (["1"], [1]), (None, [1]),
                     ([2**2000], [0])):
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                wj(a, b)


if __name__ == "__main__":
    unittest.main()
