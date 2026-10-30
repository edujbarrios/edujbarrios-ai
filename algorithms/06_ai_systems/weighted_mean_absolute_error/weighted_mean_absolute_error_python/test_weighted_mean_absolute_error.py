import math
import unittest
from weighted_mean_absolute_error import weighted_mean_absolute_error as wmae

class WeightedMAETests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(wmae([1,3],[2,1],[1,3]),1.75)
        self.assertEqual(wmae([1,3],[1,3],[1,3]),0.0)
        self.assertEqual(wmae([1,3],[2,1],[0,2]),2.0)
        self.assertEqual(wmae([1],[2],[1]),1.0)

    def test_invalid(self):
        cases = [([],[],[]),([1],[2],[]),([1],[2],[0]),
                 ([1],[2],[-1]),([math.nan],[2],[1]),
                 ([1],[math.inf],[1]),([1],[2],[math.inf]),
                 ([True],[2],[1]),([1],[2],[True]),
                 ([1000001],[2],[1]),([1],[2],[1000001]),
                 ([1],[2],["1"])]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                wmae(*args)
if __name__ == "__main__":
    unittest.main()
