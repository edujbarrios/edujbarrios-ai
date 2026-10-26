import unittest
from total_variation_distance import total_variation_distance as tv


class TotalVariationTests(unittest.TestCase):
    def test_reference_and_symmetry(self):
        self.assertEqual(tv([0.5, 0.5], [0.5, 0.5]), 0)
        self.assertEqual(tv([1, 0], [0, 1]), 1)
        self.assertAlmostEqual(tv([1, 0], [0.25, 0.75]), 0.75)
        self.assertAlmostEqual(tv([0.25, 0.75], [1, 0]), 0.75)
        self.assertEqual(tv([1], [1]), 0)
        self.assertAlmostEqual(tv([0.1, 0.2, 0.7], [0.2, 0.3, 0.5]), 0.2)

    def test_invalid(self):
        for a, b in (([], []), ([1], [1, 0]),
                     ([0.4, 0.4], [0.5, 0.5]),
                     ([1.1, -0.1], [1, 0]),
                     ([float("nan")], [1]), ([float("inf")], [1]),
                     ([True], [1]), (["1"], [1]), (None, [1])):
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                tv(a, b)


if __name__ == "__main__":
    unittest.main()
