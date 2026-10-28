import math
import unittest
from log_cosh_loss import log_cosh_loss as loss


class LogCoshLossTests(unittest.TestCase):
    def test_reference(self):
        self.assertAlmostEqual(loss([0,1,2], [0,2,0]), 0.5862611926136305)
        self.assertAlmostEqual(loss([0,1,2], [0,1,2]), 0)
        self.assertAlmostEqual(loss([1000000], [-1000000]), 2000000-math.log(2), places=8)
        self.assertAlmostEqual(loss([1], [2]), math.log(math.cosh(1)))

    def test_invalid(self):
        cases = [
            ([], []), ([1,2], [1]), ([1], [float("nan")]),
            ([1], [float("inf")]), ([1], [1000001]),
            ([True], [1]), (["1"],[1]), ([1+0j],[1]),
            (list(range(10001)),list(range(10001)))
        ]
        for actual,pred in cases:
            with self.subTest(n=len(actual)):
                with self.assertRaises(ValueError):
                    loss(actual,pred)


if __name__ == "__main__":
    unittest.main()
