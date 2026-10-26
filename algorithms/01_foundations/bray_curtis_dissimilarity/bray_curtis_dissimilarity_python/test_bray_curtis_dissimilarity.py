import unittest
from bray_curtis_dissimilarity import bray_curtis_dissimilarity as bc


class BrayCurtisTests(unittest.TestCase):
    def test_reference(self):
        self.assertAlmostEqual(bc([1, 2], [2, 4]), 1 / 3)
        self.assertAlmostEqual(bc([2, 4], [1, 2]), 1 / 3)
        self.assertEqual(bc([0, 0], [0, 0]), 0)
        self.assertEqual(bc([0, 1], [1, 0]), 1)
        self.assertEqual(bc([1, 2], [1, 2]), 0)
        self.assertEqual(bc([0, 0], [0, 1]), 1)
        self.assertEqual(bc([1e308, 1e308], [1e308, 0]), 1 / 3)

    def test_invalid(self):
        for a, b in (([], []), ([1], [1, 2]), ([-1], [1]),
                     ([True], [1]), (["1"], [1]), ([float("nan")], [1]),
                     ([float("inf")], [1]), ([2**2000], [0]), (None, [0])):
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                bc(a, b)


if __name__ == "__main__":
    unittest.main()
