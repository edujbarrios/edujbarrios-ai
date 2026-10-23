import unittest
import math
from binary_cross_entropy import binary_cross_entropy as bce
class Tests(unittest.TestCase):
    def test_values(self):
        self.assertEqual(bce([1,0],[1,0]),0)
        self.assertAlmostEqual(bce([1,0],[.5,.5]),math.log(2))
    def test_invalid(self):
        for y,p in [([],[]),([1],[0]),([0],[1]),([2],[.5]),([1],[float("nan")]),([1,0],[.5])]:
            with self.assertRaises(ValueError): bce(y,p)
if __name__=="__main__": unittest.main()
