import math
import unittest
from trimmed_mean import trimmed_mean as tmean


class TrimmedMeanTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(tmean([1,2,3,4,100], 0.2), 3.0)
        self.assertEqual(tmean([1,2,3,100], 0.25), 2.5)
        self.assertEqual(tmean([1,2,3,100], 0), 26.5)
        self.assertEqual(tmean([100,3,2,1], 0.25), 2.5)
        self.assertEqual(tmean([42], 0.49), 42.0)
        self.assertEqual(tmean([1,2,3], 0.49), 2.0)
        self.assertAlmostEqual(tmean([7] * 512, 0.49), 7.0, places=12)
        self.assertEqual(tmean([-1e308, 1e308], 0), 0.0)
        self.assertEqual(tmean([0, 2, 100], 0.3), 34.0)

    def test_invalid(self):
        for values, fraction in [
            ([], 0), ([1] * 513, 0), ([1], -0.01),
            ([1], 0.5), ([1], 1), ([1], float("nan")),
            ([1], math.inf), ([1], None), ([1], True),
            ([math.inf], 0), ([math.nan], 0), ([True], 0),
            ([1, "2"], 0), (None, 0), ("123", 0), ([10**400], 0),
        ]:
            with self.subTest(values=str(values)[:30], fraction=fraction):
                with self.assertRaises(ValueError):
                    tmean(values, fraction)


if __name__ == "__main__":
    unittest.main()
