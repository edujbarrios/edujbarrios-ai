import math
import unittest
from warmup_cosine_learning_rate import warmup_cosine_learning_rate as rate

class WarmupCosineTests(unittest.TestCase):
    def test_curve(self):
        expected = [0,0.5,1,0.8535533905932737,0.5,0.1464466094067262,0]
        for step, value in enumerate(expected):
            with self.subTest(step=step):
                self.assertAlmostEqual(rate(1,0,2,6,step),value)

    def test_constant_and_short(self):
        self.assertEqual(rate(2,2,1,2,1),2)
        self.assertEqual(rate(2,2,1,2,2),2)
        self.assertEqual(rate(0.5,0.2,1,2,0),0)

    def test_invalid(self):
        args = [(1,0,0,6,1),(1,0,6,6,1),(1,0,2,6,7),
                (0,1,2,6,1),(math.inf,0,2,6,1),
                (1,0,2,100001,1),(1,0,2,6,True),
                (1,0,2.0,6,1),(1,0,2,6,-1)]
        for case in args:
            with self.subTest(case=case), self.assertRaises(ValueError):
                rate(*case)

if __name__ == "__main__":
    unittest.main()
