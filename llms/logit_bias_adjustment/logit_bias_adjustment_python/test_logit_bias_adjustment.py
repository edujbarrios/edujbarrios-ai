import math
import unittest
from logit_bias_adjustment import logit_bias_adjustment as adjust


class LogitBiasTests(unittest.TestCase):
    def test_example_and_noop(self):
        self.assertEqual(adjust([1,2,3],[0,2],[-1,0.5]),[0,2,3.5])
        self.assertEqual(adjust([-1,0],[],[]),[-1,0])
        self.assertEqual(adjust([1,2],[1],[0]),[1,2])
        self.assertEqual(adjust([-1000,1000],[0,1],[-100,100]),[-1100,1100])

    def test_invalid(self):
        cases = [([],[],[]),([1],[0,0],[1,1]),([1],[1],[1]),
                 ([1],[True],[1]),([1],[0],[math.nan]),
                 ([1],[0],[101]),([math.inf],[],[]),
                 ([1001],[],[]),([1],[0],[]),([1],[0],[True])]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                adjust(*args)


if __name__ == "__main__":
    unittest.main()
