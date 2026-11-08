import math
import unittest
from weighted_median import weighted_median as wmedian


class WeightedMedianTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(wmedian([1, 2, 10], [1, 1, 8]), 10.0)
        self.assertEqual(wmedian([1, 2, 3, 4], [1, 1, 1, 1]), 2.0)
        self.assertEqual(wmedian([10, 1, 3], [0, 2, 1]), 1.0)
        self.assertEqual(wmedian([5, 5, 9], [1, 1, 1]), 5.0)
        self.assertEqual(wmedian([1, 100], [0, 2]), 100.0)
        self.assertEqual(wmedian([-5, 2], [1, 1]), -5.0)
        self.assertEqual(wmedian([42], [0.01]), 42.0)
        self.assertEqual(wmedian([3] * 512, [1] * 512), 3.0)
        self.assertEqual(wmedian([-1e308, 1e308], [1, 1]), -1e308)
        self.assertEqual(wmedian([1, 2], [0.0, 5e-324]), 2.0)

    def test_invalid(self):
        cases = [
            ([], []), ([1], []), ([1], [0]),
            ([1, 2], [0, 0]), ([1, 2], [-1, 2]),
            ([1], [math.inf]), ([math.nan], [1]),
            ([math.inf], [1]), ([1], [math.nan]),
            ([1, 2], [1e308, 1e308]), ([1] * 513, [1] * 513),
            ([True], [1]), ([1], [True]), ([1], ["1"]),
            (None, [1]), ([1], None), ([10**400], [1]),
        ]
        for values, weights in cases:
            with self.subTest(values=str(values)[:40], weights=str(weights)[:40]):
                with self.assertRaises(ValueError):
                    wmedian(values, weights)


if __name__ == "__main__":
    unittest.main()
