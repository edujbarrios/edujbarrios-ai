import math
import unittest
from swish import swish

class SwishTests(unittest.TestCase):
    def test_known_and_extreme(self):
        values,derivatives=swish([0,1,-1,1000,-1000])
        s=1/(1+math.exp(-1))
        self.assertEqual((values[0],derivatives[0]),(0,0.5))
        self.assertAlmostEqual(values[1],s)
        self.assertAlmostEqual(derivatives[1],s+s*(1-s))
        self.assertAlmostEqual(values[2],s-1)
        self.assertAlmostEqual(derivatives[2],(1-s)*(1-s+s))
        self.assertEqual((values[3],derivatives[3]),(1000,1))
        self.assertEqual((values[4],derivatives[4]),(0,0))
    def test_invalid(self):
        for values in ([],[float("nan")],[float("inf")],[True],[1,"bad"]):
            with self.subTest(values=values),self.assertRaises(ValueError):
                swish(values)

if __name__=="__main__":unittest.main()
