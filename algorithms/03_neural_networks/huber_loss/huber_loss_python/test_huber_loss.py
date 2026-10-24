import math
import unittest
from huber_loss import huber_loss

class HuberTests(unittest.TestCase):
    def test_example_and_threshold(self):
        value,grad=huber_loss([2,0,-2],[0,1,0],1)
        self.assertAlmostEqual(value,7/6)
        for got,want in zip(grad,[1/3,-1/3,-1/3]):
            self.assertAlmostEqual(got,want)
        self.assertEqual(huber_loss([1,-1],[0,0],1),(.5,[.5,-.5]))
        self.assertEqual(huber_loss([0,0],[0,0],1),(0,[0,0]))
        self.assertEqual(huber_loss([.5],[0],1),(.125,[.5]))
    def test_invalid(self):
        cases=(([],[],1),([1],[],1),([1],[0],0),
               ([1],[0],float("nan")),([math.nan],[0],1),
               ([True],[0],1),([1.7e308],[-1.7e308],1),
               ([1e308],[0],1e308))
        for case in cases:
            with self.subTest(args=case),self.assertRaises(ValueError):
                huber_loss(*case)

if __name__=="__main__":unittest.main()
