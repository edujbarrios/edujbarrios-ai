import math
import unittest
from matthews_correlation_coefficient import matthews_correlation_coefficient as mcc

class MCCTests(unittest.TestCase):
    def test_values(self):
        y=[1,1,0,0]
        self.assertEqual(mcc(y,y),1)
        self.assertEqual(mcc(y,[0,0,1,1]),-1)
        self.assertEqual(mcc(y,[1,0,1,0]),0)
        self.assertEqual(mcc(y,[0,0,0,0]),0)
        self.assertAlmostEqual(mcc([1,1,1,0,0],[1,1,0,1,0]),
                               1/6)
    def test_invalid(self):
        cases=(([],[]),([0],[]),([0,2],[0,1]),
               ([0,1],[0,3]),([True,0],[0,1]),([0,1],[0,False]))
        for a,b in cases:
            with self.subTest(a=a,b=b),self.assertRaises(ValueError):mcc(a,b)

if __name__=="__main__":unittest.main()
