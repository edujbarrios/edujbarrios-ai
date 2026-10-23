import unittest

from cosine_similarity import cosine_similarity


class CosineSimilarityTests(unittest.TestCase):
    def test_parallel_and_opposite(self):
        self.assertAlmostEqual(cosine_similarity([1, 2], [2, 4]), 1.0)
        self.assertAlmostEqual(cosine_similarity([1, 2], [-1, -2]), -1.0)

    def test_orthogonal(self):
        self.assertAlmostEqual(cosine_similarity([1, 2], [-2, 1]), 0.0)

    def test_extreme_magnitudes(self):
        self.assertAlmostEqual(cosine_similarity([1e308, 1e308], [1e-308, 1e-308]), 1.0)

    def test_invalid_values(self):
        for left, right in (([], []), ([1.0], []), ([0, 0], [1, 2]),
                            ([float("nan")], [1]), ([1], [float("inf")]),
                            ([True], [1]), (["1"], [1])):
            with self.subTest(left=left, right=right), self.assertRaises(ValueError):
                cosine_similarity(left, right)


if __name__ == "__main__":
    unittest.main()
