import unittest
from momentum_sgd import momentum_sgd as step

class MomentumTests(unittest.TestCase):
    def test_sequential_steps(self):
        theta,v=step([2,-3],[1,-2],[0,0],.1)
        self.assertAlmostEqual(theta[0],1.9)
        self.assertAlmostEqual(theta[1],-2.8)
        self.assertEqual(v,[1,-2])
        theta,v=step(theta,[0,0],v,.1)
        self.assertAlmostEqual(theta[0],1.81)
        self.assertAlmostEqual(theta[1],-2.62)
        self.assertAlmostEqual(v[0],.9)
        self.assertAlmostEqual(v[1],-1.8)
        self.assertEqual(step([1],[0],[0],.1),([1.0],[0.0]))
    def test_invalid(self):
        cases=(([],[],[],.1),([1],[0],[],.1),
               ([1],[0],[0],0),([1],[0],[0],-1),
               ([1],[float("nan")],[0],.1),
               ([1e308],[1e308],[0],1e308),
               ([True],[0],[0],.1),([1],[0],[0],True))
        for case in cases:
            with self.subTest(args=case),self.assertRaises(ValueError):
                step(*case)

if __name__=="__main__":unittest.main()
