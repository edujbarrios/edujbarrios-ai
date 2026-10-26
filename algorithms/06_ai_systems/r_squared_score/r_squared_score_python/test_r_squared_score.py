import unittest
from r_squared_score import r_squared_score as r2


class RSquaredTests(unittest.TestCase):
    def test_reference(self):
        self.assertAlmostEqual(r2([1, 2, 3], [1, 2, 3]), 1)
        self.assertAlmostEqual(r2([1, 2, 3], [1, 2, 2]), 0.5)
        self.assertAlmostEqual(r2([1, 2, 3], [0, 0, 0]), -6)
        self.assertEqual(r2([4, 4, 4], [4, 4, 4]), 1)
        self.assertEqual(r2([4, 4, 4], [4, 4, 0]), 0)
        self.assertAlmostEqual(r2([1e308, -1e308], [0, 0]), 0)
        self.assertEqual(r2([1e308, -1e308], [1e308, -1e308]), 1)

    def test_invalid(self):
        for a, b in (([], []), ([1], [1]), ([1, 2], [1]),
                     ([True, 1], [1, 1]), (["1", 2], [1, 2]),
                     ([float("nan"), 2], [1, 2]),
                     ([float("inf"), 2], [1, 2]), (None, [1, 2])):
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                r2(a, b)


if __name__ == "__main__":
    unittest.main()
