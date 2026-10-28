import math
import unittest
from cross_modal_attention import cross_modal_attention as attend


class CrossAttentionTests(unittest.TestCase):
    def test_reference(self):
        q=[1,0]; keys=[1,0,0,1]; values=[10,0,0,20]
        weights,context=attend(q,keys,values,2,2)
        expected=math.exp(1/math.sqrt(2))/(1+math.exp(1/math.sqrt(2)))
        self.assertAlmostEqual(weights[0],expected)
        self.assertAlmostEqual(weights[1],1-expected)
        self.assertAlmostEqual(context[0],10*expected)
        self.assertAlmostEqual(context[1],20*(1-expected))
        self.assertEqual(attend([0,0],keys,values,2,2),([.5,.5],[5,10]))
        self.assertEqual(attend(q,keys[:2],values[:2],1,2),([1],[10,0]))

    def test_invalid(self):
        q=[1,0]; k=[1,0,0,1]; v=[10,0,0,20]
        cases=[(q,k,v,0,2),(q,k,v,65,2),(q,k,v,2,0),
               (q,k,v,2,65),(q,k,v,True,2),
               ([float('nan'),0],k,v,2,2),
               (q,[1,1001,0,1],v,2,2),
               (q,k,[10,float('inf'),0,20],2,2),
               (q,k[:-1],v,2,2),
               (q,k,[10,True,0,20],2,2)]
        for args in cases:
            with self.subTest(args=str(args)[:60]):
                with self.assertRaises(ValueError):
                    attend(*args)


if __name__=="__main__":
    unittest.main()
