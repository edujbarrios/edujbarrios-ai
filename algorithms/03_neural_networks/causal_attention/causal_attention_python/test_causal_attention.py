import unittest
from causal_attention import causal_attention

class CausalTests(unittest.TestCase):
    def test_uniform_prefixes(self):
        out=causal_attention([[0],[0],[0]],[[1],[2],[3]],
                             [[2,4],[6,8],[10,12]])
        self.assertEqual(out,[[2.0,4.0],[4.0,6.0],[6.0,8.0]])
        self.assertEqual(causal_attention([[3]],[[3]],[[7]]),[[7.0]])
    def test_stable_large_scores(self):
        self.assertEqual(causal_attention([[1000],[1000]],
                          [[-1000],[1000]],[[0],[7]]),[[0.0],[7.0]])
    def test_invalid(self):
        cases=(([],[],[]), ([[1]],[],[[1]]),
               ([[1]],[[1]],[]),
               ([[1,2]],[[1]],[[1]]),
               ([[1],[2]],[[1],[2]],[[1]]),
               ([[float("nan")]],[[1]],[[1]]),
               ([[1]],[[1]],[[float("inf")]]),
               ([[1e308]],[[1e308]],[[1]]))
        for q,k,v in cases:
            with self.subTest(q=q,k=k,v=v),self.assertRaises(ValueError):
                causal_attention(q,k,v)

if __name__=="__main__":unittest.main()
