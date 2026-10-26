import unittest
from dice_similarity import dice_similarity


class DiceSimilarityTests(unittest.TestCase):
    def test_sets_and_boundaries(self):
        self.assertAlmostEqual(dice_similarity([1, 1, 2, 3], [2, 3, 4]), 2 / 3)
        self.assertEqual(dice_similarity([], []), 1.0)
        self.assertEqual(dice_similarity([], [7]), 0.0)
        self.assertEqual(dice_similarity([7], []), 0.0)
        self.assertEqual(dice_similarity([-5, 3], [3, -5]), 1.0)
        self.assertEqual(dice_similarity([1], [2]), 0.0)
        self.assertEqual(dice_similarity([-(2**31)], [2**31 - 1]), 0.0)
        self.assertEqual(dice_similarity([-(2**31)], [-(2**31)]), 1.0)

    def test_invalid(self):
        for pair in (([True], []), ([1.0], []), ([2**31], []),
                     ([-(2**31) - 1], []), (["1"], []),
                     ([], [None]), (None, [])):
            with self.subTest(pair=pair), self.assertRaises(ValueError):
                dice_similarity(*pair)


if __name__ == "__main__":
    unittest.main()
