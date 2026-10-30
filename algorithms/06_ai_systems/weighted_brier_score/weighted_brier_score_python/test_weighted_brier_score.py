import math
import unittest
from weighted_brier_score import weighted_brier_score as brier


class WeightedBrierTests(unittest.TestCase):
    def test_fixtures(self):
        self.assertEqual(brier([0,1],[0.5,0],[1,3]),0.8125)
        self.assertEqual(brier([0,1],[0,1],[1,3]),0.0)
        self.assertEqual(brier([0,1],[0.5,0],[0,1]),1.0)
        self.assertEqual(brier([0],[0.5],[1]),0.25)

    def test_invalid(self):
        cases = [([],[],[]),([0],[0.5],[]),([0],[0.5],[0]),
                 ([2],[0.5],[1]),([0],[1.1],[1]),([0],[0.5],[-1]),
                 ([0],[0.5],[math.nan]),([True],[0.5],[1]),
                 ([0],[False],[1]),([0],[0.5],[True]),([0],[0.5],[1_000_001])]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                brier(*args)


if __name__ == "__main__":
    unittest.main()
