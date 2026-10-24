import unittest
from hinge_loss import hinge_loss

class HingeLossTests(unittest.TestCase):
    def test_example(self):
        value,grad=hinge_loss([2,0,-.5,1],[1,1,-1,-1])
        self.assertAlmostEqual(value,.875)
        self.assertEqual(grad,[0,-.25,.25,.25])
        self.assertEqual(hinge_loss([1,-1],[1,-1]),(0,[0,0]))
        self.assertEqual(hinge_loss([0],[-1]),(1,[1.0]))
    def test_invalid(self):
        cases=(([],[]),([0],[]),([0],[0]),([0],[True]),
               ([True],[1]),([float("nan")],[1]),
               ([-1e308,-1e308],[1,1]))
        for s,y in cases:
            with self.subTest(scores=s,labels=y),self.assertRaises(ValueError):
                hinge_loss(s,y)

if __name__=="__main__":unittest.main()
