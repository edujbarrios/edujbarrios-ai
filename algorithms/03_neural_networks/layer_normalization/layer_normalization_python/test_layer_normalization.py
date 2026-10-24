import math
import unittest
from layer_normalization import layer_normalization as norm

class LayerNormTests(unittest.TestCase):
    def test_standard_and_affine(self):
        inv=1/math.sqrt(2/3+1e-5)
        result=norm([1,2,3],[1,1,1],[0,0,0],1e-5)
        for a,b in zip(result,[-inv,0,inv]):
            self.assertAlmostEqual(a,b)
        result=norm([1,2,3],[2,1,0.5],[1,2,3],1e-5)
        for a,b in zip(result,[1-2*inv,2,3+0.5*inv]):
            self.assertAlmostEqual(a,b)
    def test_constant_and_one(self):
        self.assertEqual(norm([7,7],[1,1],[4,5],1e-5),[4.0,5.0])
        self.assertEqual(norm([10],[1],[4],1e-5),[4.0])
    def test_invalid(self):
        cases=(([],[],[],1e-5),([1,2],[1],[0,0],1e-5),
               ([1,2],[1,1],[0],1e-5),
               ([1],[1],[0],0.0),([1],[1],[0],float("inf")),
               ([1,float("nan")],[1,1],[0,0],1e-5),
               ([1.7e308,-1.7e308],[1,1],[0,0],1e-5),
               ([True],[1],[0],1e-5))
        for args in cases:
            with self.subTest(args=args),self.assertRaises(ValueError):
                norm(*args)

if __name__=="__main__": unittest.main()
