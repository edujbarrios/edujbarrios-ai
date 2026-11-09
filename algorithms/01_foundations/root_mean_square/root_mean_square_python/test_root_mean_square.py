import math
import unittest
from root_mean_square import root_mean_square as rms


class RootMeanSquareTests(unittest.TestCase):
    def test_examples(self):
        self.assertTrue(math.isclose(rms([3, 4]), math.sqrt(12.5), rel_tol=1e-12))
        self.assertTrue(math.isclose(rms([-3, 4]), math.sqrt(12.5), rel_tol=1e-12))
        self.assertEqual(rms([0, 0, 0]), 0.0)
        self.assertEqual(rms([0]), 0.0)
        self.assertEqual(rms([42]), 42.0)
        self.assertEqual(rms([-42]), 42.0)
        self.assertTrue(math.isclose(rms([1e308, -1e308]), 1e308, rel_tol=1e-12))
        self.assertTrue(math.isclose(rms([1e-300, -1e-300]), 1e-300, rel_tol=1e-12))
        self.assertTrue(math.isclose(rms([3] * 512), 3.0, rel_tol=1e-12))
        self.assertTrue(math.isclose(rms((1, 2, 2)), math.sqrt(3), rel_tol=1e-12))

    def test_invalid(self):
        for values in [[], [1]*513, [math.nan], [math.inf], [-math.inf],
                       [True], [1, "2"], [1+0j], None, "123", [10**400]]:
            with self.subTest(values=str(values)[:30]), self.assertRaises(ValueError):
                rms(values)


if __name__ == "__main__":
    unittest.main()
