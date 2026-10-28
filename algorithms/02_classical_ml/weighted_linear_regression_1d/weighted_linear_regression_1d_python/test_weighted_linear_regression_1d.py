import unittest
from weighted_linear_regression_1d import weighted_linear_regression_1d as fit


class WeightedLinearRegressionTests(unittest.TestCase):
    def test_fits(self):
        slope,intercept=fit([0,1,2],[1,2,2],[1,1,2])
        self.assertAlmostEqual(slope,5/11)
        self.assertAlmostEqual(intercept,13/11)
        slope,intercept=fit([0,1,2],[1,3,5],[1,1,1])
        self.assertAlmostEqual(slope,2)
        self.assertAlmostEqual(intercept,1)
        slope,intercept=fit([0,1],[1,3],[1,1])
        self.assertAlmostEqual(slope,2)
        self.assertAlmostEqual(intercept,1)

    def test_invalid(self):
        cases=[
            ([],[],[]),([0],[1],[1]),([0,1],[1],[1,1]),
            ([0,1],[1,2],[1]),([0,1],[1,2],[0,0]),
            ([0,1],[1,2],[1,0]),([2,2],[1,2],[1,1]),
            ([0,1],[1,2],[-1,1]),([0,1],[1,2],[1,1000001]),
            ([0,1],[1,2],[1,float('nan')]),
            ([0,float('inf')],[1,2],[1,1]),
            ([0,True],[1,2],[1,1]),([0,1],[1,'2'],[1,1]),
            (list(range(129)),list(range(129)),[1]*129),
        ]
        for x,y,w in cases:
            with self.subTest(n=len(x)):
                with self.assertRaises(ValueError):
                    fit(x,y,w)


if __name__=="__main__":
    unittest.main()
