import math
import unittest
from gradient_clip_global_norm import gradient_clip_global_norm as clip


class GradientClipTests(unittest.TestCase):
    def test_clip(self):
        for x, expected in zip(clip([3, 4], 1), [0.6, 0.8]):
            self.assertAlmostEqual(x, expected)
        self.assertEqual(clip([-3, 4], 10), [-3.0, 4.0])

    def test_zero_extreme(self):
        self.assertEqual(clip([0, 0], 1), [0.0, 0.0])
        result = clip([1e308, -1e308], 1)
        self.assertTrue(all(math.isfinite(x) for x in result))
        self.assertAlmostEqual(result[0], math.sqrt(0.5))
        self.assertAlmostEqual(result[1], -math.sqrt(0.5))

    def test_invalid(self):
        for args in (([], 1), ([1], 0), ([1], -1), ([1], math.nan),
                     ([math.inf], 1), ([True], 1), (["1"], 1),
                     ([10**400], 1), ([1], 10**400)):
            with self.subTest(args=args), self.assertRaises(ValueError):
                clip(*args)


if __name__ == "__main__":
    unittest.main()
