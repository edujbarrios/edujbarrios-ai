import math
import unittest
from triplet_margin_loss import triplet_margin_loss as loss


class TripletMarginLossTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(loss([0,0], [1,0], [3,0], 1), 0)
        self.assertEqual(loss([0,0], [1,0], [3,0], 3), 1)
        self.assertEqual(loss([0,0], [3,4], [0,0], 1), 6)
        self.assertEqual(loss([0,0], [0,0], [0,0], 0.5), 0.5)
        self.assertEqual(loss([-2,-2], [-2,-2], [-2,-2], 0), 0)
        self.assertEqual(loss([0]*512, [0]*512, [1]*512, 0), 0)
        self.assertAlmostEqual(loss([0]*512, [1]*512, [0]*512, 0), math.sqrt(512))
        self.assertEqual(loss([1], [2], [-2], 0), 0)

    def test_invalid(self):
        cases = [
            ([], [], [], 1), ([0], [], [1], 1),
            ([0], [1], [1,2], 1), ([0]*513, [0]*513, [0]*513, 0),
            ([0], [0], [1], -1), ([0], [0], [1], 1001),
            ([0], [0], [1], float("inf")), ([0], [0], [1], True),
            ([0], [float("nan")], [1], 1),
            ([0], [1_000_001], [1], 1), ([0], [True], [1], 1),
            (None, [0], [1], 1),
        ]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                loss(*args)


if __name__ == "__main__":
    unittest.main()
