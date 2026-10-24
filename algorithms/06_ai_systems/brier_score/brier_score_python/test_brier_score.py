import unittest
from brier_score import brier_score

class BrierTests(unittest.TestCase):
    def test_values(self):
        self.assertAlmostEqual(brier_score([.1,.8,.6,.2],[0,1,1,0]),.0625)
        self.assertEqual(brier_score([0,1],[0,1]),0)
        self.assertEqual(brier_score([1,0],[0,1]),1)
        self.assertEqual(brier_score([.5,.5],[0,1]),.25)
        self.assertEqual(brier_score([.5],[0]),.25)
    def test_invalid(self):
        cases=(([],[]),([.2],[]),([-.1,.5],[0,1]),
               ([1.1,.5],[0,1]),([float("nan"),.5],[0,1]),
               ([0,1],[0,2]),([True,.5],[0,1]),([0,.5],[False,1]))
        for p,y in cases:
            with self.subTest(p=p,y=y),self.assertRaises(ValueError):
                brier_score(p,y)

if __name__=="__main__":unittest.main()
