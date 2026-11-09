import math
import unittest
from harmonic_mean import harmonic_mean as mean


class HarmonicMeanTests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(mean([2, 6]), 3.0)
        self.assertAlmostEqual(mean([1, 2, 4]), 12 / 7)
        self.assertEqual(mean([0, 2, 6]), 0.0)
        self.assertAlmostEqual(mean((1, 1, 1)), 1.0)
        self.assertAlmostEqual(mean([42]), 42.0)
        self.assertAlmostEqual(mean([7] * 512), 7.0)
        self.assertEqual(mean([0, 1e308]), 0.0)
        self.assertTrue(math.isclose(mean([1e-300, 1e300]), 2e-300, rel_tol=1e-12))
        self.assertTrue(math.isclose(mean([1e-320, 1e-320]), 1e-320, rel_tol=1e-12))
        self.assertTrue(math.isclose(mean([1e308, 1e308]), 1e308, rel_tol=1e-12))

    def test_invalid(self):
        for values in [
            [], [1] * 513, [-1], [math.nan], [math.inf], [1, -math.inf],
            [True], ["2"], [1, None], [complex(1, 0)], None, "123", [10**400],
        ]:
            with self.subTest(values=str(values)[:40]), self.assertRaises(ValueError):
                mean(values)


if __name__ == "__main__":
    unittest.main()
