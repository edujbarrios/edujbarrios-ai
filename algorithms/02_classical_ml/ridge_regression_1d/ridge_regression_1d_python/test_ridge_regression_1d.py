import unittest
from ridge_regression_1d import ridge_regression_1d as fit

class RidgeTests(unittest.TestCase):
    def test_fits(self):
        a,b=fit([0,1,2],[1,3,5],0)
        self.assertAlmostEqual(a,1)
        self.assertAlmostEqual(b,2)
        a,b=fit([0,1,2],[1,3,5],2)
        self.assertAlmostEqual(a,2)
        self.assertAlmostEqual(b,1)
        self.assertAlmostEqual(a+3*b,5)
        self.assertEqual(fit([2,2,2],[1,2,3],0),(2.0,0.0))
        self.assertEqual(fit([0],[1],0),(1.0,0.0))
    def test_invalid(self):
        for args in (([],[],0),([0,1],[1],0),([1],[1],-1),
                     ([1],[1],float("inf")),([float("nan")],[1],0),
                     ([1000001],[1],0),([True],[1],0),([1],[1],True)):
            with self.subTest(args=args),self.assertRaises(ValueError):fit(*args)

if __name__=="__main__":unittest.main()
