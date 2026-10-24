import math
import unittest
from rmsprop_optimizer import rmsprop_optimizer as step

class RMSPropTests(unittest.TestCase):
    def test_stateful_steps(self):
        theta,v=step([2,-3],[1,-2],[0,0],.1)
        self.assertAlmostEqual(v[0],.1)
        self.assertAlmostEqual(v[1],.4)
        self.assertAlmostEqual(theta[0],2-.1/(math.sqrt(.1)+1e-8))
        self.assertAlmostEqual(theta[1],-3+.2/(math.sqrt(.4)+1e-8))
        theta2,v2=step(theta,[0,0],v,.1)
        self.assertEqual(theta2,theta)
        self.assertAlmostEqual(v2[0],.09)
        self.assertAlmostEqual(v2[1],.36)
        self.assertEqual(step([1],[0],[0],.1),([1.0],[0.0]))
    def test_invalid(self):
        cases=(([],[],[],.1),([1],[1],[],.1),
               ([1],[1],[-1],.1),([1],[1],[0],0),
               ([1],[float("nan")],[0],.1),([1e308],[1e308],[0],.1),
               ([True],[0],[0],.1),([1],[0],[0],True))
        for case in cases:
            with self.subTest(args=case),self.assertRaises(ValueError):
                step(*case)

if __name__=="__main__":unittest.main()
