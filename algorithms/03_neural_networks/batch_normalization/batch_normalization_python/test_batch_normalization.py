import math
import unittest
from batch_normalization import batch_normalization as bn

class BatchNormTests(unittest.TestCase):
    def test_normalization(self):
        x=bn([[1,10],[3,10]],[1,2],[0,-1])
        s=1/math.sqrt(1+1e-5)
        self.assertAlmostEqual(x[0][0],-s)
        self.assertAlmostEqual(x[1][0],s)
        self.assertEqual([x[0][1],x[1][1]],[-1,-1])
        self.assertEqual(bn([[7,4]],[1,2],[0,-1]),[[0,-1]])
        negative=bn([[1,10],[3,10]],[-1,0],[0,-1])
        self.assertAlmostEqual(negative[0][0],s)
    def test_invalid(self):
        cases=(([],[1],[0]),([[1,2]],[1],[0]),
               ([[1],[2,3]],[1],[0]),
               ([[1],[math.nan]],[1],[0]),([[True]],[1],[0]),
               ([[1000001]],[1],[0]),([[1]],[1],[math.inf]))
        for args in cases:
            with self.subTest(args=args),self.assertRaises(ValueError):bn(*args)

if __name__=="__main__":unittest.main()
