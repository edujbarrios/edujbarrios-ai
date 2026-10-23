import math
import unittest
from scaled_dot_product_attention import scaled_dot_product_attention as attention

class AttentionTests(unittest.TestCase):
    def test_weighted_and_equal(self):
        weight=math.exp(1/math.sqrt(2))/(math.exp(1/math.sqrt(2))+1)
        result=attention([1,0],[[1,0],[0,1]],[[10,0],[0,20]])
        self.assertAlmostEqual(result[0],10*weight)
        self.assertAlmostEqual(result[1],20*(1-weight))
        self.assertEqual(attention([1,1],[[1,0],[0,1]],[[2,4],[6,8]]),[4.0,6.0])
        self.assertEqual(attention([1],[[1]],[[7]]),[7.0])
    def test_stable_logits(self):
        self.assertEqual(attention([1000],[[-1000],[1000]],[[0],[1]]),[1.0])
    def test_invalid(self):
        for q,k,v in (([],[[1]],[[1]]),([1],[],[]),([1],[[1]],[]),
                      ([1,2],[[1]],[[1]]),([1],[[1],[2]],[[1],[1,2]]),
                      ([float("nan")],[[1]],[[1]]),([1],[[1]],[[float("inf")]]),
                      ([1e308],[[1e308]],[[1]]),([True],[[1]],[[1]])):
            with self.subTest(q=q,k=k,v=v),self.assertRaises(ValueError):
                attention(q,k,v)

if __name__=="__main__":unittest.main()
