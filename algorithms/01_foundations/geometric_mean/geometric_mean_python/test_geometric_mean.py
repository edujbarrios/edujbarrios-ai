import math
import unittest
from geometric_mean import geometric_mean as gm


class GeometricMeanTests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(gm([2, 8]), 4)
        self.assertAlmostEqual(gm([1, 1, 1]), 1)
        self.assertEqual(gm([0, 2, 8]), 0)
        self.assertAlmostEqual(gm([1e-300, 1e300]), 1, places=12)
        self.assertAlmostEqual(gm((2, 4, 8)), 4)
        self.assertAlmostEqual(gm([0.5, 2]), 1)
        self.assertEqual(gm([0]), 0)
        self.assertAlmostEqual(gm([42]), 42)
        self.assertAlmostEqual(gm([7] * 512), 7, places=11)
        self.assertEqual(gm([0, 1e308]), 0)

    def test_invalid(self):
        for values in [
            [], [1] * 513, [-1], [-0.01, 1], [math.nan], [math.inf],
            [1, -math.inf], [1, "2"], [True], [1 + 0j], None, "123",
            [10**400], [1, None], 123,
        ]:
            with self.subTest(values=str(values)[:45]):
                with self.assertRaises(ValueError):
                    gm(values)


if __name__ == "__main__":
    unittest.main()
