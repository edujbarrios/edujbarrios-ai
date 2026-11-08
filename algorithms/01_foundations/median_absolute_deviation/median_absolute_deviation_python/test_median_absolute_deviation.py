import math
import unittest
from median_absolute_deviation import median_absolute_deviation as mad


class MADTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(mad([1, 1, 2, 2, 4, 6, 9]), 1.0)
        self.assertEqual(mad([1, 2, 3, 10]), 1.0)
        self.assertEqual(mad([5, 5, 5]), 0.0)
        self.assertEqual(mad([42]), 0.0)
        self.assertEqual(mad([-3, -1, 2]), 2.0)
        self.assertEqual(mad((10, 3, 2, 1)), 1.0)
        self.assertEqual(mad([7] * 512), 0.0)
        self.assertEqual(mad([-1e308, 1e308]), 1e308)

    def test_invalid(self):
        for values in [
            [], [1] * 513, [1, math.inf], [math.nan],
            [-math.inf, 1], [1e308, -1e308, 1e308],
            [True, 2], [1, "2"], [1 + 0j], None,
            3, [10**400], {"x": 1},
        ]:
            with self.subTest(values=str(values)[:50]), self.assertRaises(ValueError):
                mad(values)


if __name__ == "__main__":
    unittest.main()
