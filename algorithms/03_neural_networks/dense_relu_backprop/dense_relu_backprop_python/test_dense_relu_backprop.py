import unittest
from dense_relu_backprop import dense_relu_backprop as backward

class DenseReluBackpropTests(unittest.TestCase):
    def test_active_and_inactive(self):
        self.assertEqual(backward([2,-1],[[1,1],[-1,2]],[0,1],[3,4]),
                         ([3,3],[[6,-3],[0,0]],[3,0]))
    def test_zero_subgradient(self):
        self.assertEqual(backward([0],[[3]],[0],[5]),([0],[[0]],[0]))
    def test_invalid(self):
        for x,w,b,g in (([],[[1]],[0],[1]), ([1],[],[],[]),
                        ([1,2],[[1]],[0],[1]), ([1],[[1]],[0],[]),
                        ([1],[[1]],[float("inf")],[1]),
                        ([1],[[float("nan")]],[0],[1]),
                        ([1e308],[[1e308]],[0],[1]), ([True],[[1]],[0],[1])):
            with self.subTest(x=x,w=w,b=b,g=g),self.assertRaises(ValueError):
                backward(x,w,b,g)

if __name__=="__main__":unittest.main()
