import math
import unittest
from binary_focal_loss import binary_focal_loss

class FocalTests(unittest.TestCase):
    def test_known_values(self):
        self.assertAlmostEqual(binary_focal_loss([.5,.5],[1,0]),math.log(2)/8)
        self.assertEqual(binary_focal_loss([0,1],[0,1]),0)
        self.assertAlmostEqual(binary_focal_loss([0,1],[1,0]),math.log(1e12)/2)
        self.assertAlmostEqual(binary_focal_loss([.5],[1]),math.log(2)/16)
        self.assertGreater(binary_focal_loss([.1],[1]),
                           binary_focal_loss([.9],[1]))
    def test_invalid(self):
        cases=(([],[]),([.5],[]),([1.2,.5],[1,0]),
               ([-.1,.5],[0,1]),([float("nan")],[0]),
               ([.5],[2]),([True],[1]),([.5],[True]))
        for p,y in cases:
            with self.subTest(p=p,y=y),self.assertRaises(ValueError):
                binary_focal_loss(p,y)

if __name__=="__main__":unittest.main()
